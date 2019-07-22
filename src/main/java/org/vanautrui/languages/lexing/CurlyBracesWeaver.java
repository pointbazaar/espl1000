package org.vanautrui.languages.lexing;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class CurlyBracesWeaver {


    public static String weave_scoping_curly_braces_and_remove_newlines(String sourceCodeWithoutComments) throws Exception{

        //because the language should work with indentation, not requiring the use of curly braces,

        /*

        public void main([String] args)
            println("hi")

        //should become:

        public void main([String] args){
            println("hi");
        }

         */

        //all this is so that the lexer / tokenizer can later ignore newlines.
        // which would make it maybe easier to implement

        //when going from a higher to a lower indentation, a "}" should be inserted inbetween
        //when going from a lower to a higher indentation, a "{" should be inserted inbetween

        //but how do we know if it is indented only, or just with braces, or indented and with braces?
        //the code could also have been generated, in which case it probably would have no indentation, but braces

        //there could also be 2 people working on the code, one using braces, the other using indentation.



        //we can make a list of all lines, and make a class to contain a number (of the indentation level) and the string of that line
        //then we can go over these lines and grab their contents and insert braces where appropriate

        List<IndentedString> indentedStrings = Arrays.stream(sourceCodeWithoutComments.split("\n")).map(str-> {
            try {
                return new IndentedString(str);
            } catch (Exception e) {
                e.printStackTrace();
                throw new RuntimeException("something gone terribly wrong");
            }
        }).collect(Collectors.toList());


        return weave_curly_braces(indentedStrings);
    }

    private static String weave_curly_braces(List<IndentedString> lines){

        //TODO: what if there are already braces? handle this case!

        StringBuilder sb=new StringBuilder("");

        int last_indentation=0;

        for (IndentedString line : lines) {

            if (line.indentation > last_indentation) {
                //add "{"
                sb.append("{");
            } else if (line.indentation < last_indentation) {
                //add "}"
                sb.append("}");
            }

            last_indentation = line.indentation;
        }

        if(last_indentation>0){
            sb.append("}");
        }

        return sb.toString();
    }
}
