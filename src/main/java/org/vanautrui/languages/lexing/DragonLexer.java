
package org.vanautrui.languages.lexing;

import org.vanautrui.languages.model.tokens.ClassToken;
import org.vanautrui.languages.model.tokens.DragonToken;
import org.vanautrui.languages.model.tokens.AccessModifierToken;
import org.vanautrui.languages.model.tokens.IdentifierToken;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class DragonLexer {
    public List<DragonToken> lex(String sourcecodeWithoutComments) throws Exception {

        System.out.println("PHASE: LEXING");

        String just_code_with_braces_without_comments_without_newlines = CurlyBracesWeaver.weave_scoping_curly_braces_and_remove_newlines(sourcecodeWithoutComments);

        System.out.println("Code after weaving in curly braces, if they werent there before:");

        System.out.println(just_code_with_braces_without_comments_without_newlines);


        return tokenize(new CharacterList(just_code_with_braces_without_comments_without_newlines));
    }

    private List<DragonToken> tokenize(CharacterList sourceCodeWithBracesWithoutCommentsWithoutNewlines) throws Exception{
        CharacterList myCode = new CharacterList(sourceCodeWithBracesWithoutCommentsWithoutNewlines);

        List<DragonToken> result = new ArrayList<>();

        //TODO

        //every class implementing DragonToken
        //should have a constructor which consumes Characters of a list,
        //if it is able to construct a token.
        //it creates a copy of the list, and if it is successful, replaces the contents of the original list with that

        //so we can try all tokens, but the order of trying is important,
        //access modifier token should be tried before identifier token
        //also the choice of which tokens we implement is important

        boolean did_something=true;

        while (myCode.size()>0){
            //while not every last char is tokenized,
            //try to tokenize something

            try{
                result.add(new AccessModifierToken(myCode));
                continue;
            }catch (Exception e){
                //pass
            }

            try{
                result.add(new ClassToken(myCode));
                continue;
            }catch (Exception e){
                //pass
            }

            try{
                result.add(new IdentifierToken(myCode));
                continue;
            }catch (Exception e){
                //pass
            }

            if(myCode.startsWith(" ")){
                //white space is always allowed between tokens
                myCode.consumeTokens(1);
                continue;
            }


            throw
                    new Exception(
                            "tried to tokenize all manner of tokens, but could not make progress. string : "
                            +sourceCodeWithBracesWithoutCommentsWithoutNewlines.getList()
                                    .stream()
                                    .map(character -> character+"")
                                    .collect(Collectors.joining(""))
                    );
        }

        return result;
    }

}
