package org.vanautrui.languages.commandline;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import org.apache.commons.cli.CommandLine;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.codegeneration.dracovmbackend.DracoVMCodeGenerator;
import org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler.DracoVMCompiler;
import org.vanautrui.languages.codegeneration.jvmbackend.JavaByteCodeGenerator;
import org.vanautrui.languages.lexing.Lexer;
import org.vanautrui.languages.lexing.utils.CharacterList;
import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.parsing.Parser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.phase_clean_the_input.CommentRemoverAndWhitespaceRemover;
import org.vanautrui.languages.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.typechecking.TypeChecker;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.stream.Collectors;

import static org.fusesource.jansi.Ansi.Color.RED;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printBeginPhase;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printEndPhase;
import static org.vanautrui.languages.commandline.dragonc.*;
import static org.vanautrui.languages.phase_clean_the_input.CommentRemoverAndWhitespaceRemover.remove_unneccessary_whitespace;
import static org.vanautrui.languages.symboltablegenerator.SymbolTableGenerator.createSubroutineSymbolTable;

public class CompilerPhases {

    private final boolean debug;
    private final boolean timed;
    private final boolean printLong;

    public CompilerPhases(CommandLine cmd){
        this.debug=cmd.hasOption(FLAG_DEBUG);
        this.timed=cmd.hasOption(FLAG_TIMED);
        this.printLong=debug||timed;
    }

    public void phase_typecheck(List<AST> asts, CommandLine cmd)throws Exception{
        printBeginPhase("TYPE CHECKING",printLong);

        //this should throw an exception, if it does not typecheck
        try {
            TypeChecker typeChecker=new TypeChecker();
            typeChecker.doTypeCheck(asts);

            //TerminalUtil.println("✓", Ansi.Color.GREEN);
            printEndPhase(true,printLong);
        }catch (Exception e){
            //TerminalUtil.println("⚠", RED);
            printEndPhase(true,printLong);
            throw e;
        }
    }

    public List<Path> phase_codegeneration(List<AST> asts, CommandLine cmd)throws Exception{
        printBeginPhase("CODE GENERATION",printLong);

        List<Path> generatedFilesPaths=new ArrayList<>();

        try {
            if(cmd.hasOption(FLAG_TARGET_NATIVE)){
                SubroutineSymbolTable subTable = createSubroutineSymbolTable(new HashSet<>(asts));
                List<String> dracoVMCodes = DracoVMCodeGenerator.generateDracoVMCode(new HashSet<>(asts), subTable);

                if(cmd.hasOption(FLAG_PRINT_VM_CODES) || cmd.hasOption(FLAG_DEBUG)){
                    System.out.println("GENERATED VM CODES");
                    dracoVMCodes.stream().forEach(str-> System.out.println(str));
                    System.out.println();
                }

                final List<String> assembly_codes = DracoVMCompiler.compileVMCode(dracoVMCodes);
                //$ nasm -f elf hello.asm  # this will produce hello.o ELF object file
                //$ ld -s -o hello hello.o # this will produce hello executable

                final String asm_codes = (assembly_codes
                        .stream()
                        .collect(Collectors.joining("\n"))+"\n");

                if(cmd.hasOption(FLAG_PRINT_ASM)){
                    System.out.println(asm_codes);
                }

                if(debug){
                    System.out.println("call nasm");
                }

                String filename = "main";
                String asm_file_name = filename+".asm";
                Files.write(Paths.get(asm_file_name),asm_codes.getBytes());

                Process p = Runtime.getRuntime().exec("nasm -f elf " + asm_file_name);
                p.waitFor();

                if(p.exitValue()!=0){
                    throw new Exception("nasm exit with nonzero exit code");
                }

                Process p2 = Runtime.getRuntime().exec("ld -melf_i386 -s -o "+filename+" "+asm_file_name+".o");

                if(p2.exitValue() != 0){
                    throw new Exception("ld exit with nonzero exit code");
                }

                //add generated executable to generatedFilesPaths
                generatedFilesPaths.add(Paths.get(filename));

            }else if(cmd.hasOption(FLAG_TARGET_JVM) || true){
                //targetjvm is the default option currently
                SubroutineSymbolTable subroutineSymbolTable = createSubroutineSymbolTable(new HashSet<>(asts));

                for (AST ast : asts) {
                    for (ClassNode classNode : ast.classNodeList) {

                        if (debug || cmd.hasOption(FLAG_PRINT_SYMBOLTABLES)) {
                            System.out.println(subroutineSymbolTable.toString());
                        }

                        //generate bytecode for that class
                        byte[] classResult = JavaByteCodeGenerator.generateByteCodeForClass(classNode, subroutineSymbolTable, debug);

                        //System.out.println(ast.srcPath.toAbsolutePath().getParent());
                        String dir = ast.srcPath.toAbsolutePath().getParent().toString();
                        Path classFilePath = Paths.get(dir + "/" + classNode.name.typeName + ".class");
                        Files.write(classFilePath, classResult);
                        generatedFilesPaths.add(classFilePath);
                    }
                }
            }
            printEndPhase(true,printLong);
            return generatedFilesPaths;

        }catch (Exception e){
            printEndPhase(false,printLong);
            throw e;
        }
    }

    private Path makeCleanPhaseCacheFilePathFromHash(int hash){
        final String extension = ".dragon.cleaned";
        //hidden file. important, so that it does not be visible and bother people
        return Paths.get(phase_clean_cache_dir+"."+hash+extension);
    }

    private static final String phase_clean_cache_dir=System.getProperty("user.home")+"/.dragoncache/clean/";

    public List<CharacterList> phase_clean(List<String> sources, List<File> sourceFiles, CommandLine cmd)throws Exception{

        printBeginPhase("CLEAN",printLong);
        //(remove comments, empty lines, excess whitespace)
        List<CharacterList> results=new ArrayList();

        for(int i=0;i<sources.size();i++){
            String source=sources.get(i);
            if(!Files.exists(Paths.get(phase_clean_cache_dir))){
                Files.createDirectories(Paths.get(phase_clean_cache_dir));
            }

            int hash = source.hashCode();
            if(debug) {
                System.out.println("phase clean: Hashcode of source string: " + hash);
            }
            boolean foundCachedCleanedFile = false;

            if(Files.exists(makeCleanPhaseCacheFilePathFromHash(hash))){
                foundCachedCleanedFile=true;
            }

            String codeWithoutCommentsWithoutUnneccesaryWhitespace;

            if(foundCachedCleanedFile){
                if(debug) {
                    System.out.println("found a cached version that is already cleaned");
                }
                codeWithoutCommentsWithoutUnneccesaryWhitespace = new String(Files.readAllBytes(makeCleanPhaseCacheFilePathFromHash(hash)));
            }else {

                String codeWithoutCommentsAndWithoutEmptyLines = (new CommentRemoverAndWhitespaceRemover()).strip_all_comments_and_empty_lines(source);

                codeWithoutCommentsWithoutUnneccesaryWhitespace =
                        remove_unneccessary_whitespace(codeWithoutCommentsAndWithoutEmptyLines);

                //write file for caching
                Files.write(makeCleanPhaseCacheFilePathFromHash(hash),codeWithoutCommentsWithoutUnneccesaryWhitespace.getBytes());
            }
            if(debug) {
                System.out.println(codeWithoutCommentsWithoutUnneccesaryWhitespace);
            }
            results.add(new CharacterList(codeWithoutCommentsWithoutUnneccesaryWhitespace,sourceFiles.get(i).toPath()));
        }
        printEndPhase(true,printLong);
        return results;
    }

    public List<AST> phase_parsing(List<TokenList> list, CommandLine cmd)throws Exception{
        printBeginPhase("PARSING",printLong);
        List<AST> asts=new ArrayList<>();
        boolean didThrow=false;
        List<Exception> exceptions=new ArrayList<>();

        for(TokenList tokens : list){
            try {
                AST ast = (new Parser()).parse(tokens,tokens.relPath);

                if (debug || cmd.hasOption(FLAG_PRINT_AST)) {

                    TerminalUtil.println("\nDEBUG: PRINT AST JSON ", RED);

                    ObjectMapper mapper = new ObjectMapper();
                    mapper.enable(SerializationFeature.INDENT_OUTPUT);
                    System.out.println(mapper.writeValueAsString(ast));
                    System.out.println();
                }

                asts.add(ast);
            }catch (Exception e) {
                didThrow=true;
                exceptions.add(e);
            }
        }

        if(didThrow){
            printEndPhase(false,printLong);
            throw new Exception(exceptions.stream().map(e->e.getMessage()).collect(Collectors.joining("\n")));
        }else{
            printEndPhase(true,printLong);
            return asts;
        }

    }

    public List<TokenList> phase_lexing(List<CharacterList> just_codes_with_braces_without_comments, CommandLine cmd)throws Exception{
        printBeginPhase("LEXING",printLong);
        List<TokenList> list=new ArrayList();
        boolean didThrow = false;
        List<Exception> exceptions=new ArrayList<>();

        for(CharacterList just_code_with_braces_without_comments: just_codes_with_braces_without_comments){
            try {
                TokenList tokens = (new Lexer()).lexCodeWithoutComments(just_code_with_braces_without_comments);

                if (debug || cmd.hasOption(FLAG_PRINT_TOKENS)) {
                    System.out.println(tokens.toString());
                }
                list.add(tokens);
            }catch (Exception e){
                exceptions.add(e);
                didThrow=true;
            }
        }

        if(didThrow){
            printEndPhase(false,printLong);
            //collect all the exceptions throw during lexing,
            //and combine their messages to throw a bigger exception
            throw new Exception(exceptions.stream().map(Throwable::getMessage).collect(Collectors.joining("\n")));
        }else {
            printEndPhase(true,printLong);
            return list;
        }
    }

}
