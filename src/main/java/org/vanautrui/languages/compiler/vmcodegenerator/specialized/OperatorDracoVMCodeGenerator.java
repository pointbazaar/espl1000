package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;

import java.util.Arrays;
import java.util.List;

public final class OperatorDracoVMCodeGenerator {

  public static List<String> genDracoVMCodeForOp(OperatorNode opNode, boolean isFloatNode)throws Exception{
    return Arrays.asList(genDracoVMCodeForOpSingleInstr(opNode,isFloatNode));
  }

  private static String genDracoVMCodeForOpSingleInstr(OperatorNode opNode, boolean isFloatOperands)throws Exception{

    switch (opNode.operator){
      case "+":
        if(isFloatOperands){
          return "fadd";
        }else {
          return "iadd";
        }
      case "-":
        if(isFloatOperands) {
          return "fsub";
        }else {
          return "isub";
        }
      case ">":
        if(isFloatOperands){
          return "fgt";
        }else {
          return "igt";
        }
      case "<":
        return (isFloatOperands)?"flt":"ilt";
      case ">=":
        return (isFloatOperands)?"fgeq":"igeq";
      case "<=":
        return (isFloatOperands)?"fleq":"ileq";

      case "*":
        return (isFloatOperands)?"fmul":"imul";
      case "/":
        return (isFloatOperands)?"fdiv":"idiv";

      case "!=":
        if(isFloatOperands){
          throw new Exception("not implemented for float: !=");
        }
        return "ineq";
      case "%":
        return (isFloatOperands)?"fmod":"imod";
      case "==":
        if(isFloatOperands){
          throw new Exception("not implemented for float: ==");
        }
        return "ieq";

      case "&&":
        return "and";
      case "||":
        return "or";

      case "<<":
        return "lshiftl";
      case ">>":
        return "lshiftr";

      case "^":
        return "iexp";

      default:
        throw new Exception("currently unsupported op: '"+opNode.operator+"'");
    }
  }
}
