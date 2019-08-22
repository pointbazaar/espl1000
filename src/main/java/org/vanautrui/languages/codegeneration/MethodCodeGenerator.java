package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.symboltables.nameconversions.TypeNameToJVMInternalTypeNameConverter;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;

import static org.vanautrui.languages.symboltablegenerator.SymbolTableGenerator.*;

import java.util.stream.Collectors;

import static org.objectweb.asm.Opcodes.*;

public class MethodCodeGenerator {

    public static void visitMethodNode(ClassWriter cw, ClassNode classNode, MethodNode methodNode, SubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception {
        String returnTypeName=methodNode.type.typeName;


        String owner = classNode.name.typeName;
        String descriptor = TypeNameToJVMInternalTypeNameConverter
                .convertSubroutineName(
                        returnTypeName,
                        methodNode.arguments
                                .stream()
                                .map(arg->arg.type.typeName)
                                .collect(Collectors.toList()),
                        debug
                );
        String methodName = methodNode.methodName.methodName.name;

        //figure it out
        //cw.newMethod(owner,this.methodName.methodName.name.getContents(),descriptor,false);

        //make the method scope symbol table
        LocalVarSymbolTable methodScopeSymbolTable = createMethodScopeSymbolTable(methodNode,subroutineSymbolTable);

        if(debug) {
            System.out.println(methodScopeSymbolTable.toString());
        }

        //TODO: do not make this static
        //TODO: actually care about it
        //TODO: while typechecking, it should be seen
        //that there is an entry point in the program somewhere
        {
            //the main method
            if(methodName.equals("main")){
                descriptor="([Ljava/lang/String;)V";
            }

            MethodVisitor mv=cw.visitMethod(ACC_PUBLIC+ACC_STATIC,
                    methodName,
                    descriptor,
                    null,
                    null);

            //TODO: compile the local variable declarations
            //TODO: compile the statements in the method

            //stmt->stmt.visit(mv,Optional.of(classNode),Optional.of(methodNode))
            for (StatementNode stmt : methodNode.statements) {
                StatementCodeGenerator.visitStatement(cw, mv, classNode, methodNode, stmt,subroutineSymbolTable,methodScopeSymbolTable,debug);
            }


            if(returnTypeName.equals("Void")) {
                mv.visitInsn(RETURN);
            }

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
