package org.vanautrui.languages.vmcompiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;
import org.vanautrui.languages.CodeGeneratorTestUtils;

import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class SubroutineCallVMCodeGeneratorTest {

  @Test
  public void test_compile_putchar()throws Exception{
    List<String> vmcodes = Arrays.asList(
            "subroutine main 0",
            "cconst c",
            "call putchar", //should consume its one argument
            "pop", //we pop putchar ' s return value as its return value is not assigned
            "iconst 0",
            "return"
    );
    Process pr = CodeGeneratorTestUtils.compile_and_run_vm_codes_for_testing(vmcodes, "putchartesting");

    //assertEquals(0,pr.exitValue());
    assertEquals("c", IOUtils.toString(pr.getInputStream()));
  }
}
