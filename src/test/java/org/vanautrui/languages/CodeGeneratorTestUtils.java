package org.vanautrui.languages;

import org.vanautrui.languages.commandline.CompilerPhases;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class CodeGeneratorTestUtils {

    public static List<String> generateVMCodeFromDragonCode(String source,String filename) throws Exception{
        TokenList tokens = (new Lexer()).lexCodeTestMode(source);
        Parser parser = new Parser();
        AST ast= parser.parseTestMode(tokens,false);
        List<AST> asts = new ArrayList<>();
        asts.add(ast);
        //we are in debug mode since we are running tests

        CompilerPhases phases = new CompilerPhases();
        List<String> vmcodes = phases.phase_vm_codegeneration(asts, filename,false,false);
        return vmcodes;
    }

    public static Path generateCodeAndWriteExecutable(String source, String filename)throws Exception{
        return generateFromVMCodeAndWriteExecutable(generateVMCodeFromDragonCode(source,filename),filename);
    }

    public static Path generateFromVMCodeAndWriteExecutable(List<String> vmcodes,String filename)throws Exception{
        Path path = (new VMCompilerPhases()).compile_vm_codes_and_generate_executable(vmcodes,filename);

        return path;
    }

    public static Process compile_and_run_but_not_waitFor(String source,String filename_without_extension) throws Exception{
        generateCodeAndWriteExecutable(source,filename_without_extension);

        Process pr = Runtime.getRuntime().exec("./"+filename_without_extension);
        return pr;
    }

    public static Process compile_and_run_vmcodes_but_not_waitFor(List<String> vmcodes,String filename_without_extension) throws Exception{
        generateFromVMCodeAndWriteExecutable(vmcodes,filename_without_extension);
        Process pr = Runtime.getRuntime().exec("./"+filename_without_extension);
        return pr;
    }

    public static Process compile_and_run_vm_codes_for_testing(List<String> vmcodes,String filename_without_extension) throws Exception{

        Process pr = compile_and_run_vmcodes_but_not_waitFor(vmcodes,filename_without_extension);

        pr.waitFor();
        Files.delete(Paths.get(filename_without_extension));
        Path vmpath = Paths.get(filename_without_extension + ".dracovm");
        if(Files.exists(vmpath)) {
            Files.delete(vmpath);
        }
        Files.delete(Paths.get(filename_without_extension+".asm"));

        return pr;
    }

    public static Process compile_and_run_program_for_testing(String source, String filename_without_extension) throws Exception{
        return compile_and_run_program_for_testing_with_cmd_args(source,filename_without_extension,new String[0]);
    }

    public static Process compile_and_run_program_for_testing_with_cmd_args(String src, String filename_without_extension, String[] args) throws Exception {
        List<String> vmcodes = generateVMCodeFromDragonCode(src,filename_without_extension);
        Process pr = compile_and_run_vm_codes_for_testing(vmcodes,filename_without_extension);
        return pr;
    }
}
