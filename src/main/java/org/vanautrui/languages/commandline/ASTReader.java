package org.vanautrui.languages.commandline;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ArrayTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeParameterNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Optional;

import static java.lang.System.out;

public final class ASTReader {
    /*
    This class reads tokens in a structured AST File Format from a String[] and builds
    an AST from them.
     */

    static NamespaceNode parseNamespaceFromASTFile(final File astFile, boolean debug) throws Exception {
        if(debug){
            out.println(String.format("ASTReader::parseNamespaceFromASTFile(%s,%b)",astFile.toPath().toString(), debug));
        }
        final String astJSON = Files.readString(astFile.toPath());
        final String[] parts = astJSON.split("\t");

        //This reads our custom AST Format
        //i chose this over JSON, as there are no dependencies needed to parse it.
        //and it uses less space on disk, as there is no extra json specific syntax
        final String name = astFile.getName().substring(1,astFile.getName().substring(1).indexOf("."));
        if(debug){
            out.println("name of namespace: "+name);
        }
        final NamespaceNode nsn = new NamespaceNode(name);

        int i = 0;
        nsn.srcPath = Path.of(parts[i++]);

        //TODO: investigate. this causes issues, as a '.' is often appended to the file
        //nsn.name = parts[i++];
        //for now: skip it
        i++;

        final int count_methods = Integer.parseInt(parts[i++]);
        for(int k=0;k<count_methods;k++){
            final MethodNode m = new MethodNode();
            i = parseMethod(parts,m,i);
            nsn.methods.add(m);
        }
        return nsn;
    }

    private static int parseMethod(String[] arr, MethodNode m, int i){

        final String verify = arr[i++];
        if(!verify.equals("Method")){throw  new RuntimeException("AST Parse Error");}

        m.isPublic = Boolean.parseBoolean(arr[i++]);
        m.hasSideEffects = Boolean.parseBoolean(arr[i++]);
        m.methodName = arr[i++];

        final TypeNode returnType = new TypeNode();
        i = parseType(arr, returnType, i);
        m.returnType = returnType;

        final int argCount = Integer.parseInt(arr[i++]);
        for(int k=0;k<argCount;k++){
            final DeclaredArgumentNode decl = new DeclaredArgumentNode();
            i = parseDeclaredArg(arr, decl, i);
            m.arguments.add(decl);
        }
        final int stmtCount = Integer.parseInt(arr[i++]);
        for(int k=0;k<stmtCount;k++){
            final StatementNode stmt = new StatementNode();
            i = parseStmt(arr, stmt, i);
            m.statements.add(stmt);
        }
        return i;
    }

    private static int parseDeclaredArg(String[] arr, DeclaredArgumentNode decl, int i) {
        final String verify = arr[i++];
        if(!verify.equals("DeclaredArg")){throw  new RuntimeException("AST Parse Error");}

        decl.type = new TypeNode();
        i = parseType(arr, decl.type, i);

        final String next = arr[i];
        if (!next.equals("NULL")) {
            decl.name = Optional.of(next);
        }
        i++;
        return i;
    }

    private static int parseStmt(String[] arr, StatementNode stmt, int i) {
        final String verify = arr[i++];
        if(!verify.equals("Stmt")){throw  new RuntimeException("AST Parse Error");}

        final int which = Integer.parseInt(arr[i++]);
        switch (which) {
            case 1 -> {
                stmt.statementNode = new MethodCallNode();
                i = parseMethodCall(arr, (MethodCallNode) stmt.statementNode, i);
            }
            case 2 -> {
                stmt.statementNode = new WhileStatementNode();
                i = parseWhileStmt(arr, (WhileStatementNode) stmt.statementNode, i);
            }
            case 3 -> {
                stmt.statementNode = new IfStatementNode();
                i = parseIfStmt(arr, (IfStatementNode) stmt.statementNode, i);
            }
            case 4 -> {
                stmt.statementNode = new ReturnStatementNode();
                i = parseRetStmt(arr, (ReturnStatementNode) stmt.statementNode, i);
            }
            case 5 -> {
                stmt.statementNode = new AssignmentStatementNode();
                i = parseAssignStmt(arr, (AssignmentStatementNode) stmt.statementNode, i);
            }
        }
        return i;
    }

    private static int parseType(String[] arr, TypeNode type, int i){

        final String verify = arr[i++];
        if(!verify.equals("Type")){throw  new RuntimeException("AST Parse Error");}

        final int which = Integer.parseInt(arr[i++]);
        switch (which) {
            case 1 -> {
                type.typeNode = new BasicTypeWrappedNode();
                i = parseBasicTypeWrapped(arr, (BasicTypeWrappedNode) type.typeNode, i);
            }
            case 2 -> {
                type.typeNode = new TypeParameterNode();
                i = parseTypeParam(arr, (TypeParameterNode) type.typeNode, i);
            }
            case 3 -> {
                type.typeNode = new ArrayTypeNode();
                i = parseArrayTypeNode(arr, (ArrayTypeNode) type.typeNode, i);
            }
        }
        return i;
    }

    private static int parseExpr(String[] arr, ExpressionNode expr, int i){

        final String verify = arr[i++];
        if(!verify.equals("Expr")){throw  new RuntimeException("AST Parse Error");}

        expr.term1 = new TermNode();
        i = parseTerm(arr, expr.term1, i);

        String next = arr[i];
        if(next.equals("NULL")){
            i++;
        }else{
            expr.op = Optional.of(new OperatorNode());
            i = parseOp(arr, expr.op.get(), i);

            expr.term2 = Optional.of(new TermNode());
            i = parseTerm(arr, expr.term2.get(), i);
        }
        return i;
    }

    private static int parseOp(String[] arr, OperatorNode op, int i) {

        final String verify = arr[i++];
        if(!verify.equals("Op")){throw  new RuntimeException("AST Parse Error");}

        op.operator = arr[i++];
        return i;
    }

    private static int parseTerm(String[] arr, TermNode term, int i){
        final String verify = arr[i++];
        if(!verify.equals("Term")){throw  new RuntimeException("AST Parse Error");}

        final int which = Integer.parseInt(arr[i++]);
        switch (which) {
            case 1 -> {
                term.termNode = new BoolConstNode();
                i = parseBoolConst(arr, (BoolConstNode) term.termNode, i);
            }
            case 2 -> {
                term.termNode = new IntConstNode();
                i = parseIntConst(arr, (IntConstNode) term.termNode, i);
            }
            case 3 -> {
                term.termNode = new CharConstNode();
                i = parseCharConst(arr, (CharConstNode) term.termNode, i);
            }
            case 4 -> {
                term.termNode = new MethodCallNode();
                i = parseMethodCall(arr, (MethodCallNode) term.termNode, i);
            }
            case 5 -> {
                term.termNode = new ExpressionNode();
                i = parseExpr(arr, (ExpressionNode) term.termNode, i);
            }
            case 6 -> {
                term.termNode = new VariableNode();
                i = parseVariable(arr, (VariableNode) term.termNode, i);
            }
            case 7 -> {
                term.termNode = new FloatConstNode();
                i = parseFloatConst(arr, (FloatConstNode) term.termNode, i);
            }
        }
        return i;
    }

    private static int parseVariable(String[] arr, VariableNode varNode, int i){

        final String verify = arr[i++];
        if(!verify.equals("Variable")){throw  new RuntimeException("AST Parse Error");}

        varNode.simpleVariableNode = new SimpleVariableNode();
        i = parseSimpleVariable(arr, varNode.simpleVariableNode, i);
        final String next = arr[i];
        if(next.equals("NULL")){
            i++;
        }else{
            final VariableNode v = new VariableNode();
            i = parseVariable(arr, v, i);
            varNode.memberAccessList.add(v);
        }
        return i;
    }

    private static int parseSimpleVariable(String[] arr, SimpleVariableNode simpleVariableNode, int i) {

        final String verify = arr[i++];
        if(!verify.equals("SimpleVariable")){throw  new RuntimeException("AST Parse Error");}

        simpleVariableNode.name = arr[i++];
        final String next = arr[i];
        if(next.equals("NULL")){
            i++;
        }else{
            simpleVariableNode.indexOptional = Optional.of(new ExpressionNode());
            i = parseExpr(arr, simpleVariableNode.indexOptional.get(), i);
        }
        return i;
    }

    private static int parseMethodCall(String[] arr, MethodCallNode mcn, int i){

        final String verify = arr[i++];
        if(!verify.equals("MethodCall")){throw  new RuntimeException("AST Parse Error");}

        mcn.methodName = arr[i++];
        final int countArgs = Integer.parseInt(arr[i++]);
        for(int k=0;k<countArgs;k++){
            final ExpressionNode expr = new ExpressionNode();
            i = parseExpr(arr, expr, i);
            mcn.arguments.add(expr);
        }
        return i;
    }

    private static int parseWhileStmt(String[] arr, WhileStatementNode wstmt, int i){

        final String verify = arr[i++];
        if(!verify.equals("WhileStmt")){throw  new RuntimeException("AST Parse Error");}

        final ExpressionNode condition = new ExpressionNode();
        i = parseExpr(arr, condition, i);
        wstmt.condition = condition;

        final int countStmt = Integer.parseInt(arr[i++]);
        for(int k=0;k<countStmt;k++){
            final StatementNode stmt = new StatementNode();
            i = parseStmt(arr, stmt, i);
            wstmt.statements.add(stmt);
        }
        return i;
    }

    private static int parseIfStmt(String[] arr, IfStatementNode ifstmt, int i){

        final String verify = arr[i++];
        if(!verify.equals("IfStmt")){throw  new RuntimeException("AST Parse Error");}

        final ExpressionNode condition = new ExpressionNode();
        i = parseExpr(arr, condition, i);
        ifstmt.condition = condition;

        final int countStmt = Integer.parseInt(arr[i++]);
        for(int k=0;k<countStmt;k++){
            final StatementNode stmt = new StatementNode();
            i = parseStmt(arr, stmt, i);
            ifstmt.statements.add(stmt);
        }

        final int countStmtElse = Integer.parseInt(arr[i++]);
        for(int k=0;k<countStmtElse;k++){
            final StatementNode stmt = new StatementNode();
            i = parseStmt(arr, stmt, i);
            ifstmt.elseStatements.add(stmt);
        }

        return i;
    }

    private static int parseRetStmt(String[] arr, ReturnStatementNode retStmt, int i){

        final String verify = arr[i++];
        if(!verify.equals("RetStmt")){throw  new RuntimeException("AST Parse Error");}

        final ExpressionNode expr = new ExpressionNode();
        i = parseExpr(arr, expr, i);
        retStmt.returnValue = expr;
        return i;
    }

    private static int parseAssignStmt(String[] arr, AssignmentStatementNode assignStmt, int i){

        final String verify = arr[i++];
        if(!verify.equals("AssignStmt")){throw  new RuntimeException("AST Parse Error");}

        final String next = arr[i];
        if(next.equals("NULL")){
            i++;
        }else{
            assignStmt.optTypeNode = Optional.of(new TypeNode());
            i = parseType(arr, assignStmt.optTypeNode.get(), i);
        }

        assignStmt.variableNode = new VariableNode();
        i = parseVariable(arr, assignStmt.variableNode, i);

        assignStmt.expressionNode = new ExpressionNode();
        i = parseExpr(arr, assignStmt.expressionNode, i);
        return i;
    }

    private static int parseBoolConst(String[] arr, BoolConstNode bc, int i){

        final String verify = arr[i++];
        if(!verify.equals("BoolConst")){throw  new RuntimeException("AST Parse Error");}

        bc.boolValue = Boolean.parseBoolean(arr[i++]);
        return i;
    }

    private static int parseIntConst(String[] arr, IntConstNode ic, int i){
        final String verify = arr[i++];
        if(!verify.equals("IntConst")){throw  new RuntimeException("AST Parse Error");}
        ic.number = Integer.parseInt(arr[i++]);
        return i;
    }

    private static int parseCharConst(String[] arr, CharConstNode cc, int i){
        final String verify = arr[i++];
        if(!verify.equals("CharConst")){throw  new RuntimeException("AST Parse Error");}
        cc.content = arr[i++].charAt(1);
        return i;
    }

    private static int parseFloatConst(String[] arr, FloatConstNode fc, int i){
        final String verify = arr[i++];
        if(!verify.equals("FloatConst")){throw  new RuntimeException("AST Parse Error");}
        fc.floatValue = Float.parseFloat(arr[i++]);
        return i;
    }

    private static int parseBasicTypeWrapped(String[] arr, BasicTypeWrappedNode btw, int i){

        final String verify = arr[i++];
        if(!verify.equals("BasicTypeWrapped")){throw  new RuntimeException("AST Parse Error");}

        final int which = Integer.parseInt(arr[i++]);
        switch (which) {
            case 1 -> {
                btw.typeNode = new SimpleTypeNode();
                i = parseSimpleType(arr, (SimpleTypeNode) btw.typeNode, i);
            }
            case 2 -> {
                btw.typeNode = new SubroutineTypeNode();
                i = parseSubrType(arr, (SubroutineTypeNode) btw.typeNode, i);
            }
        }
        return i;
    }

    private static int parseSubrType(String[] arr, SubroutineTypeNode typeNode, int i) {
        final String verify = arr[i++];
        if(!verify.equals("SubrType")){throw  new RuntimeException("AST Parse Error");}

        typeNode.hasSideEffects = Boolean.parseBoolean(arr[i++]);

        final int countArgTypes = Integer.parseInt(arr[i++]);
        for(int k=0;k<countArgTypes;k++){
            TypeNode t = new TypeNode();
            i = parseType(arr, t, i);
            typeNode.argumentTypes.add(t);
        }
        return i;
    }

    private static int parseSimpleType(String[] arr, SimpleTypeNode typeNode, int i) {
        final String verify = arr[i++];
        if(!verify.equals("SimpleType")){throw  new RuntimeException("AST Parse Error");}

        typeNode.typeName = arr[i++];
        return i;
    }

    private static int parseTypeParam(String[] arr, TypeParameterNode tp, int i){

        final String verify = arr[i++];
        if(!verify.equals("TypeParam")){throw  new RuntimeException("AST Parse Error");}

        tp.typeParameterIndex = Integer.parseInt(arr[i++]);
        return i;
    }

    private static int parseArrayTypeNode(String[] arr, ArrayTypeNode at, int i){

        final String verify = arr[i++];
        if(!verify.equals("ArrayType")){throw  new RuntimeException("AST Parse Error");}

        at.element_type = new TypeNode();
        i = parseType(arr, at.element_type, i);
        return i;
    }
}
