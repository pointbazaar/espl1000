package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;

import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class IntegerConstantToken implements DragonToken {

    private static final String regex_integer_constant = "^[1-9][0-9]*";

    private static final int MAX_INT_CONSTANT_LENGTH = 20;

    public long value;

    public IntegerConstantToken(CharacterList list) throws Exception {

        Pattern p = Pattern.compile(regex_integer_constant);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_INT_CONSTANT_LENGTH));

        if (m.find()) {
            this.value = Long.parseLong(m.group(0));
            list.consumeTokens((this.value+"").length());
        } else {
            throw new Exception("could not recognize integer");
        }
    }

    public IntegerConstantToken(int newcontents) throws Exception {
        this.value = newcontents;
    }

    @Override
    public String getContents() {
        return this.value+"";
    }

    @Override
    public Color getDisplayColor() {
        return Color.GREEN;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof IntegerConstantToken)) return false;
        IntegerConstantToken that = (IntegerConstantToken) o;
        return value==that.value;
    }

}
