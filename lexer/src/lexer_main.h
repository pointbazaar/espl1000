#pragma once

struct LexerFlags;

// @returns < 0 on error
// @returns file descriptor on success.
//           Needs to be closed by the caller.
int lexer_main(struct LexerFlags* flags);
