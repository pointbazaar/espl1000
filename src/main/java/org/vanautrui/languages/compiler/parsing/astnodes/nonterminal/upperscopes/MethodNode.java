package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.MethodNameNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.AccessModifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.TypeIdentifierNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class MethodNode implements IASTNode {

    public final boolean isPublic;

    public final String type;

    public final String methodName;

    public List<DeclaredArgumentNode> arguments = new ArrayList<>();

    public List<StatementNode> statements = new ArrayList<>();

    public MethodNode(TokenList tokens,boolean debug) throws Exception {
        if(debug){
            System.out.println("try to parse "+this.getClass().getSimpleName()+" from '"+tokens.toSourceCodeFragment()+"'");
        }
        //TODO: consider the 2 alternative ways a method can be declared

        TokenList copy = tokens.copy();

        AccessModifierNode access = new AccessModifierNode(copy);
        this.isPublic=access.is_public;

        this.type = new TypeIdentifierNode(copy).typeName;

        this.methodName = new MethodNameNode(copy).methodName;

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
                +this.type
                +" "
                +this.methodName
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
