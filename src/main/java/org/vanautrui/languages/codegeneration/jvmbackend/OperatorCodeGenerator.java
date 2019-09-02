package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;

import static org.objectweb.asm.Opcodes.*;

public class OperatorCodeGenerator {

    public static void visitOperatorNode(ClassWriter cw, MethodVisitor mv, ClassNode classNode, MethodNode methodNode, OperatorNode operatorNode, String operandType) throws Exception {

	    //TODO: this method needs a string of the type of the 
	    //values (for now, they should have the same type)
	    //it is supposed to generate the operator for
	    //for now it can only operate on Int
	switch(operandType){
		case "Int":
			visitOperandNodeForInt(mv,operatorNode);
			break;
		case "Float":
			visitOperandNodeForFloat(mv,operatorNode);
			break;
		case "Bool":
			visitOperandNodeForBool(mv,operatorNode);
			break;
		default:
			throw new Exception("unrecognized operand type "+operandType);
	}
    }

	private static void visitOperandNodeForBool(MethodVisitor mv, OperatorNode opNode) throws Exception{
		switch (opNode.operator) {
			case "&&":
				mv.visitInsn(IAND);
				break;
			case "||":
				mv.visitInsn(IOR);
				break;
			default:
				throw new Exception("unsupported operation on Bool values " + opNode.operator);
		}
	}


	private static void visitOperandNodeForFloat(MethodVisitor mv, OperatorNode opNode) throws Exception{
		switch (opNode.operator){
		    case "+":
			mv.visitInsn(FADD);
			break;
		    case "-":
			mv.visitInsn(FSUB);
			break;
		    case "/":
			mv.visitInsn(FDIV);
			break;
		    case "*":
			mv.visitInsn(FMUL);
			break;
		    case "<":
			//fcmpl
			//push 1
			//if_icmpEQ,truthlabel
		    case ">":
			//fcmpg
			//push 1
			//if_icmpEQ,truthlabel
		    case "==":
			//fcmpg
			//push 0
			//if_icmpEQ,truthlabel
		    case "<=":
			//fcmpl
			//push 0
			//if_icmpGE,truthlabel
		    case ">=":
			//fcmpg
			//push 0
			//if_icmpGE,truthlabel
		    case "!=":
			//fcmpl
			//push 0
			//if_icmpNE,truthlabel

			Label truthCaseLabel = new Label();
			Label endLabel = new Label();

			if(opNode.operator.equals("<")) {
				mv.visitInsn(FCMPL);
				mv.visitInsn(ICONST_1);
				mv.visitJumpInsn(IF_ICMPEQ, truthCaseLabel);
			}else if(opNode.operator.equals(">")){
				mv.visitInsn(FCMPG);
				mv.visitInsn(ICONST_1);
				mv.visitJumpInsn(IF_ICMPEQ, truthCaseLabel);
			}else if(opNode.operator.equals("==")){
				mv.visitInsn(FCMPL);
				mv.visitInsn(ICONST_0);
				mv.visitJumpInsn(IF_ICMPEQ, truthCaseLabel);
			}else if(opNode.operator.equals("<=")) {
				mv.visitInsn(FCMPL);
				mv.visitInsn(ICONST_0);
				mv.visitJumpInsn(IF_ICMPGE, truthCaseLabel);
			}else if(opNode.operator.equals(">=")){
				mv.visitInsn(FCMPG);
				mv.visitInsn(ICONST_0);
				mv.visitJumpInsn(IF_ICMPGE, truthCaseLabel);
			}else if(opNode.operator.equals("!=")) {
				mv.visitInsn(FCMPL);
				mv.visitInsn(ICONST_0);
				mv.visitJumpInsn(IF_ICMPNE, truthCaseLabel);
			}

			mv.visitInsn(ICONST_0);
			mv.visitJumpInsn(GOTO,endLabel);


			mv.visitLabel(truthCaseLabel);
			mv.visitInsn(ICONST_1);
			mv.visitJumpInsn(GOTO,endLabel);

			mv.visitLabel(endLabel);
			break;
		    default:
			throw new Exception("unhandled case in DragonOperatorCodeGenerator : operator is "+opNode.operator);
		}
	}

	private static void visitOperandNodeForInt(MethodVisitor mv, OperatorNode opNode) throws Exception{
		switch (opNode.operator){
		    case "+":
			mv.visitInsn(IADD);
			break;
		    case "-":
			mv.visitInsn(ISUB);
			break;
		    case "/":
			mv.visitInsn(IDIV);
			break;
		    case "*":
			mv.visitInsn(IMUL);
			break;
		    case "<":
		    case ">":
		    case "==":
		    case "<=":
		    case ">=":
		    case "!=":
			//if less than, push 1
			//else push 0

			Label truthCaseLabel = new Label();
			Label endLabel = new Label();

			if(opNode.operator.equals("<")) {
			    mv.visitJumpInsn(IF_ICMPLT, truthCaseLabel);
			}else if(opNode.operator.equals(">")){
			    mv.visitJumpInsn(IF_ICMPGT, truthCaseLabel);
			}else if(opNode.operator.equals("==")){
			    mv.visitJumpInsn(IF_ICMPEQ,truthCaseLabel);
			}else if(opNode.operator.equals(">=")) {
			    mv.visitJumpInsn(IF_ICMPGE,truthCaseLabel);
			}else if(opNode.operator.equals("<=")){
			    mv.visitJumpInsn(IF_ICMPLE,truthCaseLabel);
			}else if(opNode.operator.equals("!=")) {
			    mv.visitJumpInsn(IF_ICMPNE, truthCaseLabel);
			}

			mv.visitInsn(ICONST_0);
			mv.visitJumpInsn(GOTO,endLabel);


			mv.visitLabel(truthCaseLabel);
			mv.visitInsn(ICONST_1);
			mv.visitJumpInsn(GOTO,endLabel);

			mv.visitLabel(endLabel);
			break;
		    default:
			throw new Exception("unhandled case in DragonOperatorCodeGenerator : operator is "+opNode.operator);
		}
	}
}
