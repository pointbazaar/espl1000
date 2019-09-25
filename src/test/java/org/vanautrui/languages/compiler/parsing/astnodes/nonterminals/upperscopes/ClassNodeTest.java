package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.upperscopes;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

import java.nio.file.Paths;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class ClassNodeTest {

    @Test
    public void test_can_parse_class_with_1_empty_method() throws Exception {

        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public class Main { public ()~>PInt main{} }");

        ClassNode classNode = new ClassNode(list, Paths.get("/dev/null"),false);

        assertEquals(classNode.name.typeName,"Main");
        assertTrue(classNode.isPublic);
        assertEquals(1,classNode.methodNodeList.size());
    }

    @Test
    public void test_can_parse_class_with_1_method() throws Exception {

        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public class Main { public ()~>PInt main{main();} }");

        ClassNode classNode = new ClassNode(list,Paths.get("/dev/null"),false);
        assertEquals(classNode.name.typeName,"Main");
        assertTrue(classNode.isPublic);
        assertEquals(1,classNode.methodNodeList.size());
        MethodNode methodNode = classNode.methodNodeList.get(0);
        assertEquals("main",methodNode.methodName);
        assertTrue(methodNode.has_side_effects);

    }

    @Test
    public void test_rejects_class_with_function_type()throws Exception{
        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public class ()->Main { public ()~>PInt main{main();} }");

        try {
            ClassNode classNode = new ClassNode(list, Paths.get("/dev/null"), false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }
}
