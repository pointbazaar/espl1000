package org.vanautrui.languages.compiler.vmcodegenerator;

import java.util.ArrayList;
import java.util.List;

/**
 * this class provides an abstraction,
 * in order to write correct dracovm programs easily via
 * IntelliJ's autocomplete.
 * It provides subroutines for almost? every dracovm instruction
 */
public class DracoVMCodeWriter {

  private List<String> instrs=new ArrayList<>();
  public List<String> getDracoVMCodeInstructions(){
    return this.instrs;
  }

  public static final String SEGMENT_ARG = "ARG";
  public static final String SEGMENT_LOCAL = "LOCAL";
  public static final String SEGMENT_STATIC = "]STATIC";

  public DracoVMCodeWriter(){}

  public void subroutine(String name,int nLocals){
    instrs.add("subroutine "+name+" "+nLocals);
  }

  public void push(String segment, int index){
    instrs.add("push "+segment+" "+index);
  }

  public void pop(String segment, int index){
    instrs.add("pop "+segment+" "+index);
  }

  public void pop(){
    instrs.add("pop");
  }

  public void label(String name){
    instrs.add("label "+name);
  }

  public void if_goto(String label){
    instrs.add("if-goto "+label);
  }

  public void _goto(String label){
    instrs.add("goto "+label);
  }

  public void neg(){
    instrs.add("neg");
  }

  public void dup() {
    instrs.add("dup");
  }

  public void iconst(int i) {
    instrs.add("iconst "+i);
  }

  public void eq() {
    instrs.add("eq");
  }

  public void dec() {
    instrs.add("dec");
  }
  public void inc() {
    instrs.add("inc");
  }

  public void fconst(float value) {
    instrs.add("fconstn "+value);
  }

  public void call(String methodName) {
    instrs.add("call "+methodName);
  }

  public void add() {
    instrs.add("add");
  }

  public void sub() {
    instrs.add("sub");
  }

  public void exit() {
    instrs.add("exit");
  }

  public void _return() {
    instrs.add("return");
  }

  public void cconst(char c) {
    instrs.add("cconst "+c);
  }
}
