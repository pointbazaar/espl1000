package org.vanautrui.languages.compiler;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;

import static org.vanautrui.languages.TestUtils.parse_for_test;

public final class JSONDeserializationIntegrationTest {

	@Test
	public void testCanDeserializeEmptyNamespace()throws Exception{
		//depends on loop statement,return statement already working

		final String source=
				" ";

		final AST_Whole_Program ast = parse_for_test(source , false);
		Assert.assertEquals(1,ast.namespaceNodeList.size());
	}

	@Test
	public void testCanDeserializeSimpleFunction()throws Exception{
		//depends on loop statement,return statement already working

		final String source=
				"fn main ()->PInt { x = 3; } ";

		final AST_Whole_Program ast = parse_for_test(source , true);
		Assert.assertEquals(1,ast.namespaceNodeList.size());
	}
}
