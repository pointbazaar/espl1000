package org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler.model;

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

    //stack pointer
    public static final Register esp=new Register("esp");

    @Override
    public String toString(){
        return this.name;
    }
}
