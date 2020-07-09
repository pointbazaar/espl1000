package org.vanautrui.languages.compiler.codegenerator.specialized;

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
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.List;

import static org.vanautrui.languages.compiler.codegenerator.JavaCodeGenerator.*;
import static org.vanautrui.languages.compiler.codegenerator.specialized.ExpressionJavaCodeGenerator.genJavaCodeForExpression;
import static org.vanautrui.languages.compiler.codegenerator.specialized.MethodCallJavaCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.codegenerator.specialized.VariableJavaCodeGenerator.genJavaCodeForVariable;

public final class TermJavaCodeGenerator {

    public static List<String> genJavaCodeForTerm(
            final TermNode tNode,
            final SymbolTableContext ctx
    ) throws Exception {
        final ITermNode t = tNode.termNode;

        if (t instanceof FloatConstNode fcn) {
            return genJavaCodeForFloatConst(fcn.floatValue);
        } else if (t instanceof IntConstNode icn) {
            return genJavaCodeForIntConst(icn.number);
        } else if (t instanceof ExpressionNode expressionNode) {
            return List.of(genJavaCodeForExpression(expressionNode,ctx));
        } else if (t instanceof VariableNode variableNode) {
            return genJavaCodeForVariable(variableNode, ctx);
        } else if (t instanceof MethodCallNode methodCallNode) {
            return List.of(genVMCodeForMethodCall(methodCallNode, ctx));
        } else if (t instanceof BoolConstNode) {
            return genJavaCodeForBoolConst((BoolConstNode) t);
        } else if (t instanceof ArrayConstantNode arrayConstantNode) {
            return genJavaCodeForArrayConstant(arrayConstantNode, ctx);
        } else if (t instanceof CharConstNode ccn) {
            return List.of("'"+ccn.content+"'");
        } else {
            throw new Exception("unhandled case");
        }
    }
}
