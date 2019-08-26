package org.vanautrui.languages.codegeneration;

import org.vanautrui.languages.lexing.Lexer;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.Parser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;

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
        byte[] result = JavaByteCodeGenerator.generateByteCodeForOneClass(ast.classNodeList.toArray(new ClassNode[]{})[0],true);
        Path path = Paths.get(classNameWithoutExtension+".class");
        Files.write(path,result);
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
