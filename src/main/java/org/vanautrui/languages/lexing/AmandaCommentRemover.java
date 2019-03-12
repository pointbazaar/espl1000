package org.vanautrui.languages.lexing;

public class AmandaCommentRemover {
    public String strip_comments(String sourcecode) throws Exception {
        StringBuilder result=new StringBuilder();

        result.append(
                strip_single_line_comments(
                        strip_multi_line_comments(sourcecode)
                )
        );

        System.out.println("source code after removing comments:");
        System.out.println(result.toString());

        return result.toString();
    }

    public String strip_single_line_comments(String sourcecode){
        StringBuilder result = new StringBuilder();

        for(String line : sourcecode.split("\n")){
            if(line.contains("//")){
                result.append(line.substring(0,line.indexOf("//")));
            }else{
                result.append(line);
            }
            result.append("\n");
        }
        return result.toString();
    }

    public String strip_multi_line_comments(String sourcecode) throws Exception{
        StringBuilder result=new StringBuilder();
        int comment_depth=0;
        int i=0;

        while(i<sourcecode.length()){
            char c=sourcecode.charAt(i);
            char c_next='x';
            boolean has_c_next=i<sourcecode.length()-1;
            if(has_c_next){
                c_next=sourcecode.charAt(i+1);
            }


            //we look for comment start
            if(c=='/' && has_c_next && c_next=='*'){
                //comment start found
                i++;
                comment_depth++;
            }else if(comment_depth==0){
                result.append(c);

            }else {
                //we look for comment end
                if(c=='*' && has_c_next && c_next=='/'){
                    //end of comment found
                    i++;
                    comment_depth--;
                }
            }
            i++;

        }

        if(comment_depth!=0){
            throw new Exception("A Multiline Comment is not formed correctly");
        }

        return result.toString();
    }
}
