package org.vanautrui.languages.lexing;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class CharacterList {

    private List<Character> list;

    public CharacterList(String s){
        this(s.toCharArray());
    }

    public CharacterList(List<Character> list){
        this.list=list;
    }

    public CharacterList(char[] chars){
        for (char c : chars){
            this.list.add(c);
        }
    }

    public CharacterList(CharacterList other){
        this.list=new ArrayList<>(other.list);
    }

    public void consumeTokens(int amount){
        this.list=this.list.subList(amount,this.list.size()-1);
    }

    public int size(){
        return this.list.size();
    }

    public List<Character> getList(){
        return this.list;
    }

    public boolean startsWith(String s){
        if(list.size()>=s.length()){
            return new CharacterList(
                    list.subList(0,s.length()-1)
            )
                    .toString()
                    .equals(s);
        }
        return false;
    }

    @Override
    public String toString(){
        return this.list
                .stream()
                .map(ch->ch+"")
                .collect(Collectors.joining(""));
    }
}
