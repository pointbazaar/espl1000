package org.vanautrui.languages.phase_clean_the_input;

import java.util.Arrays;
import java.util.stream.Collectors;

public class DragonUnneccessaryWhiteSpaceRemover {

    public static String remove_unneccessary_whitespace(String codeWithoutCommentsAndWithoutEmptyLines) {

        String result = Arrays.stream(codeWithoutCommentsAndWithoutEmptyLines.split("\n"))
                .map(
                        str->remove_unneccessary_whitespace_line(str)
                )
                .collect(Collectors.joining("\n"));

        return result;
    }

    private static String remove_unneccessary_whitespace_line(String str) {
        //the initial whitespace, and whitespace within strings should remain
        //1 whitespace between words should remain

        StringBuilder sb=new StringBuilder("");

        //TODO: save the indentation from being removed

        boolean last_was_whitespace=true;
        boolean inside_string=false;
        boolean start_of_line=false;

        int i=0;

        while(i<str.length() && !start_of_line){
            char c = str.charAt(i);
            switch (c) {
                case '\t':
                case ' ':
                    sb.append(c);
                    i++;
                    break;
                default:
                    start_of_line = true;
            }
        }

        while(i<str.length()){
            char c=str.charAt(i);

            switch (c){
                case '\"':
                    inside_string=!inside_string;
                    last_was_whitespace=false;
                    sb.append(c);
                    break;
                case ' ':
                    if(last_was_whitespace && !inside_string){
                        //we skip it
                    }else{
                        sb.append(c);
                    }
                    last_was_whitespace=true;
                    break;
                default:
                    last_was_whitespace=false;
                    sb.append(c);
                    break;
            }
            i++;
        }

        return sb.toString();
    }
}
