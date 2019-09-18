package org.vanautrui.languages;

import org.vanautrui.languages.commandline.dragonc;

import java.util.Arrays;

public class App {

    //Assembly debugging:
    //(they are missing a stack display, but it be ok)
    //https://carlosrafaelgn.com.br/Asm86/

    //general having fun and trying different programming languages:
    //https://tio.run/#
    //https://www.tutorialspoint.com/codingground.htm

    public static final String lang_name = "DRAGON ";

    public static void main(String[] args) {

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
