package org.vanautrui.languages.model.tokens;

public class IdentifierToken implements DragonToken {

    private String content;

    public IdentifierToken(String newcontents) throws Exception {
        this.content=newcontents;
    }

    @Override
    public String getContents() {
        return this.content;
    }
}
