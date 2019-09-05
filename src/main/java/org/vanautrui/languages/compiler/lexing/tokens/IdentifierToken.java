package org.vanautrui.languages.compiler.lexing.tokens;

import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import com.fasterxml.jackson.annotation.*;

import java.nio.file.Paths;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class IdentifierToken implements IToken {

    //it should start with a lowercase letter,
    //to differentiate between variables, subroutines, and types
    public static final String regex_alphanumeric_identifier = "^[a-z][a-zA-Z0-9_]*";

    public static final int MAX_IDENTIFIER_LENGTH = 100;

    public String content;

    public IdentifierToken(CharacterList list) throws Exception {
        super();
        CharacterList copy = new CharacterList(list);


        Pattern p = Pattern.compile(regex_alphanumeric_identifier);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_IDENTIFIER_LENGTH));

        if (m.find()) {

            this.content = m.group(0);

            try{
                new KeywordToken(this.content);
                throw new Exception("identifier token should not parse as keyword token");
            }catch (Exception e){
                //pass
            }

            list.consumeTokens(this.content.length());
        } else {
            throw new Exception("could not recognize identifier");
        }

    }

    public IdentifierToken(String newcontents) throws Exception {
        this(new CharacterList(newcontents, Paths.get("/dev/null")));
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return this.content;
    }

}
