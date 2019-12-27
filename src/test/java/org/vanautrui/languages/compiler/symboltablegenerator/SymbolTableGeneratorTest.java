package org.vanautrui.languages.compiler.symboltablegenerator;

import org.junit.Test;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.ArrayList;
import java.util.Optional;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.vanautrui.languages.TestUtils.parse_for_test;

public final class SymbolTableGeneratorTest {



    @Test
    public void test_finds_1_local_var_and_1_arg()throws Exception{

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        final MethodNode m1 = new MethodNode(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("PInt"))),"main");
        final MethodNode m2 = new MethodNode(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("PInt"))),"subr");

        m2.arguments.add(new DeclaredArgumentNode(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("PInt"))), Optional.of("n")));

        m2.statements.add(
            new StatementNode(
            new AssignmentStatementNode(
                        new VariableNode(new SimpleVariableNode("x")),
                    new ExpressionNode(new TermNode(new IntConstNode(3)))
                )
            )
        );

        final NamespaceNode namespaceNode = new NamespaceNode(m1,"Main");
        namespaceNode.methods.add(m2);

        //final NamespaceNode namespaceNode = new NamespaceNode(tokens,"Main",false);


        final StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(new AST_Whole_Program(namespaceNode),false);
        final LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(namespaceNode.methods.get(1),subTable,structsTable);

        assertTrue(localVarTable.containsVariable("x"));

        assertEquals(1,localVarTable.countArgs());
        assertEquals(1,localVarTable.countLocals());

        assertEquals("PInt",localVarTable.getTypeOfVariable("x").getTypeName());
        assertEquals(2,localVarTable.size());

        assertEquals(0,localVarTable.getIndexOfVariable("x"));
        assertEquals(0,localVarTable.getIndexOfVariable("n"));
    }

    @Test
    public void test_finds_local_vars_nested()throws Exception{

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        final AST_Whole_Program ast = parse_for_test("fn main ()~>PInt { x=3; if(x==3){y=5;} } ", false);

        final StructsSymbolTable structsTable =
                SymbolTableGenerator.createStructsSymbolTable(ast,false);
        final LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(new ArrayList<>(ast.namespaceNodeList).get(0).methods.get(0),subTable,structsTable);

        assertEquals(2,localVarTable.countLocals());
        assertEquals(0,localVarTable.countArgs());

        assertEquals(2,localVarTable.size());

        assertTrue(localVarTable.containsVariable("x"));
        assertEquals(0,localVarTable.getIndexOfVariable("x"));

        assertTrue(localVarTable.containsVariable("y"));
        assertEquals(1,localVarTable.getIndexOfVariable("y"));
    }

    @Test
    public void test_does_not_produce_duplicates()throws Exception{
        final AST_Whole_Program ast = parse_for_test("fn main ()~>PInt { x=3; x=2; return 0;} ", false);

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        final MethodNode myMethod = new ArrayList<>(ast.namespaceNodeList).get(0).methods.get(0);

        final StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(ast,false);
        final LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable,structsTable);

        assertEquals(1,localVarTable.countLocals());
        assertEquals(0,localVarTable.countArgs());

        assertTrue(localVarTable.containsVariable("x"));
        assertEquals(0,localVarTable.getIndexOfVariable("x"));
        assertEquals(1,localVarTable.size());
    }

    @Test
    public void test_does_not_produce_duplicates_with_different_types()throws Exception{

        final AST_Whole_Program ast = parse_for_test("fn main ()~>PInt { x=3; x=[1,2]; return 0;} ", false);

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        final MethodNode myMethod = new ArrayList<>(ast.namespaceNodeList).get(0).methods.get(0);

        final StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(ast,false);
        final LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable,structsTable);

        assertEquals(1,localVarTable.countLocals());
        assertEquals(0,localVarTable.countArgs());

        assertTrue(localVarTable.containsVariable("x"));
        assertEquals(0,localVarTable.getIndexOfVariable("x"));
        assertEquals(1,localVarTable.size());
    }

    @Test
    public void test_subroutine_symbol_table_generation()throws Exception{

        final AST_Whole_Program ast = parse_for_test("fn main ()~>PInt { x = 3; x = [ 1 , 2 ]; return 0; }  ", true);

        final SubroutineSymbolTable subTable = SymbolTableGenerator.createSubroutineSymbolTable(ast, false);

        assertEquals(1,subTable.getNumberOfLocalVariablesOfSubroutine("main"));
        assertEquals(0,subTable.getNumberOfArgumentsOfSubroutine("main"));
    }
}
