package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.nameconversions.TypeNameToJVMInternalTypeNameConverter;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.typeresolution.TypeResolver;

import java.util.stream.Collectors;

import static org.objectweb.asm.Opcodes.*;

public class MethodCallCodeGenerator {

    //https://tomassetti.me/generating-bytecode/

    private static void compile_printing_statement(
            ClassWriter cw, MethodVisitor mv, ClassNode classNode, MethodNode methodNode,
            MethodCallNode methodCallNode, LocalVarSymbolTable methodScopeSymbolTable,
            SubroutineSymbolTable subroutineSymbolTable, boolean debug)throws Exception{

        //TODO: actually compile the stuff, not just fake

        mv.visitFieldInsn(GETSTATIC,
                "java/lang/System",
                "out",
                "Ljava/io/PrintStream;");

        //get the jvm internal type for the descriptor of the method
        String methodDescriptor ="(Ljava/lang/String;)V";

        if(methodCallNode.argumentList.size()>0) {

            for(ExpressionNode expressionNode : methodCallNode.argumentList){
                //TODO: make getTypeJVMInternal() to make this easier? or just make a translator class for it
                String expressionType= TypeResolver.getTypeExpressionNode(expressionNode,methodNode,subroutineSymbolTable,methodScopeSymbolTable);

                //set the method signature to the type
                //which accepts our arguments
                switch(expressionType){
                    case "Int":
                        methodDescriptor="(I)V";
                        break;
                    case "Char":
                        methodDescriptor="(C)V";
                        break;
                    case "Float":
                        methodDescriptor="(F)V";
                        break;
                }

                ExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,expressionNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
            }

        }else{
            mv.visitLdcInsn("");
        }

        switch (methodCallNode.identifierMethodName.name) {
            case "println":
                mv.visitMethodInsn(INVOKEVIRTUAL,
                        "java/io/PrintStream",
                        "println",
                        methodDescriptor);
                break;
            case "print":
                mv.visitMethodInsn(INVOKEVIRTUAL,
                        "java/io/PrintStream",
                        "print",
                        methodDescriptor);
                break;
            default:
                throw new Exception("unrecognized method: '" + methodCallNode.identifierMethodName.name+ "'");

        }
    }

    public static void visitMethodCallNode(
            ClassWriter cw, MethodVisitor mv, ClassNode classNode,
            MethodNode methodNode, MethodCallNode methodCallNode,
            LocalVarSymbolTable methodScopeSymbolTable,
            SubroutineSymbolTable subroutineSymbolTable, boolean debug
    ) throws Exception {

        if(subroutineSymbolTable.containsVariable(methodCallNode.identifierMethodName.name)){
            String subrType = TypeResolver.getTypeMethodCallNode(methodCallNode,subroutineSymbolTable);

            String methodName = methodCallNode.identifierMethodName.name;
            String owner=classNode.name.typeName;
            String descriptor= TypeNameToJVMInternalTypeNameConverter.convertSubroutineName(
                    subrType,methodCallNode.argumentList.stream().map(expressionNode -> {
                        try {
                            return TypeResolver.getTypeExpressionNode(expressionNode,methodNode,subroutineSymbolTable,methodScopeSymbolTable);
                        } catch (Exception e) {
                            e.printStackTrace();
                            throw new RuntimeException(" FATAL error in DragonMethodCallCodeGenerator");
                        }
                    }).collect(Collectors.toList()),
                    debug
            );

            for(ExpressionNode expr : methodCallNode.argumentList){
                ExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,expr,methodScopeSymbolTable,subroutineSymbolTable,debug);
            }

            mv.visitMethodInsn(INVOKESTATIC,owner,methodName,descriptor);

            if(debug) {
                System.out.println("found method in symbol table");
            }
        }else {
            switch (methodCallNode.identifierMethodName.name) {
                case "readln":
                    compile_readln(cw,mv,classNode,methodNode,methodCallNode,
                            methodScopeSymbolTable,subroutineSymbolTable,debug);
                    break;
                case "print":
                case "println":
                    compile_printing_statement(cw, mv, classNode, methodNode, methodCallNode,
                            methodScopeSymbolTable, subroutineSymbolTable,debug);
            }
        }
    }

    private static void compile_readln(
            ClassWriter cw, MethodVisitor mv, ClassNode classNode, MethodNode methodNode,
            MethodCallNode methodCallNode, LocalVarSymbolTable methodScopeSymbolTable,
            SubroutineSymbolTable subroutineSymbolTable, boolean debug
    )throws Exception{
        //the new instruction must get an index into
        //the runtime constant pool of the current class
        //int myconst = cw.newConst("java/util/Scanner");

        mv.visitTypeInsn(NEW, "java/util/Scanner");

        mv.visitInsn(DUP);

        mv.visitFieldInsn(GETSTATIC, "java/lang/System", "in", "Ljava/io/InputStream;");
        mv.visitMethodInsn(INVOKESPECIAL, "java/util/Scanner", "<init>", "(Ljava/io/InputStream;)V");

        mv.visitMethodInsn(INVOKEVIRTUAL, "java/util/Scanner", "nextLine", "()Ljava/lang/String;", false);
    }
}
