package org.vanautrui.languages.commandline;

import org.apache.commons.cli.CommandLine;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typechecking.TypeChecker;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
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

    public CompilerPhases(final CommandLine cmd){
        this.debug=cmd.hasOption(FLAG_DEBUG);
        this.timed=cmd.hasOption(FLAG_TIMED);
    }

    public CompilerPhases() {
        //for testing of compiler phases
        this.debug=false;
        this.timed=false;
    }

    public void phase_typecheck(final AST_Whole_Program ast)throws Exception{
        System.out.println("TYPE CHECKING");

        //this should throw an exception, if it does not typecheck
        try {
            TypeChecker.doTypeCheck(ast,debug);

        }catch (Exception e){
            throw e;
        }
    }

    public List<Path> phase_vm_codegeneration(final AST_Whole_Program ast, final boolean printsymboltables)throws Exception{
        System.out.println("VM CODE GENERATION");

        final SubroutineSymbolTable subTable = createSubroutineSymbolTable(ast,debug);
        final StructsSymbolTable structsTable = createStructsSymbolTable(ast,debug);
        final Map<String,List<String>> dracoVMCodes = DracoVMCodeGenerator.generateDracoVMCode(ast, subTable,structsTable,debug,printsymboltables);

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
