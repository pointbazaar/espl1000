package org.vanautrui.languages.vmcompiler.codegenerator;

import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;
import org.vanautrui.languages.vmcompiler.AssemblyWriter;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import static java.lang.Integer.parseInt;
import static org.vanautrui.languages.vmcompiler.codegenerator.StackFocusedAssemblyCodeGenerator.*;
import static org.vanautrui.languages.vmcompiler.codegenerator.SubroutineFocusedAssemblyCodeGenerator.*;
import static org.vanautrui.languages.vmcompiler.model.Register.*;

public class AssemblyCodeGenerator {

    //https://en.wikipedia.org/wiki/X86_instruction_listings

    public static List<String> compileVMCode(List<String> vmcode)throws Exception{
        //clean the vm code
        List<String> clean_vm_codes = clean_vm_code(vmcode);

        List<VMInstr> VMInstrs = parseVMInstrs(clean_vm_codes);

        //generate assembly
        List<String> assembly_codes = vm_codes_to_assembly(VMInstrs);

        return assembly_codes;
    }

    private static List<VMInstr> parseVMInstrs(List<String> clean_vm_codes) {
        return clean_vm_codes.stream().map(VMInstr::new).collect(Collectors.toList());
    }

    private static void compile_iconst(VMInstr instr, AssemblyWriter a) {
        int x = parseInt(instr.arg1.get());
        //a.mov(eax,x,instr.toString());
        //a.push(eax,instr.toString());
        a.push(x,instr.toString());
    }

    private static void compile_fconst(VMInstr instr,AssemblyWriter a) {
        float f = Float.parseFloat(instr.arg1.get());
        //a.mov(eax,f,instr.toString());
        //a.push(eax,instr.toString());
        a.push(f,instr.toString());
    }

    private static void compile_vm_instr(VMInstr instr,AssemblyWriter a)throws Exception{
        //uniqueness for labels
        long uniq = DracoVMCodeGenerator.unique();

        switch (instr.cmd){
            //stack related commands
            case "iconst": compile_iconst(instr,a); break;
            case "fconst": compile_fconst(instr,a); break;
            case "cconst": compile_cconst(instr,a); break;
            case "pop":
                Optional<Integer> index = Optional.empty();
                if(instr.arg2.isPresent()){
                    index=Optional.of(parseInt(instr.arg2.get()));
                }
                compile_pop(instr.arg1,index,a);
                break;
            case "push": compile_push(instr.arg1.get(),parseInt(instr.arg2.get()),a); break;
            case "dup": compile_dup(instr,a); break;
            case "swap": compile_swap("",a); break;

            //subroutine related commands
            case "subroutine": compile_subroutine(instr.arg1.get(),a); break;
            case "call": compile_call(instr,a); break;
            case "return": compile_return(a); break;
            case "exit": compile_exit(instr,a); break;

            //arithmetic commands
            case "add": compile_add(instr,a); break;
            case "sub": compile_sub(instr,a); break;
            case "mul": compile_mul(instr,a); break;
            case "div": compile_div(instr,a); break;
            case "mod": compile_mod(instr,a); break;
            case "neg": compile_neg(instr,a); break;

            //logic commands
            case "eq": compile_eq(instr,a,uniq); break;
            case "gt": compile_gt(instr,a,uniq); break;
            case "lt": compile_lt(instr,a,uniq); break;
            case "not": compile_not(instr,a); break;

            //inc,dec
            case "inc": compile_inc(a); break;
            case "dec": compile_dec(a); break;

            //control flow
            case "goto": a.jmp(instr.arg1.get()); break;
            case "if-goto": compile_if_goto(instr,a,uniq); break;
            case "label": a.label(instr.arg1.get()); break;

            //array related
            case "arraystore": compile_arraystore(instr,a); break;
            case "arrayread": compile_arrayread(instr,a); break;

            //unhandled cases
            default: throw new Exception("unrecognized vm instr "+instr.cmd);
        }
    }

    /**
     * compiles the 'mod' dracovm instruction to assembly code
     */
    private static void compile_mod(VMInstr instr, AssemblyWriter a) {
        //https://stackoverflow.com/questions/8021772/assembly-language-how-to-do-modulo
        final String comment = instr.toString();
        a.pop(ebx,comment);
        a.pop(eax,comment);
        a.div_eax_by(ebx,comment);
        a.push(eax,comment);
    }

    private static void compile_not(VMInstr instr, AssemblyWriter a) {
        //logical not
        //if there is a 1 (true) on the stack, after this vm command, there must be a 0 (false) on the stack
        //if there is a 0 (false) on the stack, after this vm command, there must be a 1 (true) on the stack

        //https://www.tutorialspoint.com/assembly_programming/assembly_logical_instructions.htm

        final String comment = instr.toString();

        a.mov(ebx,1,comment);
        a.pop(eax,comment);
        a.xor(eax,ebx,comment);
        a.push(eax,comment);
    }

    private static void compile_div(VMInstr instr, AssemblyWriter a) {
        a.pop(ecx);
        a.pop(eax);
        a.div_eax_by(ecx);
        a.push(eax);
    }

    private static void compile_mul(VMInstr instr, AssemblyWriter a) {

        a.pop(ebx,instr.toString());
        a.pop(eax,instr.toString());

        a.mul_eax_with(ebx,instr.toString());
        a.push(eax,instr.toString());
    }

    private static void compile_arrayread(VMInstr instr, AssemblyWriter a) {
        /*
         arrayread
        //stack looks like:
        //|undefined
        //|array address
        //|array index <- esp
        ////after execution of this command, stack looks like
        //|undefined
        //|array[index] <-esp
        //meaning this vm command reads from an array, and places the value on the stack

         */
        a.pop(ebx,instr.toString()); //array index
        a.pop(eax,instr.toString()); //array address in memory
        a.add(eax,ebx,instr.toString()); //address of the value we want

        a.dereference(eax,instr.toString()); //eax = memory[eax] <=> mov eax,[eax]

        a.push(eax,instr.toString()); //push the value
    }

    /**
     * Documentation for the behavior, expected stack state before and after are to be found in
     * spec/dracovm-specification.txt
     */
    private static void compile_arraystore(VMInstr instr, AssemblyWriter a) {

        a.pop(ebx,instr.toString()); //value to store //-1

        a.pop(ecx,instr.toString()); //index //-1

        a.pop(eax,instr.toString()); //array_address //-1

        //address to store into = addray_address + index
        a.add(eax,ecx,instr.toString()); //0

        a.store_second_into_memory_location_pointed_to_by_first(eax,ebx,instr.toString()); //0
    }

    private static void compile_dec(AssemblyWriter a){
        a.pop(eax,"dec");
        a.dec(eax,"dec");
        a.push(eax,"dec");
    }

    private static void compile_inc(AssemblyWriter a){
        final String comment="inc";
        a.pop(eax,comment);
        a.inc(eax,comment);
        a.push(eax,comment);
    }



    private static void compile_lt(VMInstr instr, AssemblyWriter a, long uniq) {

        final String comment = "lt";

        String labeltrue="lt_push1"+uniq;
        String labelend="lt_end"+uniq;

        a.pop(ebx,comment);
        a.pop(eax,comment);
        a.cmp(eax, ebx,comment);
        a.jl(labeltrue,comment);

        //push 0 (false)
        a.push(0,comment);
        a.jmp(labelend,comment);

        a.label(labeltrue,comment);
        //push 1 (true)
        a.push(1,comment);

        a.label(labelend,comment);
    }

    private static void compile_gt(VMInstr instr, AssemblyWriter a, long uniq) {
        final String comment = "lt";
        String labeltrue="gt_push1"+uniq;
        String labelend="gt_end"+uniq;

        a.pop(ebx,comment);
        a.pop(eax,comment);
        a.cmp(eax, ebx,comment);
        a.jg(labeltrue,comment);

        //push 0 (false)
        a.push(0,comment);
        a.jmp(labelend,comment);

        a.label(labeltrue,comment);
        //push 1 (true)
        a.push(1,comment);

        a.label(labelend,comment);
    }

    private static List<String> vm_codes_to_assembly(List<VMInstr> vmcodes) throws Exception {
        //receives only clean VM Codes

        AssemblyWriter a=new AssemblyWriter();

        a.section(".text","must be declared for linker (ld)");
        a.global("_start","");
        a.label("_start","tell linker entry point");

        a.nop(); //the 2 nops are recommended by my assembly book

        //at the start of the code, jump to main
        a.jmp("main");

        for(VMInstr instr : vmcodes){
            compile_vm_instr(instr,a);
        }

        //add the builtin subroutines
        BuiltinSubroutinesToBeAddedOnceToEveryAssemblyFile.compile_all_builtin_subroutines(a);

        a.nop(); //the 2 nops are recommended by my assembly book

        a.section(".data","");
        //a.any("achar db '0'","");

        a.section(".bss","");
        //a.any("buffer:	resb	64","");

        ////https://cs.lmu.edu/~ray/notes/nasmtutorial/
        ////https://stackoverflow.com/questions/8201613/printing-a-character-to-standard-output-in-assembly-x86
        return a.getAssemblyProgram();
    }

    private static void compile_exit(VMInstr instr, AssemblyWriter a) {
        a.mov(eax,1,"exit: sytem call number (sys_exit)");
        a.pop(ebx,"exit: pop exit status code from stack");
        a.call_kernel();
    }

    private static void compile_cconst(VMInstr instr, AssemblyWriter a) {
        char c = instr.arg1.get().charAt(0);
        a.push((int)c,instr.toString());
    }



    private static void compile_if_goto(VMInstr instr, AssemblyWriter a,long uniq) {

        final String comment="if-goto";

        a.pop(eax,comment); //pops the condition into eax
        a.mov(ebx,1,comment);  //ebx := true==1
        a.cmp(eax, ebx,comment);
        a.je(instr.arg1.get(),comment); //jumps, if eax==ebx
    }

    private static void compile_eq(VMInstr instr, AssemblyWriter a,long uniq) {
        final String comment ="eq";

        String labeltrue="eq_push1"+uniq;
        String labelend="eq_end"+uniq;

        a.pop(eax,comment);
        a.pop(ebx,comment);
        a.cmp(eax, ebx,comment);
        a.je(labeltrue,comment);

        //push 0 (false)
        a.push(0,comment);
        a.jmp(labelend,comment);

        a.label(labeltrue,comment);
        //push 1 (true)
        a.push(1,comment);

        a.label(labelend,comment);
    }

    private static void compile_neg(VMInstr instr, AssemblyWriter a) {
        a.pop(eax,instr.toString());
        a.mov(ebx,-1,instr.toString());
        a.mul_eax_with(ebx,instr.toString());
        a.push(eax,instr.toString());
    }

    private static void compile_sub(VMInstr instr, AssemblyWriter a) {
        a.pop(ebx,instr.toString());
        a.pop(eax,instr.toString());
        a.sub(eax, ebx,instr.toString());
        a.push(eax,instr.toString());
    }

    private static void compile_add(VMInstr instr, AssemblyWriter a) {
        a.pop(eax,instr.toString());
        a.pop(ebx,instr.toString());
        a.add(eax, ebx,instr.toString());
        a.push(eax,instr.toString());
    }


    private static List<String> clean_vm_code(List<String> vmcodes){
        //remove comments and empty lines
        //and remove indentation

        List<String> result = new ArrayList<>();

        for(String instr : vmcodes){
            String s=new String(instr);
            if(instr.contains("//")){
                s=s.substring(0,instr.indexOf("//"));
            }
            s=s.trim();

            if(!s.isEmpty()){
                result.add(s);
            }
        }

        return result;
    }
}
