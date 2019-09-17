package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.lang3.StringUtils;

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

  private static final int default_indent=4;
  private static final int label_indent=1;
  private static final int subroutine_indent=0;

  private int indent=default_indent;

  public static final String SEGMENT_ARG = "ARG";
  public static final String SEGMENT_LOCAL = "LOCAL";
  public static final String SEGMENT_STATIC = "]STATIC";

  public DracoVMCodeWriter(){}

  /**
   * all access to this.instrs must flow through here because this subroutine
   * keeps track of correct indentation
   */
  private void any(String command,String comment){
    String myindent = StringUtils.repeat("  ", this.indent);

    String mycomment="";
    if(!comment.isEmpty()){
      mycomment="       //"+comment;
    }

    instrs.add(myindent+command+mycomment);
  }

  private void any(String command){
    any(command,"");
  }

  public void subroutine(String name,int nArgs,int nLocals){
    indent=subroutine_indent;
    any("subroutine "+name+" "+nArgs+" args "+nLocals+" locals");
    indent=default_indent;
  }

  public void push(String segment, int index){
    any("push "+segment+" "+index);
  }

  public void pop(String segment, int index){
    any("pop "+segment+" "+index);
  }

  public void pop(){
    pop("");
  }
  public void pop(String comment){
    any("pop",comment);
  }

  public void label(String name){
    indent=label_indent;
    any("label "+name);
    indent=default_indent;
  }

  public void if_goto(String label){
    any("if-goto "+label);
  }

  public void _goto(String label){
    any("goto "+label);
  }

  public void neg(){
    any("neg");
  }

  public void dup() {
    any("dup");
  }


  public void iconst(int i,String comment){

    any("iconst "+i,comment);
  }
  public void iconst(int i) {
    iconst(i,"");
  }

  public void eq() {
    any("eq");
  }

  public void dec() {
    any("dec");
  }
  public void inc() {
    any("inc");
  }

  public void fconst(float value) {
    any("fconstn "+value);
  }

  public void call(String methodName) {
    any("call "+methodName);
  }

  public void add() {
    any("add");
  }

  public void sub() {
    any("sub");
  }

  public void exit() {
    any("exit");
  }

  public void _return() {
    any("return");
  }

  public void cconst(char c) {
    any("cconst "+c);
  }

  public void gt() {
    any("gt");
  }

  public void lt() {
    any("lt");
  }

  /**
   * swaps the topmost 2 items on the stack
   */
  public void swap() {
    any("swap");
  }
  public void swap(String comment) {
    any("swap",comment);
  }
}
