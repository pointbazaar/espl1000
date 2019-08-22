package org.vanautrui.languages.lexing.tokens.utils;

import java.awt.*;

public interface Token {

    public String getContents();

    //for syntax highlighting and such
    public Color getDisplayColor();

    //for error reporting,
    //get the line in the source code where the token originates
    public long getOriginalLineNumber();
}
