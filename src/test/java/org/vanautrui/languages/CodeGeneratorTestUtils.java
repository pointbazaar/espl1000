package org.vanautrui.languages;

import org.vanautrui.languages.commandline.CompilerPhases;
import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public final class CodeGeneratorTestUtils {

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


    public static Process compile_and_run_but_not_waitFor(String dragon_source,String filename_without_extension) throws Exception{
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

        final TokenList tokens = ParserPhases.makeTokenList(source);
        final Parser parser = new Parser();
        final AST ast= parser.parseTestMode(tokens,false);
        final List<AST> asts = new ArrayList<>();
        asts.add(ast);
        //we are in debug mode since we are running tests

        CompilerPhases phases = new CompilerPhases();

        return phases.phase_vm_codegeneration(asts,false);
    }

    private static void generateFromVMCodeAndWriteExecutable(List<Path> vmcodes, Path filename) throws IOException, InterruptedException {
        //writes an executable with the name we requested

        final String call = "dracovm -o "+filename.toString()+" "+vmcodes.stream().map(p->p.toString()).collect(Collectors.joining(" "));
        System.out.println(call);

        final Process process = Runtime.getRuntime().exec(call);
        process.waitFor();

    }

    private static Process compile_and_run_vmcodes_but_not_waitFor(final List<Path> vmcode_paths, final Path filename_without_extension, String[] args) throws Exception{

        generateFromVMCodeAndWriteExecutable(vmcode_paths,filename_without_extension);

        final String call = "./"+filename_without_extension+" "+ String.join(" ", Arrays.asList(args));
        System.out.println(call);

        Process pr = Runtime.getRuntime().exec(call);
        return pr;
    }

    private static Process compile_and_run_vm_codes(final List<Path> vmcode_paths, final Path filename_without_extension, final String[] args) throws Exception{

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

            //delete the assembly file
            final Path asm_path = Paths.get(filename_without_extension + "_main.asm");
            System.out.println("delete: " + asm_path.toString());
            Files.delete(asm_path);
        }

        //TODO: there could be multiple assembly files, as the dracovm compiler becomes incremental
        //TODO: delete those aswell, as we are in a test environment

        return pr;
    }
}
