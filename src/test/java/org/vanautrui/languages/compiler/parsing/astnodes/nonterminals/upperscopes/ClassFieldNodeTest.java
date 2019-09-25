package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.upperscopes;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassFieldNode;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class ClassFieldNodeTest {

    @Test
    public void test_can_parse_class_field_node() throws Exception {

        final String source="public PInt myfield;";
        Lexer lexer = new Lexer();
        TokenList list = lexer.lexCodeTestMode(source);

        ClassFieldNode fieldNode = new ClassFieldNode(list, true);

        assertEquals(list.size(),0);
        assertTrue(fieldNode.access.is_public);
        assertEquals(fieldNode.type.getTypeName(),"PInt");
        assertEquals(fieldNode.name,"myfield");
    }

    @Test
    public void test_can_parse_class_field_node_with_subroutine_type() throws Exception {

        final String source="public (PInt)->PInt myfield;";
        Lexer lexer = new Lexer();
        TokenList list = lexer.lexCodeTestMode(source);

        ClassFieldNode fieldNode = new ClassFieldNode(list, true);

        assertEquals(list.size(),0);
        assertTrue(fieldNode.access.is_public);
        assertEquals(fieldNode.type.getTypeName(),"(PInt)->PInt");
        assertEquals(fieldNode.name,"myfield");
    }

}
