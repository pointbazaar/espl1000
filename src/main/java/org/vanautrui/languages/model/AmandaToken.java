
package org.vanautrui.languages.model;

public class AmandaToken {
    private String contents;

    public AmandaToken(String newcontents) throws Exception{
        if(newcontents==null || newcontents.isEmpty()){
            throw new Exception("Trying to create empty token");
        }
        this.contents=newcontents;
    }

    public String getContents(){
        return new String(contents.getBytes());
    }
}
