package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.upperscopes;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;

import java.nio.file.Paths;

import static org.junit.Assert.*;

public class NamespaceNodeTest {

    @Test
    public void test_can_parse_class_with_1_empty_method() throws Exception {

        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public namespace Main { public ()~>PInt main{} }");

        NamespaceNode namespaceNode = new NamespaceNode(list, Paths.get("/dev/null"),false);

        assertEquals(namespaceNode.name.typeName,"Main");
        assertTrue(namespaceNode.isPublic);
        assertEquals(1, namespaceNode.methodNodeList.size());
    }

    @Test
    public void test_can_parse_class_with_1_method() throws Exception {

        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public namespace Main { public ()~>PInt main{main();} }");

        NamespaceNode namespaceNode = new NamespaceNode(list,Paths.get("/dev/null"),false);
        assertEquals(namespaceNode.name.typeName,"Main");
        assertTrue(namespaceNode.isPublic);
        assertEquals(1, namespaceNode.methodNodeList.size());
        MethodNode methodNode = namespaceNode.methodNodeList.get(0);
        assertEquals("main",methodNode.methodName);
        assertTrue(methodNode.has_side_effects);

    }

    @Test
    public void test_rejects_class_with_function_type()throws Exception{
        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public namespace ()->Main { public ()~>PInt main{main();} }");

        try {
            NamespaceNode namespaceNode = new NamespaceNode(list, Paths.get("/dev/null"), false);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }

    @Test
    public void test_can_call_subroutine_argument()throws Exception{
        String method1="public (PInt n)->PInt id{  return n; }";
        String method2="public ((PInt)~>PInt subr2)->PInt subr{  return subr2(2); }";
        String source="public namespace MainTest23138 { public ()~>PInt main{ putdigit(subr(id)); return 0;} "+method1+method2+" }";

        Lexer lexer = new Lexer();
        TokenList list = lexer.lexCodeTestMode(source);

        NamespaceNode namespaceNode = new NamespaceNode(list,Paths.get("/dev/null"),false);

        assertEquals(0,list.size());
    }

    @Test
    public void test_can_parse_class_subroutine_calling_subroutine_argument() throws Exception {

        Lexer lexer=new Lexer();
        TokenList list = lexer.lexCodeTestMode("public namespace Main { public ()~>PInt main{return subr(id);} (PInt n)->PInt id{return n;} ((PInt)->PInt f)~>PInt subr{return f(3);} }");

        NamespaceNode namespaceNode = new NamespaceNode(list, Paths.get("/dev/null"),false);

        assertEquals(namespaceNode.name.typeName,"Main");
        assertTrue(namespaceNode.isPublic);
        assertEquals(3, namespaceNode.methodNodeList.size());
    }
}
