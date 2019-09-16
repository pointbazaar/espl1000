package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.*;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

public class MethodNodeTest {

    @Test
    public void test_can_parse_method_with_arguments() throws Exception {

        TokenList list = new TokenList();


        list.add(new AccessModifierToken("public"));
        list.add(new TypeIdentifierToken("Void"));
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
        list.add(new TypeIdentifierToken("Void"));
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
}
