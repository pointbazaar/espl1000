
package org.vanautrui.languages.lexing;

import org.vanautrui.languages.lexing.tokens.*;

import java.util.ArrayList;
import java.util.List;

public class DragonLexer {
    public List<DragonToken> lexCodeWithoutComments(String sourcecodeWithoutComments) throws Exception {

        System.out.println("PHASE: LEXING");

        String just_code_with_braces_without_comments_without_newlines = CurlyBracesWeaver.weave_scoping_curly_braces_and_remove_newlines(sourcecodeWithoutComments);

        System.out.println("Code after weaving in curly braces, if they werent there before:");

        System.out.println(just_code_with_braces_without_comments_without_newlines);


        List<DragonToken> tokens = tokenize(new CharacterList(just_code_with_braces_without_comments_without_newlines));

        System.out.println("TOKENS:");
        for(DragonToken token : tokens){
            System.out.println(
                    "<"
                    //+token.getClass().getName()
                    +token.getClass().getSimpleName()
                    +": "
                    +token.getContents()
                    +">"
            );
        }

        return tokens;
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

            //System.out.println(myCode.size());

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

            try{
                result.add(new StringConstantToken(myCode));
                continue;
            }catch (Exception e){
                //pass
            }

            try{
                result.add(new SymbolToken(myCode));
                continue;
            }catch (Exception e){
                //pass
            }

            //TODO: operator tokens
            //TODO: make more kinds of tokens to enrich the token stream

            if(myCode.startsWith(" ") || myCode.startsWith("\t")){
                //white space is always allowed between tokens
                myCode.consumeTokens(1);
                continue;
            }


            throw
                    new Exception(
                            "tried to tokenize all manner of tokens, but could not make progress. string : '"
                            +myCode.getLimitedStringMaybeShorter(10)+"'"
                    );
        }

        return result;
    }

}
