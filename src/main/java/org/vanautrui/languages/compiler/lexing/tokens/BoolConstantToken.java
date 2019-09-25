package org.vanautrui.languages.compiler.lexing.tokens;

import org.vanautrui.languages.compiler.lexing.utils.*;
import com.fasterxml.jackson.annotation.*;

public class BoolConstantToken implements IToken {

	//this class represents 'true' || 'false'

    public boolean value;
    private final long lineNumber;

    @Override
    public long getLineNumber() {
        return this.lineNumber;
    }

    public BoolConstantToken(CharacterList list) throws Exception {
        this.lineNumber=list.getCurrentLineNumber();

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

}
