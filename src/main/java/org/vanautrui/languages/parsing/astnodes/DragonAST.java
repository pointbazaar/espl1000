
package org.vanautrui.languages.parsing.astnodes;


import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class DragonAST implements IDragonASTNode {

    public List<DragonClassNode> classNodeList=new ArrayList<>();

    @Override
    public String toSourceCode() {
        return this.classNodeList.stream().map(node->node.toSourceCode()).collect(Collectors.joining(" "));
    }
}
