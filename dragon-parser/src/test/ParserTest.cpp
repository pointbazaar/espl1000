package org.vanautrui.languages.test;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.test.astnodes.nonterminals.*;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.AssignmentStatementNodeTest;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.MethodCallNodeTest;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.StatementNodeTest;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow.IfStatementNodeTest;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow.LoopStatementNodeTest;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow.ReturnStatementNodeTest;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow.WhileStatementNodeTest;
import org.vanautrui.languages.test.astnodes.nonterminals.statements.upperscopes.MethodNodeTest;
import org.vanautrui.languages.test.astnodes.terminal.*;
import org.vanautrui.languages.test.astnodes.typenodes.BasicTypeWrappedNodeTest;
import org.vanautrui.languages.test.astnodes.typenodes.SimpleTypeNodeTest;
import org.vanautrui.languages.test.astnodes.typenodes.SubroutineTypeNodeTest;

public final class ParserTest {

	public static void test_all() throws Exception {
		//TODO: execute all tests and give feedback
		//every test should stdout its name, such that failing tests can be identified easily

		int count=0;
		int passed=0;

		passed += ParserTest.test_can_parse_some_class();
		passed += ParserTest.test_can_parse_some_class2();
		passed += ParserTest.test_can_parse_struct_access();
		count+=3;

		//nonterminals
			//statements
				//controlflow
				passed += IfStatementNodeTest.test1();
				passed += IfStatementNodeTest.test2();
				passed += IfStatementNodeTest.test3();
				count += 3;

				passed += LoopStatementNodeTest.test1();
				count += 1;

				passed += ReturnStatementNodeTest.test1();
				passed += ReturnStatementNodeTest.test2();
				passed += ReturnStatementNodeTest.test3();
				count += 3;

				passed += WhileStatementNodeTest.test1();
				count += 1;

			passed += AssignmentStatementNodeTest.test1();
			passed += AssignmentStatementNodeTest.test_assign_char();
			passed += AssignmentStatementNodeTest.test_assign_method_call_result();
			passed += AssignmentStatementNodeTest.test_assign_method_call_result_2();
			passed += AssignmentStatementNodeTest.test_assign_variable_with_array_index();
			passed += AssignmentStatementNodeTest.test_can_assign_to_struct_member();
			passed += AssignmentStatementNodeTest.test_type_declaration_for_variable();
			count += 7;

			passed += MethodCallNodeTest.test1();
			passed += MethodCallNodeTest.test2();
			passed += MethodCallNodeTest.test3();
			passed += MethodCallNodeTest.test_can_parse_subroutine_call();
			passed += MethodCallNodeTest.test_can_parse_subroutine_call2();
			passed += MethodCallNodeTest.test_can_parse_subroutine_call_with_type_param_argument();
			count += 6;

			passed += StatementNodeTest.test_assignment_statement_with_method_call();
			passed += StatementNodeTest.test_assignment_statement_with_struct_access();
			count += 2;

			//upperscopes
			passed += MethodNodeTest.test2();
			passed += MethodNodeTest.test_can_parse_method_with_arguments();
			passed += MethodNodeTest.test_can_parse_method_with_subroutine_argument();
			passed += MethodNodeTest.test_can_parse_method_without_arguments();
			passed += MethodNodeTest.test_can_parse_subroutine();
			passed += MethodNodeTest.test_parse_methodnode_with_struct_access_statements();
			count += 6;

		passed += ArrayConstantNodeTest.test_array_multiple_elements();
		passed += ArrayConstantNodeTest.test_empty_array();
		passed += ArrayConstantNodeTest.test_simple_array();
		count+=3;

		passed += DeclaredArgumentNodeTest.test_parse_declared_argument();
		count++;

		passed += StructMemberDeclNodeTest.test_can_parse_struct_member();
		count++;

		passed += TermNodeTest.test_simple_term();
		passed += TermNodeTest.test_variable_term();
		count+=2;

		passed += VariableNodeTest.test_parse_index_access();
		passed += VariableNodeTest.test_parse_struct_member_access();
		passed += VariableNodeTest.test_parse_struct_member_access_and_index_access();
		count  += 3;

		//terminal
		passed += BoolConstNodeTest.test_parse_bool_constant_node();
		passed += CharConstNodeTest.test_parse_char_constant_node();
		passed += CharConstNodeTest.test_parse_char_constant_node_newline();
		count+=3;

		passed += ExpressionNodeTest.recognize_string_constant_expression();
		passed += ExpressionNodeTest.test_simple_expression();
		passed += ExpressionNodeTest.test_variable_name_expression();
		count+=3;

		passed += FloatConstNodeTest.test1();
		passed += FloatConstNodeTest.test2();
		count+=2;

		passed += SimpleVariableNodeTest.test_parse_simple_indexed_variable();
		passed += SimpleVariableNodeTest.test_parse_simple_variable();
		count += 2;

		//typenodes
		passed += BasicTypeWrappedNodeTest.test_type_parsing2();
		passed += BasicTypeWrappedNodeTest.test_type_parsing_simple_type();
		passed += BasicTypeWrappedNodeTest.test_type_parsing_complicated();
		passed += BasicTypeWrappedNodeTest.test_type_parsing_basic_type_wrapped_node();
		count+=4;

		passed += SimpleTypeNodeTest.test_typenode_parsing();
		passed += SimpleTypeNodeTest.test_typenode_parsing_anytype();
		passed += SimpleTypeNodeTest.test_typenode_parsing_fails();
		count+=3;

		passed += SubroutineTypeNodeTest.test_subroutine_type_parsing_subroutine_with_side_effects();
		passed += SubroutineTypeNodeTest.test_subroutine_type_parsing_subroutine_with_subroutine_argument();
		passed += SubroutineTypeNodeTest.test_subroutine_type_parsing_subroutine_without_side_effects();
		passed += SubroutineTypeNodeTest.test_typename();
		passed += SubroutineTypeNodeTest.test_typename_subroutine_return_type();
		count+=5;

		System.out.println(String.format("passed %d of %d ",passed,count));
	}

	public static int test_can_parse_some_class() throws Exception {
		System.out.println("test_can_parse_some_class");

		final TokenList tokens = ParserPhases.makeTokenList(
				"fn main ()~>PInt {" +
							"println(1);" +
							"return 0;" +
						"}",
				false
		);


		try {
			AST_Whole_Program astWholeProgram = new AST_Whole_Program(tokens, "Main", false);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_can_parse_some_class2() throws Exception {
		System.out.println("test_can_parse_some_class2");

		final TokenList tokens = ParserPhases.makeTokenList(
				"fn main ()~>PInt {" +

						"x=3;" +

						"if(x<5){" +
						"println(\"x<5\");" +
						"}" +

						"return 0;" +
						"}",
				false);
		//System.out.println(tokens.toString());

		try {
			final AST_Whole_Program astWholeProgram = new AST_Whole_Program(tokens, "Main", false);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_can_parse_struct_access() throws Exception {
		System.out.println("test_can_parse_struct_access");

		final TokenList tokens = ParserPhases.makeTokenList(

				"struct MyStruct{"
						+ "PInt a"
						+ "}"

						+ "fn main ()~>PInt {"
						+ "x=flip(y);"
						+ "x.a=3;"

						+ "}",
				false);

		final AST_Whole_Program astWholeProgram = new AST_Whole_Program(tokens, "Main", false);

		boolean assrt = 0 == tokens.size(); //all tokens should have been consumed

		return (assrt)?1:0;
	}
}
