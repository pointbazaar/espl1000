package org.vanautrui.languages.vmcompiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;
import org.vanautrui.languages.CodeGeneratorTestUtils;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class SubroutineFocusedAssemblyCodeGeneratorTest {

  @Test
  public void test_compile_readchar()throws Exception{

    DracoVMCodeWriter a=new DracoVMCodeWriter();

    a.subroutine("main",0,0);

    a.call("readchar");
    a.call("putchar");
    a.iconst(0);
    a.exit();

    List<String> vmcodes=a.getDracoVMCodeInstructions();

    Process pr = CodeGeneratorTestUtils.compile_and_run_vmcodes_but_not_waitFor(vmcodes, "readchartesting");

    pr.getOutputStream().write('c');
    pr.getOutputStream().write('\n');
    pr.getOutputStream().close();

    pr.waitFor();

    assertEquals(0,pr.exitValue());
    assertEquals("c", IOUtils.toString(pr.getInputStream()));

    Files.deleteIfExists(Paths.get("readchartesting"));
    Files.deleteIfExists(Paths.get("readchartesting.asm"));
  }

  @Test
  public void test_compile_putchar()throws Exception{

    DracoVMCodeWriter a=new DracoVMCodeWriter();

    a.subroutine("main",0,0);
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
  public void test_compile_putdigit()throws Exception{

    DracoVMCodeWriter a=new DracoVMCodeWriter();

    a.subroutine("main",0,0);
    a.iconst(3);
    a.call("putdigit");
    a.pop();
    a.iconst(0);
    a.exit();

    List<String> vmcodes=a.getDracoVMCodeInstructions();

    Process pr = CodeGeneratorTestUtils.compile_and_run_vm_codes_for_testing(vmcodes, "putchartesting");

    assertEquals(0,pr.exitValue());
    assertEquals("3", IOUtils.toString(pr.getInputStream()));
  }

  @Test
  public void test_compile_putchar_multile()throws Exception{
    final int exit_code=234;
    DracoVMCodeWriter a=new DracoVMCodeWriter();

    a.subroutine("main",0,0);

    //push the exit code to use later
    a.iconst(exit_code);

    a.cconst('a');
    a.call("putchar");
    a.pop(); //caller removes the return value, if it doesn't need it
    a.pop(); //caller is supposed to remove the arguments

    a.cconst('b');
    a.call("putchar");
    a.pop(); //caller removes the return value, if it doesn't need it
    a.pop(); //caller is supposed to remove the arguments

    a.cconst('c');
    a.call("putchar");
    a.pop(); //caller removes the return value, if it doesn't need it
    a.pop(); //caller is supposed to remove the arguments


    a.exit();

    List<String> vmcodes=a.getDracoVMCodeInstructions();

    Process pr = CodeGeneratorTestUtils.compile_and_run_vm_codes_for_testing(vmcodes, "putchartesting");

    assertEquals(exit_code,pr.exitValue());
    assertEquals("abc", IOUtils.toString(pr.getInputStream()));
  }
}
