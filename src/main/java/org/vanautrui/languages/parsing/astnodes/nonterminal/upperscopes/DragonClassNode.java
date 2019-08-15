package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.ElementList;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.ClassToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeIdentifierNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

public class DragonClassNode implements IDragonASTNode {

    //test simplexml
    @Attribute
    public String getAccess(){
        return this.access.toSourceCode();
    }

    public DragonAccessModifierNode access;

    //for xml
    @Attribute
    public String getType(){
        return this.name.typeName.getContents();
    }

    public DragonTypeIdentifierNode name;

    @ElementList
    public List<DragonClassFieldNode> fieldNodeList = new ArrayList<>();

    @ElementList
    public List<DragonMethodNode> methodNodeList = new ArrayList<>();

    public DragonClassNode(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonClassNode");

        DragonTokenList copy = tokens.copy();

        //System.out.println(copy.toString());

        this.access = new DragonAccessModifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new ClassToken());

        this.name = new DragonTypeIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        //i hope, that with this piece of code, the method should always be tried out first
        //because classField is a prefix of Method.
        //similar errors could maybe be fixed by just looking at the Dragon Grammar
        //and structuring the parser accordingly



        boolean success_method = true;
        boolean success_field = true;
        Optional<Exception> e1=Optional.empty();
        Optional<Exception> e2=Optional.empty();
        while (success_field || success_method) {
            try {
                this.methodNodeList.add(new DragonMethodNode(copy));
                e1=Optional.empty();
                success_method = true;
            } catch (Exception e11) {
                success_method = false;
                e1=Optional.of(e11);
            }

            try {
                this.fieldNodeList.add(new DragonClassFieldNode(copy));
                e2=Optional.empty();
                success_field = true;
            } catch (Exception e22) {
                success_field = false;
                e2=Optional.of(e22);
            }

        }

        //System.out.println(copy.toString());
        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }


    @Override
    public String toSourceCode() {
        String result = "";

        result+=this.access.toSourceCode()+" class "+this.name.toSourceCode()+" {";

        result += fieldNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));
        result+="\n";

        result += methodNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));

        result+="\n";
        result+="}";
        return result;
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {

        int count=0;
        for(DragonAST ast : asts){
            for(DragonClassNode classNode : ast.classNodeList){
                if(classNode.name.typeName.getContents().equals(this.name.typeName.getContents())){
                    count++;
                }
            }
        }

        //check that fields and methods are typesafe

        for(DragonClassFieldNode fieldNode : this.fieldNodeList){
            fieldNode.doTypeCheck(asts,Optional.of(this),Optional.empty());
        }

        for(DragonMethodNode methodNode : this.methodNodeList){
            methodNode.doTypeCheck(asts,Optional.of(this),Optional.empty());
        }

        if(count!=1){
            throw new Exception("multiple definitions of class '"+this.name+"'");
        }
    }
}
