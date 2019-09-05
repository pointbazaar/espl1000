package org.vanautrui.languages.commandline;

import org.apache.commons.cli.*;
import org.apache.commons.io.FileUtils;
import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.lexing.utils.CharacterList;
import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

import static java.lang.System.currentTimeMillis;
import static org.fusesource.jansi.Ansi.ansi;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.*;

public class dragonc {
    //this should be the compiler

    //one thing to note would be that
    //we can include multiple stages of tokenization
    //and parsing.
    //parsing can include multiple passes and also
    //the AST and related data could undergo multiple representations
    //before reaching the final representation
    //from which code can be generated

    public static void compile_main(List<String> args) throws Exception {
        //Apache  CLI tools is just AWESOME!!
        Options options = createOptions();

        CommandLineParser parser = new DefaultParser();
        CommandLine cmd = parser.parse(options, args.toArray(new String[]{}));

        //as no option currently has an argument,
        //this simplifies the usage of the compiler
        //also, everything that doesnt start with '-' is either a source file or directory

        if(cmd.hasOption("help")){
            printHelp();
        }else {

            if(cmd.hasOption("clean")){
                if(cmd.hasOption("debug")){
                    System.out.println("clearing the cache");
                }
                final String cache_dir=System.getProperty("user.home")+"/dragoncache";
                FileUtils.deleteDirectory(Paths.get(cache_dir).toFile());
            }

            List<String> fileArgs = args.stream().filter(str -> !str.startsWith("-")).collect(Collectors.toList());

            compile_main_inner(getAllDragonFilesRecursively(fileArgs), cmd);
        }
    }

    private static List<File> getAllDragonFilesRecursively(List<String> fileArgs)throws Exception{
        //from dgc options, this can either be files or directories
        //which must be compiled together.
        List<File> results=new ArrayList<>();
        for(String s : fileArgs) {
            Path path = Paths.get(s);
            if(path.toFile().isDirectory()) {
                //add all the files recursively
                Collection<File> files = FileUtils.listFiles(
                        path.toFile(),
                        new String[]{"dg"},
                        true
                );
                results.addAll(files);
            }else {
                if (s.endsWith(".dg")) {
                    results.add(path.toFile());
                }
            }
        }
        if(results.size()==0){
            //use stdin to receive codes
            //if no files or directories are given as arguments
            //(unix philosophy)
            //throw new Exception("could not find any files with '.dg' extension.");
            //results.add(Paths.get("/dev/stdin").toFile());
        }
        return results;
    }

    public static void printHelp(){
        Options options = createOptions();
        HelpFormatter help = new HelpFormatter();
        StringBuilder sbh = new StringBuilder("");
        StringBuilder sbf = new StringBuilder("");

        sbh.append(ansi().bold().a("\ndraco - dragon compiler\n\n").reset().toString());

        sbf.append("\n\n");
        sbf.append(ansi().bold().a("EXAMPLES\n\n").reset().toString());
        sbf.append("    draco Main.dg\n");
        sbf.append("    draco -debug -ast Main.dg\n");
        sbf.append("\n");


        sbf.append(ansi().bold().a("GITHUB\n\n").reset().toString());
        sbf.append("    https://github.com/pointbazaar/dragon/\n");
        sbf.append("\n");

        sbf.append(ansi().bold().a("AUTHOR\n\n").reset().toString());
        sbf.append(
                String.format("    %-15s %-20s\n","@pointbazaar","alex23667@gmail.com")+
                String.format("    %-15s %-20s\n","@Milo-D","David.Milosevic@web.de")
        );
        sbf.append("\n");

        sbf.append(ansi().bold().a("REPORTING BUGS\n\n").reset().toString());
        sbf.append("    https://github.com/pointbazaar/dragon/issues\n\n");

        String header=sbh.toString();
        String footer=sbf.toString();
        help.printHelp("draco FILE...",header,options,footer,true);
    }

    //declaring these identifiers so that we can change the actual names of the options
    //without having to comb through all the code
    public static final String FLAG_DEBUG="debug";
    public static final String FLAG_TIMED="timed";

    public static final String FLAG_PRINT_TOKENS="tokens";
    public static final String FLAG_PRINT_AST="ast";
    public static final String FLAG_PRINT_SYMBOLTABLES="symboltables";
    public static final String FLAG_PRINT_VM_CODES="vmcodes";
    public static final String FLAG_PRINT_HELP="help";
    public static final String FLAG_PRINT_ASM="asm";

    public static final String FLAG_STRICT="strict";
    public static final String FLAG_CLEAN="clean";
    public static final String FLAG_OPTIMIZE="optimize";

    public static final String FLAG_TARGET_NATIVE="native";
    public static final String FLAG_TARGET_JVM="jvm";

    private static Options createOptions(){
        //https://commons.apache.org/proper/commons-cli/usage.html

        Options opts = new Options();

        opts.addOption(new Option(FLAG_DEBUG,false,"provides debug output for development of the compiler"));

        opts.addOption(new Option(FLAG_TIMED,false,"provides a breakdown of how long each compiler phase took"));

        opts.addOption(new Option(FLAG_PRINT_TOKENS,false,"print the tokens as json"));
        opts.addOption(new Option(FLAG_PRINT_AST,false,"print the AST as json"));
        opts.addOption(new Option(FLAG_PRINT_SYMBOLTABLES,false,"print the symbol tables"));
        opts.addOption(new Option(FLAG_PRINT_HELP,false,"display an overview of the command line options"));
        opts.addOption(new Option(FLAG_PRINT_VM_CODES,false,"outputs the vm codes generated to the console. doesnt work with jvm backend"));
        opts.addOption(new Option(FLAG_PRINT_ASM,false,"prints the assembly code generated"));

        opts.addOption(new Option(FLAG_STRICT,false,"do not compile if the code is likely to have bugs (TODO)"));

        opts.addOption(new Option(FLAG_CLEAN,false,"clears the cache"));

        opts.addOption(new Option(FLAG_OPTIMIZE,false,"try to optimize the code."));

        OptionGroup optGroup = new OptionGroup();

        optGroup.addOption(new Option(FLAG_TARGET_NATIVE,false,"compile a native executable"));
        optGroup.addOption(new Option(FLAG_TARGET_JVM,false,"compile .class files to execute on the jvm"));

        opts.addOptionGroup(optGroup);
        return opts;
    }

    private static void compile_main_inner(List<File> sources,CommandLine cmd){

        boolean debug=cmd.hasOption(FLAG_DEBUG);
        boolean timed=cmd.hasOption(FLAG_TIMED);

        long start_time_ms = currentTimeMillis();

        try {
            List<String> codes=new ArrayList<String>();
            for(File file : sources){
                String sourceCode = new String(Files.readAllBytes(file.toPath()));
                codes.add(sourceCode);
                if(debug) {
                    System.out.println(sourceCode);
                }
            }

            if(sources.size()==0){
                //use stdin to receive codes
                //if no files or directories are given as arguments
                //(unix philosophy)
                //this enables the program to better be used with other programs, piping text and outputs and such
                StringBuilder sb=new StringBuilder();
                Scanner sc = new Scanner(System.in);
                while(sc.hasNextLine()){
                    sb.append(sc.nextLine());
                }
                sc.close();
                codes.add(sb.toString());
                sources.add(Paths.get("Main.dg").toFile());
            }

            CompilerPhases phases = new CompilerPhases(cmd);

            long start,end;

            start = currentTimeMillis();
            //PHASE CLEAN
            List<CharacterList> codeWithoutCommentsWithoutUnneccesaryWhitespace
                    = phases.phase_clean(codes,sources,cmd);

            end= currentTimeMillis();
            if(timed) {
                printDuration(start, end);
            }

            start= currentTimeMillis();
            //PHASE LEXING
            List<TokenList> tokens = phases.phase_lexing(codeWithoutCommentsWithoutUnneccesaryWhitespace,cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE PARSING
            List<AST> asts = phases.phase_parsing(tokens,cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE TYPE CHECKING
            phases.phase_typecheck(asts,cmd);
            end = currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start = currentTimeMillis();
            //PHASE CODE GENERATION
            List<Path> classFilePaths = phases.phase_codegeneration(asts, cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            long end_time_ms = currentTimeMillis();
            long duration = end_time_ms-start_time_ms;

            //https://www.utf8icons.com/
            if(timed) {
                printDurationFeedback(duration);
            }
            {
                //System.out.println();
                Ansi ansi1= ansi();
                ansi1.cursorToColumn(0);
                ansi1.eraseLine(Ansi.Erase.FORWARD);
                ansi1.reset();
                System.out.print(ansi1);
                System.out.flush();
                printBuildConclusion(true);

            }

        } catch (Exception e) {

            System.err.println(e.getMessage());
            if(cmd.hasOption(FLAG_DEBUG)) {
                //only print the stack trace for
                //compiler developers
                e.printStackTrace();
            }
            printBuildConclusion(false);
        }
    }
}
