package org.vanautrui.languages.test.astnodes.nonterminals.statements.upperscopes;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;

public final class StructDeclNodeTest {

	public static int test_can_parse_empty_struct_decl() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("struct MyStruct{}", false);

		StructDeclNode node = new StructDeclNode(list, false);

		boolean assert1 = (0 == node.members.size());

		return (assert1)?1:0;
	}

	public static int test_will_not_parse_invalid_typename_for_struct() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("struct myStruct{}", false);

		try {
			final StructDeclNode node = new StructDeclNode(list, false);
			return 0;
		} catch (Exception e) {
			return 1;
		}
	}

	public static int test_rejects_struct_with_subroutine_type() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("struct ()~>PInt{PInt a}", false);

		try {
			StructDeclNode node = new StructDeclNode(list, false);
			return 0;
		} catch (Exception e) {
			return 1;
		}
	}

	public static int test_can_parse_struct_with_1_member() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("struct MyStruct{PInt a}", false);

		final StructDeclNode node = new StructDeclNode(list, false);

		final boolean assert1 = (1 == node.members.size());
		final boolean assert2 = "a".equals( node.members.get(0).name);

		return (assert1&&assert2)?1:0;
	}

	public static int test_can_parse_struct_with_2_members() throws Exception {

		final TokenList list = ParserPhases.makeTokenList("struct MyStruct{PInt a,MyStruct b}", false);

		final StructDeclNode node = new StructDeclNode(list, false);

		final boolean assert1 = (2 == node.members.size());

		final boolean assert2 = "a".equals( node.members.get(0).name);

		final boolean assert3 = "b".equals( node.members.get(1).name);

		return (assert1&&assert2&&assert3)?1:0;
	}
}
