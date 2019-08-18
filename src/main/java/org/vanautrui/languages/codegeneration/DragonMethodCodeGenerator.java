package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.rows.DragonMethodScopeVariableSymbolTableRow;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.typeresolution.DragonTypeResolver;

import static org.objectweb.asm.Opcodes.*;

public class DragonMethodCodeGenerator {

    private static DragonMethodScopeVariableSymbolTable createMethodScopeSymbolTable(DragonMethodNode methodNode,DragonSubroutineSymbolTable subroutineSymbolTable)throws Exception{
        DragonMethodScopeVariableSymbolTable methodScopeSymbolTable=new DragonMethodScopeVariableSymbolTable();
        for(DragonStatementNode stmt : methodNode.statements) {

            //TODO: also get the assignment statements recursively
            //that are inside the control flow statements.
            //but that is for later

            if(stmt.statementNode instanceof DragonAssignmentStatementNode) {
                DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode)stmt.statementNode;

                String expressionType = DragonTypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subroutineSymbolTable);

                methodScopeSymbolTable.add(
                        new DragonMethodScopeVariableSymbolTableRow(
                                assignmentStatementNode.variableNode.name.getContents(),
                                expressionType
                        )
                );
            }
        }
        return methodScopeSymbolTable;
    }

    public static void visitMethodNode(ClassWriter cw, DragonClassNode classNode, DragonMethodNode methodNode, DragonSubroutineSymbolTable subroutineSymbolTable,boolean debug) throws Exception {
        String owner = classNode.name.typeName.getContents();
        String descriptor = "i do not know";
        String methodName = methodNode.methodName.methodName.name.getContents();

        //figure it out
        //cw.newMethod(owner,this.methodName.methodName.name.getContents(),descriptor,false);

        //make the method scope symbol table
        DragonMethodScopeVariableSymbolTable methodScopeSymbolTable = createMethodScopeSymbolTable(methodNode,subroutineSymbolTable);

        if(debug) {
            System.out.println(methodScopeSymbolTable.toString());
        }

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
                DragonStatementCodeGenerator.visitStatement(cw, mv, classNode, methodNode, stmt,subroutineSymbolTable,methodScopeSymbolTable);
            }

            mv.visitInsn(RETURN);

            //TODO: find out how to compute maxStack,
            //TODO: and maxLocals correctly

            int maxStack;

            final int maxLocals = methodScopeSymbolTable.size()+10;

            //maxLocals needs to be a bit bigger than
            //how many local variables we have, since it apparently
            //also has to be big enough to hold any arguments to methods
            //we might put on the stack

            mv.visitMaxs(10,maxLocals);
            mv.visitEnd();
        }
    }
}
