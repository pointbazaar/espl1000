package org.vanautrui.languages.compiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.vanautrui.languages.TestUtils.compileAndRunProgramForTesting;

public final class WhileStatementCodeGeneratorTest {


    @Test
    public void test_can_while_statement()throws Exception{
        final String source="fn main ()~>PInt { i = 0; while ( i < 3 ) { putchar('1'); i = i+1; } return 0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest22",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("111",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_while_statement_and_call_builtin_subroutines()throws Exception{
        final String source="fn main ()~>PInt { [PInt] arr=new(2); i=0; while (i<len(arr)) { putchar('1'); i=i+1; } return 0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest223",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("11",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_while_statement_and_call_builtin_subroutines_other_subroutine()throws Exception{

        final String printarr = "fn printarr ([PInt] arr)~>PInt { i = 0; while ( i < len(arr)) { putchar('1'); i = i+1; } return 0;}";

        final String main = "fn main ()~>PInt { [PInt] arr=new(2); printarr(arr); return 0;}";

        final String source=" "+main+" "+printarr+" ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest223",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("11",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_while_statement_and_call_builtin_subroutines_other_subroutine_and_assign()throws Exception{

        final String printarr = "fn printarr ([PInt] arr)~>PInt { i = 0; while (i < len(arr)) { putchar('1'); i = i+1; } return 0;}";

        //arr[0]=0; arr[1]=1; seems to be causing problems later on
        final String main = "fn main ()~>PInt { [PInt] arr=new(2); arr[0]=0; arr[1]=1; printarr(arr); return 0;}";

        final String source=" "+main+" "+printarr+" ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest223",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("11",IOUtils.toString(pr.getInputStream()));
    }
}
