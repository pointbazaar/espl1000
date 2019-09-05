package org.vanautrui.languages.compiler.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class FloatNonNegativeConstantToken implements IToken {

    private static final String regex_float_constant = "^(0|[1-9][0-9]*)\\.([0-9]+)";

    //this class should represent a nonnegative token.
    //the AST node later can be negative, depending on circumstance

    private static final int MAX_FLOAT_CONSTANT_LENGTH = 20;

    public float value;

    public FloatNonNegativeConstantToken(CharacterList list) throws Exception {
        super();
        Pattern p = Pattern.compile(regex_float_constant);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_FLOAT_CONSTANT_LENGTH));

        if (m.find()) {
            this.value = Float.parseFloat(m.group());
            list.consumeTokens((this.value+"").length());
        } else {
            throw new Exception("could not recognize float");
        }
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return this.value+"";
    }

}
