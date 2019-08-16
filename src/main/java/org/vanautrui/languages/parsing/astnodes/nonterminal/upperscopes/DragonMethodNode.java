package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.ElementList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonDeclaredArgumentNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodNameNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeIdentifierNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

public class DragonMethodNode implements IDragonASTNode {

    //for xml
    @Attribute
    public String getAccess(){
        return this.access.toSourceCode();
    }

    public DragonAccessModifierNode access;

    //for xml
    @Attribute
    public String getType(){
        return this.type.typeName.getContents();
    }

    public DragonTypeIdentifierNode type;

    //for xml
    @Attribute
    public String getName(){
        return this.methodName.methodName.name.getContents();
    }

    public DragonMethodNameNode methodName;

    @ElementList
    public List<DragonDeclaredArgumentNode> arguments = new ArrayList<>();

    @ElementList
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
