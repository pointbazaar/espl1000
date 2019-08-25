package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.collections.*;
import org.vanautrui.languages.lexing.tokens.utils.*;
import com.fasterxml.jackson.annotation.*;
import java.awt.*;

public class BoolConstantToken implements IToken {

	//this class represents 'true' || 'false'

    public boolean value;

    public BoolConstantToken(CharacterList list) throws Exception {
        super();

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
    @JsonIgnore
    public String getContents() {
        return this.value+"";
    }

    @Override
	@JsonIgnore
    public Color getDisplayColor() {
        return Color.GREEN;
    }

}
