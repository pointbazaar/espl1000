package org.vanautrui.languages.lexing.tokens;

import org.vanautrui.languages.lexing.CharacterList;

import java.util.Objects;

public class AccessModifierToken implements DragonToken {

    public boolean is_public;

    public AccessModifierToken(CharacterList list) throws Exception{

        if(list.startsWith("public")){
            this.is_public=true;
            list.consumeTokens("public".length());
        }else if(list.startsWith("private")){
            this.is_public=false;
            list.consumeTokens("private".length());
        }else {
            throw new Exception("could not recognize access modifier token");
        }
    }

    public AccessModifierToken(String newcontents) throws Exception {
        switch (newcontents){
            case "public":
                is_public=true;
                break;
            case "private":
                is_public=false;
                break;
            default:
                throw new Exception("invalid acces modifier token: \""+newcontents+"\"");
        }
    }

    @Override
    public String getContents() {
        return (this.is_public)?"public":"private";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof AccessModifierToken)) return false;
        AccessModifierToken that = (AccessModifierToken) o;
        return is_public == that.is_public;
    }
}
