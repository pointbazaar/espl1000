package org.vanautrui.languages;

import org.vanautrui.languages.commandline.CompilerPhases;
import org.vanautrui.languages.commandline.DragonCompiler;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public final class TestUtils {

    public static AST_Whole_Program parse_for_test(final String sourceCode) throws Exception {
        final boolean debug=true;

        //Write to file
        final String filename = "Main.dg";
        Files.writeString(Paths.get(filename),sourceCode);

        //invoke dragon-lexer
        DragonCompiler.invokeDragonLexer(Paths.get(filename).toFile(),debug);

        //invoke dragon-parser
        DragonCompiler.invokeDragonParser(Paths.get("."+filename+".tokens").toFile(),debug);

        final ArrayList<File> files=new ArrayList<>();
        
        return DragonCompiler.parseAST_from_JSON(files,debug);
    }

    public static Process compile_and_run_program_for_testing_with_cmd_args(String dragon_source_code, String filename_without_extension, String[] args) throws Exception {
        //gets a dragon source code, compiles to vm code, calls dracovm, and starts the executable

        //should create the .dracovm files for it
        final List<Path> vmcodes =
                generateVMCodeFromDragonCode(
                        dragon_source_code
                );

        //should create the executable and run it
        final Process pr = compile_and_run_vm_codes(
                vmcodes,
                Paths.get(filename_without_extension,args),
                args
        );
        return pr;
    }


    public static Process compile_and_run_but_not_waitFor(
            final String dragon_source,final String filename_without_extension
    ) throws Exception{
        final Path filename_without_extns = Paths.get(filename_without_extension);

        //generate the vm code
        final List<Path> paths = generateVMCodeFromDragonCode(dragon_source);

        //generate the executable
        generateFromVMCodeAndWriteExecutable(paths,filename_without_extns);

        //run that executable
        final Process pr = Runtime.getRuntime().exec("./"+filename_without_extension);
        return pr;
    }


    private static List<Path> generateVMCodeFromDragonCode(final String source) throws Exception{
        //generates vm codes from dragon codes, and writes them to files. returns paths to those files

        final List<File> files = new ArrayList<>();
        final String filename = "Main.dg";
        final boolean debug=false;

        //write dragon code to file
        Files.writeString(Paths.get(filename),source);

        //invoke dragon-lexer
        DragonCompiler.invokeDragonLexer(Paths.get(filename).toFile(),debug);

        final File tokensFile = Paths.get("."+filename+".tokens").toFile();

        //invoke dragon-parser
        DragonCompiler.invokeDragonParser(tokensFile,false);

        files.add(Paths.get(".Main.dg.json").toFile());

        final AST_Whole_Program ast = DragonCompiler.parseAST_from_JSON(files,false);
        //we are in debug mode since we are running tests

        return CompilerPhases.phase_vm_codegeneration(ast,false,false);
    }

    private static void generateFromVMCodeAndWriteExecutable(
            final List<Path> vmcodes, final Path filename
    ) throws Exception {
        //writes an executable with the name we requested

        //dracovm only accepts filenames as arguments
        final boolean debug=true;

        DragonCompiler.invokeDracoVMCompiler(vmcodes,debug);


        //move our 'main' executable into the desired filename
        Runtime.getRuntime().exec("mv main "+filename.toString()).waitFor();

    }

    private static Process compile_and_run_vmcodes_but_not_waitFor(
            final List<Path> vmcode_paths, final Path filename_without_extension, String[] args
    ) throws Exception{

        generateFromVMCodeAndWriteExecutable(vmcode_paths,filename_without_extension);

        final String call = "./"+filename_without_extension+" "+ String.join(" ", Arrays.asList(args));
        System.out.println(call);

        Process pr = Runtime.getRuntime().exec(call);
        return pr;
    }

    private static Process compile_and_run_vm_codes(
            final List<Path> vmcode_paths,
            final Path filename_without_extension,
            final String[] args
    ) throws Exception{

        final Process pr = compile_and_run_vmcodes_but_not_waitFor(
                vmcode_paths,
                filename_without_extension,
                args
        );

        pr.waitFor();

        //for debugging purposes
        final boolean deleteArtifacts = false;

        if(deleteArtifacts) {

            //delete all the .dracovm  files that have been created
            for (Path dracovmfilepath : vmcode_paths) {
                System.out.println("delete: " + dracovmfilepath);
                Files.delete(dracovmfilepath);
            }

            //delete the executable
            System.out.println("delete: " + filename_without_extension);
            Files.delete(filename_without_extension);

            //TODO: delete the assembly files of the subroutines

        }

        //TODO: there could be multiple assembly files, as the dracovm compiler becomes incremental
        //TODO: delete those aswell, as we are in a test environment

        return pr;
    }
}
