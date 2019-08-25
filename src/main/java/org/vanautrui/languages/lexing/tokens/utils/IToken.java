package org.vanautrui.languages.lexing.tokens.utils;

import java.awt.*;

public interface IToken {

    public String getContents();

    //for syntax highlighting and such
    public Color getDisplayColor();

    //for error reporting,
    //get the line in the source code where the token originates
    //public long getOriginalLineNumber();

    public default boolean tokenEquals(IToken other){
        boolean b1= this.getClass().getName().equals(other.getClass().getName());
        boolean b2=this.getContents().equals(other.getContents());
        return b1 && b2;
    }
}
