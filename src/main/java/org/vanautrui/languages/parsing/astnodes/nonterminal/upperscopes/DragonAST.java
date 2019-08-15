package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import java.util.*;
import java.util.stream.Collectors;

public class DragonAST implements IDragonASTNode {

    public Set<DragonClassNode> classNodeList = new HashSet<>();

    public DragonAST(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonAST");

        //List<DragonToken> copy = new ArrayList<>(tokens);


        boolean success_class = true;
        while (success_class) {

            try {
                this.classNodeList.add(new DragonClassNode(tokens));
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

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {

        for(DragonClassNode classNode : this.classNodeList){
            classNode.doTypeCheck(asts,Optional.of(classNode),Optional.empty());
        }
    }
}
