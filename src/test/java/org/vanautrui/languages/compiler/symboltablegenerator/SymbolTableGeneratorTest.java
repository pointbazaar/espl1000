package org.vanautrui.languages.compiler.symboltablegenerator;

import org.junit.Test;
import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.tokens.*;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.*;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.keywords.FnToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.keywords.IfToken;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.nio.file.Paths;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public final class SymbolTableGeneratorTest {

    private AST_Whole_Program parse_for_test(String s)throws Exception{

        final TokenList tokens = ParserPhases.makeTokenList(s);

        final AST_Whole_Program ast = Parser.parseTestMode(tokens,false,"Main");
        return ast;
    }

    @Test
    public void test_finds_1_local_var_and_1_arg()throws Exception{

        //final AST_Whole_Program ast = parse_for_test(" fn main () ~> PInt {  } fn subr (PInt n) ~> PInt { x = 3; } ");

        final TokenList tokens = new TokenList(Paths.get("/dev/null"));

        tokens.addAll(new IToken[]{
            new FnToken(),
            new IdentifierToken("main"),
            new LParensToken(),
            new RParensToken(),
            new ArrowToken(false,false),
            new TypeIdentifierToken("PInt"),
            new LCurlyToken(),
            new RCurlyToken(),

            new FnToken(),
            new IdentifierToken("subr"),
            new LParensToken(),
                new TypeIdentifierToken("PInt"),
                new IdentifierToken("n"),
            new RParensToken(),
            new ArrowToken(false,false),
            new TypeIdentifierToken("PInt"),

            new LCurlyToken(),
                new IdentifierToken("x"),
                new OperatorToken("="),
                new IntegerNonNegativeConstantToken(3),
                new SemicolonToken(),
            new RCurlyToken(),

        });

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        final NamespaceNode namespaceNode = new NamespaceNode(tokens,"Main",false);


        final StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(new AST_Whole_Program(namespaceNode),false);
        final LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(namespaceNode.methodNodeList.get(1),subTable,structsTable);

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

        final TokenList tokens = new TokenList(Paths.get("/dev/null"));
        tokens.addAll(new IToken[]{
            new FnToken(),
            new IdentifierToken("main"),
            new LParensToken(),
            new RParensToken(),
            new ArrowToken(false,false),
            new TypeIdentifierToken("PInt"),

            new LCurlyToken(),
                new IdentifierToken("x"),
                new OperatorToken("="),
                new IntegerNonNegativeConstantToken(3),
                new SemicolonToken(),
                new IfToken(),
                new LParensToken(),
                    new IdentifierToken("x"),
                    new OperatorToken("=="),
                    new IntegerNonNegativeConstantToken(3),
                new RParensToken(),
                new LCurlyToken(),
                    new IdentifierToken("y"),
                    new OperatorToken("="),
                    new IntegerNonNegativeConstantToken(5),
                    new SemicolonToken(),
                new RCurlyToken(),
            new RCurlyToken(),
        });

        final MethodNode myMethod = new MethodNode(tokens,false);

        final StructsSymbolTable structsTable =
                SymbolTableGenerator.createStructsSymbolTable(new AST_Whole_Program(new NamespaceNode(myMethod,"Main")),false);
        final LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable,structsTable);

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
        final AST_Whole_Program ast = parse_for_test("fn main ()~>PInt { x=3; x=2; return 0;} ");

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        final MethodNode myMethod = ast.namespaceNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(0);

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

        final AST_Whole_Program ast = parse_for_test("fn main ()~>PInt { x=3; x=[1,2]; return 0;} ");

        final SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        final MethodNode myMethod = ast.namespaceNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(0);

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

        final AST_Whole_Program ast = parse_for_test("fn main ()~>PInt { x=3; x=[1,2]; return 0;}  ");

        final SubroutineSymbolTable subTable = SymbolTableGenerator.createSubroutineSymbolTable(ast, false);

        assertEquals(1,subTable.getNumberOfLocalVariablesOfSubroutine("main"));
        assertEquals(0,subTable.getNumberOfArgumentsOfSubroutine("main"));
    }
}
