
package org.vanautrui.languages.model.tokens;

public class DragonToken {
    private String contents;

    public DragonToken(String newcontents) throws Exception{
        if(newcontents==null || newcontents.isEmpty()){
            throw new Exception("Trying to create empty token");
        }
        this.contents=newcontents;
    }

    public String getContents(){
        return new String(contents.getBytes());
    }
}
