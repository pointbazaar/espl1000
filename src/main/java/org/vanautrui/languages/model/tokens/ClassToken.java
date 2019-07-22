package org.vanautrui.languages.model.tokens;

public class ClassToken implements DragonToken {

    private String content;

    public ClassToken(String newcontents) throws Exception {
        this.content=newcontents;
    }

    @Override
    public String getContents() {
        return this.content;
    }
}
