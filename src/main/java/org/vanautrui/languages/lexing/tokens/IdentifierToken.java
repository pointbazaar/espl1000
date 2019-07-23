package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class IdentifierToken implements DragonToken {

    public static final String regex_alphanumeric_identifier = "^[a-zA-Z][a-zA-Z0-9]+";

    public static final int MAX_IDENTIFIER_LENGTH=100;

    private String content;

    public IdentifierToken(CharacterList list)throws Exception{

        Pattern p = Pattern.compile(regex_alphanumeric_identifier);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_IDENTIFIER_LENGTH));

        if(m.find()){
            this.content = m.group(0);
            list.consumeTokens(this.content.length());
        }else{
            throw new Exception("could not recognize identifier");
        }
    }

    public IdentifierToken(String newcontents) throws Exception {
        this.content=newcontents;
    }

    @Override
    public String getContents() {
        return this.content;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof IdentifierToken)) return false;
        IdentifierToken that = (IdentifierToken) o;
        return content.equals(that.content);
    }

}
