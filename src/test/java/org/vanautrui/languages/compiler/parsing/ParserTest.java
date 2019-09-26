package org.vanautrui.languages.compiler.parsing;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;

import java.util.stream.Collectors;

public class ParserTest {

    Parser parser = new Parser();

    @Test
    public void test_can_create_correct_classnode() throws Exception {
        Lexer lexer=new Lexer();
        TokenList tokens = lexer.lexCodeTestMode(
                "public class ExampleClass{" +
                        "}");

        AST ast = parser.parseTestMode(tokens,false);

        Assert.assertEquals("ExampleClass", ast.classNodeList.stream().collect(Collectors.toList()).get(0).name.getTypeName());
    }

    @Test
    public void test_can_parse_some_class()throws Exception{
        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode("class MyOutput{" +
                "()~>PInt main{" +
                "println(1);" +
                "return 0;" +
                "}" +
                "}");


        Parser parser = new Parser();
        AST ast = parser.parseTestMode(tokens, false);
    }

    @Test
    public void test_can_parse_some_class2()throws Exception{
        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode("public class IfStatement{\n" +
                "\tpublic ()~>PInt main{\n" +
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
        AST ast = parser.parseTestMode(tokens, false);
    }
}
