#if !defined(SEPT_RULEPARSER)
#define SEPT_RULEPARSER


#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>


namespace RuleParser {

enum class RuleType {AREA, FEAT, EXTRA};

	
const uint32_t OP_AND = 1;
const uint32_t OP_OR  = 2;
const uint32_t OP_XOR = 3;
const uint32_t OP_NOT = 4;


enum class TokenType { OBJECT, OPERATOR, LPAREN, RPAREN, END, SPECIAL };

const int RULESPECIAL_MONKPOWER = 0;
const int RULESPECIAL_DETECTMODE = 1;
const int RULESPECIAL_STEALTHMODE = 2;
const int RULESPECIAL_TRACKMODE = 3;
const int RULESPECIAL_ENCUMBRANCE0 = 10;
const int RULESPECIAL_ENCUMBRANCE1 = 11;
const int RULESPECIAL_ENCUMBRANCE2 = 12;


struct Token {
	TokenType type;
	uint32_t intValue; // Store operator as int
};


struct Rule {
	TokenType type;
	uint32_t value;
	Rule* left;
	Rule* right;

	~Rule();
};


extern std::unordered_map<std::string, uint32_t> operatorValues;
extern std::unordered_map<std::string, uint32_t> m_areaTypeMap;
extern std::unordered_set<int> m_activatedRuleSet;


std::vector<Token> tokenize(const std::string& expression, RuleType cRuleType, bool bManageRule=true);
Rule* parseExpression(std::vector<Token>& tokens, int& pos);
bool evaluateRule(Rule* node, RuleType cRuleType, int areaTypeOrPCBlock);


} // namespace RuleParser


#endif
