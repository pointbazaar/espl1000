package org.vanautrui.languages.vmcompiler;

import org.vanautrui.languages.vmcompiler.model.Register;

import java.util.ArrayList;
import java.util.List;

public class AssemblyWriter {

    //TODO check for correct assembly syntax,
    //and that the right registers are used.
    //make this class and API typesafe :)

    //this is not typesafe but improves readability
    //and you can write your code faster
    /*
    public static final String eax="eax";
    public static final String ebx="ebx";
    public static final String ecx="ecx";
    public static final String edx="edx";

     */

    private List<String> instrs=new ArrayList<>();
    private boolean indented=false;
    private boolean finalized=false;

    public AssemblyWriter(){

    }

    public List<String> getAssemblyProgram(){
        return this.instrs;
    }

    private void any(String cmd) {
        any(cmd,"");
    }

    private void any(String cmd,String comment){
        final String spacer="        ";
        //base
        String prefix="";
        if(indented){
            prefix=spacer;
        }
        String suffix="";
        if(!indented){
            suffix=spacer;
        }
        instrs.add(
                String.format(prefix+"%-20s "+suffix+";%-20s",cmd,comment)
        );
    }

    public void section(String name,String comment) {
        indented=false;
        any("section "+name,comment);
        indented=true;
    }
    public void section(String name) {
        section(name,"");
    }

    public void global(String s,String comment) {
        indented=false;
        any("global "+s,"");
        indented=true;
    }
    public void global(String s) {
        global(s,"");
    }

    public void mov(Register dest, int value, String comment) {
        any("mov "+dest+","+value,comment);
    }
    public void mov(Register dest,float value) {
        any("mov "+dest+","+value);
    }
    public void mov(Register dest,int value) {
        any("mov "+dest+","+value);
    }
    public void mov(Register dest, Register src, String comment) {
        any("mov "+dest+","+src,comment);
    }
    public void mov(Register dest,Register src) {
        mov(dest,src,"");
    }

    public void push(Register register) {
        any("push "+register);
    }
    public void pop(Register register) {
        any("pop "+register);
    }

    public void call(String methodName) {
        any("call "+methodName);
    }

    public void add(Register dest,int val) {
        any("add "+dest+","+val);
    }
    public void add(Register dest,Register reg2) {
        any("add "+dest+","+reg2);
    }
    public void sub(Register dest,Register reg2) {
        any("sub "+dest+","+reg2);
    }

    public void label(String label,String comment) {
        indented=false;
        any(label+":");
        indented=true;
    }
    public void label(String label) {
        label(label,"");
    }

    public void jmp(String targetLabel) {
        any("jmp "+targetLabel);
    }

    public void writeReturn() {
        any("ret");
    }

    public void mul(Register dest, Register other) {
        //not sure about it
        any("mul "+dest+","+other);
    }

    public void call_kernel() {
        any("int 0x80","call kernel");
    }

    public void cmp(Register reg1, Register reg2) {
        any("cmp "+reg1+","+reg2);
    }

    public void je(String label) {
        any("je "+label);
    }

    public void xor(Register reg1, Register reg2, String comment) {
        any("xor "+reg1+","+reg2,comment);
    }
}
