
package org.vanautrui.languages.lexing;

import org.vanautrui.languages.model.AmandaToken;

import java.util.ArrayList;
import java.util.List;

public class AmandaLexer {
    public List<AmandaToken> lex(String sourcecode) throws Exception {
        String source_without_comments = (new AmandaCommentRemover()).strip_comments(sourcecode);
        //TODO
        List<AmandaToken> result = new ArrayList<AmandaToken>();
        String[] lines = source_without_comments.split("\n");
        for(String s : lines){
            String[] words = s.split(" ");
            for(String word : words){
                result.add(new AmandaToken(word));
            }
            result.add(new AmandaToken("\n"));
        }
        return result;
    }


}
