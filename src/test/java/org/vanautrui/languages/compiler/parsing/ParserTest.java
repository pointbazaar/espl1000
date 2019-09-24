package org.vanautrui.languages.compiler.parsing;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.*;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

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

        AST ast = parser.parseTestMode(tokens,true);

        Assert.assertEquals(classname, ast.classNodeList.stream().collect(Collectors.toList()).get(0).name.typeName);
    }

    @Test
    public void test_can_parse_subroutine()throws Exception{
        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode(
                "PInt main(){" +
                "println(1);" +
                "return 0;" +
                "}");


        Parser parser = new Parser();
        MethodNode methodNode = new MethodNode(tokens,true);
    }

    @Test
    public void test_can_parse_some_class()throws Exception{
        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode("class MyOutput{" +
                "PInt main(){" +
                "println(1);" +
                "return 0;" +
                "}" +
                "}");


        Parser parser = new Parser();
        AST ast = parser.parseTestMode(tokens, true);
    }

    @Test
    public void test_can_parse_some_class2()throws Exception{
        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode("public class IfStatement{\n" +
                "\tpublic PInt main(){\n" +
                "\n" +
                "\t\tx=3;\n" +
                "\n" +
                "\t\tif(x<5){\n" +
                "\t\t\tprintln(\"x<5\");\n" +
                "\t\t}\n" +
                "\n" +
                "\t\treturn 0;\n" +
                "\t}\n" +
                "}\n");
        //System.out.println(tokens.toString());

        Parser parser = new Parser();
        AST ast = parser.parseTestMode(tokens, true);
    }
}
