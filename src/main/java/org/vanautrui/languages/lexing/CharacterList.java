package org.vanautrui.languages.lexing;

import java.util.ArrayList;
import java.util.List;

public class CharacterList {

    private List<Character> list;

    private boolean stringValid = false;
    private String stringValue = "";

    public CharacterList(String s) {
        this(s.toCharArray());
    }

    public CharacterList(List<Character> list) {
        this.list = list;
    }

    public CharacterList(char[] chars) {
        this.list = new ArrayList<>();
        for (char c : chars) {
            this.list.add(c);
        }
    }

    public CharacterList(CharacterList other) {
        this.list = new ArrayList<>(other.list);
        this.stringValid = other.stringValid;
        this.stringValue = other.stringValue;
    }

    public void consumeTokens(int amount) {
        this.list = this.list.subList(amount, this.list.size());
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
