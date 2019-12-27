package org.vanautrui.languages.compiler.typechecking;

import org.junit.Test;
import org.vanautrui.languages.TestUtils;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;

import static org.junit.Assert.fail;

public final class StructDeclNodeTypeCheckerTest {


  @Test
  public void test_typechecking_struct_valid() throws Exception{

    final String source = " struct MyStruct{} fn main ()~>PInt { return 0;} ";

    final AST_Whole_Program ast = TestUtils.parse_for_test(source, false);

    TypeChecker.doTypeCheck(ast,false);
  }

  @Test
  public void test_typechecking_struct_invalid_primitive_typename() throws Exception{

    final String source = " struct PInt{} ";

    final AST_Whole_Program ast = TestUtils.parse_for_test(source, false);

    try {
      TypeChecker.doTypeCheck(ast, false);
      fail();
    }catch (Exception e){
      //
    }
  }

  @Test
  public void test_typechecking_struct_invalid_multiple_declarations() throws Exception{

    final String source = " struct MyStruct{} struct MyStruct{} ";

    final AST_Whole_Program ast = TestUtils.parse_for_test(source, false);

    try {
      TypeChecker.doTypeCheck(ast, false);
      fail();
    }catch (Exception e){
      //
    }
  }

}
