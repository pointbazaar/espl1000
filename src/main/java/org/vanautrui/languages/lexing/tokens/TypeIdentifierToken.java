package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TypeIdentifierToken implements DragonToken {

    //it should start with a lowercase letter,
    //to differentiate between variables, subroutines, and types
    public static final String regex_alphanumeric_type_identifier = "^[A-Z][a-zA-Z0-9_]*";

    public static final int MAX_IDENTIFIER_LENGTH = 100;

    private String content;

    public TypeIdentifierToken(CharacterList list) throws Exception {

        CharacterList copy = new CharacterList(list);
        try{
            new KeywordToken(copy);
            throw new Exception("type identifier token should not parse as keyword token");
        }catch (Exception e){
            //pass
        }

        Pattern p = Pattern.compile(regex_alphanumeric_type_identifier);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_IDENTIFIER_LENGTH));

        if (m.find()) {
            this.content = m.group(0);
            list.consumeTokens(this.content.length());
        } else {
            throw new Exception("could not recognize type identifier");
        }
    }

    public TypeIdentifierToken(String str) throws Exception{
        this(new CharacterList(str));
    }

    @Override
    public String getContents() {
        return this.content;
    }

    @Override
    public Color getDisplayColor() {
        return Color.YELLOW;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof TypeIdentifierToken)) return false;
        TypeIdentifierToken that = (TypeIdentifierToken) o;
        return content.equals(that.content);
    }

}
