package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.apache.commons.lang3.tuple.Pair;
import org.vanautrui.languages.compiler.lexing.tokens.KeywordToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.AccessModifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.simple.SimpleTypeNode;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class NamespaceNode implements IASTNode {

    public final boolean isPublic;
    public Path srcPath= Paths.get("/dev/null");

    public final SimpleTypeNode name;

    //structs must be declared before the subroutines
    public List<StructDeclNode> structDeclNodeList = new ArrayList<>();

    public List<MethodNode> methodNodeList = new ArrayList<>();

    public NamespaceNode(TokenList tokens, Path path, boolean debug) throws Exception {

        if(debug){
            System.out.println("try to parse "+this.getClass().getSimpleName()+" from "+tokens.toSourceCodeFragment());
        }

        this.srcPath=path;
        TokenList copy = tokens.copy();

        AccessModifierNode access = new AccessModifierNode(copy);
        this.isPublic=access.is_public;

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("namespace"));

        this.name = new SimpleTypeNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        if(!copy.endsWith(new SymbolToken("}"))){
            //we assume the class only received a token stream with only the relavant info for it
            throw new Exception(" a namespace should end with '}' but it did not ");
        }

        boolean success_struct = true;
        while(success_struct){
            try{
                this.structDeclNodeList.add(new StructDeclNode(copy,debug));
            }catch (Exception e){
                success_struct=false;
            }
        }

        boolean success_method = true;
        Pair<TokenList, TokenList> pair = copy.split_into_tokens_and_next_block_and_later_tokens();
        while (success_method){

            if(debug) {
                System.out.println("pair:");
                System.out.println(pair.getLeft().toSourceCodeFragment());
                System.out.println(pair.getRight().toSourceCodeFragment());
            }
            try {
                int chunk_size=pair.getLeft().size();
                this.methodNodeList.add(new MethodNode(pair.getLeft(),debug));
                copy.consume(chunk_size);
                pair = pair.getRight().split_into_tokens_and_next_block_and_later_tokens();
            } catch (Exception e11) {
                success_method = false;
            }
        }

        //DEBUG:
        //System.out.println("tokens left after try to parse fields and methods:");
        //System.out.println(copy.toSourceCodeFragment());

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }


    @Override
    public String toSourceCode() {
        String result = "";

        result+=((this.isPublic)?" public":" private")+" namespace "+this.name.toSourceCode()+" {";

        result += methodNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));

        result+="\n";
        result+="}";
        return result;
    }
}
