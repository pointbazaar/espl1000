package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.tokens.utils.BasicToken;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import com.fasterxml.jackson.annotation.*;
import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class IntegerNonNegativeConstantToken extends BasicToken implements DragonToken {

    private static final String regex_integer_constant = "^(0|[1-9][0-9]*)";

    //this class should represent a nonnegative token.
    //the AST node later can be negative, depending on circumstance

    private static final int MAX_INT_CONSTANT_LENGTH = 20;

    public int value;

    public IntegerNonNegativeConstantToken(CharacterList list) throws Exception {
        super(list.getCurrentLine());
        Pattern p = Pattern.compile(regex_integer_constant);

        Matcher m = p.matcher(list.getLimitedStringMaybeShorter(MAX_INT_CONSTANT_LENGTH));

        if (m.find()) {
            this.value = Integer.parseInt(m.group(0));
            list.consumeTokens((this.value+"").length());
        } else {
            throw new Exception("could not recognize integer");
        }
    }

    public IntegerNonNegativeConstantToken(int newcontents) throws Exception {
        super(-1);
        this.value = newcontents;
    }

    @Override
    public String getContents() {
        return this.value+"";
    }

    @Override
	@JsonIgnore
    public Color getDisplayColor() {
        return Color.GREEN;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof IntegerNonNegativeConstantToken)) return false;
        IntegerNonNegativeConstantToken that = (IntegerNonNegativeConstantToken) o;
        return value==that.value;
    }

}
