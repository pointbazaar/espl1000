package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.lexing.collections.*;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.*;
import org.vanautrui.languages.parsing.astnodes.terminal.*;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class ClassNode implements IASTNode {

    public boolean isPublic;

    //for xml
    public String getType(){
        return this.name.typeName;
    }

    public TypeIdentifierNode name;
	public List<CompilerDirectiveNode> directiveNodeList = new ArrayList<>();
    public List<ClassFieldNode> fieldNodeList = new ArrayList<>();
    public List<MethodNode> methodNodeList = new ArrayList<>();

    public ClassNode(TokenList tokens) throws Exception {
        //System.out.println("try parse DragonClassNode");

        TokenList copy = tokens.copy();

        //System.out.println(copy.toString());

        try {
            AccessModifierNode access = new AccessModifierNode(copy);
            this.isPublic=access.is_public;
        }catch (Exception e){
            //no access modifier node. then it is public by default
            this.isPublic=true;
        }

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("class"));

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

		boolean directiveSuccess=true;
		while(directiveSuccess){
			try{
				directiveNodeList.add(new CompilerDirectiveNode(copy));
			}catch (Exception e){
				directiveSuccess=false;
			}
		}


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

        result+=((this.isPublic)?" public":" private")+" class "+this.name.toSourceCode()+" {";

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
