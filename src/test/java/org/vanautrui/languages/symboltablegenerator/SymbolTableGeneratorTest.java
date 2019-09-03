package org.vanautrui.languages.symboltablegenerator;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.Lexer;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.Parser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;

import java.util.stream.Collectors;

public class SymbolTableGeneratorTest {

    @Test
    public void test_finds_1_local_var()throws Exception{

        Lexer lexer = new Lexer();

        TokenList tokens = lexer.lexCodeWithoutComments(new CharacterList(
                "class Main{ Int main(){  } Int subr(Int n){x=3;} }"
        ));

        Parser parser = new Parser();
        AST ast = parser.parseTestMode(tokens);

        SubroutineSymbolTable subTable = new SubroutineSymbolTable();

        MethodNode myMethod = ast.classNodeList.stream().collect(Collectors.toList()).get(0).methodNodeList.get(1);

        LocalVarSymbolTable localVarTable = SymbolTableGenerator.createMethodScopeSymbolTable(myMethod,subTable);

        Assert.assertTrue(localVarTable.containsVariable("x"));

        Assert.assertEquals(1,localVarTable.countArgs());
        Assert.assertEquals(1,localVarTable.countLocals());

        Assert.assertEquals("Int",localVarTable.getTypeOfVariable("x"));
        Assert.assertEquals(2,localVarTable.size());
        Assert.assertEquals(1,localVarTable.getIndexOfVariable("x"));
    }
}
