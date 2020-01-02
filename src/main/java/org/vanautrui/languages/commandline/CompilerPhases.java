package org.vanautrui.languages.commandline;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
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

import static org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator.createStructsSymbolTable;
import static org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator.createSubroutineSymbolTable;

public final class CompilerPhases {

    //prevents instance creation
    private CompilerPhases(){}

    public static void phase_typecheck(
            final AST_Whole_Program ast, final boolean debug
    )throws Exception{
        if(debug) {
            System.out.println("TYPE CHECKING");
        }

        //this should throw an exception, if it does not typecheck
        try {
            TypeChecker.doTypeCheck(ast,debug);

        }catch (Exception e){
            throw e;
        }
    }

    public static List<Path> phase_vm_codegeneration(
            final AST_Whole_Program ast,
            final boolean printsymboltables,
            final boolean debug
    )throws Exception{

        if(debug) {
            System.out.println("VM CODE GENERATION");
        }

        final SubroutineSymbolTable subTable = createSubroutineSymbolTable(ast,debug);
        final StructsSymbolTable structsTable = createStructsSymbolTable(ast,debug);
        final Map<String,List<String>> dracoVMCodes = DracoVMCodeGenerator.generateDracoVMCode(ast, subTable,structsTable,debug,printsymboltables);

        final List<Path> paths = new ArrayList<>();

        for(Map.Entry<String,List<String>> subr : dracoVMCodes.entrySet()){

            //write a file with the vmcode for each subroutine
            final Path path = Paths.get(subr.getKey()+".subroutine.dracovm");

            if(debug) {
                System.out.println("write: " + path);
            }

            Files.write(
                    path,
                    (String.join("\n", subr.getValue()) +"\n").getBytes()
            );

            paths.add(path);
        }

        return paths;
    }

    public static void phase_simplify(
            final AST_Whole_Program ast,
            final boolean debug
    ) {
        //TODO: simplify and evaluate constant expressions like '1+2' -> '3'
        if(debug){
            System.out.println("CompilerPhases::phase_simplify");
        }

        ast.namespaceNodeList.forEach(namespaceNode -> {
            //we can only simplify subroutines
            namespaceNode.methods.forEach(method->{
                //first focus on the statements
                method.statements.forEach(stmt->{
                    final IStatementNode istmt = stmt.statementNode;
                    if(istmt instanceof AssignmentStatementNode){
                        final AssignmentStatementNode astmt = (AssignmentStatementNode)istmt;
                        final ExpressionNode expr = astmt.expressionNode;

                        //TODO: look if expr is composed of only constant terms
                    }
                });
            });
        });
    }
}
