package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.IMethodWriterByteCodeGeneratorVisitor;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.Optional;
import java.util.Set;

public class DragonStatementNode implements IDragonASTNode, IMethodWriterByteCodeGeneratorVisitor {

    //TODO: add more statement types

    public DragonMethodCallNode methodCallNode;

    public DragonStatementNode(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonStatementNode");

        DragonTokenList copy = tokens.copy();

        this.methodCallNode = new DragonMethodCallNode(copy);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.methodCallNode.toSourceCode();
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        methodCallNode.doTypeCheck(asts,currentClass,currentMethod);
    }

    @Override
    public void visit(MethodVisitor mv, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) {
        //TODO: consider other statement types and such
        this.methodCallNode.visit(mv,currentClass,currentMethod);
    }
}
