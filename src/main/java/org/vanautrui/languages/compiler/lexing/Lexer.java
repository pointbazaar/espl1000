package org.vanautrui.languages.compiler.lexing;

import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.*;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import static org.fusesource.jansi.Ansi.ansi;

public class Lexer {

    public TokenList lexCodeTestMode(String s)throws Exception{
        return tokenize(new CharacterList(s, Paths.get("/dev/null")));
    }

    public TokenList lexCodeWithoutComments(CharacterList list) throws Exception {
        return tokenize(list);
    }

    private TokenList tokenize(CharacterList myCode) throws Exception {

        List<IToken> result = new ArrayList<>();
        List<Integer> lineNumbers = new ArrayList<>();

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

            //since we will parse a token or throw an exception,
            //just add the line number
            //the case where we just consume a whitespace or tab will be handled below
            lineNumbers.add(myCode.getCurrentLineNumber());

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
                result.add(new CharConstantToken(myCode));
                continue;
            } catch (Exception e) {
                //pass
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
                lineNumbers.remove(lineNumbers.size()-1);
                continue;
            }



            String msg=TerminalUtil.gererateErrorString("Tokenize Error: ")+ansi().a(
                    "'"+myCode.getLimitedStringMaybeShorter(20) + "' \t "
            + TerminalUtil.generateFileNameWithLine(myCode.relSrcPath,lineNumbers.get(0))).reset()
                    .toString();

            lineNumbers.remove(lineNumbers.size()-1);

            throw new Exception(msg);
        }

        return new TokenList(result,myCode.relSrcPath);
    }

}
