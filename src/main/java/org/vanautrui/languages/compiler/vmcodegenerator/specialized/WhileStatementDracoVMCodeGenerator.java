package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public final class WhileStatementDracoVMCodeGenerator {


    public static List<String> genVMCodeForWhileStatement(
            WhileStatementNode whileStmt,
            MethodNode containerMethod,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception {

        final List<String> vm = new ArrayList<>();

        final long unique = unique();
        final String startlabel = "whilestart" + unique;
        final String endlabel = "whileend" + unique;

        vm.add("label "+startlabel);

        //push the expression
        vm.addAll(genDracoVMCodeForExpression(whileStmt.condition, subTable, varTable, structsTable)); //+1

        vm.add("not");
        //if condition is false, jump to end
        vm.add("if-goto "+endlabel);

        //execute statements
        for (StatementNode stmt : whileStmt.statements) {
            vm.addAll(generateDracoVMCodeForStatement(stmt, containerMethod, subTable, varTable, structsTable));
        }

        vm.add("goto "+startlabel);
        vm.add("label "+endlabel);

        return vm;
    }
}
