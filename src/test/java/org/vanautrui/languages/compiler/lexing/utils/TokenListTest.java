package org.vanautrui.languages.compiler.lexing.utils;

import org.apache.commons.lang3.tuple.Pair;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class TokenListTest {

  @Test
  public void test_until() throws Exception{
    Lexer lexer = new Lexer();
    TokenList tokens = lexer.lexCodeTestMode("public class Main { public PInt }");

    TokenList untilTokens = tokens.until(new SymbolToken("{"));

    assertEquals(3,untilTokens.size());
    assertTrue(untilTokens.startsWith(new AccessModifierToken("public")));
  }

  @Test
  public void test_split_blocks()throws Exception{
    Lexer lexer = new Lexer();
    TokenList tokens = lexer.lexCodeTestMode("PInt main(){ PInt } PInt subr(){ Char } ");

    Pair<TokenList, TokenList> pair = tokens.split_into_tokens_and_next_block_and_later_tokens();

    TokenList left = pair.getLeft();
    TokenList right = pair.getRight();

    assertEquals(7,left.size());
    assertEquals(7,right.size());
    assertTrue(left.startsWith(new TypeIdentifierToken("PInt")));
    assertTrue(right.startsWith(new TypeIdentifierToken("PInt")));
  }

  @Test
  public void test_split_blocks_2()throws Exception{
    Lexer lexer = new Lexer();
    TokenList tokens = lexer.lexCodeTestMode("PInt main ( ) { x = 3 ; if ( x == 3 ) { y = 5 ; } } }");

    Pair<TokenList, TokenList> pair = tokens.split_into_tokens_and_next_block_and_later_tokens();

    TokenList left = pair.getLeft();
    TokenList right = pair.getRight();

    assertEquals(1,right.size());
  }
}
