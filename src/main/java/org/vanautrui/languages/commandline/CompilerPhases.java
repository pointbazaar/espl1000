package org.vanautrui.languages.commandline;

import org.apache.commons.cli.CommandLine;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typechecking.TypeChecker;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

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
        System.out.println("TYPE CHECKING");

        //this should throw an exception, if it does not typecheck
        try {
            TypeChecker.doTypeCheck(asts,debug);

        }catch (Exception e){
            throw e;
        }
    }

    public List<Path> phase_vm_codegeneration(final List<AST> asts, boolean printsymboltables)throws Exception{
        System.out.println("VM CODE GENERATION");

        final SubroutineSymbolTable subTable = createSubroutineSymbolTable(asts,debug);
        final StructsSymbolTable structsTable = createStructsSymbolTable(asts,debug);
        final Map<String,List<String>> dracoVMCodes = DracoVMCodeGenerator.generateDracoVMCode(new HashSet<>(asts), subTable,structsTable,debug,printsymboltables);

        final List<Path> paths = new ArrayList<>();

        for(Map.Entry<String,List<String>> subr : dracoVMCodes.entrySet()){

            //write a file with the vmcode for each subroutine
            final Path path = Paths.get(subr.getKey()+".subroutine.dracovm");
            System.out.println("write: "+path);
            Files.write(
                    path,
                    (subr.getValue().stream().collect(Collectors.joining("\n"))+"\n").getBytes()
            );
            paths.add(path);
        }

        return paths;
    }

}
