package org.vanautrui.languages.compiler.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

public class AccessModifierToken implements IToken {

    public final boolean is_public;    //TODO: Type/Field/Subroutine can be used everywhere
    public final boolean is_private;   //TODO: Field/Subroutine can only be used within the class
    public final boolean is_dir_private; //TODO: Field/Subroutine/Type can only be seen,constructed, returned and in any way interacted with by classes in the same directory
    private final long lineNumber;

    private static final String str_public="public";
    private static final String str_private="private";
    private static final String str_dir_private="dir-private";

    @Override
    public long getLineNumber() {
        return this.lineNumber;
    }

    public AccessModifierToken(CharacterList list) throws Exception {

        this.lineNumber=list.getCurrentLineNumber();

        if (list.startsWith(str_public)) {
            this.is_public = true;
            this.is_dir_private=false;
            this.is_private=false;
            list.consumeTokens(str_public.length());
        } else if (list.startsWith(str_private)) {
            this.is_public = false;
            this.is_dir_private=false;
            this.is_private=true;
            list.consumeTokens(str_private.length());
        } else if (list.startsWith(str_dir_private)) {
            this.is_public = false;
            this.is_dir_private=true;
            this.is_private=false;
            list.consumeTokens(str_dir_private.length());
        }else {
            throw new Exception("could not recognize access modifier token");
        }
    }

    public AccessModifierToken(String newcontents) throws Exception {
        this(new CharacterList(newcontents));
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return (this.is_public) ? str_public : (this.is_dir_private)? str_dir_private:str_private;
    }



}
