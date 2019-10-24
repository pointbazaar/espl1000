package org.vanautrui.languages.compiler.typechecking;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;

import java.util.ArrayList;

public final class MethodCallNodeTypeCheckerTest {

    @Test
    public void test_methodcall_must_supply_correct_number_of_arguments() throws Exception{

        final TokenList tokens = (new Lexer()).lexCodeTestMode(
                "public namespace ExampleClass{ (PInt x)~>PInt subr{return 0;} ()~>PInt main{ subr(); return 0;}" +
                        "}");

        final AST ast = (new Parser()).parseTestMode(tokens,false);

        final ArrayList<AST> asts = new ArrayList<>();
        asts.add(ast);

        try {
            TypeChecker.doTypeCheck(asts, false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }

    @Test
    public void test_methodcall_must_supply_correct_type_of_arguments() throws Exception{

        final TokenList tokens = (new Lexer()).lexCodeTestMode(
                "public namespace ExampleClass{ (PInt x)~>PInt subr{return 0;} ()~>PInt main{ subr('c'); return 0;}" +
                        "}");

        final AST ast = (new Parser()).parseTestMode(tokens,false);

        final ArrayList<AST> asts = new ArrayList<>();
        asts.add(ast);

        try {
            TypeChecker.doTypeCheck(asts, false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }
}
