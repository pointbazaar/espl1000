package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.BasicToken;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;

import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class FloatNonNegativeConstantToken extends BasicToken implements DragonToken {

    private static final String regex_integer_constant = "^(0|[1-9]+)\\.([0-9]*)";

    //this class should represent a nonnegative token.
    //the AST node later can be negative, depending on circumstance

    private static final int MAX_FLOAT_CONSTANT_LENGTH = 20;

    public float value;

    public FloatNonNegativeConstantToken(CharacterList list) throws Exception {
        super(list.getCurrentLine());
        Pattern p = Pattern.compile(regex_integer_constant);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_FLOAT_CONSTANT_LENGTH));

        if (m.find()) {
            this.value = Float.parseFloat(m.group(0));
            list.consumeTokens((this.value+"").length());
        } else {
            throw new Exception("could not recognize float");
        }
    }

    @Override
    public String getContents() {
        return this.value+"";
    }

    @Override
    public Color getDisplayColor() {
        return Color.GREEN;
    }

}
