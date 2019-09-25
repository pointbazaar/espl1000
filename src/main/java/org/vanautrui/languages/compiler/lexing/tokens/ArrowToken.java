package org.vanautrui.languages.compiler.lexing.tokens;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;

import java.nio.file.Paths;

public class ArrowToken implements IToken {

    private static final String[] arrow_symbols = new String[]{
            "->","~>"
    };

    public final boolean is_functional;
    private final String content;
    private final long lineNumber;

    @Override
    public long getLineNumber() {
      return this.lineNumber;
    }

    public ArrowToken(CharacterList list) throws Exception {
      this.lineNumber=list.getCurrentLineNumber();

      for (String sym : arrow_symbols) {
        if (list.startsWith(sym)) {
          if(sym.equals("->")){
            is_functional=true;
          }else{
            is_functional=false;
          }
          this.content=sym;
          list.consumeTokens(sym.length());
          return;
        }
      }

      throw new Exception("could not recognize an arrow token");
    }

    public ArrowToken(String newcontents) throws Exception {
      this(new CharacterList(newcontents, Paths.get("/dev/null")));
    }

    @Override
    @JsonIgnore
    public String getContents() {
      return this.content;
    }



}
