package org.vanautrui.languages.lexing.collections;

import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;
import org.vanautrui.languages.lexing.tokens.DragonToken;

import javax.xml.bind.annotation.XmlRootElement;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.commandline.dragonc.getPreferredXMLSerializationStrategyHumanReadable;

public class DragonTokenList {

    //this is to facilitate special features
    //which would be convenient in a token list for our compiler

    @ElementList(name="tokens",entry="token")
    private List<DragonToken> tokens;

    public DragonTokenList() {
        this.tokens = new ArrayList<>();
    }

    public List<DragonToken> getTokens() {
        return this.tokens;
    }

    public DragonTokenList(List<DragonToken> list) {
        this.tokens = new ArrayList<>(list);
    }

    public DragonTokenList(DragonTokenList other) {
        this.tokens = new ArrayList<>(other.tokens);
    }

    public void add(DragonToken token) {
        this.tokens.add(token);
    }

    public void consume(int amount) {

        //DEBUG
        //System.out.print("consuming: ");
        //System.out.println(head().getContents());
        //System.out.println();

        this.tokens = this.tokens.subList(amount, this.tokens.size());
    }

    public int size() {
        return this.tokens.size();
    }

    public boolean startsWith(DragonToken token) {
        if (this.tokens.size() > 0) {
            return this.tokens.get(0).equals(token);
        }
        return false;
    }

    public void expectAndConsumeOtherWiseThrowException(DragonToken token) throws Exception {

        //equals() should be implemented correctly in all the tokens

        if (this.startsWith(token)) {
            this.consume(1);
        } else {
            String expectedTokenMessage = "'" + token.getContents() + "' (" + token.getClass().getSimpleName() + ")";
            String actualTokenMessage = "'" + this.head().getContents() + "' (" + this.head().getClass().getSimpleName() + ")";

            String sourceCodeFragment = (this.toSourceCodeFragment().substring(0, Math.min(this.toSourceCodeFragment().length(), 100)));

            throw new Exception(
                    " expected a specific token:  \n" + expectedTokenMessage
                            + ",\n but it was otherwise: \n" + actualTokenMessage + "\n"
                            + " in '" + sourceCodeFragment + "'\n"
            );
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

    public DragonTokenList copy() {
        return new DragonTokenList(this);
    }

    public void set(DragonTokenList copy) {
        this.tokens.clear();
        this.tokens.addAll(copy.tokens);
    }

    public DragonToken get(int i) {
        return this.tokens.get(i);
    }

    public DragonToken head() {
        return this.get(0);
    }

    public String toSourceCodeFragment() {
        return this.tokens.stream().map(token -> token.getContents()).collect(Collectors.joining(" "));
    }
}
