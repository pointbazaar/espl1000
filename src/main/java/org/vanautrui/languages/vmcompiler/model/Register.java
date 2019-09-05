package org.vanautrui.languages.vmcompiler.model;

public class Register {

    private String name;

    //private means no class besides this one can call it
    private Register(String name){
        this.name=name;
    }

    public static final Register eax=new Register("eax");
    public static final Register ebx=new Register("ebx");
    public static final Register ecx=new Register("ecx");
    public static final Register edx=new Register("edx");

    public static final Register esi=new Register("esi");

    //stack pointer
    public static final Register esp=new Register("esp");

    //stack base pointer
    public static final Register ebp=new Register("ebp");

    public static final Register edi=new Register("edi");

    @Override
    public String toString(){
        return this.name;
    }
}
