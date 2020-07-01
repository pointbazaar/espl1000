package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import static java.util.Map.entry;

public final class OperatorJavaCodeGenerator {

  private static Map<String,String> javacodes_for_op_bool = Map.ofEntries(
      entry("&&","&&"),
      entry("||","||"),
      entry("==","=="),
      entry("!=","!=")
  );

  private static Map<String,String> javacodes_for_op_int = Map.ofEntries(
      entry("+","+"),
      entry("-","-"),
      entry("*","*"),
      entry("/","/"),

      entry(">",">"),
      entry("<","<"),
      entry("<=","<="),
      entry(">=",">="),

      entry("!=","!="),
      entry("==","=="),

      entry( "%","%"),

      entry( "<<","<<"),
      entry( ">>",">>")
  );

  private static Map<String,String> vmcodes_for_op_float = Map.of(
          "+","+",
          "-","-",
          "*","*",
          "/","/",

          ">",">",
          "<","<",
          "<=","<=",
          ">=",">=",

          "%","%"
  );

  public static List<String> genJavaCodeForOp(OperatorNode opNode, boolean isFloatNode, final boolean isBoolOperands)throws Exception{
    return Arrays.asList(genJavaCodeForOpSingleInstr(opNode,isFloatNode,isBoolOperands));
  }

  private static String genJavaCodeForOpSingleInstr(OperatorNode opNode, boolean isFloatOperands, boolean isBoolOperands)throws Exception{

    if(isFloatOperands){
      if(vmcodes_for_op_float.containsKey(opNode.operator)){
        return vmcodes_for_op_float.get(opNode.operator);
      }else{
        throw new Exception("not implemented for float: '"+opNode.operator+"'");
      }
    }else if(isBoolOperands) {
      if(javacodes_for_op_bool.containsKey(opNode.operator)){
        return javacodes_for_op_bool.get(opNode.operator);
      }else{
        throw new Exception("not implemented for bool: '"+opNode.operator+"'");
      }
    }else{
      if(javacodes_for_op_int.containsKey(opNode.operator)){
        return javacodes_for_op_int.get(opNode.operator);
      }else{
        throw new Exception("not implemented for int: '"+opNode.operator+"'");
      }
    }

  }
}
