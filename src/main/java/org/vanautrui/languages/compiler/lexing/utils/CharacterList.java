package org.vanautrui.languages.compiler.lexing.utils;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class CharacterList{

    public Path relSrcPath;

    //the line numbers corresponding to the characters.
    //so that we are able to show meaningful error messages
    private List<Long> lineNumbers=new ArrayList<>();

    private List<Character> list;

    public CharacterList(String s,Path sourceFile) {
        //both paths need a root component for relativizing
        Path workingDir = Paths.get(System.getProperty("user.dir")).toAbsolutePath();
        Path codePath = sourceFile.toAbsolutePath();
        Path relativePath = workingDir.relativize(codePath);
        this.relSrcPath=relativePath;
        boolean insideString=false;

        char[] chars=s.toCharArray();
        long currentLine=1;
        this.list = new ArrayList<>();
        for (char c : chars) {
            if(c=='\"'){
                insideString=!insideString;
            }


            if(c=='\n'){
                currentLine++;
                if(insideString){
                    this.list.add(c);
                    this.lineNumbers.add(currentLine);
                }
            }else{
                this.list.add(c);
                this.lineNumbers.add(currentLine);
            }
        }
    }

    public CharacterList(String s){
        this(s, Paths.get("/dev/null"));
    }

    public CharacterList(CharacterList other) {
        this.relSrcPath = other.relSrcPath;

        this.lineNumbers    = new ArrayList<>(other.lineNumbers);
        this.list           = new ArrayList<>(other.list);
    }

    public int getCurrentLineNumber(){
        //+1 because line number is different from line index
        return lineNumbers.get(0).intValue()+1;
    }

    public void consumeTokens(int amount) {
        this.list           = this.list.subList(amount, this.list.size());
        this.lineNumbers    = this.lineNumbers.subList(amount, this.lineNumbers.size());
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

        StringBuilder sb = new StringBuilder("");

        for (Character ch : this.list.subList(0, amount)) {
            sb.append(ch);
        }

        String result = sb.toString();

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
