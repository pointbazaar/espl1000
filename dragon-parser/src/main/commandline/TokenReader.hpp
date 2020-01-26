#ifndef TOKENREADER
#define TOKENREADER

//standard includes
#include <vector> 
#include <string>
#include <stdio.h>
#include <map>
#include <optional>

//project includes
#include "../lexing/BaseToken.hpp"
#include "../lexing/TokenList.hpp"
#include "TokenKeys.hpp"

BaseToken recognizeStrConstToken(string strconst);
BaseToken recognizeCharConstToken(string charconst);

optional<BaseToken> recognizeToken(string tkn, bool debug) throws Exception;

#endif