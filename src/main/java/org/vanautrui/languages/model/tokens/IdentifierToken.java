package org.vanautrui.languages.model.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class IdentifierToken implements DragonToken {

    private static final String regex_alphanumeric_identifier = "^[a-zA-Z][a-zA-Z0-9]+";

    private String content;

    public IdentifierToken(CharacterList list)throws Exception{

        Pattern p = Pattern.compile(regex_alphanumeric_identifier);

        Matcher m = p.matcher(list.toString());

        if(m.matches()){
            this.content = m.group();
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
}
