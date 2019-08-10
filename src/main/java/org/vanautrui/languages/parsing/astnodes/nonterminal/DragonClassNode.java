package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.objectweb.asm.ClassWriter;
import org.vanautrui.languages.codegeneration.IClassWriterByteCodeGeneratorVisitor;
import org.vanautrui.languages.lexing.tokens.ClassToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeIdentifierNode;

import javax.swing.text.html.Option;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import static org.objectweb.asm.Opcodes.*;

public class DragonClassNode implements IDragonASTNode, IClassWriterByteCodeGeneratorVisitor {

    public DragonAccessModifierNode access;

    public DragonTypeIdentifierNode name;

    public List<DragonClassFieldNode> fieldNodeList = new ArrayList<>();

    public List<DragonMethodNode> methodNodeList = new ArrayList<>();


    public DragonClassNode(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonClassNode");

        DragonTokenList copy = tokens.copy();

        //System.out.println(copy.toString());

        this.access = new DragonAccessModifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new ClassToken());

        this.name = new DragonTypeIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        //System.out.println(copy.toString());

        /*
        boolean success_field=true;
        while(success_field) {

            try {
                this.fieldNodeList.add(new DragonClassFieldNode(copy));
            } catch (Exception e) {
                success_field = false;
            }
        }
        //Thread.sleep(100);
        //System.out.println(copy.toString());
        boolean success_method=true;
        while(success_method) {

            try {
                this.methodNodeList.add(new DragonMethodNode(copy));
            } catch (Exception e) {
                success_method = false;
            }
        }
         */

        //i hope, that with this piece of code, the method should always be tried out first
        //because classField is a prefix of Method.
        //similar errors could maybe be fixed by just looking at the Dragon Grammar
        //and structuring the parser accordingly
        boolean success_method = true;
        boolean success_field = true;
        while (success_field || success_method) {
            try {
                this.methodNodeList.add(new DragonMethodNode(copy));
                success_method = true;
            } catch (Exception e) {
                success_method = false;
            }
            try {
                this.fieldNodeList.add(new DragonClassFieldNode(copy));
                success_field = true;
            } catch (Exception e) {
                success_field = false;
            }
        }


        //System.out.println(copy.toString());

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }


    @Override
    public String toSourceCode() {
        String result = "";

        result+=this.access.toSourceCode()+" class "+this.name.toSourceCode()+" {";

        result += fieldNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));
        result+="\n";

        result += methodNodeList
                .stream()
                .map(node -> node.toSourceCode())
                .collect(Collectors.joining("\n"));

        result+="\n";
        result+="}";
        return result;
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {

        int count=0;
        for(DragonAST ast : asts){
            for(DragonClassNode classNode : ast.classNodeList){
                if(classNode.name.typeName.getContents().equals(this.name.typeName.getContents())){
                    count++;
                }
            }
        }

        //check that fields and methods are typesafe

        for(DragonClassFieldNode fieldNode : this.fieldNodeList){
            fieldNode.doTypeCheck(asts,Optional.of(this),Optional.empty());
        }

        for(DragonMethodNode methodNode : this.methodNodeList){
            methodNode.doTypeCheck(asts,Optional.of(this),Optional.empty());
        }

        if(count!=1){
            throw new Exception("multiple definitions of class '"+this.name+"'");
        }
    }

    @Override
    public void visit(ClassWriter cw, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) {
        //cw.newClass(this.name.typeName.getContents());

        //TODO: handle access modifiers
        //TODO: handle other modifiers

        int access = ACC_SUPER;
        if(this.access.is_public){
            access+=ACC_PUBLIC;
        }else{
            access+=ACC_PRIVATE;
        }

        int classFileVersion = 49;

        String superClassName = "java/lang/Object";

        cw.visit(classFileVersion,
                access,
                this.name.typeName.getContents(),
                null,
                superClassName,
                null);

        for(DragonClassFieldNode fieldNode : this.fieldNodeList){

            fieldNode.visit(cw, Optional.of(this),Optional.empty());
        }

        for(DragonMethodNode methodNode : this.methodNodeList){
            methodNode.visit(cw,Optional.of(this),Optional.empty());
        }

        cw.visitEnd();
    }
}
