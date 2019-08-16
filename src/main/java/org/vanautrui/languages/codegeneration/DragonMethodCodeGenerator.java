package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import static org.objectweb.asm.Opcodes.*;

public class DragonMethodCodeGenerator {

    public static void visitMethodNode(ClassWriter cw, DragonClassNode classNode, DragonMethodNode methodNode) throws Exception {
        String owner = classNode.name.typeName.getContents();
        String descriptor = "i do not know";
        String methodName = methodNode.methodName.methodName.name.getContents();

        //figure it out
        //cw.newMethod(owner,this.methodName.methodName.name.getContents(),descriptor,false);

        //make the method scope symbol table
        DragonMethodScopeSymbolTable methodScopeSymbolTable=new DragonMethodScopeSymbolTable();
        for(DragonStatementNode stmt : methodNode.statements) {

            //TODO: also get the assignment statements recursively
            //that are inside the control flow statements.
            //but that is for later

            if(stmt.statementNode instanceof DragonAssignmentStatementNode) {
                DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode)stmt.statementNode;
                methodScopeSymbolTable.add(assignmentStatementNode.variableNode.name.getContents(),assignmentStatementNode.expressionNode.getType());
            }

            //DEBUG
            System.out.println(stmt.toSourceCode());
            System.out.println(stmt.statementNode.getClass().getName());
            System.out.println(stmt.statementNode instanceof DragonAssignmentStatementNode);
        }



        //DEBUG
        System.out.println(methodScopeSymbolTable.toString());

        //TODO: do not make this static
        //TODO: actually care about it
        //TODO: while typechecking, it should be seen
        //that there is an entry point in the program somewhere
        {
            MethodVisitor mv=cw.visitMethod(ACC_PUBLIC+ACC_STATIC,
                    methodName,
                    "([Ljava/lang/String;)V",
                    null,
                    null);

            //TODO: compile the local variable declarations
            //TODO: compile the statements in the method

            //stmt->stmt.visit(mv,Optional.of(classNode),Optional.of(methodNode))
            for (DragonStatementNode stmt : methodNode.statements) {
                DragonStatementCodeGenerator.visitStatement(cw, mv, classNode, methodNode, stmt,methodScopeSymbolTable);
            }

            mv.visitInsn(RETURN);

            //TODO: find out how to compute maxStack
            //and maxLocals

            int maxStack;
            int maxLocals;

            //mv.visitMaxs(maxStack,maxLocals);
            //this apparently tells asm that we want this
            //to be calculated automatically
            //mv.visitMaxs(-1,-1);

            //TODO: this seems to work but it should
            //really be calculated correctly
            mv.visitMaxs(10,10);
            mv.visitEnd();
        }
    }
}
