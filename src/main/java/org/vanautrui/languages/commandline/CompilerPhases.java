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

public final class CompilerPhases {

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

}
