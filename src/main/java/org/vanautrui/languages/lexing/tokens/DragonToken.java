package org.vanautrui.languages.lexing.tokens;

import java.awt.*;

public interface DragonToken {

    public String getContents();

    //for syntax highlighting and such
    public Color getDisplayColor();
}
