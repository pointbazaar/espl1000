package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.IASTNode;
import java.util.*;
import java.util.stream.Collectors;

public class AST implements IASTNode {

    public Set<ClassNode> classNodeList = new HashSet<>();

    public AST(TokenList tokens) throws Exception {
        //System.out.println("try parse DragonAST");

        //List<DragonToken> copy = new ArrayList<>(tokens);


        boolean success_class = true;
        while (success_class) {

            try {
                this.classNodeList.add(new ClassNode(tokens));
            } catch (Exception e) {

                if (this.classNodeList.size() == 0) {
                    throw e;
                }
                success_class = false;
            }
        }
    }

    @Override
    public String toSourceCode() {
        return this.classNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));
    }
}
