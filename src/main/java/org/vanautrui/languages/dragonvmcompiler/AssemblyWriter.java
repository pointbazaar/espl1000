package org.vanautrui.languages.dragonvmcompiler;

import org.vanautrui.languages.dragonvmcompiler.model.Register;

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

    public List<String> finalizeAndReturnAssemblyProgram(){


        this.finalized=true;
        return this.instrs;
    }

    private void any(String cmd)throws Exception{
        any(cmd,"");
    }

    private void any(String cmd,String comment) throws Exception {
        if(finalized){
            throw new Exception("this assembly writer already has been finalized. ");
        }
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

    public void section(String name,String comment)throws Exception{
        indented=false;
        any("section "+name,comment);
        indented=true;
    }
    public void section(String name)throws Exception{
        section(name,"");
    }

    public void global(String s,String comment)throws Exception{
        indented=false;
        any("global "+s,"");
        indented=true;
    }
    public void global(String s)throws Exception{
        global(s,"");
    }

    public void mov(Register dest, int value, String comment)throws Exception{
        any("mov "+dest+","+value,comment);
    }
    public void mov(Register dest,float value)throws Exception{
        any("mov "+dest+","+value);
    }
    public void mov(Register dest,int value)throws Exception{
        any("mov "+dest+","+value);
    }
    public void mov(Register dest, Register src, String comment)throws Exception{
        any("mov "+dest+","+src,comment);
    }
    public void mov(Register dest,Register src)throws Exception{
        mov(dest,src,"");
    }

    public void push(Register register) throws Exception {
        any("push "+register);
    }
    public void pop(Register register) throws Exception {
        any("pop "+register);
    }

    public void call(String methodName) throws Exception {
        any("call "+methodName);
    }

    public void add(Register dest,int val)throws Exception{
        any("add "+dest+","+val);
    }
    public void add(Register dest,Register reg2) throws Exception {
        any("add "+dest+","+reg2);
    }
    public void sub(Register dest,Register reg2) throws Exception {
        any("sub "+dest+","+reg2);
    }

    public void label(String label,String comment)throws Exception{
        indented=false;
        any(label+":");
        indented=true;
    }
    public void label(String label) throws Exception {
        label(label,"");
    }

    public void jmp(String targetLabel) throws Exception {
        any("jmp "+targetLabel);
    }

    public void writeReturn() throws Exception {
        any("ret");
    }

    public void mul(Register dest, Register other) throws Exception {
        //not sure about it
        any("mul "+dest+","+other);
    }

    public void call_kernel() throws Exception{
        any("int 0x80","call kernel");
    }

    public void cmp(Register reg1, Register reg2) throws Exception{
        any("cmp "+reg1+","+reg2);
    }

    public void je(String label) throws Exception{
        any("je "+label);
    }
}
