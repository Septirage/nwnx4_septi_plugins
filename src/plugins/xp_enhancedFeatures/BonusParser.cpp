
#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>

#include "BonusParser.h"

namespace MathExpressionParser
{

int FunctionClassBased(const std::vector<int>& params, int creaBlock, FunctionType tType) 
{
	int iValue = 0;
	// For each, check if in params, if yes, summ or take max
	uint8_t iNbClass   = *(uint8_t*)(creaBlock + 0x14);
	int iBaseClass = 0x10C;

	for (uint8_t i = 0; i < iNbClass; i++) {
		uint8_t iClassID = *(uint8_t*)(creaBlock + iBaseClass + 0x4);

		auto it = std::find(params.begin(), params.end(), iClassID);

		// The current class is one of the parameters
		if (it != params.end()) {
			// Somme
			if (tType == FunctionType::CLASSLVLSUM)
				iValue += *(uint8_t*)(creaBlock + iBaseClass + 0x5);
			else //Max
			{
				uint8_t tVal = *(uint8_t*)(creaBlock + iBaseClass + 0x5);
				if (iValue < tVal)
					iValue = tVal;
			}
		}
		iBaseClass += 0x0124;
	}

	return iValue;
}

int ClassLevelSum(const std::vector<int>& params, int creaBlock)
{
	return FunctionClassBased(params, creaBlock, FunctionType::CLASSLVLSUM);
}


int ClassLevelMax(const std::vector<int>& params, int creaBlock) 
{ 
	return FunctionClassBased(params, creaBlock, FunctionType::CLASSLVLMAX);
}






//Number Struct Implementation
Number::Number(double value) : value(value) {}

double Number::evaluate(int creaBlock) const { return value; }
int Number::evaluateInt(int creaBlock) const { return static_cast<int>(value); }



//Interger struct implementation
Integer::Integer(int value) : value(value) {}

double Integer::evaluate(int creaBlock) const { return static_cast<double>(value); }
int Integer::evaluateInt(int creaBlock) const { return value; }



//FunctionT struct implementation
FunctionT::FunctionT(FunctionType functionToUse, std::vector<int> params)
    : functionToUse(functionToUse), params(std::move(params)) {}


double FunctionT::evaluate(int creaBlock) const
{
	if (functionToUse == FunctionType::CLASSLVLSUM)
		return ClassLevelSum(params, creaBlock);
	else if (functionToUse == FunctionType::CLASSLVLMAX)
		return ClassLevelMax(params, creaBlock);
	return 0;
}

int FunctionT::evaluateInt(int creaBlock) const
{
	if (functionToUse == FunctionType::CLASSLVLSUM)
		return ClassLevelSum(params, creaBlock);
	else if (functionToUse == FunctionType::CLASSLVLMAX)
		return ClassLevelMax(params, creaBlock);

	return 0;
}

//BinaryOp Struct implementation

BinaryOp::BinaryOp(Expr* left, char op, Expr* right)
    : left(left)
    , right(right)
    , op(op)
{
}

BinaryOp::~BinaryOp()
{
	delete left;
	delete right;
}

double BinaryOp::evaluate(int creaBlock) const
{
	double leftVal  = left->evaluate(creaBlock);
	double rightVal = right->evaluate(creaBlock);
	switch (op) {
		case OPERATION_ADDITION:
			return leftVal + rightVal;
		case OPERATION_SUBSTRACTION:
			return leftVal - rightVal;
		case OPERATION_MULTIPLICATION:
			return leftVal * rightVal;
		case OPERATION_DIVISION:
			return leftVal / rightVal;
		case OPERATION_MODULUS:
			return fmod(leftVal, rightVal);
		case OPERATION_INTDIVISION:
			return static_cast<int>(leftVal) / static_cast<int>(rightVal);
		default:
			return 0;
			//throw std::runtime_error("Unknown operator");
	}
}

int BinaryOp::evaluateInt(int creaBlock) const
{
	int leftVal  = left->evaluateInt(creaBlock);
	int rightVal = right->evaluateInt(creaBlock);

	switch (op) {
		case OPERATION_ADDITION:
			return leftVal + rightVal;
		case OPERATION_SUBSTRACTION:
			return leftVal - rightVal;
		case OPERATION_MULTIPLICATION:
			return leftVal * rightVal;
		case OPERATION_INTDIVISION:
		case OPERATION_DIVISION:
			return leftVal / rightVal;
		case OPERATION_MODULUS:
			return leftVal % rightVal;
		default:
			return 0;
			//throw std::runtime_error("Unknown operator");
	}

	return 0;
}


// Fonction to skip whitespace
void skipWhitespace(const std::string& expr, size_t& pos)
{
	while (pos < expr.size() && isspace(expr[pos])) {
		++pos;
	}
}

// Fonction to parse the minus sign
double parseUnarySign(const std::string& expr, size_t& pos)
{
	double sign = 1.0;
	skipWhitespace(expr, pos);
	if (pos < expr.size() && expr[pos] == '-') {
		sign = -1.0;
		++pos;
		skipWhitespace(expr, pos);
	}
	return sign;
}


Expr* parseNumber(const std::string& expr, size_t& pos, Mode mode)
{
	double sign  = parseUnarySign(expr, pos);
	size_t start = pos;
	while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) {
		++pos;
	}
	if (mode == Mode::INTEGER && expr.substr(start, pos - start).find('.') != std::string::npos) {
		throw std::runtime_error("Floating point numbers are not allowed in INTEGER mode");
	}
	double value = std::stod(expr.substr(start, pos - start));
	if (mode == Mode::INTEGER) {
		return new Integer(static_cast<int>(sign * value));
	} else {
		return new Number(sign * value);
	}
}

Expr* parseFunction(const std::string& expr, size_t& pos)
{
	FunctionType funcChoice;
	std::string sFunction;
	if (expr.substr(pos, 13) == "ClassLevelSum") {
		funcChoice = FunctionType::CLASSLVLSUM;
		sFunction  = "ClassLevelSum";
		pos += 13;
	} else if (expr.substr(pos, 13) == "ClassLevelMax") {
		funcChoice = FunctionType::CLASSLVLMAX;
		sFunction  = "ClassLevelMax";
		pos += 13;
	} else {
		throw std::runtime_error("Unknown function");
	}

	//Now, get the parameters
	{
		skipWhitespace(expr, pos);
		if (expr[pos] != '(') {
			throw std::runtime_error("Expected '(' after " + sFunction);
		}
		++pos;
		skipWhitespace(expr, pos);

		std::vector<int> params;
		while (pos < expr.size() && expr[pos] != ')') {
			size_t start = pos;
			while (pos < expr.size() && isdigit(expr[pos])) {
				++pos;
			}
			int param = std::stoi(expr.substr(start, pos - start));
			if (param <= 0) {
				throw std::runtime_error(sFunction + " parameter must be a positive integer");
			}
			params.push_back(param);
			skipWhitespace(expr, pos);
			if (expr[pos] == ',') {
				++pos;
				skipWhitespace(expr, pos);
			}
		}

		if (expr[pos] != ')') {
			throw std::runtime_error("Expected ')' after " + sFunction + " parameters");
		}
		++pos;
		return new FunctionT(funcChoice, std::move(params));
	}
}

Expr* parseFactor(const std::string& expr, size_t& pos, Mode mode)
{
	skipWhitespace(expr, pos);
	if (expr[pos] == '(') {
		++pos;
		auto result = parseExpression(expr, pos, mode);
		skipWhitespace(expr, pos);
		if (expr[pos] != ')') {
			throw std::runtime_error("Expected ')'");
		}
		++pos;
		return result;
	} else if (isdigit(expr[pos]) || expr[pos] == '.' || expr[pos] == '-') {
		return parseNumber(expr, pos, mode);
	} else if (isalpha(expr[pos])) {
		return parseFunction(expr, pos);
	} else {
		throw std::runtime_error("Unexpected character");
	}
}

Expr* parseTerm(const std::string& expr, size_t& pos, Mode mode)
{
	auto left = parseFactor(expr, pos, mode);
	skipWhitespace(expr, pos);
	while (pos < expr.size() && (expr[pos] == '*' || expr[pos] == '/' || expr[pos] == '%')) {
		char op;
		switch (expr[pos]) {
			case '*':
				op = OPERATION_MULTIPLICATION;
				break;
			case '%':
				op = OPERATION_MODULUS;
				break;
			case '/':
				if ((pos + 1) < expr.size() && expr[pos + 1] == '/') {
					op = OPERATION_INTDIVISION;
					pos++;
				} else {
					op = OPERATION_DIVISION;
				}
				break;
		}
		++pos;
		skipWhitespace(expr, pos);
		auto right = parseFactor(expr, pos, mode);
		left       = new BinaryOp(left, op, right);
		skipWhitespace(expr, pos);
	}
	return left;
}

Expr* parseExpression(const std::string& expr, size_t& pos, Mode mode)
{
	auto left = parseTerm(expr, pos, mode);
	skipWhitespace(expr, pos);
	while (pos < expr.size() && (expr[pos] == '+' || expr[pos] == '-')) {
		char op;
		if (expr[pos] == '+')
			op = OPERATION_ADDITION;
		else
			op = OPERATION_SUBSTRACTION;
		++pos;
		skipWhitespace(expr, pos);
		auto right = parseTerm(expr, pos, mode);
		left       = new BinaryOp(left, op, right);
		skipWhitespace(expr, pos);
	}
	return left;
}

Expr* parse(const std::string& expr, Mode mode)
{
	size_t pos  = 0;
	auto result = parseExpression(expr, pos, mode);
	skipWhitespace(expr, pos);
	if (pos != expr.size()) {
		throw std::runtime_error("Unexpected characters at the end of the expression");
	}
	return result;
}

// Simply function for AST. Remove "constant" stuff
Expr* simplify(Expr* expr, Mode mode)
{
	if (mode == Mode::INTEGER && dynamic_cast<Integer*>(expr) != nullptr) {
		return expr; // Integer are already constants
	}
	else if (mode == Mode::FLOAT && dynamic_cast<Number*>(expr) != nullptr) {
		return expr; // Number are already constants
	} else if (auto functiont = dynamic_cast<FunctionT*>(expr)) {
		return expr; // cant simplify functions
	} else if (auto binaryOp = dynamic_cast<BinaryOp*>(expr)) {
		auto left  = simplify(binaryOp->left, mode);
		auto right = simplify(binaryOp->right, mode);

		// if new left is same as old one, need to remove it from expr.
		if (left == binaryOp->left)
			binaryOp->left = nullptr;

		if (right == binaryOp->right)
			binaryOp->right = nullptr;

		if (mode == Mode::INTEGER) {
			if (auto leftNumber = dynamic_cast<Integer*>(left)) {
				if (auto rightNumber = dynamic_cast<Integer*>(right)) {
					// Both are number, evaluate operation
					int result = 0;
					switch (binaryOp->op) {
						case OPERATION_ADDITION:
							result = leftNumber->value + rightNumber->value;
							break;
						case OPERATION_SUBSTRACTION:
							result = leftNumber->value - rightNumber->value;
							break;
						case OPERATION_MULTIPLICATION:
							result = leftNumber->value * rightNumber->value;
							break;
						case OPERATION_DIVISION: // In INTEGER mode, both division and int division
						                         // are the same
						case OPERATION_INTDIVISION:
							result = leftNumber->value / rightNumber->value;
							break;
						case OPERATION_MODULUS:
							result = (leftNumber->value % rightNumber->value);
							break;
						default:
							delete left;
							delete right;
							throw std::runtime_error("Unknown operator");
					}

					//No more need of left and right
					delete left;
					delete right;
					return new Integer(result);
				}
			}
		} else {
			if (auto leftNumber = dynamic_cast<Number*>(left)) {
				if (auto rightNumber = dynamic_cast<Number*>(right)) {
					// Both are number, evaluate operation
					double result = 0;
					switch (binaryOp->op) {
						case OPERATION_ADDITION:
							result = leftNumber->value + rightNumber->value;
							break;
						case OPERATION_SUBSTRACTION:
							result = leftNumber->value - rightNumber->value;
							break;
						case OPERATION_MULTIPLICATION:
							result = leftNumber->value * rightNumber->value;
							break;
						case OPERATION_DIVISION:
							result = leftNumber->value / rightNumber->value;
							break;
						case OPERATION_MODULUS:
							result = fmod(leftNumber->value, rightNumber->value);
							break;
						case OPERATION_INTDIVISION:
							result = static_cast<int>(leftNumber->value)
							    / static_cast<int>(rightNumber->value);
							break;
						default:
							delete left;
							delete right;
							throw std::runtime_error("Unknown operator");
					}

					// No more need of left and right
					delete left;
					delete right;

					return new Number(result);
				}
			}
		}

		return new BinaryOp(left, binaryOp->op, right);
	}
	throw std::runtime_error("Unknown expression type");
}


// Evaluation functions
int evaluateInt(Expr* expr, int creaBlock) 
{
	return expr->evaluateInt(creaBlock); 
}

double evaluate(Expr* expr, int creaBlock) 
{
	return expr->evaluate(creaBlock); 
}

// Parsing function
Expr* prepareSimplified(const std::string& expr, Mode mode)
{
	auto ast           = parse(expr, mode);
	auto simplifiedAst = simplify(ast, mode);
	if (ast != simplifiedAst)
		delete ast;

	return simplifiedAst;
}


}
