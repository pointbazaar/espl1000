<img src="https://raw.githubusercontent.com/pointbazaar/dragon-lexer/master/img/dragon-logo.svg" width="100" heigth="100"/>

# Dragon-Lexer
this is the lexer for [smalldragon](https://github.com/pointbazaar/smalldragon/)

## Tokens Grammar (the tokens recognized by the lexer)

the tokens from here can later be used by the parser.

```
int ::= ... | -1 | 0 | 1 | 2 | ...
upperCaseLetter ::= 'A' | ... | 'Z'
charConstant ::= 'a' | ... | 'z' | 'A' | ... | 'Z' | '\n'
alphanumeric ::= int | charConstant | '_'

stringConstant ::= '"' alphanumeric* '"'

IDENTIFIER ::= charConstant alphanumeric*
TYPEIDENTIFIER ::= upperCaseLetter alphanumeric*

op ::= '+' | '-' | '*' | '/' | ...
unaryOp ::= '-' | '!'
boolConstant ::= 'true' | 'false'

// --- Keywords ---

RETURN ::= 'return'
WHILE ::= 'while'
FOR ::= 'for'
LOOP ::= 'loop'
IF ::= 'if'
ELSE ::= 'else'
STRUCT ::= 'struct'

arrow ::= '->' | '~>'

structMemberAccess ::= '.'

anyTypeToken ::= '#'
```
## Performance Characteristics

let n be the size of the input file in bytes
- Memory Consumption is in O(1) due to inputs being read into a fixed size buffer and processed piece by piece
  - the buffer for the tokens and for the input are (at time of writing) 5000 bytes each, so minimum memory usage would be 10000 bytes
- Runtime is in O(n) where n is the size of the input file

## Implementation

The Lexer is implemented as a deterministic finite state machine (dfa).

inspired by this paper:
[some strategies for fast lexical analysis](https://nothings.org/computer/lexing.html)


You can fork it if you want to build a lexer from scratch for your own language.
You could also use it to build syntax highlighting for [dragon](https://github.com/pointbazaar/dragon/)

**DO NOT TRY THIS AT HOME** . Writing a deterministic finite state-machine by hand in a 2d array in C 
is a bad idea. It takes so much time and probably still contains bugs. The only justification would be the speeds 
that might be possible. 

## Contributions
- Forks are welcome
- Issues are welcome
- Pull Requests are not welcome

