package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.upperscopes;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

import static org.junit.Assert.assertEquals;

public class MethodNodeTest {

    @Test
    public void test_can_parse_method_with_arguments() throws Exception {

        TokenList list = (new Lexer()).lexCodeTestMode("public (String hello)~>PInt main { main(); }");

        MethodNode methodNode = new MethodNode(list,false);

        assertEquals(1,methodNode.arguments.size());
    }

    @Test
    public void test_can_parse_method_with_subroutine_argument()throws Exception{
        TokenList list = (new Lexer()).lexCodeTestMode("public (()~>PInt subr)~>PInt main { return subr(); }");

        MethodNode methodNode = new MethodNode(list,false);

        assertEquals(1,methodNode.arguments.size());
    }

    @Test
    public void test_can_parse_subroutine()throws Exception{
        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode(
                "()~>PInt main{" +
                        "println(1);" +
                        "return 0;" +
                        "}");

        MethodNode methodNode = new MethodNode(tokens,true);
    }

    @Test
    public void test_can_parse_method_without_arguments() throws Exception {

        TokenList list = (new Lexer()).lexCodeTestMode("public ()~>PInt main { main(); }");

        MethodNode mthd = new MethodNode(list,false);
        assertEquals(0,mthd.arguments.size());
    }

    @Test
    public void test2()throws Exception{
        TokenList list = (new Lexer()).lexCodeTestMode("public ()~>PInt main { x = 3 ; if ( x < 5 ) { println ( x<5 ) ; } return 0 ; }");

        MethodNode mthd = new MethodNode(list,false);
        assertEquals(0,mthd.arguments.size());
    }
}
