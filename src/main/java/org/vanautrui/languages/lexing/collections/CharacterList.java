package org.vanautrui.languages.lexing.collections;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class CharacterList {

    //the line numbers corresponding to the characters.
    //so that we are able to show meaningful error messages
    private List<Long> lineNumbers=new ArrayList<>();

    private List<Character> list;

    private boolean stringValid = false;
    private String stringValue = "";

    public CharacterList(String s) {
        this(s.toCharArray());
    }

    public CharacterList(List<Character> list) {
        this(list.stream().map(ch->ch+"").collect(Collectors.joining("")).toString());
    }

    public CharacterList(char[] chars) {
        long currentLine=1;
        this.list = new ArrayList<>();
        for (char c : chars) {

            this.list.add(c);
            this.lineNumbers.add(currentLine);

            if(c=='\n'){
                currentLine++;
            }
        }
    }

    public CharacterList(CharacterList other) {
        this.lineNumbers    = new ArrayList<>(other.lineNumbers);
        this.list           = new ArrayList<>(other.list);

        this.stringValid    = other.stringValid;
        this.stringValue    = other.stringValue;
    }

    public long getCurrentLine(){
        //get the line the next char is on
        return this.lineNumbers.get(0);
    }

    public void consumeTokens(int amount) {
        this.list           = this.list.subList(amount, this.list.size());
        this.lineNumbers    = this.lineNumbers.subList(amount, this.lineNumbers.size());

        this.stringValid = false;
    }

    public int size() {
        return this.list.size();
    }

    public List<Character> getList() {
        return this.list;
    }

    public boolean startsWith(String s) {
        if (list.size() >= s.length()) {
            try {
                return this.getLimitedString(s.length()).equals(s);
            } catch (Exception e) {
                return false;
            }
        }
        return false;
    }

    public String getLimitedString(int amount) throws Exception {

        if (amount > this.list.size()) {
            throw new Exception("amount is too big. list not that long");
        }

        if (this.stringValid && this.stringValue.length() >= amount) {
            return this.stringValue.substring(0, amount);
        }

        StringBuilder sb = new StringBuilder("");

        for (Character ch : this.list.subList(0, amount)) {
            sb.append(ch);
        }

        String result = sb.toString();

        this.stringValue = result;
        this.stringValid = true;

        return result;
    }


    @Override
    public String toString() {
        try {
            return getLimitedString(this.list.size());
        } catch (Exception e) {
            throw new RuntimeException("terrible stuff");
        }
    }

    public String getLimitedStringMaybeShorter(int length) throws Exception {
        return this.getLimitedString(Math.min(length, this.list.size()));
    }
}
