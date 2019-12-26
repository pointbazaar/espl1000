package org.vanautrui.languages.commandline;

public final class CompilerPhaseUtils {

    //prevents instance creation
    private CompilerPhaseUtils(){}

    static void printBuildConclusion(final boolean success){
        if(success) {
            System.out.println("BUILD SUCCESS");
        }else {
            System.out.println("BUILD FAILURE");
        }
    }

    static void printDurationFeedback(final long duration_millis){
        final String str = duration_millis + " ms";
        if(duration_millis>500) {
            System.out.println("☠ "+str+" Compilation took too long. This needs to be fixed. Please file an Issue on GitHub.");
        }else if(duration_millis>200) {
            System.out.println("☠ "+str+" we are truly sorry for the delay :(");
        }else if(duration_millis>100){
            System.out.println("✝ "+str+" sorry it took so long!");
        }else {
            System.out.println("☕ " + str);
        }
    }
}
