#include "RuleParser.h"
#include "RuleCreatureFct.h"
#include "../../septutil/NwN2DataPos.h"

#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <regex>



#define OFFS_FCT_DetectModeState	0x005bdc90
#define OFFS_FCT_TrackModeState		0x005bdce0


namespace RuleParser
{

	std::unordered_map<std::string, uint32_t> operatorValues
		= {{"&", OP_AND}, {"|", OP_OR}, {"^", OP_XOR}, {"!", OP_NOT}};

	std::unordered_map<std::string, uint32_t> operatorNumValues
		= { {"==", OP_EQUAL}, {"!=", OP_NOTEQUAL}, {"<", OP_LESS}, {"<=", OP_LESSEQUAL}, {">", OP_GREATER}, {">=", OP_GREATEREQUAL} };

	std::unordered_map<std::string, uint32_t> functionValues
		= { {"skill", FCT_SKILL}, {"ability", FCT_ABILITY}, {"classlevelsum", FCT_CLASSSUM}, {"classlevelmax", FCT_CLASSMAX} };


	std::unordered_map<std::string, uint32_t> m_areaTypeMap;
	std::unordered_set<int> m_activatedRuleSet;

	void processCurrentToken(const std::string& currentToken, std::vector<Token>& tokens, RuleType cRuleType, bool bManageRule=true);
////////////////////////////////

Rule::~Rule() 
{
	delete left;
	delete right;
}

void processCurrentToken(const std::string& currentToken, std::vector<Token>& tokens, RuleType cRuleType, bool bManageRule) 
{
	if (cRuleType == RuleType::FEAT) {
		try {
			int number = std::stoi(currentToken);
			tokens.push_back({TokenType::OBJECT, static_cast<uint32_t>(number)});
		} catch (const std::exception& e) {
			throw std::invalid_argument("Unknown token: " + currentToken);
		}
	} else if(cRuleType == RuleType::AREA){
		if (m_areaTypeMap.find(currentToken) != m_areaTypeMap.end()) {
			tokens.push_back({TokenType::OBJECT, m_areaTypeMap[currentToken]});
		} else {
			throw std::invalid_argument("Unknown token: " + currentToken);
		}
	}
	else
	{

		std::string lowerToken = currentToken;
		std::transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);
		if (lowerToken == "monkpower")
		{
			tokens.push_back({TokenType::SPECIAL, RULESPECIAL_MONKPOWER});
		}
		else if (lowerToken == "detectmode")
		{
			tokens.push_back({TokenType::SPECIAL, RULESPECIAL_DETECTMODE});
		}
		else if (lowerToken == "stealthmode")
		{
			tokens.push_back({TokenType::SPECIAL, RULESPECIAL_STEALTHMODE});
		}
		else if (lowerToken == "trackmode")
		{
			tokens.push_back({TokenType::SPECIAL, RULESPECIAL_TRACKMODE});
		}
		else if (lowerToken == "encumbrancenone")
		{
			tokens.push_back({TokenType::SPECIAL, RULESPECIAL_ENCUMBRANCE0});
		}
		else if (lowerToken == "encumbrancenormal")
		{
			tokens.push_back({TokenType::SPECIAL, RULESPECIAL_ENCUMBRANCE1});
		}
		else if (lowerToken == "encumbranceheavy")
		{
			tokens.push_back({TokenType::SPECIAL, RULESPECIAL_ENCUMBRANCE2});
		}
		else if (operatorNumValues.find(currentToken) != operatorNumValues.end())
		{
			tokens.push_back({TokenType::OPNUM, operatorNumValues[currentToken]});
		}
		else if (functionValues.find(lowerToken) != functionValues.end())
		{
			tokens.push_back({ TokenType::FUNCTION, functionValues[currentToken] });
		}
		else
		{
			std::regex pattern("(Rule)(\\d+)", std::regex::icase);
			std::smatch match;
			if (bManageRule && std::regex_match(currentToken, match, pattern)) {
				int number = std::stoi(match[2].str());
				tokens.push_back({TokenType::OBJECT, static_cast<uint32_t>(number)});
			} else {
				try {
					int number = std::stoi(currentToken);
					tokens.push_back({TokenType::NUM, static_cast<uint32_t>(number)});
				} catch (const std::exception& e) {
					throw std::invalid_argument("Unknown token: " + currentToken);
				}
				//throw std::invalid_argument("Unknow token: " + currentToken);
			}
		}
	}
}

// Tokenizer that manage no space between token and the "number" version
std::vector<Token> tokenize(const std::string& expression, RuleType cRuleType, bool bManageRule)
{
	std::vector<Token> tokens;
	std::stringstream stream(expression);
	std::string token;
	std::string currentToken;
	bool insideFct = false;

	while (stream >> token) {
		currentToken.clear();
		for (size_t i = 0; i < token.size(); ++i) {
			char c = token[i];

			//Just to detect and manage possible case of '<=' and '>=' in case of 
			if (c == '<' || c == '>') {
				if (!currentToken.empty()) {
					processCurrentToken(currentToken, tokens, cRuleType, bManageRule);
					currentToken.clear();
				}

				if ( ((i + 1) < token.size()) && token[i+1] == '=')
				{
					currentToken += c;
					i++;
					c = token[i];
				}
			}

			//Really manage tokens here
			if (insideFct && c == ',')
			{
				if (!currentToken.empty()) {
					processCurrentToken(currentToken, tokens, cRuleType, bManageRule);
					currentToken.clear();
				}
			}
			else if (operatorValues.find(std::string(1, c)) != operatorValues.end()) {
				if (!currentToken.empty()) {
					processCurrentToken(currentToken, tokens, cRuleType, bManageRule);
					currentToken.clear();
				}
				tokens.push_back({TokenType::OPERATOR, operatorValues[std::string(1, c)]});
			} else if (c == '(') {
				if (!currentToken.empty()) {  //Cant have an OBJECT dirrectly followed by a (
					//Only function can (and must) be dirrectly followed by a (
					processCurrentToken(currentToken, tokens, cRuleType, bManageRule);
					if (tokens.size() > 0 && tokens.back().type == TokenType::FUNCTION)
					{
						currentToken.clear();
						insideFct = true;
					}
					else
					{
						throw std::invalid_argument("Bad format for the rule : " + expression);
					}
					/*					
					processCurrentToken(currentToken, token, cRuleType, bManageRule);
					currentToken.clear();
					*/
				}
				else if (tokens.size() > 0 && tokens.back().type == TokenType::FUNCTION)
				{
					insideFct = true;
				}
				tokens.push_back({TokenType::LPAREN, 0});
			} else if (c == ')') {
				if (!currentToken.empty()) {
					processCurrentToken(currentToken, tokens, cRuleType, bManageRule);
					currentToken.clear();
				}
				insideFct = false; //No matter if we are in or not, we are no more
				tokens.push_back({TokenType::RPAREN, 0});
			} else {
				currentToken += c;
			}
		}
		if (!currentToken.empty()) {
			processCurrentToken(currentToken, tokens, cRuleType, bManageRule);
		}
	}

	tokens.push_back({TokenType::END, 0});
	return tokens;
}

Rule* parseExpression(std::vector<Token>& tokens, int& pos)
{
	auto parseNumPrimary = [&]() -> Rule* {
		if (pos < tokens.size() && tokens[pos].type == TokenType::FUNCTION) {
			uint32_t iFctType = tokens[pos].intValue;
			pos++;
			if (pos >= tokens.size() || tokens[pos].type != TokenType::LPAREN)
				throw std::invalid_argument("Expected '(' after the a function name");
			pos++; //LParen ok

			std::vector<int> tParams;
			//Now, add the parameter(s)
			while (pos < tokens.size() && tokens[pos].type != TokenType::RPAREN)
			{
				if(tokens[pos].type != TokenType::NUM)
					throw std::invalid_argument("Functions parameters should be integer");

				tParams.push_back(tokens[pos].intValue);
				pos++;
			}

			//Here, we have the rParen, just pass it
			if (pos >= tokens.size())
			{
				throw std::invalid_argument("Functions should end with ')'");
			}
			pos++;
			
			//Check tParams number
			if ((iFctType == FCT_SKILL || iFctType == FCT_ABILITY) && tParams.size() != 1)
			{
				throw std::runtime_error("Expected exactly one paramter for function Skill or Ability");
			}
			else if (tParams.size() < 1) {
				throw std::runtime_error("Expected at least one paramter for function");
			}

			//Check param values
			if(iFctType == FCT_ABILITY)
			{
				int iAbility = tParams.back();
				if (iAbility < 0 || iAbility > 5)
					throw std::runtime_error("Ability function must have a parameter from 0 to 5, you have put " + std::to_string(iAbility));
			}

			Rule* node = new Rule{ TokenType::FUNCTION, iFctType, nullptr, nullptr, tParams};

			return node;

		}
		else if (pos < tokens.size() && tokens[pos].type == TokenType::NUM) {
			Rule* node = new Rule{ TokenType::NUM, tokens[pos].intValue, nullptr, nullptr};
			pos++;
			return node;
		} else {
			throw std::invalid_argument("Expected Function or numeric value around a numerical comparator");
		}
	};

	auto parseNumComparaison = [&]() -> Rule* {
		if (tokens[pos].type == TokenType::FUNCTION || tokens[pos].type == TokenType::NUM)
		{
			Rule* left = parseNumPrimary();
			if (pos >= tokens.size() || tokens[pos].type != TokenType::OPNUM)
			{
				throw std::invalid_argument("Numeric value or function can only be used with numeric comparators");
			}

			Token op = tokens[pos];
			pos++;

			Rule* right = parseNumPrimary();

			Rule* node = new Rule{ TokenType::OPNUM, op.intValue, left, right };
			return node;
		}
		else {
			throw std::invalid_argument("Problem during parse of NumComparaison");
		}
	};

	auto parsePrimary = [&]() -> Rule* {
		if (tokens[pos].type == TokenType::OBJECT) {
			Rule* node = new Rule {TokenType::OBJECT, tokens[pos].intValue, nullptr, nullptr};
			pos++;
			return node;
		} else if (tokens[pos].type == TokenType::SPECIAL) {
			Rule* node = new Rule {TokenType::SPECIAL, tokens[pos].intValue, nullptr, nullptr};
			pos++;
			return node;
		} else if (tokens[pos].type == TokenType::FUNCTION || tokens[pos].type == TokenType::NUM) {
			return parseNumComparaison();
		} else if (tokens[pos].type == TokenType::LPAREN) {
			pos++;
			Rule* expr = parseExpression(tokens, pos);
			if (tokens[pos].type != TokenType::RPAREN) {
				throw std::invalid_argument("Expected ')'");
			}
			pos++;
			return expr;
		} else {
			throw std::invalid_argument("Expected primary expression");
		}
	};

	auto parseUnary = [&]() -> Rule* {
		if (tokens[pos].type == TokenType::OPERATOR && tokens[pos].intValue == OP_NOT) {
			Token op      = tokens[pos++];
			Rule* operand = parsePrimary();
			return new Rule {TokenType::OPERATOR, op.intValue, operand, nullptr};
		} else {
			return parsePrimary();
		}
	};

	auto parseFactor = [&]() -> Rule* {
		Rule* left = parseUnary();
		while (pos < tokens.size() && tokens[pos].type == TokenType::OPERATOR
		       && tokens[pos].intValue == OP_AND) {
			Token op    = tokens[pos++];
			Rule* right = parseUnary();
			left        = new Rule {TokenType::OPERATOR, op.intValue, left, right};
		}
		return left;
	};

	auto parseTerm = [&]() -> Rule* {
		Rule* left = parseFactor();
		while (pos < tokens.size() && tokens[pos].type == TokenType::OPERATOR
		       && tokens[pos].intValue == OP_XOR) {
			Token op    = tokens[pos++];
			Rule* right = parseFactor();
			left        = new Rule {TokenType::OPERATOR, op.intValue, left, right};
		}
		return left;
	};

	Rule* left = parseTerm();
	while (pos < tokens.size() && tokens[pos].type == TokenType::OPERATOR
	       && tokens[pos].intValue == OP_OR) {
		Token op    = tokens[pos++];
		Rule* right = parseTerm();
		left        = new Rule {TokenType::OPERATOR, op.intValue, left, right};
	}
	return left;
}


bool TestIfFeat(int uPcBlock, uint16_t uFeat)
{
	uint32_t currentNbOfFeats  = *(uint32_t*)(uPcBlock + 0x1C);
	if (currentNbOfFeats != 0) 
	{
		uint16_t* currentFeatsList = *(uint16_t**)(uPcBlock + 0x18);

		return std::binary_search(currentFeatsList, currentFeatsList + currentNbOfFeats, uFeat);
	}

	return false;
}
__declspec(naked) uint8_t __fastcall DetectModeState(__in int pCreature)
{
	__asm
	{
		MOV EAX, OFFS_FCT_DetectModeState
		JMP EAX
	}
}

__declspec(naked) uint8_t __fastcall TrackModeState(__in int pCreature)
{
	__asm
	{
		MOV EAX, OFFS_FCT_TrackModeState
		JMP EAX
	}
}


bool TestSpecial(int iSpecialType, int creaPtr)
{
	bool bResult = false;
	uint32_t iVal;
	switch(iSpecialType) 
	{
	case RULESPECIAL_MONKPOWER:
		iVal = *(uint8_t*)(creaPtr + AmCrtMonkSpeed);
		bResult = (iVal == 1);
		break;
	case RULESPECIAL_DETECTMODE:
		//iVal = *(uint8_t*)(creaPtr + AmCrtDetectMode);
		iVal = DetectModeState(creaPtr);
		bResult = (iVal == 1);
		break;
	case RULESPECIAL_STEALTHMODE:
		iVal = *(uint8_t*)(creaPtr + AmCrtStealthMode);
		bResult = (iVal == 1);
		break;
	case RULESPECIAL_TRACKMODE:
		//iVal = *(uint8_t*)(creaPtr + AmCrtTrackingMode);
		iVal = TrackModeState(creaPtr);
		bResult = (iVal == 1);
		break;
	case RULESPECIAL_ENCUMBRANCE0:
		iVal = *(uint8_t*)(creaPtr + AmCrtEncumbrance);
		bResult = (iVal == 0);
		break;
	case RULESPECIAL_ENCUMBRANCE1:
		iVal = *(uint8_t*)(creaPtr + AmCrtEncumbrance);
		bResult = (iVal == 1);
		break;
	case RULESPECIAL_ENCUMBRANCE2:
		iVal = *(uint8_t*)(creaPtr + AmCrtEncumbrance);
		bResult = (iVal == 2);
		break;
	default:
		bResult = false;
	}

	return bResult;
}

int evaluateRuleInt(Rule* node, RuleType cRuleType, int iPcAddr, bool& isValid)
{
	isValid = true;
	if (node->type == TokenType::FUNCTION)	//Exist only on extra, so 
	{
		int CreaBlockStat = *(int*)(iPcAddr + AmCrtPtrAppBlock);
		if (node->value == FCT_SKILL)
			return GetBaseSkill(node->params.back(), CreaBlockStat);
		else if (node->value == FCT_ABILITY)
			return GetBaseAbility(node->params.back(), CreaBlockStat);
		else if (node->value == FCT_CLASSSUM)
			return ClassLevelSum(node->params, CreaBlockStat);
		else if (node->value == FCT_CLASSSUM)
			return ClassLevelMax(node->params, CreaBlockStat);
	}
	else if (node->type == TokenType::NUM)
	{
		return node->value;
	}
	isValid = false;
	return 0;
}

bool evaluateRule(Rule* node, RuleType cRuleType, int areaTypeOrPcBlock)
{
	if (node->type == TokenType::OBJECT) {
		if (cRuleType == RuleType::AREA)
			return ((areaTypeOrPcBlock & node->value) != 0);
		else if (cRuleType == RuleType::FEAT)
			return TestIfFeat(areaTypeOrPcBlock, node->value);
		else
			return (m_activatedRuleSet.find(node->value) != m_activatedRuleSet.end());
//			return m_activatedRuleSet.contains(node->value);
	} else if (node->type == TokenType::SPECIAL) {
		return TestSpecial(node->value, areaTypeOrPcBlock);
	} else if (node->type == TokenType::OPERATOR) {
		if (node->value == OP_NOT) {
			return !evaluateRule(node->left, cRuleType, areaTypeOrPcBlock);
		} else {
			bool leftValue = evaluateRule(node->left, cRuleType, areaTypeOrPcBlock);
			if (node->value == OP_AND) {
				if (!leftValue) {
					return false; // Short circuit for AND
				} else {
					return leftValue && evaluateRule(node->right, cRuleType, areaTypeOrPcBlock);
				}
			} else if (node->value == OP_OR) {
				if (leftValue) {
					return true; // Short circuit for OR
				} else {
					return leftValue || evaluateRule(node->right, cRuleType, areaTypeOrPcBlock);
				}
			} else if (node->value == OP_XOR) {
				return leftValue
				    != evaluateRule(node->right, cRuleType, areaTypeOrPcBlock); // For boolean, != is equal to XOR
			} else {
				//throw std::invalid_argument("Unknown operator: " + std::to_string(node->value));
				return 0;
			}
		}
	} else if(cRuleType == RuleType::EXTRA && node->type == TokenType::OPNUM) {
		bool bValid = true;
		int iLeft = evaluateRuleInt(node->left, cRuleType, areaTypeOrPcBlock, bValid);
		if (!bValid)
			return false;
		int iRight = evaluateRuleInt(node->right, cRuleType, areaTypeOrPcBlock, bValid);
		if (!bValid)
			return false;

		if (node->value == OP_EQUAL)
			return (iLeft == iRight);
		else if (node->value == OP_NOTEQUAL)
			return (iLeft != iRight);
		else if (node->value == OP_LESS)
			return (iLeft < iRight);
		else if (node->value == OP_LESSEQUAL)
			return (iLeft <= iRight);
		else if (node->value == OP_GREATER)
			return (iLeft > iRight);
		else if (node->value == OP_GREATEREQUAL)
			return (iLeft >= iRight);
	} else {
		//throw std::invalid_argument("Unknown node type");
		return 0;
	}
}

}