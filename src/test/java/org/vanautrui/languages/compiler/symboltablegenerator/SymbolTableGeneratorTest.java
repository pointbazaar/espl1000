package org.vanautrui.languages.compiler.symboltablegenerator;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.Parser;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;

public class SymbolTableGeneratorTest {

    private AST parse_for_test(String s)throws Exception{
        Lexer lexer = new Lexer();

        TokenList tokens = lexer.lexCodeWithoutComments(new CharacterList(
                s
        ));

        Parser parser = new Parser();
        AST ast = parser.parseTestMode(tokens);
        return ast;
    }

    @Test
    public void test_finds_1_local_var_and_1_arg()throws Exception{

        AST ast = parse_for_test("class Main{ Int main(){  } Int subr(Int n){x=3;} }");

        SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        MethodNode myMethod = ast.classNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(1);

        LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable);

        Assert.assertTrue(localVarTable.containsVariable("x"));

        assertEquals(1,localVarTable.countArgs());
        assertEquals(1,localVarTable.countLocals());

        assertEquals("Int",localVarTable.getTypeOfVariable("x"));
        assertEquals(2,localVarTable.size());
        assertEquals(1,localVarTable.getIndexOfVariable("x"));
    }

    @Test
    public void test_finds_local_vars_nested()throws Exception{
        AST ast = parse_for_test("class Main{ Int main(){ x=3; if(x==3){y=5;} } }");

        SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        MethodNode myMethod = ast.classNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(0);

        LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable);

        //System.out.println(localVarTable.toString());
        assertEquals(2,localVarTable.countLocals());
    }

    //TODO: test the subroutine symbol table generation
}
