package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.objectweb.asm.ClassWriter;
import org.vanautrui.languages.codegeneration.IClassWriterByteCodeGeneratorVisitor;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeIdentifierNode;

import java.util.Optional;
import java.util.Set;

public class DragonClassFieldNode implements IDragonASTNode, IClassWriterByteCodeGeneratorVisitor {

    public DragonAccessModifierNode access;

    public DragonTypeIdentifierNode type;

    public DragonIdentifierNode name;

    public DragonClassFieldNode(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonClassFieldNode");

        DragonTokenList copy = tokens.copy();

        this.access = new DragonAccessModifierNode(copy);

        this.type = new DragonTypeIdentifierNode(copy);

        this.name = new DragonIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.access.toSourceCode()
                +" "+this.type.toSourceCode()
                +" "+this.name.toSourceCode()
                +";";
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //check that the type exists
        this.type.doTypeCheck(asts,currentClass,currentMethod);
    }

    @Override
    public void visit(ClassWriter cw, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) {
        String owner = currentClass.get().name.typeName.getContents();
        //TODO: figure out if we are doing this correctly, i doubt it
        String descriptor="i dont know";
        cw.newField(owner,this.name.name.getContents(),descriptor);
    }
}
