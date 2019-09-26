package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.upperscopes;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

import java.nio.file.Paths;

import static org.junit.Assert.*;

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

    @Test
    public void test_can_call_subroutine_argument()throws Exception{
        String method1="public (PInt n)->PInt id{  return n; }";
        String method2="public ((PInt)~>PInt subr2)->PInt subr{  return subr2(2); }";
        String source="public class MainTest23138 { public ()~>PInt main{ putdigit(subr(id)); return 0;} "+method1+method2+" }";

        Lexer lexer = new Lexer();
        TokenList list = lexer.lexCodeTestMode(source);

        ClassNode classNode = new ClassNode(list,Paths.get("/dev/null"),false);

        assertEquals(0,list.size());
    }

    @Test
    public void test_can_parse_class_subroutine_calling_subroutine_argument() throws Exception {

        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public class Main { public ()~>PInt main{return subr(id);} (PInt n)->PInt id{return n;} ((PInt)->PInt f)~>PInt subr{return f(3);} }");

        ClassNode classNode = new ClassNode(list, Paths.get("/dev/null"),false);

        assertEquals(classNode.name.typeName,"Main");
        assertTrue(classNode.isPublic);
        assertEquals(3,classNode.methodNodeList.size());
    }
}
