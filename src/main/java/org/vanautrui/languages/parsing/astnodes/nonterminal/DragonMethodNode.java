package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class DragonMethodNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeNode type;

    public DragonMethodNameNode methodName;

    public List<DragonDeclaredArgumentNode> arguments = new ArrayList<>();

    public List<DragonStatementNode> statements = new ArrayList<>();

    public DragonMethodNode(DragonTokenList tokens) throws Exception {

        //System.out.println("try parse DragonMethodNode");

        //TODO: consider the 2 alternative ways a method can be declored

        DragonTokenList copy = tokens.copy();

        this.access = new DragonAccessModifierNode(copy);

        this.type = new DragonTypeNode(copy);

        this.methodName = new DragonMethodNameNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        //TODO: parse arguments

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        //TODO: parse statements

        boolean success_statements = true;
        while (success_statements) {
            try {
                this.statements.add(new DragonStatementNode(copy));
            } catch (Exception e) {
                success_statements = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {

        return
                this.access.toSourceCode()
                +" "
                +this.type.toSourceCode()
                +" "
                +this.methodName.toSourceCode()
                +"("
                +this.arguments.stream()
                .map(argument->argument.toSourceCode())
                .collect(Collectors.joining(","))
                +")"
                +"{"
                +"\n"
                +this.statements.stream()
                .map(statement->statement.toSourceCode())
                .collect(Collectors.joining(";\n"))
                +"\n"
                +"}"
        ;
    }
}
