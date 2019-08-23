package org.vanautrui.languages.lexing;

import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.lexing.tokens.utils.Token;

import java.util.ArrayList;
import java.util.List;

public class Lexer {
    public TokenList lexCodeWithoutComments(String sourcecodeWithoutComments) throws Exception {

        TokenList tokens = tokenize(new CharacterList(sourcecodeWithoutComments));

        //System.out.println("TOKENS:");
        //System.out.println(tokens.toString());

        return tokens;
    }

    private TokenList tokenize(CharacterList sourceCodeWithBracesWithoutCommentsWithoutNewlines) throws Exception {
        CharacterList myCode = new CharacterList(sourceCodeWithBracesWithoutCommentsWithoutNewlines);

        List<Token> result = new ArrayList<>();

        //TODO

        //every class implementing DragonToken
        //should have a constructor which consumes Characters of a list,
        //if it is able to construct a token.
        //it creates a copy of the list, and if it is successful, replaces the contents of the original list with that

        //so we can try all tokens, but the order of trying is important,
        //access modifier token should be tried before identifier token
        //also the choice of which tokens we implement is important


        while (myCode.size() > 0) {
            //while not every last char is tokenized,
            //try to tokenize something

            //System.out.println(myCode.size());

            try {
                result.add(new AccessModifierToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }

			try{
				result.add(new BoolConstantToken(myCode));
				continue;
			}catch(Exception e){
				//pass
			}

            try {
                result.add(new ClassToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }

            try {
                result.add(new KeywordToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }

            try {
                result.add(new IdentifierToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }

            try {
                result.add(new TypeIdentifierToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }

	    try {
                result.add(new FloatNonNegativeConstantToken(myCode));
                continue;
            } catch (Exception e) {
                    try {
		        result.add(new IntegerNonNegativeConstantToken(myCode));
		        continue;
		    } catch (Exception e2) {
		        //pass
		    }
            }

            

            try {
                result.add(new StringConstantToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }

            try {
                result.add(new SymbolToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }

            try {
                result.add(new OperatorToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
            }
            //TODO: make more kinds of tokens to enrich the token stream

            if (myCode.startsWith(" ") || myCode.startsWith("\t")) {
                //white space is always allowed between tokens
                myCode.consumeTokens(1);
                continue;
            }


            throw
                    new Exception(
                            "tried to tokenize all manner of tokens, but could not make progress. string : '"
                                    + myCode.getLimitedStringMaybeShorter(20) + "'"
                    );
        }

        return new TokenList(result);
    }

}
