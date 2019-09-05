package org.vanautrui.languages.compiler.phase_clean_the_input;

import java.util.Arrays;
import java.util.stream.Collectors;

public class CommentRemoverAndWhitespaceRemover {
    public String strip_all_comments_and_empty_lines(String sourcecode) throws Exception {

        StringBuilder result = new StringBuilder();

        result.append(
                strip_single_line_comments(
                        strip_multi_line_comments(sourcecode)
                )
        );

        String res = result.toString();

        //remove empty lines
        res = Arrays.stream(res.split("\n")).filter(x -> !x.trim().isEmpty()).collect(Collectors.joining("\n"));

        return res;
    }

    public String strip_single_line_comments(String sourcecode) {
        StringBuilder result = new StringBuilder();

        for (String line : sourcecode.split("\n")) {

            boolean in_string=false;
            for(int i=0;i<line.length();i++){
                char c=line.charAt(i);
                if(c=='"'){
                    in_string=!in_string;
                    result.append(c);
                }else if(c=='/' && i< (line.length()-1) && line.charAt(i+1)=='/' && !in_string){
                    break;
                }else{
                    result.append(c);
                }
            }
            /*
            if (line.contains("//")) {
                //TODO: consider if the comment is in a string
                int i = line.indexOf("//");

                result.append(line.substring(0, line.indexOf("//")));
            } else {
                result.append(line);
            }
             */
            result.append("\n");
        }
        return result.toString();
    }

    public String strip_multi_line_comments(String sourcecode) throws Exception {
        StringBuilder result = new StringBuilder();
        int comment_depth = 0;
        int i = 0;

        while (i < sourcecode.length()) {
            char c = sourcecode.charAt(i);
            char c_next = 'x';
            boolean has_c_next = i < sourcecode.length() - 1;
            if (has_c_next) {
                c_next = sourcecode.charAt(i + 1);
            }


            //we look for comment start
            if (c == '/' && has_c_next && c_next == '*') {
                //comment start found
                i++;
                comment_depth++;
            } else if (comment_depth == 0) {
                result.append(c);

            } else {
                //we look for comment end
                if (c == '*' && has_c_next && c_next == '/') {
                    //end of comment found
                    i++;
                    comment_depth--;
                }
            }
            i++;

        }

        if (comment_depth != 0) {
            throw new Exception("A Multiline Comment is not formed correctly");
        }

        return result.toString();
    }

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
