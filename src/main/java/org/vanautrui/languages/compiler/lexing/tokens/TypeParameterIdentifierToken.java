package org.vanautrui.languages.compiler.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TypeParameterIdentifierToken implements IToken {

    //for now, maximum of 10 type parameters are allowed (should be plenty)
    public static final String regex_type_parameter_identifier = "^?T[0-9]";

    public static final int MAX_IDENTIFIER_LENGTH = 5;

    public final int type_parameter_number;
    private final long lineNumber;

    @Override
    public long getLineNumber() {
        return this.lineNumber;
    }

    public TypeParameterIdentifierToken(CharacterList list) throws Exception {
        this.lineNumber=list.getCurrentLineNumber();
        CharacterList copy = new CharacterList(list);
        try{
            new KeywordToken(copy);
            throw new Exception("type parameter identifier token should not parse as keyword token");
        }catch (Exception e){
            //pass
        }

        Pattern pNormal = Pattern.compile(regex_type_parameter_identifier);

        Matcher m = pNormal.matcher(list.getLimitedStringMaybeShorter(MAX_IDENTIFIER_LENGTH));

        if (m.find()) {
            //finds just the number
            this.type_parameter_number = Integer.parseInt(m.group(0).substring(2));
            list.consumeTokens(m.group(0).length());
        } else {
            throw new Exception("could not recognize type identifier");
        }
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return "?T"+this.type_parameter_number;
    }

}
