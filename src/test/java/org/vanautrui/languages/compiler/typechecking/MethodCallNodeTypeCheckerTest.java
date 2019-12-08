package org.vanautrui.languages.compiler.typechecking;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;

public final class MethodCallNodeTypeCheckerTest {

    @Test
    public void test_methodcall_must_supply_correct_number_of_arguments() throws Exception{

        final TokenList tokens = ParserPhases.makeTokenList(
                "(PInt x)~>PInt subr{return 0;} ()~>PInt main{ subr(); return 0;} ");

        final AST_Whole_Program ast = (new Parser()).parseTestMode(tokens,false,"Main");


        try {
            TypeChecker.doTypeCheck(ast, false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }

    @Test
    public void test_methodcall_must_supply_correct_type_of_arguments() throws Exception{

        final TokenList tokens = ParserPhases.makeTokenList(
                " (PInt x)~>PInt subr{return 0;} ()~>PInt main{ subr('c'); return 0;} ");

        final AST_Whole_Program ast = (new Parser()).parseTestMode(tokens,false,"Main");


        try {
            TypeChecker.doTypeCheck(ast, false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }
}
