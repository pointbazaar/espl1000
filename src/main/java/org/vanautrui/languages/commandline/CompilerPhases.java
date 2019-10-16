package org.vanautrui.languages.commandline;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import org.apache.commons.cli.CommandLine;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;
import org.vanautrui.languages.vmcompiler.codegenerator.AssemblyCodeGenerator;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.phase_clean_the_input.CommentRemoverAndWhitespaceRemover;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typechecking.TypeChecker;

import java.io.File;
import java.nio.file.*;
import java.util.*;
import java.util.stream.Collectors;
import static java.lang.System.out;
import static org.fusesource.jansi.Ansi.Color.RED;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.*;
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

    public void phase_typecheck(List<AST> asts)throws Exception{
        printBeginPhase("TYPE CHECKING",printLong);

        //this should throw an exception, if it does not typecheck
        try {
            TypeChecker typeChecker=new TypeChecker();
            typeChecker.doTypeCheck(asts,debug);

            //TerminalUtil.println("✓", Ansi.Color.GREEN);
            printEndPhase(true,printLong);
        }catch (Exception e){
            //TerminalUtil.println("⚠", RED);
            printEndPhase(true,printLong);
            throw e;
        }
    }

    public List<String> phase_vm_codegeneration(List<AST> asts,String filename_without_extension, boolean print_vm_codes,boolean printsymboltables)throws Exception{
        printBeginPhase("VM CODE GENERATION",printLong);

        try {

            SubroutineSymbolTable subTable = createSubroutineSymbolTable(asts,debug);
            List<String> dracoVMCodes = DracoVMCodeGenerator.generateDracoVMCode(new HashSet<>(asts), subTable,debug,printsymboltables);

            final String vm_codes_string = dracoVMCodes.stream().collect(Collectors.joining("\n"))+"\n";

            Files.write(
                    Paths.get(filename_without_extension+".dracovm"),
                    vm_codes_string.getBytes()
            );

            if(print_vm_codes){
                out.println("GENERATED VM CODES");
                dracoVMCodes.stream().forEach(str-> out.println(str));
                out.println();
            }

            printEndPhase(true,printLong);
            return dracoVMCodes;

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

    public void phase_preprocessor(List<String> codes, List<File> sources) throws Exception {
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
