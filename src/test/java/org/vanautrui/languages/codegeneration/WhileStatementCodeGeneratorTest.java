package org.vanautrui.languages.codegeneration;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.DragonParser;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.vanautrui.languages.codegeneration.JavaByteCodeGeneratorTest.compile_and_run_one_class_for_testing;

public class WhileStatementCodeGeneratorTest {


    @Test
    public void test_can_while_statement()throws Exception{
        String source="public class MainTest22 { public Void main(){ i=0; while (i<1) { print(\"1\"); i=i+1; } } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest22");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }


}
