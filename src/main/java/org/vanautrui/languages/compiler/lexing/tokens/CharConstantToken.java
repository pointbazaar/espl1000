package org.vanautrui.languages.compiler.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

public class CharConstantToken implements IToken {

    private static final String regex_char_constant = "^'(.|\\\\.)'";
    //public static final Pattern p = Pattern.compile(regex_char_constant);

    public char content;
    private final long lineNumber;

    @Override
    public long getLineNumber() {
        return this.lineNumber;
    }

    public CharConstantToken(char c){
        this.content=c;
        this.lineNumber=-1;
    }

    public CharConstantToken(CharacterList list) throws Exception {
        this.lineNumber=list.getCurrentLineNumber();

        String s = list.getLimitedString(3);
        char c1=s.charAt(0);
        char c2=s.charAt(1);
        char c3=s.charAt(2);

        boolean case_normal = c1=='\'' && c3=='\'';

        if(case_normal){

            this.content = s.charAt(1);
            list.consumeTokens(3);
            return;
        }

        s=list.getLimitedString(4);
        char c4=s.charAt(3);
        boolean case_newline = c1=='\'' && c2=='\\' && c4=='\'' && c3=='n';
        if(case_newline){
            this.content = s.charAt(1);
            list.consumeTokens(4);
            return;
        }

        throw new Exception("could not recognize char constant token");
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return this.content+"";
    }


}
