package org.vanautrui.languages.model.tokens;

public class AccessModifierToken extends DragonToken {

    public boolean is_public;


    public AccessModifierToken(String newcontents) throws Exception {
        super(newcontents);
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
}
