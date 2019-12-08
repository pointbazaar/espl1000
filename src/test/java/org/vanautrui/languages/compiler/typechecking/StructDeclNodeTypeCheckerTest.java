package org.vanautrui.languages.compiler.typechecking;

import org.junit.Test;
import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;

import static org.junit.Assert.fail;

public final class StructDeclNodeTypeCheckerTest {


  @Test
  public void test_typechecking_struct_valid() throws Exception{

    Parser parser=new Parser();
    TokenList tokens = ParserPhases.makeTokenList(
            " struct MyStruct{} ()~>PInt main{ return 0;} " );

    final AST_Whole_Program ast = parser.parseTestMode(tokens,false,"Main");

    TypeChecker.doTypeCheck(ast,false);
  }

  @Test
  public void test_typechecking_struct_invalid_primitive_typename() throws Exception{

    Parser parser=new Parser();
    TokenList tokens = ParserPhases.makeTokenList(
            " struct PInt{} " );

    final AST_Whole_Program ast = parser.parseTestMode(tokens,false,"Main");

    try {
      TypeChecker.doTypeCheck(ast, false);
      fail();
    }catch (Exception e){
      //
    }
  }

  @Test
  public void test_typechecking_struct_invalid_multiple_declarations() throws Exception{

    final TokenList tokens = ParserPhases.makeTokenList(
            " struct MyStruct{} struct MyStruct{} " );

    final AST_Whole_Program ast = (new Parser()).parseTestMode(tokens,false,"Main");

    try {
      TypeChecker.doTypeCheck(ast, false);
      fail();
    }catch (Exception e){
      //
    }
  }

}
