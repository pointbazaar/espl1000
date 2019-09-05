package org.vanautrui.languages.parsing;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

import java.util.stream.Collectors;

public class ParserTest {

    Parser parser = new Parser();

    @Test
    public void test_can_create_correct_classnode() throws Exception {
        TokenList tokens = new TokenList();
        tokens.add(new AccessModifierToken("public"));
        tokens.add(new KeywordToken("class"));

        String classname = "Exampleclass";

        tokens.add(new TypeIdentifierToken(classname));

        tokens.add(new SymbolToken("{"));
        tokens.add(new SymbolToken("}"));

        AST ast = parser.parseTestMode(tokens);

        Assert.assertEquals(classname, ast.classNodeList.stream().collect(Collectors.toList()).get(0).name.typeName);
    }

    public static class DragonASTTest {

        //TODO
    }
}
