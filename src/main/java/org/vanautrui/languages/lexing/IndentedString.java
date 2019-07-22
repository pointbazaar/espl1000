package org.vanautrui.languages.lexing;

public class IndentedString {

    private static final String four_spaces = "    ";

    public int indentation=0;

    public String content;

    private IndentedString(int indentation,String content) throws Exception{

        if (indentation<0){
            throw new Exception("code cannot be indented negatively");
        }

        this.indentation=indentation;
        this.content=content;
    }

    public IndentedString(String content) throws Exception{

        //TODO: figure out the indentation
        //indentation is counted in tabs, or 4 spaces.
        int tabs_count=0;
        int four_spaces_count=0;



        while (content.startsWith("\t") || content.startsWith(four_spaces)){
            if(content.startsWith("\t")) {
                tabs_count++;
                content = content.substring(1);
            }else if(content.startsWith(four_spaces)){
                four_spaces_count++;
                content = content.substring(4);
            }
        }

        this.indentation=tabs_count+four_spaces_count;
        this.content=content;
    }

}
