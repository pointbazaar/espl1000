package org.vanautrui.languages.vmcompiler.codegenerator;

import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;
import org.vanautrui.languages.vmcompiler.AssemblyWriter;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.vmcompiler.codegenerator.SubroutineFocusedAssemblyCodeGenerator.compile_call;
import static org.vanautrui.languages.vmcompiler.codegenerator.SubroutineFocusedAssemblyCodeGenerator.compile_subroutine;
import static org.vanautrui.languages.vmcompiler.model.Register.*;

public class AssemblyCodeGenerator {

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
        int x = Integer.parseInt(instr.arg1.get());
        a.mov(eax,x,instr.toString());
        a.push(eax,instr.toString());
    }

    private static void compile_fconst(VMInstr instr,AssemblyWriter a) {
        float f = Float.parseFloat(instr.arg1.get());
        a.mov(eax,f,instr.toString());
        a.push(eax,instr.toString());
    }

    private static void compile_vm_instr(VMInstr instr,AssemblyWriter a)throws Exception{
        //uniqueness for labels
        long uniq = DracoVMCodeGenerator.unique();

        switch (instr.cmd){
            //stack related commands
            case "iconst": compile_iconst(instr,a); break;
            case "fconst": compile_fconst(instr,a); break;
            case "cconst": compile_cconst(instr,a); break;
            case "pop": compile_pop(instr,a); break;
            case "push": compile_push(instr,a); break;
            case "dup": compile_dup(instr,a); break;
            case "swap": compile_swap(instr,a); break;

            //subroutine related commands
            case "subroutine": compile_subroutine(instr,a); break;
            case "call": compile_call(instr,a); break;
            case "return": a.writeReturn(); break;
            case "exit": compile_exit(instr,a); break;

            //arithmetic commands
            case "add": compile_add(instr,a); break;
            case "sub": compile_sub(instr,a); break;
            case "mul": compile_mul(instr,a); break;
            case "div": compile_div(instr,a); break;
            case "neg": compile_neg(instr,a); break;
            case "eq": compile_eq(instr,a,uniq); break;
            case "gt": compile_gt(instr,a,uniq); break;
            case "lt": compile_lt(instr,a,uniq); break;

            //inc,dec
            case "inc": compile_inc(a); break;
            case "dec": compile_dec(a); break;

            //control flow
            case "goto": a.jmp(instr.arg1.get()); break;
            case "if-goto": compile_if_goto(instr,a,uniq); break;
            case "label": a.label(instr.arg1.get()); break;

            //memory management
            case "malloc": compile_malloc(instr,a); break;
            case "free": throw new Exception("free not yet implemented");

            //array related
            case "arraystore": compile_arraystore(instr,a); break;
            case "arrayread": compile_arrayread(instr,a); break;

            //unhandled cases
            default: throw new Exception("unrecognized vm instr "+instr.cmd);
        }
    }

    private static void compile_div(VMInstr instr, AssemblyWriter a) {
        a.pop(ecx);
        a.pop(eax);
        a.div_eax_by(ecx);
    }

    private static void compile_mul(VMInstr instr, AssemblyWriter a) {

        a.pop(ebx);
        a.pop(eax);

        a.mul_eax_with(ebx);
        a.push(eax);
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

        a.pop(ebx,instr.toString()); //value to store

        a.pop(ecx,instr.toString()); //index

        a.pop(eax,instr.toString()); //array_address

        //address to store into = addray_address + index
        a.add(eax,ecx);

        a.store_second_into_memory_location_pointed_to_by_first(eax,ebx,instr.toString());
    }

    private static void compile_dec(AssemblyWriter a){
        a.pop(eax,"dec");
        a.dec(eax,"dec");
        a.push(eax,"dec");
    }

    private static void compile_inc(AssemblyWriter a){
        a.pop(eax);
        a.inc(eax);
        a.push(eax);
    }

    /**
     * swaps the 2 topmost items on the stack
     * @param instr
     * @param a
     */
    private static void compile_swap(VMInstr instr, AssemblyWriter a) {

        a.pop(eax,instr.toString());
        a.pop(ebx,instr.toString());

        a.push(eax,instr.toString());
        a.push(ebx,instr.toString());
    }

    private static void compile_lt(VMInstr instr, AssemblyWriter a, long uniq) {

        String labeltrue="gt_push1"+uniq;
        String labelend="gt_end"+uniq;

        a.pop(eax);
        a.pop(ebx);
        a.cmp(eax, ebx);
        a.jg(labeltrue);

        //push 0 (false)
        a.mov(eax,0);
        a.push(eax);
        a.jmp(labelend);

        a.label(labeltrue);
        //push 1 (true)
        a.mov(eax,1);
        a.push(eax);
        a.jmp(labelend);

        a.label(labelend);
    }

    private static void compile_gt(VMInstr instr, AssemblyWriter a, long uniq) {

        String labeltrue="gt_push1"+uniq;
        String labelend="gt_end"+uniq;

        a.pop(eax);
        a.pop(ebx);
        a.cmp(eax, ebx);
        a.jl(labeltrue);

        //push 0 (false)
        a.mov(eax,0);
        a.push(eax);
        a.jmp(labelend);

        a.label(labeltrue);
        //push 1 (true)
        a.mov(eax,1);
        a.push(eax);
        a.jmp(labelend);

        a.label(labelend);
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

        a.mov(eax,(int)c,instr.toString());
        a.push(eax,instr.toString());
    }

    private static void compile_malloc(VMInstr instr, AssemblyWriter a) {
        //malloc receives as an argument the amount of DWORDs to allocate
        int amount = Integer.parseInt(instr.arg1.get());

        a.mov(eax,192,"192 : mmap system call");
        a.xor(ebx,ebx,"addr=NULL");
        a.mov(ecx, amount,"size of segment to be allocated?");
        a.mov(edx,0x7,"prot = PROT_READ | PROT_WRITE | PROT_EXEC");
        a.mov(esi,0x22,"flags=MAP_PRIVATE | MAP_ANONYMOUS");
        a.mov(edi,-1,"fd=-1");

        a.push(ebp,"save ebp as we should not mess with it"); //LINKED CODE 1 (they only work together)

        a.xor(ebp,ebp,"offset=0");
        a.call_kernel();

        a.pop(ebp,"restore ebp as we should not mess with it"); //LINKED CODE 1 (they only work together)

        //eax should now contain
        //the address of the allocated memory segment

        //now we push that pointer on the stack
        a.push(eax);
    }

    private static void compile_if_goto(VMInstr instr, AssemblyWriter a,long uniq) {

        String truelabel = "ifgoto_true"+uniq;
        String endlabel = "ifgoto_end"+uniq;

        a.pop(eax); //pops the condition into eax
        a.mov(ebx,1);  //ebx := true==1
        a.cmp(eax, ebx);
        a.je(truelabel); //jumps, if eax==ebx
        a.jmp(endlabel);

        //in case top of stack is 1 (true)
        a.label(truelabel);
        a.jmp(instr.arg1.get());

        //otherwise, just continue execution
        a.label(endlabel);
    }

    private static void compile_eq(VMInstr instr, AssemblyWriter a,long uniq) {
        String labeltrue="eq_push1"+uniq;
        String labelend="eq_end"+uniq;

        a.pop(eax);
        a.pop(ebx);
        a.cmp(eax, ebx);
        a.je(labeltrue);

        //push 0 (false)
        a.mov(eax,0);
        a.push(eax);
        a.jmp(labelend);

        a.label(labeltrue);
        //push 1 (true)
        a.mov(eax,1);
        a.push(eax);
        a.jmp(labelend);

        a.label(labelend);
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

    private static void compile_dup(VMInstr instr, AssemblyWriter a) {
        //duplicates top of stack
        a.pop(eax,instr.toString());
        a.push(eax,instr.toString());
        a.push(eax,instr.toString());
    }

    private static void compile_push(VMInstr instr, AssemblyWriter a) throws Exception {
        //TODO: add comments to our generated vm code, to understand what is going on
        String segment = instr.arg1.get();
        int index = Integer.parseInt(instr.arg2.get());
        switch (segment){
            case "ARG":
                //arguments are on stack in reverse order
                //ebp is on the caller's return address, with the arguments above

                //eax = ebp + index + 2 //the +2 is because there is the saved ebp on stack also,
                //and the variables are indexed starting with 0
                a.mov(eax,2);
                a.add(eax,ebp);
                a.add(eax,index);

                // a.dereference(eax) -> mov eax,[eax]
                a.dereference(eax);

                //push eax
                a.push(eax);
                break;
            case "LOCAL":

                //locals are on stack in order
                //ebp is on the caller's return address, with the local variables below

                //eax = ebp - index - 1 //the +1 is because there is the saved ebp on stack also

                //eax=ebp
                a.mov(eax,ebp);

                //eax -= 1
                a.mov(ebx,1);
                a.sub(eax,ebx);

                //eax -= index
                a.mov(ebx,index);
                a.sub(eax,ebx);

                // a.dereference(eax) -> mov eax,[eax]
                a.dereference(eax);

                //push eax
                a.push(eax);
                break;
            case "STATIC":
                throw new Exception("not yet implemented");
            default:
                throw new Exception("fatal");
        }
    }

    /**
     * @param instr the pop instruction to be compiled ta assembly
     * @param a the AssemblyWriter we are using
     * @throws Exception if we are pop - ing from a nonexisting segment
     */
    private static void compile_pop(VMInstr instr, AssemblyWriter a) throws Exception{

        if(!instr.arg1.isPresent()){
            a.pop(eax);
        }else {

            String segment = instr.arg1.get();

            int index = Integer.parseInt(instr.arg2.get());

            switch (segment) {
                case "ARG":
                    //arguments are on stack in reverse order
                    //ebp is on the caller's return address, with the arguments above

                    //eax = ebp + index + 2 //the +2 is because there is the saved ebp on stack also,
                    //and the variables are indexed starting with 0
                    a.mov(eax,2);
                    a.add(eax,ebp);
                    a.add(eax,index);

                    //get the value we want to pop
                    a.pop(ebx);

                    //get that value into the address pointed to by eax
                    a.store_second_into_memory_location_pointed_to_by_first(eax,ebx);
                    break;
                case "LOCAL":

                    //locals are on stack in order
                    //ebp is on the caller's return address, with the local variables below

                    //eax = ebp - index - 1 //the +1 is because there is the saved ebp on stack also

                    //eax=ebp
                    a.mov(eax,ebp);

                    //eax -= 1
                    a.mov(ebx,1);
                    a.sub(eax,ebx);

                    //eax -= index
                    a.mov(ebx,index);
                    a.sub(eax,ebx);

                    //get the value we want to pop
                    a.pop(ebx);

                    a.store_second_into_memory_location_pointed_to_by_first(eax,ebx);
                    break;
                case "STATIC":
                    throw new Exception("not yet implemented");
                default:
                    throw new Exception("fatal");
            }
        }
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
