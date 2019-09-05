package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminals.statements;

import org.junit.Test;
import org.vanautrui.languages.dragoncompiler.lexing.utils.CharacterList;
import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.*;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;

public class AssignmentStatementNodeTest {

    @Test
    public void test1()throws Exception{

        TokenList tokens = new TokenList();

        tokens.add(new IdentifierToken("x"));
        tokens.add(new OperatorToken("="));
        tokens.add(new IntegerNonNegativeConstantToken(4));
        tokens.add(new SymbolToken(";"));

        AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
    }

    @Test
    public void test_assign_method_call_result()throws Exception{

        TokenList tokens = new TokenList();

        tokens.add(new IdentifierToken("x"));
        tokens.add(new OperatorToken("="));

        //nop refers to the no operation method. it does nothing with its argument
        tokens.add(new IdentifierToken("nop"));

        tokens.add(new SymbolToken("("));
        tokens.add(new IntegerNonNegativeConstantToken(4));
        tokens.add(new SymbolToken(")"));

        tokens.add(new SymbolToken(";"));

        AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
    }

    @Test
    public void test_assign_method_call_result_2()throws Exception{

        TokenList tokens = new TokenList();

        tokens.add(new IdentifierToken("x"));
        tokens.add(new OperatorToken("="));

        //nop refers to the no operation method. it does nothing with its argument
        tokens.add(new IdentifierToken("nop"));

        tokens.add(new SymbolToken("("));
        tokens.add(new SymbolToken(")"));

        tokens.add(new SymbolToken(";"));

        AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
    }

    @Test
    public void test_assign_variable_with_array_index()throws Exception{

        TokenList tokens = new TokenList();

        tokens.add(new IdentifierToken("x"));
        tokens.add(new OperatorToken("="));

        //nop refers to the no operation method. it does nothing with its argument
        tokens.add(new IdentifierToken("arr"));

        tokens.add(new SymbolToken("["));
        tokens.add(new IntegerNonNegativeConstantToken(4));
        tokens.add(new SymbolToken("]"));

        tokens.add(new SymbolToken(";"));

        AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
    }

    @Test
    public void test_assign_char()throws Exception{

        TokenList tokens = new TokenList();

        tokens.add(new IdentifierToken("x"));
        tokens.add(new OperatorToken("="));

        tokens.add(new CharConstantToken(new CharacterList("'x'")));

        tokens.add(new SymbolToken(";"));

        AssignmentStatementNode assignmentStatementNode = new AssignmentStatementNode(tokens);
    }
}
