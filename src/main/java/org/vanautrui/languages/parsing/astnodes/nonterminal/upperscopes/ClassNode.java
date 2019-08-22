package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.ElementList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.ClassToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.AccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.TypeIdentifierNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class ClassNode implements IASTNode {

    //test simplexml
    @Attribute
    public String getAccess(){
        return this.access.toSourceCode();
    }

    public AccessModifierNode access;

    //for xml
    @Attribute
    public String getType(){
        return this.name.typeName;
    }

    public TypeIdentifierNode name;

    @ElementList
    public List<ClassFieldNode> fieldNodeList = new ArrayList<>();

    @ElementList
    public List<MethodNode> methodNodeList = new ArrayList<>();

    public ClassNode(TokenList tokens) throws Exception {
        //System.out.println("try parse DragonClassNode");

        TokenList copy = tokens.copy();

        //System.out.println(copy.toString());

        this.access = new AccessModifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new ClassToken());

        this.name = new TypeIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        if(!copy.endsWith(new SymbolToken("}"))){
            //we assume the class only received a token stream with only the relavant info for it
            throw new Exception(" a class should end with '}' but it did not ");
        }

        //i hope, that with this piece of code, the method should always be tried out first
        //because classField is a prefix of Method.
        //similar errors could maybe be fixed by just looking at the Dragon Grammar
        //and structuring the parser accordingly



        boolean success_method = true;
        boolean success_field = true;

        while (success_field || success_method) {
            try {
                this.methodNodeList.add(new MethodNode(copy));
                success_method = true;
            } catch (Exception e11) {
                success_method = false;
            }

            try {
                this.fieldNodeList.add(new ClassFieldNode(copy));
                success_field = true;
            } catch (Exception e22) {
                success_field = false;
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
}
