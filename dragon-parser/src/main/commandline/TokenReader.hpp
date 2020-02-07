#ifndef TOKENREADER
#define TOKENREADER

#include <string>
#include <stdio.h>
#include <optional>
#include "BaseToken.hpp"
#include "TokenList.hpp"
#include "TokenKeys.hpp"

optional<BaseToken> recognizeToken(string tkn, bool debug);

#endif