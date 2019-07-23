package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.CharacterList;
import org.vanautrui.languages.lexing.tokens.DragonToken;

import java.util.ArrayList;
import java.util.List;

public class DragonTokenList {

    //this is to facilitate special features
    //which would be convenient in a token list for our compiler

    private List<DragonToken> tokens;

    public DragonTokenList() {
        this.tokens=new ArrayList<>();
    }

    public List<DragonToken> getTokens(){
        return this.tokens;
    }

    public DragonTokenList(List<DragonToken> list){
        this.tokens=new ArrayList<>(list);
    }

    public DragonTokenList(DragonTokenList other){
        this.tokens=new ArrayList<>(other.tokens);
    }

    public void add(DragonToken token){
        this.tokens.add(token);
    }

    public void consume(int amount){
        this.tokens=this.tokens.subList(amount,this.tokens.size());
    }

    public int size(){
        return this.tokens.size();
    }

    public boolean startsWith(DragonToken token){
        if(this.tokens.size()>0) {
            return this.tokens.get(0).equals(token);
        }
        return false;
    }

    public void expectAndConsumeOtherWiseThrowException(DragonToken token)throws Exception{

        //equals() should be implemented correctly in all the tokens

        if(this.startsWith(token)){
            this.consume(1);
        }else{
            throw new Exception(" expected a specific token, "+token.toString()+", but it was otherwise");
        }
    }

    @Override
    public String toString(){

        StringBuilder sb=new StringBuilder("");

        for(DragonToken token : tokens){
            String tokenString = (
                    "<"
                    //+token.getClass().getName()
                    +token.getClass().getSimpleName()
                    +": "
                    +token.getContents()
                    +">"
            );
            sb.append(tokenString);
        }

        return sb.toString();
    }

    public DragonTokenList copy() {
        return new DragonTokenList(this);
    }

    public void set(DragonTokenList copy) {
        this.tokens.clear();
        this.tokens.addAll(copy.tokens);
    }

    public DragonToken get(int i) {
        return this.tokens.get(0);
    }
}
