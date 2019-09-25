package org.vanautrui.languages.compiler.lexing.utils;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.tuple.Pair;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.TerminalUtil.generateFileNameWithLine;
import static org.vanautrui.languages.TerminalUtil.gererateErrorString;

public class TokenList {

    //this is to facilitate special features
    //which would be convenient in a token list for our compiler

    //relative path of the source file
    public Path relPath = Paths.get("/dev/null");

    private List<IToken> tokens=new ArrayList<>();


    public TokenList(){}

    public TokenList(List<IToken> result, Path sourceFile) {
        this.tokens = result;
        this.relPath =sourceFile;
    }

    public TokenList(TokenList other) {
        this.tokens = new ArrayList<>(other.tokens);
        this.relPath =other.relPath;
    }

    public List<IToken> getTokens() {
        return this.tokens;
    }

    public void add(IToken token) {
        this.tokens.add(token);
    }

    public void consume(int amount) {

        this.tokens = this.tokens.subList(amount, this.tokens.size());
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
        if(this.size()==0){
            throw new Exception("no tokens");
        }

        if (this.startsWith(token)) {
            this.consume(1);
        } else {
            String expectedStart =String.format("\t%-20s","expected:");
            String expectedEnd = String.format("%-20s", StringUtils.wrap(token.getContents(),"'"));
            String expectedTokenMessage = expectedStart+expectedEnd+" ("+token.getClass().getSimpleName()+")";

            String actualStart = String.format("\t%-20s","actual:");
            String actualEnd = String.format("%-20s",StringUtils.wrap(this.head().getContents(),"'"));
            String actualTokenMessage = actualStart+actualEnd+" ("+this.head().getClass().getSimpleName()+")";;

            String sourceCodeFragment = (this.toSourceCodeFragment().substring(0, Math.min(this.toSourceCodeFragment().length(), 100)));

            String message= gererateErrorString("Parsing Error: \n")
                    + "\t"+expectedTokenMessage+"\n"
                    + "\t"+ actualTokenMessage +"\n"
                    + "in '" + sourceCodeFragment + "'\n"
                    + "in "+ generateFileNameWithLine(relPath,this.head().getLineNumber());

            throw new Exception(message);
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

    /**
     * @param token the token to read until
     * @return returns a tokenlist with tokens up until that token
     */
    public TokenList until(IToken token){
        TokenList left = new TokenList();
        IToken tk;
        int j=0;
        while(j<tokens.size()){
            tk=tokens.get(j);
            if(tk.tokenEquals(token)){
                break;
            }else {
                left.add(tk);
                j++;
            }
        }
        return left;
    }

    /**
     * @return returns the tokenlist, split in 2,
     * with the first half being all tokens and the next block ( {...} is a block)
     */
    public Pair<TokenList,TokenList> split_into_tokens_and_next_block_and_later_tokens()throws Exception{
        if(tokens.size()==0){
            return Pair.of(new TokenList(),new TokenList());
        }

        TokenList left = new TokenList();

        //adds all the tokens until the first '{'
        left.tokens.addAll(this.until(new SymbolToken("{")).tokens);

        int block_nesting_level=0;
        IToken tk;

        //add the block
        int i=left.size();
        for(;i<this.tokens.size();i++){
            tk = this.tokens.get(i);

            if(tk.tokenEquals(new SymbolToken("{"))){
                block_nesting_level++;
            }else if(tk.tokenEquals(new SymbolToken("}"))){
                if(block_nesting_level==0){
                    break;
                }
                block_nesting_level--;
            }else{
                if(block_nesting_level==0){
                    break;
                }
            }
            left.add(tk);
        }

        TokenList right = new TokenList();

        right.tokens.addAll(this.tokens.subList(i,this.tokens.size()));

        return Pair.of(left,right);
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
