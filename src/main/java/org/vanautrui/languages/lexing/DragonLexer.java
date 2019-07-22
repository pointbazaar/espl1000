
package org.vanautrui.languages.lexing;

import org.vanautrui.languages.model.tokens.ClassToken;
import org.vanautrui.languages.model.tokens.DragonToken;
import org.vanautrui.languages.model.tokens.AccessModifierToken;

import java.util.ArrayList;
import java.util.List;

public class DragonLexer {
    public List<DragonToken> lex(String sourcecodeWithoutComments) throws Exception {

        //TODO
        List<DragonToken> result = new ArrayList<DragonToken>();
        String[] lines = sourcecodeWithoutComments.split("\n");
        for(String s : lines){
            String[] words = s.split(" ");
            for(String word : words){
                result.add(new ClassToken(word));
            }
        }
        return result;
    }

    private List<DragonToken> weave_scoping_curly_braces(String sourceCodeWithoutComments) throws Exception{

        //because the language should work with indentation, not requiring the use of curly braces,

        /*

        public void main([String] args)
            println("hi")

        //should become:

        public void main([String] args){
            println("hi");
        }

         */

        //all this is so that the lexer / tokenizer can later ignore newlines.
        // which would make it maybe easier to implement

        //TODO
    }
}
