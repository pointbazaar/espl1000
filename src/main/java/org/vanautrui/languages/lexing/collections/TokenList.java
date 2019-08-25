package org.vanautrui.languages.lexing.collections;

import org.fusesource.jansi.Ansi;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.lexing.tokens.utils.Token;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.fusesource.jansi.Ansi.ansi;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.getPreferredXMLSerializationStrategyHumanReadable;

public class TokenList {

    //this is to facilitate special features
    //which would be convenient in a token list for our compiler

    //relative path of the source file
    private Path relPath = Paths.get("/dev/null");

    private List<Token> tokens=new ArrayList<>();
    private List<Integer> lineNumbers=new ArrayList<>();

    public TokenList(){}

    public TokenList(List<Token> result, List<Integer> lineNumbers,Path sourceFile) {
        this.tokens = result;
        this.lineNumbers=lineNumbers;
        this.relPath =sourceFile;
    }

    public TokenList(TokenList other) {
        this.tokens = new ArrayList<>(other.tokens);
        this.lineNumbers=new ArrayList<>(other.lineNumbers);
        this.relPath =other.relPath;
    }

    public List<Token> getTokens() {
        return this.tokens;
    }

    public void add(Token token) {
        this.tokens.add(token);
        this.lineNumbers.add(-1);
    }

    public void consume(int amount) {

        //DEBUG
        //System.out.print("consuming: ");
        //System.out.println(head().getContents());
        //System.out.println();

        this.tokens = this.tokens.subList(amount, this.tokens.size());
        this.lineNumbers = this.lineNumbers.subList(amount, this.lineNumbers.size());
    }

    public int size() {
        return this.tokens.size();
    }

    public boolean startsWith(Token token) {
        if (this.tokens.size() > 0) {
            return this.tokens.get(0).equals(token);
        }
        return false;
    }
    public boolean endsWith(Token token) {
        if (this.tokens.size() > 0) {
            return this.tokens.get(this.tokens.size()-1).equals(token);
        }
        return false;
    }

    public void expectAndConsumeOtherWiseThrowException(Token token) throws Exception {

        //equals() should be implemented correctly in all the tokens

        if (this.startsWith(token)) {
            this.consume(1);
        } else {
            //String expectedTokenMessage = "'" + token.getContents() + "' (" + token.getClass().getSimpleName() + ")";
            String expectedTokenMessage = "'" + token.getContents() + "'";
            //String actualTokenMessage = "'" + this.head().getContents() + "' (" + this.head().getClass().getSimpleName() + ")";
            String actualTokenMessage = "'" + this.head().getContents() + "'";

            String sourceCodeFragment = (this.toSourceCodeFragment().substring(0, Math.min(this.toSourceCodeFragment().length(), 100)));

            throw new Exception(
                    ansi().fg(Ansi.Color.WHITE).a("in line "+"TODO"+":\n").reset()
                    .fg(Ansi.Color.RED).a("Parsing Error: \n").reset()+
                    "expected token:  \n\n"
                            + "\t"+ansi().fg(Ansi.Color.BLUE).a(expectedTokenMessage).reset().toString()+"\n\n"
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

        Serializer serializer = new Persister(getPreferredXMLSerializationStrategyHumanReadable());

        StringBuilder sb = new StringBuilder();

        OutputStream out = new BufferedOutputStream(new OutputStream() {
            @Override
            public void write(int b) throws IOException {
                sb.append((char)b);
            }
        });

        try {
            serializer.write(this, out);
        }catch (Exception e){
            e.printStackTrace();
            return "error";
        }

        return sb.toString();

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

    public Token get(int i) {
        return this.tokens.get(i);
    }

    public Token head() {
        return this.get(0);
    }

    public String toSourceCodeFragment() {
        return this.tokens.stream().map(token -> token.getContents()).collect(Collectors.joining(" "));
    }
}
