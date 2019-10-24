package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

public class OperatorDracoVMCodeGenerator {

  public static void genDracoVMCodeForOp(OperatorNode opNode, DracoVMCodeWriter sb)throws Exception{
    switch (opNode.operator){
      case "+":
        sb.add();
        break;
      case "-":
        sb.sub();
        break;
      case ">":
        sb.gt();
        break;
      case ">=":
        sb.geq();
        break;
      case "<":
        sb.lt();
        break;
      case "<=":
        sb.leq();
        break;
      case "*":
        sb.mul();
        break;
      case "/":
        sb.div();
        break;
      case "!=":
        sb.neq();
        break;
      case "%":
        sb.mod();
        break;
      case "==":
        sb.eq();
        break;

      case "&&":
        sb.and();
        break;
      case "||":
        sb.or();

      case "<<":
        sb.lshiftl();
        break;
      case ">>":
        sb.lshiftr();
        break;

      default:
        throw new Exception("currently unsupported op: '"+opNode.operator+"'");
    }
  }
}
