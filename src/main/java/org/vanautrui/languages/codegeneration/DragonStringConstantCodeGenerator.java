package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;

public class DragonStringConstantCodeGenerator {

    public static void visitStringConstant(
            MethodVisitor mv,
            DragonStringConstantNode stringConstantNode
    ) throws Exception {
        //load constant
        mv.visitLdcInsn(stringConstantNode.str);
    }
}
