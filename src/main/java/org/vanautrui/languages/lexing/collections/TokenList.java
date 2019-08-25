package org.vanautrui.languages.lexing.collections;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.lexing.tokens.utils.IToken;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.fusesource.jansi.Ansi.ansi;

public class TokenList {

    //this is to facilitate special features
    //which would be convenient in a token list for our compiler

    //relative path of the source file
    private Path relPath = Paths.get("/dev/null");

    private List<IToken> tokens=new ArrayList<>();
    private List<Integer> lineNumbers=new ArrayList<>();

    public TokenList(){}

    public TokenList(List<IToken> result, List<Integer> lineNumbers, Path sourceFile) {
        this.tokens = result;
        this.lineNumbers=lineNumbers;
        this.relPath =sourceFile;
    }

    public TokenList(TokenList other) {
        this.tokens = new ArrayList<>(other.tokens);
        this.lineNumbers=new ArrayList<>(other.lineNumbers);
        this.relPath =other.relPath;
    }

    public List<IToken> getTokens() {
        return this.tokens;
    }

    public void add(IToken token) {
        this.tokens.add(token);
        this.lineNumbers.add(-1);
    }

    public void consume(int amount) {

        this.tokens = this.tokens.subList(amount, this.tokens.size());
        this.lineNumbers = this.lineNumbers.subList(amount, this.lineNumbers.size());
    }

    public int size() {
        return this.tokens.size();
    }

    public boolean startsWith(IToken token) {
        //we cannot rely on .equals()
        //we should use interfaces we can rely on classes to implement
        //the class and the content of the token should be the same for them to be the same

        if (this.tokens.size() > 0) {
            return this.tokens.get(0).tokenEquals(token);
        }
        return false;
    }
    public boolean endsWith(IToken token) {
        if (this.tokens.size() > 0) {
            return this.tokens.get(this.tokens.size()-1).tokenEquals(token);
        }
        return false;
    }

    public void expectAndConsumeOtherWiseThrowException(IToken token) throws Exception {

        //equals() should be implemented correctly in all the tokens

        if (this.startsWith(token)) {
            this.consume(1);
        } else {

            String expectedTokenMessage = "'" + token.getContents() + "'" + " ("+token.getClass().getName()+")";

            String actualTokenMessage = "'" + this.head().getContents() + "'" + " ("+this.head().getClass().getName()+")";;

            String sourceCodeFragment = (this.toSourceCodeFragment().substring(0, Math.min(this.toSourceCodeFragment().length(), 100)));

            throw new Exception(
                    ansi().fg(Ansi.Color.RED).a("Parsing Error: \n").reset()+
                    "expected token:  \n\n"
                            + "\t"+expectedTokenMessage+"\n\n"
                            + "actual token: \n\n"
                            +"\t"+ actualTokenMessage + "\n\n"
                            + "in '" + sourceCodeFragment + "'\n"
                            +" in "+ TerminalUtil.generateFileNameWithLine(relPath,lineNumbers.get(0))
            );
            //TODO: make sure the source file path is relative (for pretty output)
        }
    }

    @Override
    public String toString() {
        //pretty display

        ObjectMapper mapper = new ObjectMapper();
        mapper.enable(SerializationFeature.INDENT_OUTPUT);


        try {
            return mapper.writeValueAsString(this.tokens);
        } catch (JsonProcessingException e) {
            return "ERROR SERIALIZING IN TokenList.java";
        }

    }

    private String indent(int n){
        return new String(new char[n]).replace("\0","\t");
    }

    public TokenList copy() {
        return new TokenList(this);
    }

    public void set(TokenList copy) {
        this.tokens.clear();
        this.tokens.addAll(copy.tokens);
    }

    public IToken get(int i) {
        return this.tokens.get(i);
    }

    public IToken head() {
        return this.get(0);
    }

    public String toSourceCodeFragment() {
        return this.tokens.stream().map(token -> token.getContents()).collect(Collectors.joining(" "));
    }
}
