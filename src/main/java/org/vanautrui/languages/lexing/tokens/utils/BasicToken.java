package org.vanautrui.languages.lexing.tokens.utils;

public abstract class BasicToken implements DragonToken {

    private long lineNumber;

    public BasicToken(long lineNumber){
        this.lineNumber=lineNumber;
    }

    public long getOriginalLineNumber(){
        return this.lineNumber;
    }
}
