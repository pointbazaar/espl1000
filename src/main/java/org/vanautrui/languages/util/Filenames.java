package org.vanautrui.languages.util;

public final class Filenames {

    //this file mirrors the subrotines in dracovm-compiler/src/sources/dracovmcompiler.hpp

    //so that the files do not show up with 'ls'
    //i would like to make them hidden.
    //they should still be in the directory where the sources are
    //being compiled, so users can see what is going on and debug

    public static String asm_filename(final String filename){
        return "."+filename+".asm";
    }

    public static String obj_filename(final String filename){
        return "."+filename+".o";
    }
}
