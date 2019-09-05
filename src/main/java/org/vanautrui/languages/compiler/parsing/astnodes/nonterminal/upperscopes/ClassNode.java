package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.lexing.tokens.KeywordToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.AccessModifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.TypeIdentifierNode;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class ClassNode implements IASTNode {

    public boolean isPublic;
    public Path srcPath= Paths.get("/dev/null");

    public TypeIdentifierNode name;

    public List<ClassFieldNode> fieldNodeList = new ArrayList<>();
    public List<MethodNode> methodNodeList = new ArrayList<>();

    public ClassNode(TokenList tokens,Path path) throws Exception {

        this.srcPath=path;
        TokenList copy = tokens.copy();

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
