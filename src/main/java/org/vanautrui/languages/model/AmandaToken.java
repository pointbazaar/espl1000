
package org.vanautrui.languages.model;

public class AmandaToken {
    public String contents;

    public AmandaToken(String newcontents) throws Exception{
        if(newcontents==null || newcontents.isEmpty()){
            throw new Exception("Trying to create empty token");
        }
        this.contents=newcontents;
    }
}
