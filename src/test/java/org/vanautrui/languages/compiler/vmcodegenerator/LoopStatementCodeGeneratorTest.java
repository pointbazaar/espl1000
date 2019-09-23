package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.CodeGeneratorTestUtils;

public class LoopStatementCodeGeneratorTest {

  @Test
  public void test_can_compile_loop_statements()throws Exception{
    String source="public class MainTest2442 { public PInt main(){ loop 4 { putchar('n'); } return 0; } }";
    Process pr = CodeGeneratorTestUtils.compile_and_run_program_for_testing(source,"MainTest2442");

    Assert.assertEquals(0,pr.exitValue());
    Assert.assertEquals("nnnn", IOUtils.toString(pr.getInputStream()));
  }
}
