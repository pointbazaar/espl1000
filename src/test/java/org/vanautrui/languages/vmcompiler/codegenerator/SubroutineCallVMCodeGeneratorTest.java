package org.vanautrui.languages.vmcompiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;
import org.vanautrui.languages.CodeGeneratorTestUtils;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import java.util.List;

import static org.junit.Assert.assertEquals;

public class SubroutineCallVMCodeGeneratorTest {

  @Test
  public void test_compile_putchar()throws Exception{

    DracoVMCodeWriter a=new DracoVMCodeWriter();

    a.subroutine("main",0);
    a.cconst('c');
    a.call("putchar");
    a.pop();
    a.iconst(0);
    a.exit();

    List<String> vmcodes=a.getDracoVMCodeInstructions();

    Process pr = CodeGeneratorTestUtils.compile_and_run_vm_codes_for_testing(vmcodes, "putchartesting");

    assertEquals(0,pr.exitValue());
    assertEquals("c", IOUtils.toString(pr.getInputStream()));
  }

  @Test
  public void test_compile_putchar_multile()throws Exception{
    final int exit_code=234;
    DracoVMCodeWriter a=new DracoVMCodeWriter();

    a.subroutine("main",0);

    //push the exit code to use later
    a.iconst(exit_code);

    a.cconst('a');
    a.call("putchar");
    a.pop();

    a.cconst('b');
    a.call("putchar");
    a.pop();

    a.cconst('c');
    a.call("putchar");
    a.pop();


    a.exit();

    List<String> vmcodes=a.getDracoVMCodeInstructions();

    Process pr = CodeGeneratorTestUtils.compile_and_run_vm_codes_for_testing(vmcodes, "putchartesting");

    assertEquals(exit_code,pr.exitValue());
    assertEquals("abc", IOUtils.toString(pr.getInputStream()));
  }
}
