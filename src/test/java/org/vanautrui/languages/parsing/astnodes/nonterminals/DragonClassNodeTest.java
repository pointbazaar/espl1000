package org.vanautrui.languages.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;

public class DragonClassNodeTest {

    @Test
    public void test_can_parse_class_with_1_empty_method() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new AccessModifierToken("public"));
        list.add(new ClassToken());
        list.add(new TypeIdentifierToken("Main"));
        list.add(new SymbolToken("{"));

        list.add(new AccessModifierToken("public"));
        list.add(new TypeIdentifierToken("Void"));
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));
        //no statements here
        list.add(new SymbolToken("}"));

        list.add(new SymbolToken("}"));

        DragonClassNode classNode = new DragonClassNode(list);
    }

    @Test
    public void test_can_parse_class_with_1_method() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new AccessModifierToken("public"));
        list.add(new ClassToken());
        list.add(new TypeIdentifierToken("Main"));
        list.add(new SymbolToken("{"));

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

        list.add(new SymbolToken("}"));

        DragonClassNode classNode = new DragonClassNode(list);
    }
}
