package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.*;
import org.vanautrui.languages.lexing.tokens.utils.*;
import com.fasterxml.jackson.annotation.*;
import java.awt.*;

public class BoolConstantToken extends BasicToken implements Token {

	//this class represents 'true' || 'false'

    public boolean value;

    public BoolConstantToken(CharacterList list) throws Exception {
        super(list.getCurrentLine());

        if (list.startsWith("true")) {
            this.value = true;
            list.consumeTokens((this.value+"").length());
		}else if(list.startsWith("false")){
			this.value = false;
            list.consumeTokens((this.value+"").length());
        } else {
            throw new Exception("could not recognize boolean constant");
        }
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
        if (!(o instanceof BoolConstantToken)) return false;
        BoolConstantToken that = (BoolConstantToken) o;
        return value==that.value;
    }

}
