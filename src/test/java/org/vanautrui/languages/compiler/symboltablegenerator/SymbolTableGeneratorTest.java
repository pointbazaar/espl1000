package org.vanautrui.languages.compiler.symboltablegenerator;

import org.junit.Test;
import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public final class SymbolTableGeneratorTest {

    private AST_Whole_Program parse_for_test(String s)throws Exception{

        final TokenList tokens = ParserPhases.makeTokenList(s);

        final Parser parser = new Parser();
        final AST_Whole_Program ast = parser.parseTestMode(tokens,false,"Main");
        return ast;
    }

    @Test
    public void test_finds_1_local_var_and_1_arg()throws Exception{

        AST_Whole_Program ast = parse_for_test(" fn main () ~> PInt {  } fn subr (PInt n) ~> PInt { x = 3; } ");

        SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        MethodNode myMethod = ast.namespaceNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(1);


        StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(ast,false);
        LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable,structsTable);

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
        final AST_Whole_Program ast = parse_for_test(" ()~>PInt main{ x=3; if(x==3){y=5;} } ");

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        MethodNode myMethod = ast.namespaceNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(0);

        StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(ast,false);
        LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable,structsTable);

        //System.out.println(localVarTable.toString());

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
        AST_Whole_Program ast = parse_for_test(" ()~>PInt main{ x=3; x=2; return 0;} ");

        SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        MethodNode myMethod = ast.namespaceNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(0);

        StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(ast,false);
        LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable,structsTable);

        //System.out.println(localVarTable.toString());

        assertEquals(1,localVarTable.countLocals());
        assertEquals(0,localVarTable.countArgs());

        assertTrue(localVarTable.containsVariable("x"));
        assertEquals(0,localVarTable.getIndexOfVariable("x"));
        assertEquals(1,localVarTable.size());
    }

    @Test
    public void test_does_not_produce_duplicates_with_different_types()throws Exception{
        AST_Whole_Program ast = parse_for_test(" ()~>PInt main{ x=3; x=[1,2]; return 0;} ");

        SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        MethodNode myMethod = ast.namespaceNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(0);


        StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(ast,false);
        LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable,structsTable);

        //System.out.println(localVarTable.toString());

        assertEquals(1,localVarTable.countLocals());
        assertEquals(0,localVarTable.countArgs());

        assertTrue(localVarTable.containsVariable("x"));
        assertEquals(0,localVarTable.getIndexOfVariable("x"));
        assertEquals(1,localVarTable.size());
    }

    @Test
    public void test_subroutine_symbol_table_generation()throws Exception{

        AST_Whole_Program ast = parse_for_test(" ()~>PInt main{ x=3; x=[1,2]; return 0;}  ");

        SubroutineSymbolTable subTable = SymbolTableGenerator.createSubroutineSymbolTable(ast, false);

        assertEquals(1,subTable.getNumberOfLocalVariablesOfSubroutine("main"));
        assertEquals(0,subTable.getNumberOfArgumentsOfSubroutine("main"));
    }
}
