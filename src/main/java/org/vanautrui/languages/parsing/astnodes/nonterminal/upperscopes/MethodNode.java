package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.ElementList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.parsing.astnodes.terminal.MethodNameNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.terminal.AccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.TypeIdentifierNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class MethodNode implements IASTNode {

    public boolean isPublic;

    public TypeIdentifierNode type;

    public MethodNameNode methodName;

    public List<DeclaredArgumentNode> arguments = new ArrayList<>();

    public List<StatementNode> statements = new ArrayList<>();

    public MethodNode(TokenList tokens) throws Exception {

        //TODO: consider the 2 alternative ways a method can be declared

        TokenList copy = tokens.copy();

        try {
            AccessModifierNode access = new AccessModifierNode(copy);
            this.isPublic=access.is_public;
        }catch (Exception e){
            //access doesnt have to be declared, the default is public
            this.isPublic=true;
        }

        this.type = new TypeIdentifierNode(copy);

        this.methodName = new MethodNameNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        // parse arguments

        boolean success_argument = true;
        try {
            this.arguments.add(new DeclaredArgumentNode(copy));
        } catch (Exception e) {
            success_argument=false;
        }
        while (success_argument) {
            try {
                copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
                this.arguments.add(new DeclaredArgumentNode(copy));
            } catch (Exception e) {
                success_argument = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        boolean success_statements = true;
        while (success_statements) {
            try {
                this.statements.add(new StatementNode(copy));
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
                ((this.isPublic)?" public":" private")
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
