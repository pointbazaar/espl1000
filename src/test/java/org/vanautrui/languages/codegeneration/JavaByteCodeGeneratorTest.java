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

public class JavaByteCodeGeneratorTest {

    private Process compile_and_run_one_class_for_testing(String source,String classNameWithoutExtension) throws Exception{

        DragonTokenList tokens = (new DragonLexer()).lexCodeWithoutComments(source);
        DragonParser parser = new DragonParser();
        DragonAST ast= parser.parse(tokens);

        byte[] result = JavaByteCodeGenerator.generateByteCodeForClass(ast.classNodeList.toArray(new DragonClassNode[]{})[0]);
        Path path = Paths.get(classNameWithoutExtension+".class");

        Files.write(path,result);
        Process pr = Runtime.getRuntime().exec("java "+classNameWithoutExtension);

        pr.waitFor();

        //maybe it is safe to delete already
        Files.delete(path);

        return pr;
    }

    @Test
    public void test_can_compile_simple_helloworld()throws Exception{
        String source="public class MainTest1 { public Void main(){ println(\"Hello World!\"); }}";
        DragonTokenList tokens = (new DragonLexer()).lexCodeWithoutComments(source);

        DragonParser parser = new DragonParser();
        DragonAST ast= parser.parse(tokens);

        byte[] result = JavaByteCodeGenerator.generateByteCodeForClass(ast.classNodeList.toArray(new DragonClassNode[]{})[0]);
        Path path = Paths.get("MainTest1.class");

        Files.write(path,result);

        //TODO: execute that bytecode and assert the result
        Process pr = Runtime.getRuntime().exec("java MainTest1");

        pr.waitFor();

        Assert.assertEquals(0,pr.exitValue());

        String output = IOUtils.toString(pr.getInputStream());

        Files.delete(path);

        Assert.assertEquals("Hello World!\n",output);
    }

    @Test
    public void test_can_compile_loop_statements()throws Exception{
        String source="public class MainTest2 { public Void main(){ loop 4 { print(\"1\"); } } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest2");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1111",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_assignment_statements()throws Exception{
        int x=3;
        //int x=55; //with this value it worked
        //if we put x=3; it does not work because it generates the wrong bytecodes

        //HYPOTHESIS: asm could maybe OR or AND the different arguments for IntInstr(BIPUSH,3)
        //and receive lconst_0 as a result
        //maybe it assumes we use iconst_0 or similar to push a low valued integer constant

        //investigation and experimentation showed that we indeed have to use iconst_0 for pushi a 0
        //on the stack. we cannot use the more general BIPUSH
        //maybe this has optimization reasons

        String source="public class MainTest3 { public Void main(){ x="+x+"; println(x); } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest3");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals(x+"\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_multiple_assignment_to_same_variable_add()throws Exception{

        String source="public class MainTest4 { public Void main(){ x=1; x=x+1; println(x); } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest4");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_multiple_assignment_to_same_variable_multiply()throws Exception{

        String source="public class MainTest5 { public Void main(){ x=2; x=x*2; println(x); } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest5");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("4\n",IOUtils.toString(pr.getInputStream()));
    }
}
