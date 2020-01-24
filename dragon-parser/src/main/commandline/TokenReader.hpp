#ifndef TOKENREADER
#define

//standard includes
#include <vector> 
#include <stdio>
#include <map>

//project includes
#include "../compiler/lexing/tokens.*.hpp"
#include "../compiler/lexing/tokens.no_members.*.hpp"
#include "../compiler/lexing/tokens.no_members.keywords.*.hpp"
#include "../compiler/lexing/utils.IToken.hpp"

#include "../commandline/TokenKeys/*.hpp"
#include "../commandline/TokenKeys/LOOP.hpp"


StringConstantToken recognizeStrConstToken(string strconst);
CharConstantToken recognizeCharConstToken(string charconst);

Optional<BaseToken> recognizeToken(string tkn, bool debug) throws Exception;

#endif