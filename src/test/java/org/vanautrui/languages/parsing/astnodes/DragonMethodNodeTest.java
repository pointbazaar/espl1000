package org.vanautrui.languages.parsing.astnodes;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodNode;

public class DragonMethodNodeTest {

    @Test
    public void test_can_parse_method_with_arguments() throws Exception {

        DragonTokenList list = new DragonTokenList();


        list.add(new AccessModifierToken("public"));
        list.add(new IdentifierToken("void"));
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        //arguments
        list.add(new IdentifierToken("String"));
        list.add(new IdentifierToken("hello "));
        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        list.add(new SymbolToken("}"));

        DragonMethodNode methodNode = new DragonMethodNode(list);
    }

    @Test
    public void test_can_parse_method_without_arguments() throws Exception {

        DragonTokenList list = new DragonTokenList();


        list.add(new AccessModifierToken("public"));
        list.add(new IdentifierToken("void"));
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        list.add(new SymbolToken("}"));

        DragonMethodNode methodNode = new DragonMethodNode(list);
    }
}
