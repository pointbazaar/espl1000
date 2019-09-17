package org.vanautrui.languages.vmcompiler;

import org.vanautrui.languages.vmcompiler.model.Register;
import java.util.*;

public class AssemblyWriter {

    //TODO: make this class and API more typesafe :)

    private List<String> instrs=new ArrayList<>();
    private boolean indented=true;

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
        mov(dest,value,"");
    }
    public void mov(Register register, float value, String comment) {
        any("mov "+register+","+value,comment);
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
        push(register,"");
    }
    public void push(Register register,String comment) {
        any("push "+register,comment);
    }
    public void pop(Register register) {
        pop(register,"");
    }
    public void pop(Register reg, String comment) {
        any("pop "+reg,comment);
    }

    public void call(String methodName) {
        any("call "+methodName);
    }

    public void add(Register dest,int val) {
        any("add "+dest+","+val);
    }
    public void add(Register dest,Register reg2) {
        add(dest,reg2,"");
    }
    public void add(Register dest, Register reg2, String comment) {
        any("add "+dest+","+reg2,comment);
    }
    public void sub(Register dest,Register reg2) {
        sub(dest,reg2,"");
    }
    public void sub(Register dest, Register reg2, String comment) {
        any("sub "+dest+","+reg2,comment);
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

    /** i read that the 'mul' instruction only takes one operang,
     *  and computes eax=eax*other_register
     * @param other
     */
    public void mul(Register other) {

        any("mul "+other);
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


    public void nop() {
        any("nop");
    }

    /**
     * Increments the value of a register
     * @param register register to increment
     */
    public void inc(Register register) {
        any("inc "+register);
    }

    /**
     * Decrements the value of a register
     * @param register register to decrement
     */
    public void dec(Register register){
        any("dec "+register);
    }

    //jump if comparison result was 'greater'
    public void jg(String label) {
        any("jg "+label);
    }

    //jump if comparison result was 'lesser'
    public void jl(String label) {
        any("jl "+label);
    }
}
