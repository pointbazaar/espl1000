package org.vanautrui.languages.vmcompiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;
import org.vanautrui.languages.CodeGeneratorTestUtils;

import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class VMCompilerMainTest {

  @Test
  public void test_correct_return_code_is_generated()throws Exception{
    List<String> vmcodes = Arrays.asList(
            "subroutine main 0",
            "iconst 45",
            "exit"
    );
    Process pr = CodeGeneratorTestUtils.compile_and_run_vm_codes_for_testing(vmcodes, "putchartesting");

    assertEquals(45,pr.exitValue());
    assertEquals("", IOUtils.toString(pr.getInputStream()));
  }
}
