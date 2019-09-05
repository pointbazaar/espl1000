package org.vanautrui.languages.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.IToken;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CharConstantToken implements IToken {

    private static final String regex_char_constant = "^'.'";

    public char content;

    public CharConstantToken(CharacterList list) throws Exception {
        super();
        Pattern p = Pattern.compile(regex_char_constant);

        Matcher m = p.matcher(list.getLimitedString(3));

        if (m.find()) {
            String finding = m.group(0);
            this.content = finding.charAt(1);
            list.consumeTokens(finding.length());
        } else {
            throw new Exception("could not recognize char constant token");
        }
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return this.content+"";
    }


}
