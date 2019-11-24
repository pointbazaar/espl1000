package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.CharConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.FloatConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.*;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.VariableDracoVMCodeGenerator.genDracoVMCodeForVariable;

public final class TermDracoVMCodeGenerator {


    public static List<String> genDracoVMCodeForTerm(
            TermNode tNode,
            SymbolTableContext ctx
    ) throws Exception {

        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;
        final StructsSymbolTable structsTable=ctx.structsTable;

        final ITermNode t = tNode.termNode;
        if (t instanceof FloatConstNode) {
            return genVMCodeForFloatConst(
                    ((FloatConstNode) t).value
            );
        } else if (t instanceof IntConstNode) {
            return genVMCodeForIntConst(((IntConstNode) t).value);
        } else if (t instanceof ExpressionNode) {
            final ExpressionNode expressionNode = (ExpressionNode) t;
            return genDracoVMCodeForExpression(expressionNode,ctx);
        } else if (t instanceof VariableNode) {
            //find the local variable index
            // and push the variable onto the stack
            final VariableNode variableNode = (VariableNode) t;

            return genDracoVMCodeForVariable(variableNode, ctx);
        } else if (t instanceof MethodCallNode) {
            final MethodCallNode methodCallNode = (MethodCallNode) t;
            return genVMCodeForMethodCall(methodCallNode, ctx);

        } else if (t instanceof BoolConstNode) {
            return genVMCodeForBoolConst((BoolConstNode) t);
        } else if (t instanceof ArrayConstantNode) {
            final ArrayConstantNode arrayConstantNode = (ArrayConstantNode) t;
            return genVMCodeForArrayConstant(arrayConstantNode, ctx);
        } else if (t instanceof CharConstNode) {
            final CharConstNode t1 = (CharConstNode) t;

            final int ascii_value = t1.content;

            return genVMCodeForIntConst(ascii_value);
        } else {
            throw new Exception("unhandled case");
        }
    }
}
