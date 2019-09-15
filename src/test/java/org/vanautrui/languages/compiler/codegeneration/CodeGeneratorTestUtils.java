package org.vanautrui.languages.compiler.codegeneration;

import org.vanautrui.languages.commandline.CompilerPhases;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class CodeGeneratorTestUtils {

    private static Path generateCodeAndWriteExecutable(String source, String filename)throws Exception{
        TokenList tokens = (new Lexer()).lexCodeTestMode(source);
        Parser parser = new Parser();
        AST ast= parser.parseTestMode(tokens);
        List<AST> asts = new ArrayList<>();
        asts.add(ast);
        //we are in debug mode since we are running tests

        CompilerPhases phases = new CompilerPhases();
        List<String> vmcodes = phases.phase_vm_codegeneration(asts, false);
        List<String> asm_codes = phases.phase_vm_code_compilation(vmcodes, false);
        Path path = phases.phase_generate_executable(asm_codes,filename);

        return path;
    }

    public static Process compile_and_run_but_not_waitFor(String source,String filename_without_extension) throws Exception{
        generateCodeAndWriteExecutable(source,filename_without_extension);
        Process pr = Runtime.getRuntime().exec("./"+filename_without_extension);
        return pr;
    }

    public static Process compile_and_run_one_class_for_testing(String source, String filename_without_extension) throws Exception{

        return compile_and_run_one_class_for_testing_with_cmd_args(source,filename_without_extension,new String[0]);
    }

    public static Process compile_and_run_one_class_for_testing_with_cmd_args(String src,String filename_without_extension,String[] args) throws Exception {
        Path path = generateCodeAndWriteExecutable(src, filename_without_extension);
        Process pr = Runtime.getRuntime().exec("java "+filename_without_extension+" "+ Arrays.asList(args).stream().collect(Collectors.joining(" ")));

        pr.waitFor();
        Files.delete(path);
        Files.delete(Paths.get(filename_without_extension+".asm"));

        return pr;
    }
}
