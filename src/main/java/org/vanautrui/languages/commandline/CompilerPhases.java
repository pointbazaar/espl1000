package org.vanautrui.languages.commandline;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import org.apache.commons.cli.CommandLine;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;
import org.vanautrui.languages.vmcompiler.codegenerator.VMCompilerMain;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.phase_clean_the_input.CommentRemoverAndWhitespaceRemover;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typechecking.TypeChecker;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.stream.Collectors;

import static java.lang.System.out;
import static org.fusesource.jansi.Ansi.Color.RED;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printBeginPhase;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printEndPhase;
import static org.vanautrui.languages.commandline.dragonc.*;
import static org.vanautrui.languages.compiler.phase_clean_the_input.CommentRemoverAndWhitespaceRemover.remove_unneccessary_whitespace;
import static org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator.createSubroutineSymbolTable;

public class CompilerPhases {

    private final boolean debug;
    private final boolean timed;
    private final boolean printLong;

    public CompilerPhases(CommandLine cmd){
        this.debug=cmd.hasOption(FLAG_DEBUG);
        this.timed=cmd.hasOption(FLAG_TIMED);
        this.printLong=debug||timed;
    }

    public CompilerPhases() {
        //for testing of compiler phases
        this.debug=false;
        this.timed=false;
        this.printLong=false;
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

    public void generate_executable(String asm_codes,String filename_without_extension) throws Exception{
        String asm_file_name = filename_without_extension+".asm";
        Files.write(Paths.get(asm_file_name),asm_codes.getBytes());

        Process p = Runtime.getRuntime().exec("nasm -f elf " + asm_file_name);
        p.waitFor();

        if(p.exitValue()!=0){
            throw new Exception("nasm exit with nonzero exit code");
        }

        Process p2 = Runtime.getRuntime().exec("ld -melf_i386 -s -o "+filename_without_extension+" "+asm_file_name+".o");
        p2.waitFor();
        if(p2.exitValue() != 0){
            throw new Exception("ld exit with nonzero exit code");
        }
    }

    public List<Path> phase_codegeneration(List<AST> asts, CommandLine cmd)throws Exception{
        printBeginPhase("CODE GENERATION",printLong);

        List<Path> generatedFilesPaths=new ArrayList<>();

        try {

            SubroutineSymbolTable subTable = createSubroutineSymbolTable(new HashSet<>(asts));
            List<String> dracoVMCodes = DracoVMCodeGenerator.generateDracoVMCode(new HashSet<>(asts), subTable);

            if(cmd.hasOption(FLAG_PRINT_VM_CODES)){
                out.println("GENERATED VM CODES");
                dracoVMCodes.stream().forEach(str-> out.println(str));
                out.println();
            }

            final List<String> assembly_codes = VMCompilerMain.compileVMCode(dracoVMCodes);
            //$ nasm -f elf hello.asm  # this will produce hello.o ELF object file
            //$ ld -s -o hello hello.o # this will produce hello executable

            final String asm_codes = (assembly_codes
                    .stream()
                    .collect(Collectors.joining("\n"))+"\n");

            if(cmd.hasOption(FLAG_PRINT_ASM)){
                out.println(asm_codes);
            }

            if(debug){
                out.println("call nasm");
            }

            String filename = "main";
            generate_executable(asm_codes,filename);

            generatedFilesPaths.add(Paths.get(filename));

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
                out.println("phase clean: Hashcode of source string: " + hash);
            }
            boolean foundCachedCleanedFile = false;

            if(Files.exists(makeCleanPhaseCacheFilePathFromHash(hash))){
                foundCachedCleanedFile=true;
            }

            String codeWithoutCommentsWithoutUnneccesaryWhitespace;

            if(foundCachedCleanedFile){
                if(debug) {
                    out.println("found a cached version that is already cleaned");
                }
                codeWithoutCommentsWithoutUnneccesaryWhitespace = new String(Files.readAllBytes(makeCleanPhaseCacheFilePathFromHash(hash)));
            }else {
                String[] parts = source.split("\n");
                String source_without_use_directive = Arrays.stream(parts).filter(str->!str.startsWith("use")).collect(Collectors.joining("\n"));

                String codeWithoutCommentsAndWithoutEmptyLines = (new CommentRemoverAndWhitespaceRemover()).strip_all_comments_and_empty_lines(source_without_use_directive);

                codeWithoutCommentsWithoutUnneccesaryWhitespace =
                        remove_unneccessary_whitespace(codeWithoutCommentsAndWithoutEmptyLines);

                //write file for caching
                Files.write(makeCleanPhaseCacheFilePathFromHash(hash),codeWithoutCommentsWithoutUnneccesaryWhitespace.getBytes());
            }
            if(debug) {
                out.println(codeWithoutCommentsWithoutUnneccesaryWhitespace);
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

                if (cmd.hasOption(FLAG_PRINT_AST)) {

                    TerminalUtil.println("\nDEBUG: PRINT AST JSON ", RED);

                    ObjectMapper mapper = new ObjectMapper();
                    mapper.enable(SerializationFeature.INDENT_OUTPUT);
                    out.println(mapper.writeValueAsString(ast));
                    out.println();
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
        List<Exception> exceptions=new ArrayList<>();

        for(CharacterList just_code_with_braces_without_comments: just_codes_with_braces_without_comments){
            try {
                TokenList tokens = (new Lexer()).lexCodeWithoutComments(just_code_with_braces_without_comments);

                if (cmd.hasOption(FLAG_PRINT_TOKENS)) {
                    out.println(tokens.toString());
                }
                list.add(tokens);
            }catch (Exception e){
                exceptions.add(e);
            }
        }

        if(exceptions.size()>0){
            printEndPhase(false,printLong);
            //collect all the exceptions throw during lexing,
            //and combine their messages to throw a bigger exception
            throw new Exception(exceptions.stream().map(Throwable::getMessage).collect(Collectors.joining("\n")));
        }else {
            printEndPhase(true,printLong);
            return list;
        }
    }

    public void phase_preprocessor(List<String> codes, List<File> sources, CommandLine cmd) throws Exception {
        printBeginPhase("PREPROCESSING",printLong);
        //appends the 'use' used files to codes list and source paths list.
        //'use' statements will be removed later in the 'clean' phase
        //TOOD

        //'use' directives have to be at the top of the file
        //and cannot have newlines between or before them
        List<Exception> exceptions=new ArrayList<>();

        List<String> files_to_be_checked_for_includes=new ArrayList<>(codes);


        while (files_to_be_checked_for_includes.size() > 0) {

            try {

                String code = files_to_be_checked_for_includes.get(0);
                String[] lines = code.split("\n");

                for (int i = 0; i < lines.length; i++) {
                    String line = lines[i];
                    if (line.startsWith("use")) {
                        String[] parts = line.split(" ");
                        if (parts.length == 2) {

                            if(debug){
                                out.println("\nPREPROCESSOR: USE "+parts[1]);
                            }

                            Path filename_in_stdlib = Paths.get(System.getProperty("user.home")+"/dragon/stdlib/" + parts[1]);
                            Path filename_in_project = Paths.get(parts[1]);
                            //add those files contents to the files to be checked
                            //search first in dragon stdlib and then in the project folder

                            Path path;

                            if (Files.exists(filename_in_stdlib)) {
                                path = filename_in_stdlib;
                            } else if (Files.exists(filename_in_project)) {
                                path = filename_in_project;
                            } else {
                                String msg="neither "+filename_in_stdlib.toAbsolutePath().toString()+" nor "
                                        +filename_in_project.toAbsolutePath().toString()+" exists";
                                throw new Exception("tried to include file that does not exist. " + msg);
                            }
                            String content = new String(Files.readAllBytes(path));

                            files_to_be_checked_for_includes.add(content);
                            codes.add(content);
                            sources.add(path.toFile());
                        } else {
                            throw new Exception(" 'use' directive only has 2 parts");
                        }
                    } else {
                        //we hase reached the end of the include
                        //directives
                        break;
                    }
                }
            }catch (Exception e) {
                exceptions.add(e);
            }

            files_to_be_checked_for_includes.remove(0);
        }

        if(exceptions.size()>0){
            printEndPhase(false,printLong);
            throw new Exception(exceptions.stream().map(Throwable::getMessage).collect(Collectors.joining("\n")));
        }else {
            printEndPhase(true,printLong);
        }
    }
}
