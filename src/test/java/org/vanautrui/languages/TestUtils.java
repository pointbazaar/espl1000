package org.vanautrui.languages;

import org.vanautrui.languages.commandline.CompilerPhases;
import org.vanautrui.languages.commandline.DragonCompiler;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;

import java.io.File;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import static java.lang.System.out;

public final class TestUtils {

    public static AST_Whole_Program parse_for_test(
            final String sourceCode,
            final boolean debug
    ) throws Exception {

        if (debug){
            out.println("TestUtils::parse_for_test");
        }

        //Write to file
        final Path filename = Paths.get("Main.dg");

        if(debug){
            out.println("write to "+filename);
        }
        Files.writeString(filename,sourceCode);

        //invoke dragon-lexer
        DragonCompiler.invokeDragonLexer(filename.toFile(),debug);

        //we now have the token file. can delete the source file
        if(debug){ out.println("delete "+filename); }
        Files.delete(filename);

        //invoke dragon-parser
        final File tokensFile = Paths.get("."+filename+".tokens").toFile();
        DragonCompiler.invokeDragonParser(tokensFile,debug);

        //we now have the .json file. can delete the .tokens file now
        if(debug){ out.println("delete "+tokensFile.toPath()); }
        Files.delete(tokensFile.toPath());

        final ArrayList<File> files=new ArrayList<>();
        final File astFile = Paths.get("."+filename+".ast").toFile();

        files.add(astFile);

        final AST_Whole_Program ast = DragonCompiler.parseASTFromASTFiles(files, debug);

        //we have the AST now. we can delete the .json file now
        if(debug){
            out.println("delete "+astFile.toPath());
        }
        Files.delete(astFile.toPath());

        return ast;
    }

    public static Process compileAndRunProgramForTesting(
            final String sourceCode,
            final String filename_without_extension,
            final String[] args,
            final boolean debug
    ) throws Exception {
        //gets a dragon source code, compiles to vm code, calls dracovm, and starts the executable

        final Path filename = Paths.get(filename_without_extension+".dg");

        if(debug){ out.println("TestUtils::compileAndRunProgramForTesting"); }

        final List<Path> javaFilePaths = generateJavaCodeFromDragonCode(sourceCode, debug, filename.toFile());

        //should create the executable and run it
        return compile_and_run_vm_codes(
                javaFilePaths,
                Paths.get(filename.toString(),args),
                args,
                debug
        );
    }


    public static Process compile_and_run_but_not_waitFor(
            final String dragon_source,
            final String filename_without_extension,
            final boolean debug
    ) throws Exception{
        if(debug){
            out.println(String.format("compile_and_run_but_not_waitfor(...,%s,%b)",filename_without_extension,debug));
        }

        final Path filename_without_extns = Paths.get(filename_without_extension);

        //generate code from smalldragon source code
        final List<Path> paths = generateJavaCodeFromDragonCode(dragon_source, debug, Paths.get(filename_without_extension+".dg").toFile());

        //compile generated code
        generateFromJavaCodeAndWriteExecutable(paths,filename_without_extns,debug);

        //run that
        return Runtime.getRuntime().exec("./"+filename_without_extension);
    }

    private static List<Path> generateJavaCodeFromDragonCode(
            final String source,
            final boolean debug,
            final File filename
    ) throws Exception{
        if(debug){
            out.println(String.format("TestUtils::generateJavaCodeFromDragonCode(..., debug = %b, %s)",debug, filename.toString()));
        }
        //generates java codes from dragon codes, and writes them to files. returns paths to those files

        //write dragon code to file
        if(debug){ out.println("write to "+filename); }

        //set appropriate permission if file exists
        if(Files.exists(filename.toPath())){
            Files.delete(filename.toPath());
        }
        Files.writeString(filename.toPath(),source);

        //invoke dragon-lexer
        DragonCompiler.invokeDragonLexer(filename,debug);
        final File tokensFile = Paths.get("."+filename+".tokens").toFile();

        //invoke dragon-parser
        DragonCompiler.invokeDragonParser(tokensFile,debug);

        final File astFile = Paths.get("."+filename+".ast").toFile();

        final AST_Whole_Program ast = DragonCompiler.parseASTFromASTFiles(Arrays.asList(astFile),debug);

        return CompilerPhases.phase_java_codegeneration(ast,false,debug);
    }

    private static void generateFromJavaCodeAndWriteExecutable(
            final List<Path> javaCodes,
            final Path filename,
            final boolean debug
    ) throws Exception {
        if(debug){
            out.println(String.format("TestUtils::generateFromJavaCodeAndWriteExecutable(%s,%s,%b)",javaCodes.stream().map(Path::toString).collect(Collectors.joining(" ")),filename.toString(),debug));
        }
        //writes an executable with the name we requested
        DragonCompiler.invokeJavaCompiler(javaCodes,debug);
        //move our 'main' executable into the desired filename
        final String call = "mv main "+filename.toString();
        if(debug){ out.println(call); }
        Runtime.getRuntime().exec(call).waitFor();
    }

    private static Process compile_and_run_vmcodes_but_not_waitFor(
            final List<Path> javaCodePaths,
            final Path filename,
            final String[] args,
            final boolean debug
    ) throws Exception{
        generateFromJavaCodeAndWriteExecutable(javaCodePaths,filename,debug);
        //do we have the correct permissions to run the file?
        final String call = "java "+filename+" "+ String.join(" ", Arrays.asList(args));
        if(debug) { out.println(call); }
        return Runtime.getRuntime().exec(call);
    }

    private static Process compile_and_run_vm_codes(
            final List<Path> vmcode_paths,
            final Path filename,
            final String[] args,
            final boolean debug
    ) throws Exception{

        final Process pr = compile_and_run_vmcodes_but_not_waitFor(
                vmcode_paths,
                filename,
                args,
                debug
        );
        pr.waitFor();

        //for debugging purposes we can keep the artifacts to look at them later.
        final boolean deleteArtifacts = !debug;
        if(deleteArtifacts) {

            for (final Path path : vmcode_paths) {
                out.println("delete: " + path);
                Files.delete(path);
            }
            //delete the executable
            out.println("delete: " + filename);
            Files.delete(filename);
        }
        return pr;
    }
}
