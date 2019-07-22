package org.vanautrui.languages.lexing;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class CurlyBracesWeaver {

    private static final String brace_open=" { ";
    private static final String brace_close=" } ";

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

        IndentedString last_line=null;

        for (int i=0;i<lines.size();i++) {

            IndentedString line = lines.get(i);

            //System.out.println("last indentation: "+last_indentation);
            //System.out.println(line);



            if (line.indentation > last_indentation) {

                if(last_line!=null && !last_line.content.trim().endsWith("{") && !line.content.trim().startsWith("{")){
                    //add "{"
                    sb.append(brace_open);
                }

            } else if (line.indentation < last_indentation) {

                if(last_line!=null && !last_line.content.trim().endsWith("}") && !line.content.trim().startsWith("}")) {
                    //add "}"
                    sb.append(brace_close);
                }
            }

            sb.append(line.content);

            last_indentation = line.indentation;
            last_line=line;
        }

        while (last_indentation>0){
            sb.append(brace_close);
            last_indentation--;
        }

        return sb.toString();
    }
}
