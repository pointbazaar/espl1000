package org.vanautrui.languages.commandline;

import org.apache.commons.cli.CommandLine;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typechecking.TypeChecker;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.stream.Collectors;

import static java.lang.System.out;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printBeginPhase;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printEndPhase;
import static org.vanautrui.languages.commandline.dragonc.FLAG_DEBUG;
import static org.vanautrui.languages.commandline.dragonc.FLAG_TIMED;
import static org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator.createStructsSymbolTable;
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
            TypeChecker.doTypeCheck(asts,debug);

            printEndPhase(true,printLong);
        }catch (Exception e){
            printEndPhase(false,printLong);
            throw e;
        }
    }

    public List<String> phase_vm_codegeneration(List<AST> asts,String filename_without_extension, boolean print_vm_codes,boolean printsymboltables)throws Exception{
        printBeginPhase("VM CODE GENERATION",printLong);

        try {

            SubroutineSymbolTable subTable = createSubroutineSymbolTable(asts,debug);
            StructsSymbolTable structsTable = createStructsSymbolTable(asts,debug);
            List<String> dracoVMCodes = DracoVMCodeGenerator.generateDracoVMCode(new HashSet<>(asts), subTable,structsTable,debug,printsymboltables);

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

    public void phase_preprocessor(List<String> codes, List<File> sources) throws Exception {
        printBeginPhase("PREPROCESSING",printLong);
        //appends the 'use' used files to codes list and source paths list.
        //'use' statements will be removed later in the 'clean' phase
        //TOOD

        //'use' directives have to be at the top of the file
        //and cannot have newlines between or before them
        final List<Exception> exceptions=new ArrayList<>();

        final List<String> files_to_be_checked_for_includes=new ArrayList<>(codes);


        while (files_to_be_checked_for_includes.size() > 0) {

            try {

                final String code = files_to_be_checked_for_includes.get(0);
                final String[] lines = code.split("\n");

                for (final String line : lines) {
                    if (line.startsWith("use")) {
                        final String[] parts = line.split(" ");
                        if (parts.length == 2) {
                            final String filename_to_include = parts[1];

                            if (debug) {
                                out.println("\nPREPROCESSOR: USE " + filename_to_include);
                            }

                            if(sources.stream().noneMatch(file -> file.getName().equals(filename_to_include))) {
                                if(debug){
                                    out.println("\nPREPROCESSOR: actually include "+filename_to_include);
                                }

                                Path filename_in_stdlib = Paths.get(System.getProperty("user.home") + "/dragon/stdlib/" + filename_to_include);
                                Path filename_in_project = Paths.get(filename_to_include);
                                //add those files contents to the files to be checked
                                //search first in dragon stdlib and then in the project folder

                                Path path;

                                if (Files.exists(filename_in_stdlib)) {
                                    path = filename_in_stdlib;
                                } else if (Files.exists(filename_in_project)) {
                                    path = filename_in_project;
                                } else {
                                    final String msg = "neither " + filename_in_stdlib.toAbsolutePath().toString() + " nor "
                                            + filename_in_project.toAbsolutePath().toString() + " exists";
                                    throw new Exception("tried to include file that does not exist. " + msg);
                                }
                                final String content = new String(Files.readAllBytes(path));

                                files_to_be_checked_for_includes.add(content);
                                codes.add(content);
                                sources.add(path.toFile());
                            }else{
                                if(debug){
                                    out.println("\nPREPROCESSOR: skipping "+filename_to_include+" as it was already included.");
                                }
                            }
                        } else {
                            throw new Exception(" 'use' directive only has 2 parts, but this one did not: "+line);
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
