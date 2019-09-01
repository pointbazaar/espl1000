package org.vanautrui.languages.commandline;

import org.apache.commons.cli.*;
import org.apache.commons.io.FileUtils;
import org.fusesource.jansi.Ansi;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.stream.Collectors;

import static java.lang.System.currentTimeMillis;
import static org.fusesource.jansi.Ansi.ansi;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhaseUtils.*;
import static org.vanautrui.languages.commandline.compilerphases.CompilerPhases.*;

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

        if(cmd.hasOption("clean")){
            if(cmd.hasOption("debug")){
                System.out.println("clearing the cache");
            }
            final String cache_dir=System.getProperty("user.home")+"/dragoncache";
            FileUtils.deleteDirectory(Paths.get(cache_dir).toFile());
        }

        if(cmd.hasOption("help")){
            printHelp();
            return;
        }

        List<String> fileArgs = args.stream().filter(str->!str.startsWith("-")).collect(Collectors.toList());

        compile_main_inner(getAllDragonFilesRecursively(fileArgs),cmd);
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
            throw new Exception("could not find any files with '.dg' extension.");
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


    private static Options createOptions(){
        //https://commons.apache.org/proper/commons-cli/usage.html

        Options opts = new Options();
        Option option_debug = new Option("debug",false,"provides debug output for development of the compiler");
        opts.addOption(option_debug);

        opts.addOption(
                new Option("timed",false,"provides a breakdown of how long each compiler phase took")
        );

        opts.addOption(new Option("tokens",false,"print the tokens as json"));
        opts.addOption(new Option("ast",false,"print the AST as json"));
        opts.addOption(new Option("symboltables",false,"print the symbol tables"));

        opts.addOption(new Option("help",false,"display an overview of the command line options"));

        opts.addOption(new Option("strict",false,"do not compile if the code is likely to have bugs (TODO)"));

        opts.addOption(new Option("clean",false,"clears the cache"));

        opts.addOption(new Option("run",false,"execute the program after compilation"));

        opts.addOption(
                new Option(
                    "optimize",
                    false,
                    "try to optimize the code. optimization effort goes from 0 (no optimization) to 10 (maximum optimization) (TODO)"
                    )
        );


        OptionGroup optionGroup = new OptionGroup();

        optionGroup.addOption(new Option("nocurly",false,"accept only indentation for scopes (TODO)"));
        optionGroup.addOption(new Option("curly",false,"accept only curly braces  for scopes (TODO)"));

        OptionGroup optGroup = new OptionGroup();

        optGroup.addOption(new Option("targetnative",false,"compile a native executable"));
        optGroup.addOption(new Option("targetjvm",false,"compile .class files to execute on the jvm"));

        opts.addOptionGroup(optGroup);
        opts.addOptionGroup(optionGroup);
        return opts;
    }

    private static void compile_main_inner(List<File> sources,CommandLine cmd){

        boolean debug=cmd.hasOption("debug");
        boolean timed=cmd.hasOption("timed");

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

            long start,end;

            start = currentTimeMillis();
            //PHASE CLEAN
            List<CharacterList> codeWithoutCommentsWithoutUnneccesaryWhitespace
                    = phase_clean(codes,sources,cmd);

            end= currentTimeMillis();
            if(timed) {
                printDuration(start, end);
            }

            //below code is on hold until i have some time for it
            /*
            List<String> just_code_with_braces_without_comments_without_newlines;

            if(cmd.hasOption("nocurly")){
                just_code_with_braces_without_comments_without_newlines
                        =phase_conditional_weave_curly_braces(codeWithoutCommentsWithoutUnneccesaryWhitespace,cmd);
            }else{
                //the editor is curly by default
                just_code_with_braces_without_comments_without_newlines
                        =codeWithoutCommentsWithoutUnneccesaryWhitespace;
            }

             */

            start= currentTimeMillis();
            //PHASE LEXING
            List<TokenList> tokens = phase_lexing(codeWithoutCommentsWithoutUnneccesaryWhitespace,cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE PARSING
            List<AST> asts = phase_parsing(tokens,cmd);
            end= currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start= currentTimeMillis();
            //PHASE TYPE CHECKING
            phase_typecheck(asts,cmd);
            end = currentTimeMillis();
            if(timed){
                printDuration(start,end);
            }

            start = currentTimeMillis();
            //PHASE CODE GENERATION
            List<Path> classFilePaths = phase_codegeneration(asts, cmd);
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

            if(cmd.hasOption("run")){
                //TODO: support running the native executable

                //execute
                phase_run_optional(classFilePaths,cmd);
            }

        } catch (Exception e) {

            System.err.println(e.getMessage());
            if(cmd.hasOption("debug")) {
                //only print the stack trace for
                //compiler developers
                e.printStackTrace();
            }
            printBuildConclusion(false);
        }
    }
}
