package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import static java.util.Map.entry;

public final class OperatorDracoVMCodeGenerator {

  private static Map<String,String> vmcodes_for_op_bool = Map.ofEntries(
      entry("&&","and"),
      entry("||","or"),
      entry("==","ieq"),
      entry("!=","ineq")
  );

  private static Map<String,String> vmcodes_for_op_int = Map.ofEntries(
      entry("+","iadd"),
      entry("-","isub"),
      entry("*","imul"),
      entry("/","idiv"),

      entry(">","igt"),
      entry("<","ilt"),
      entry("<=","ileq"),
      entry(">=","igeq"),

      entry("!=","ineq"),
      entry("==","ieq"),

      entry( "%","imod"),

      entry( "<<","lshiftl"),
      entry( ">>","lshiftr"),

      entry( "^","iexp")
  );

  private static Map<String,String> vmcodes_for_op_float = Map.of(
          "+","fadd",
          "-","fsub",
          "*","fmul",
          "/","fdiv",

          ">","fgt",
          "<","flt",
          "<=","fleq",
          ">=","fgeq",

          "%","fmod"
  );

  public static List<String> genDracoVMCodeForOp(OperatorNode opNode, boolean isFloatNode, final boolean isBoolOperands)throws Exception{
    return Arrays.asList(genDracoVMCodeForOpSingleInstr(opNode,isFloatNode,isBoolOperands));
  }

  private static String genDracoVMCodeForOpSingleInstr(OperatorNode opNode, boolean isFloatOperands, boolean isBoolOperands)throws Exception{

    if(isFloatOperands){
      if(vmcodes_for_op_float.containsKey(opNode.operator)){
        return vmcodes_for_op_float.get(opNode.operator);
      }else{
        throw new Exception("not implemented for float: '"+opNode.operator+"'");
      }
    }else if(isBoolOperands) {
      if(vmcodes_for_op_bool.containsKey(opNode.operator)){
        return vmcodes_for_op_bool.get(opNode.operator);
      }else{
        throw new Exception("not implemented for bool: '"+opNode.operator+"'");
      }
    }else{
      if(vmcodes_for_op_int.containsKey(opNode.operator)){
        return vmcodes_for_op_int.get(opNode.operator);
      }else{
        throw new Exception("not implemented for int: '"+opNode.operator+"'");
      }
    }

  }
}
