package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.KeywordToken;
import org.vanautrui.languages.compiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;

import static org.junit.Assert.assertEquals;

public class IfStatementNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new KeywordToken("if"));

        list.add(new SymbolToken("("));

        list.add(new IntegerNonNegativeConstantToken(5));
        list.add(new OperatorToken("<"));
        list.add(new IntegerNonNegativeConstantToken(3));

        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        IfStatementNode ifStatement = new IfStatementNode(list);
        assertEquals(0,list.size());
    }

    @Test
    public void test2()throws Exception{
        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode(
                "\t\tif(x<5){\n" +
                "\t\t\tprintln(\"x<5\");\n" +
                "\t\t}\n"
                );
        //System.out.println(tokens.toString());

        Parser parser = new Parser();
        IfStatementNode stmt = new IfStatementNode(tokens);

    }
}
