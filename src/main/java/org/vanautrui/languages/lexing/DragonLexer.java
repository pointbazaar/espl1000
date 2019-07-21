
package org.vanautrui.languages.lexing;

import org.vanautrui.languages.model.tokens.AmandaToken;
import org.vanautrui.languages.model.tokens.AccessModifierToken;

import java.util.ArrayList;
import java.util.List;

public class DragonLexer {
    public List<AmandaToken> lex(String sourcecode) throws Exception {
        String source_without_comments = (new DragonCommentRemover()).strip_comments(sourcecode);
        //TODO
        List<AmandaToken> result = new ArrayList<AmandaToken>();
        String[] lines = source_without_comments.split("\n");
        for(String s : lines){
            String[] words = s.split(" ");
            for(String word : words){
                result.add(decide_which_token(word));
            }
            result.add(new AmandaToken("\n"));
        }
        return result;
    }

    public AmandaToken decide_which_token(String word) throws Exception{

        //TODO
        switch (word){
            case "public":
                return new AccessModifierToken(word);
            case "private":
                return new AccessModifierToken(word);
        }

        return new AmandaToken(word);
    }
}
