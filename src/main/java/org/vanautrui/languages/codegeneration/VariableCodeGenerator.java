package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.VariableNode;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.typeresolution.TypeResolver;

import static org.objectweb.asm.Opcodes.*;

public class VariableCodeGenerator {

    public static void visitVariableNode(
            ClassWriter cw,
            ClassNode classNode,
            VariableNode varNode,
             MethodVisitor mv,
             MethodNode methodNode,
             LocalVarSymbolTable varTable,
             SubroutineSymbolTable subTable,
            boolean debug
    ) throws Exception{
        if (varTable.containsVariable(varNode.name)) {
            String type = varTable.getTypeOfVariable(varNode.name);

            //may return something else, if variable has an index
            String instanceType = TypeResolver.getTypeVariableNode(varNode,methodNode,subTable,varTable);

            if(!instanceType.equals(type) && varNode.indexOptional.isPresent()){
                //we have an array
                //TODO: do this for types other than integer
                

                //load the array
                mv.visitIntInsn(ALOAD,varTable.getIndexOfVariable(varNode.name));

                //index the array
                //index should have type int, that should have been checked at typechecking
                ExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,varNode.indexOptional.get(),varTable,subTable,debug);

                //load from array at that index
                switch (instanceType) {
                    case "Int":
                        mv.visitInsn(IALOAD);
                        break;
                    case "String":
                        mv.visitInsn(AALOAD);
                        break;
                    default:
                        throw new Exception("only Int,String accepted for now");
                }
            }else {

                switch (type) {
                    case "Int":
                    case "Char":
                    case "Bool":
                        mv.visitIntInsn(
                                ILOAD,
                                varTable.getIndexOfVariable(
                                        varNode.name
                                )
                        );
                        break;
                    case "Float":
                        mv.visitIntInsn(
                                FLOAD,
                                varTable.getIndexOfVariable(varNode.name)
                        );
                        break;
                    case "[Int]":
                    case "[Float]":
                    case "[Bool]":
                    case "String":
                        mv.visitIntInsn(ALOAD, varTable.getIndexOfVariable(varNode.name));
                        break;

                    default:
                        throw new Exception(
                                "unconsidered case in VariableCodeGenerator: type:" + type+" in code: "
                                +"'"+varNode.toSourceCode()+"'"
                        );
                }
            }
        } else {
            throw new Exception("variable " + varNode.name + " not defined?");
        }
    }
}
