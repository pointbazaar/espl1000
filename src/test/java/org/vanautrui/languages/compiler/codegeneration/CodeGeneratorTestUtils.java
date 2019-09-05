package org.vanautrui.languages.compiler.codegeneration;

import org.vanautrui.languages.commandline.CompilerPhases;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.stream.Collectors;

public class CodeGeneratorTestUtils {

    private static Path generateCodeAndWriteFile(String source,String classNameWithoutExtension)throws Exception{
        TokenList tokens = (new Lexer()).lexCodeTestMode(source);
        Parser parser = new Parser();
        AST ast= parser.parseTestMode(tokens);

        //we are in debug mode since we are running tests
        //TODO
        CompilerPhases phases = new CompilerPhases();
        //TODO

        Path path = Paths.get(classNameWithoutExtension+".class");
        Files.write(path,new byte[0]);
        return path;
    }

    public static Process compile_and_run_but_not_waitFor(String source,String classNameWithoutExtension) throws Exception{
        generateCodeAndWriteFile(source,classNameWithoutExtension);
        Process pr = Runtime.getRuntime().exec("java "+classNameWithoutExtension);

        return pr;
    }

    public static Process compile_and_run_one_class_for_testing(String source, String classNameWithoutExtension) throws Exception{

        Path path = Paths.get(classNameWithoutExtension+".class");

        Process pr = compile_and_run_but_not_waitFor(source,classNameWithoutExtension);

        pr.waitFor();
        Files.delete(path);

        return pr;
    }

    public static Process compile_and_run_one_class_for_testing_with_cmd_args(String src,String classNameWithoutExtension,String[] args) throws Exception {
        Path path = generateCodeAndWriteFile(src, classNameWithoutExtension);
        Process pr = Runtime.getRuntime().exec("java "+classNameWithoutExtension+" "+ Arrays.asList(args).stream().collect(Collectors.joining(" ")));

        pr.waitFor();
        Files.delete(path);

        return pr;
    }
}
