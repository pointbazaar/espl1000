package org.vanautrui.languages.compiler.lexing.tokens;

import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import com.fasterxml.jackson.annotation.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class StringConstantToken implements IToken {

    private static final String regex_string_constant = "^\"[^\"]*\"";

    public static final int MAX_STRING_CONSTANT_LENGTH = 1000;

    public final String content;
    private final long lineNumber;

    @Override
    public long getLineNumber() {
        return this.lineNumber;
    }

    public StringConstantToken(CharacterList list) throws Exception {
        this.lineNumber=list.getCurrentLineNumber();
        Pattern p = Pattern.compile(regex_string_constant);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_STRING_CONSTANT_LENGTH));

        if (m.find()) {
            String finding = m.group(0);
            this.content = finding.substring(1,finding.length()-1);
            list.consumeTokens(finding.length());
        } else {
            throw new Exception("could not recognize identifier");
        }
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return this.content;
    }


}
