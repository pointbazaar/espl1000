package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeIdentifierNode;

import javax.swing.text.html.Option;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

public class DragonMethodNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeIdentifierNode type;

    public DragonMethodNameNode methodName;

    public List<DragonDeclaredArgumentNode> arguments = new ArrayList<>();

    public List<DragonStatementNode> statements = new ArrayList<>();

    public DragonMethodNode(DragonTokenList tokens) throws Exception {

        //System.out.println("try parse DragonMethodNode");

        //TODO: consider the 2 alternative ways a method can be declared

        DragonTokenList copy = tokens.copy();

        this.access = new DragonAccessModifierNode(copy);

        this.type = new DragonTypeIdentifierNode(copy);

        this.methodName = new DragonMethodNameNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        // parse arguments

        boolean success_argument = true;
        try {
            this.arguments.add(new DragonDeclaredArgumentNode(copy));
        } catch (Exception e) {
            success_argument=false;
        }
        while (success_argument) {
            try {
                copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
                this.arguments.add(new DragonDeclaredArgumentNode(copy));
            } catch (Exception e) {
                success_argument = false;
            }
        }

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

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {

        this.methodName.doTypeCheck(asts,currentClass,Optional.of(this));
        this.type.doTypeCheck(asts,currentClass,Optional.of(this));
        for(DragonStatementNode stmt : this.statements){
            stmt.doTypeCheck(asts,currentClass, Optional.of(this));
        }
        for(DragonDeclaredArgumentNode arg : this.arguments){
            arg.doTypeCheck(asts,currentClass,Optional.of(this));
        }
    }
}
