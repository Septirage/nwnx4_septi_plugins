#if !defined(SEPT_BONUSPARSER)
#define SEPT_BONUSPARSER


#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>



namespace MathExpressionParser {

const char OPERATION_ADDITION       = 0;
const char OPERATION_SUBSTRACTION   = 1;
const char OPERATION_DIVISION       = 2;
const char OPERATION_MULTIPLICATION = 3;
const char OPERATION_MODULUS        = 4;
const char OPERATION_INTDIVISION    = 5;

enum class FunctionType {
	CLASSLVLSUM,
	CLASSLVLMAX
};

enum class Mode {
	FLOAT,
	INTEGER
};

//Functions for FunctionType:

// ClassLevelSum //TODO, add the creature ptr to check class
int ClassLevelSum(const std::vector<int>& params, int creaBlock);
// TODO
int ClassLevelMax(const std::vector<int>& params, int creaBlock);



// AST definition
struct Expr {
	virtual double evaluate(int creaBlock) const = 0;
	virtual int evaluateInt(int creaBlock) const = 0;
	virtual ~Expr()                 = default;
};

struct Number : Expr {
	double value;
	Number(double value);
	double evaluate(int creaBlock) const override;
	int evaluateInt(int creaBlock) const override;
};

struct Integer : Expr {
	int value;
	Integer(int value);
	double evaluate(int creaBlock) const override;
	int evaluateInt(int creaBlock) const override;
};

struct FunctionT : Expr {
	FunctionType functionToUse;
	std::vector<int> params;
	FunctionT(FunctionType functionToUse, std::vector<int> params);

	double evaluate(int creaBlock) const override;
	int evaluateInt(int creaBlock) const override;
};

struct BinaryOp : Expr {
	Expr* left;
	Expr* right;
	char op;
	BinaryOp(Expr* left, char op, Expr* right);
	~BinaryOp();

	double evaluate(int creaBlock) const override;
	int evaluateInt(int creaBlock) const override;
};

// Fonction to skip whitespace
void skipWhitespace(const std::string& expr, size_t& pos);

// Fonction to parse the minus sign
double parseUnarySign(const std::string& expr, size_t& pos);

// Parsing function
Expr* parseExpression(const std::string& expr, size_t& pos, Mode mode);

Expr* parseNumber(const std::string& expr, size_t& pos, Mode mode);

Expr* parseFunction(const std::string& expr, size_t& pos);

Expr* parseFactor(const std::string& expr, size_t& pos, Mode mode);

Expr* parseTerm(const std::string& expr, size_t& pos, Mode mode);

Expr* parseExpression(const std::string& expr, size_t& pos, Mode mode);

Expr* parse(const std::string& expr, Mode mode);

// Function to simplify ast by removing constant part
Expr* simplify(Expr* expr, Mode mode);

// Evaluation functions
int evaluateInt(Expr* expr, int creaBlock);

double evaluate(Expr* expr, int creaBlock);

// Parsing function
Expr* prepareSimplified(const std::string& expr, Mode mode);

} // namespace MathExpressionParser



#endif //SEPT_BONUSPARSER