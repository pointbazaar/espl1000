package org.vanautrui.languages.model.astnodes;

import java.util.List;

public class AmandaClassNode {

    public List<AmandaClassFieldNode> fieldNodeList;

    public List<AmandaMethodNode> methodNodeList;

    private String name;

    public AmandaClassNode(String name){
        this.name=name;
    }

    public String getName(){
        return this.name;
    }
}
