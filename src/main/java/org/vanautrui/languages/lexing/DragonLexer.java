
package org.vanautrui.languages.lexing;

import org.vanautrui.languages.model.tokens.ClassToken;
import org.vanautrui.languages.model.tokens.DragonToken;
import org.vanautrui.languages.model.tokens.AccessModifierToken;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class DragonLexer {
    public List<DragonToken> lex(String sourcecodeWithoutComments) throws Exception {

        String just_code_with_braces = CurlyBracesWeaver.weave_scoping_curly_braces_and_remove_newlines(sourcecodeWithoutComments);

        System.out.println("Code after weaving in curly braces, if they werent there before:");

        System.out.println(just_code_with_braces);

        List<DragonToken> result = new ArrayList<DragonToken>();
        //TODO

        return result;
    }

}
