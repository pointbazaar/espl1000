package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;

import java.util.Arrays;
import java.util.List;

public final class OperatorDracoVMCodeGenerator {

  public static List<String> genDracoVMCodeForOp(OperatorNode opNode)throws Exception{
    switch (opNode.operator){
      case "+":
        return Arrays.asList("add");
      case "-":
        return Arrays.asList("sub");
      case ">":
        return Arrays.asList("igt");
      case ">=":
        return Arrays.asList("igeq");
      case "<":
        return Arrays.asList("ilt");
      case "<=":
        return Arrays.asList("ileq");
      case "*":
        return Arrays.asList("imul");
      case "/":
        return Arrays.asList("idiv");
      case "!=":
        return Arrays.asList("ineq");
      case "%":
        return Arrays.asList("imod");
      case "==":
        return Arrays.asList("ieq");

      case "&&":
        return Arrays.asList("and");
      case "||":
        return Arrays.asList("or");

      case "<<":
        return Arrays.asList("lshiftl");
      case ">>":
        return Arrays.asList("lshiftr");

      default:
        throw new Exception("currently unsupported op: '"+opNode.operator+"'");
    }
  }
}
