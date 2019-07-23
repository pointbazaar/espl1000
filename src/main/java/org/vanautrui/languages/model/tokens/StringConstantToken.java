package org.vanautrui.languages.model.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class StringConstantToken implements DragonToken {

    private static final String regex_string_constant = "^\".*\"";

    public static final int MAX_STRING_CONSTANT_LENGTH =100;

    private String content;

    public StringConstantToken(CharacterList list)throws Exception{

        Pattern p = Pattern.compile(regex_string_constant);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_STRING_CONSTANT_LENGTH));

        if(m.find()){
            this.content = m.group(0);
            list.consumeTokens(this.content.length());
        }else{
            throw new Exception("could not recognize identifier");
        }
    }

    public StringConstantToken(String newcontents) throws Exception {
        this.content=newcontents;
    }

    @Override
    public String getContents() {
        return this.content;
    }
}
