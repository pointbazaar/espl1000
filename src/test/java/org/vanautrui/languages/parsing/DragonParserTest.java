package org.vanautrui.languages.parsing;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.lexing.tokens.ClassToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;

public class DragonParserTest {

    DragonParser parser = new DragonParser();

    @Test
    public void test_can_create_correct_classnode() throws Exception {
        DragonTokenList tokens = new DragonTokenList();
        tokens.add(new AccessModifierToken("public"));
        tokens.add(new ClassToken());

        String classname = "exampleclass";

        tokens.add(new IdentifierToken(classname));

        tokens.add(new SymbolToken("{"));
        tokens.add(new SymbolToken("}"));

        DragonAST ast = parser.parse(tokens);

        Assert.assertEquals(classname, ast.classNodeList.get(0).name.name);
    }

    public static class DragonASTTest {

        //TODO
    }
}
