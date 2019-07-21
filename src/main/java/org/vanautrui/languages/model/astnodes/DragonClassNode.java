package org.vanautrui.languages.model.astnodes;

import java.util.List;

public class DragonClassNode {

    public List<DragonClassFieldNode> fieldNodeList;

    public List<DragonMethodNode> methodNodeList;

    private String name;

    public DragonClassNode(String name){
        this.name=name;
    }

    public String getName(){
        return this.name;
    }
}
