package org.vanautrui.languages.commandline;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.HashSet;

import static java.nio.file.attribute.PosixFilePermission.*;
import static java.nio.file.attribute.PosixFilePermission.OTHERS_READ;

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

    public static void giveAllPermissionsOnFile(final Path path) throws IOException {
        Files.setPosixFilePermissions(
                path,
                new HashSet<>(
                        Arrays.asList(
                                OWNER_WRITE,GROUP_WRITE,OTHERS_WRITE,
                                OWNER_READ,GROUP_READ,OTHERS_READ,
                                OWNER_EXECUTE,GROUP_EXECUTE,OTHERS_EXECUTE
                        )
                )
        );
    }
}
