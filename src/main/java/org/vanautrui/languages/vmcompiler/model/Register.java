package org.vanautrui.languages.vmcompiler.model;

public class Register {

    private String name;

    //private means no class besides this one can call it
    private Register(String name){
        this.name=name;
    }

    //general purpose registers
    public static final Register eax=new Register("eax");
    public static final Register ebx=new Register("ebx");
    public static final Register ecx=new Register("ecx");
    public static final Register edx=new Register("edx");

    public static final Register esi=new Register("esi");
    public static final Register edi=new Register("edi");

    //stack pointer
    public static final Register esp=new Register("esp");

    //stack base pointer
    public static final Register ebp=new Register("ebp");



    //general purpose registers, 64 bit
    public static final Register rax=new Register("rax");
    public static final Register rbx=new Register("rbx");
    public static final Register rcx=new Register("rcx");
    public static final Register rdx=new Register("rdx");

    //general purpose registers, 16 bit
    public static final Register ax=new Register("ax");
    public static final Register bx=new Register("bx");
    public static final Register cx=new Register("cx");
    public static final Register dx=new Register("dx");

    //general purpose registers, 16bit, lower half
    public static final Register al=new Register("al");
    public static final Register bl=new Register("bl");
    public static final Register cl=new Register("cl");
    public static final Register dl=new Register("dl");

    //general purpose registers, 16bit, higher half
    public static final Register ah=new Register("ah");
    public static final Register bh=new Register("bh");
    public static final Register ch=new Register("ch");
    public static final Register dh=new Register("dh");


    @Override
    public String toString(){
        return this.name;
    }
}
