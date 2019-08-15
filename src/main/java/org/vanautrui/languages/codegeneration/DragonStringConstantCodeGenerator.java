package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;

import static org.objectweb.asm.Opcodes.BIPUSH;

public class DragonStringConstantCodeGenerator {

    public static void visitStringConstant(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonStringConstantNode stringConstantNode,
            DragonMethodScopeSymbolTable methodScopeSymbolTable
    ) throws Exception {
        //load constant
        mv.visitLdcInsn(stringConstantNode.str);
    }
}
