package org.vanautrui.languages.compiler.typechecking;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.TestUtils;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;

public final class MethodCallNodeTypeCheckerTest {

    @Test
    public void test_methodcall_must_supply_correct_number_of_arguments() throws Exception{

        final String source =
                "fn subr (PInt x)~>PInt {return 0;} fn main ()~>PInt{ subr(); return 0;} ";

        final AST_Whole_Program ast = TestUtils.parse_for_test(source);

        try {
            TypeChecker.doTypeCheck(ast, false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }

    @Test
    public void test_methodcall_must_supply_correct_type_of_arguments() throws Exception{

        final String source =
                "fn subr (PInt x)~>PInt {return 0;} fn main ()~>PInt { subr('c'); return 0;} ";

        final AST_Whole_Program ast = TestUtils.parse_for_test(source);

        try {
            TypeChecker.doTypeCheck(ast, false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }
}
