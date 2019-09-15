package org.vanautrui.languages;

import org.vanautrui.languages.commandline.dragonc;

import java.util.Arrays;

public class App {

    //https://dzone.com/articles/generating-bytecode

    //public static final String lang_name="Dragon ";
    public static final String lang_name = "DRAGON ";

    public static void main(String[] args) {
        //https://www.javassist.org/
        //we could use this dependency to make our classes

        //https://www.javassist.org/tutorial/tutorial.html
        //https://www.beyondjava.net/blog/quick-guide-writing-byte-code-asm
        //https://asm.ow2.io/
        //https://asm.ow2.io/asm4-guide.pdf

        //https://www.youtube.com/watch?v=ubXXmQzzNGo

        try {
            dragonc.compile_main(Arrays.asList(args));
            System.exit(0);
        }catch (Exception e){
            //e.printStackTrace();
            System.err.println(e.getMessage());
            System.err.println("-help     for information about command line arguments");
            System.exit(1);
        }
    }

}
