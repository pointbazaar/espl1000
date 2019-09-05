package org.vanautrui.languages.compiler.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class KeywordToken implements IToken {

    public static final List<String> keywords = Arrays.asList(
            "class",
            "method", "function",

            "interface","wrapper","entity","model","adapter",

            "while","for","loop","do","if","else",

            "return"
    );

    public String keyword;

    public KeywordToken(CharacterList list) throws Exception {
        super();
        for (String sym : keywords) {
            Pattern p = Pattern.compile(sym+"([^a-z]|$)");
            Matcher m =p.matcher(list.getLimitedStringMaybeShorter(keywords.stream().map(s->s.length()).reduce(Integer::max).get()));

            if (m.find() && m.start()==0) {

                this.keyword = sym;
                list.consumeTokens(this.keyword.length());
                return;
            }
        }

        throw new Exception("could not recognize a keyword");
    }

    public KeywordToken(String s)throws Exception{
        this(new CharacterList(s, Paths.get("/dev/null")));
    }

    @Override
    @JsonIgnore
    public String getContents() {
        return this.keyword;
    }

}
