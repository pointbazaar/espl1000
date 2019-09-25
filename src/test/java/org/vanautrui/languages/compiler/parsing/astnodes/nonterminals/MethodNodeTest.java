package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

public class MethodNodeTest {

    @Test
    public void test_can_parse_method_with_arguments() throws Exception {

        TokenList list = new TokenList();


        list.add(new AccessModifierToken("public"));
        list.add(new TypeIdentifierToken("PInt"));
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        //arguments
        list.add(new TypeIdentifierToken("String"));
        list.add(new IdentifierToken("hello "));
        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        list.add(new SymbolToken("}"));

        MethodNode methodNode = new MethodNode(list,true);
    }

    @Test
    public void test_can_parse_method_without_arguments() throws Exception {

        TokenList list = new TokenList();


        list.add(new AccessModifierToken("public"));
        list.add(new TypeIdentifierToken("PInt"));
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        list.add(new SymbolToken("}"));

        MethodNode methodNode = new MethodNode(list,true);
    }

    @Test
    public void test2()throws Exception{
        TokenList list = (new Lexer()).lexCodeTestMode("public PInt main ( ) { x = 3 ; if ( x < 5 ) { println ( x<5 ) ; } return 0 ; }");

        MethodNode mthd = new MethodNode(list,true);
    }
}
