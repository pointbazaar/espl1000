package org.vanautrui.languages.vmcompiler;

import org.vanautrui.languages.vmcompiler.instructions.IVMInstr;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;
import org.vanautrui.languages.vmcompiler.model.Register;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.vmcompiler.model.Register.*;

public class DracoVMCompiler {

    public static List<String> compileVMCode(List<String> vmcode)throws Exception{
        //clean the vm code
        List<String> clean_vm_codes = clean_vm_code(vmcode);

        List<IVMInstr> ivmInstrs = parseVMInstrs(clean_vm_codes);

        //generate assembly
        List<String> assembly_codes = vm_codes_to_assembly(ivmInstrs);

        /*
        //TODO: open process to call nasm to compile to machine code

			//$ nasm -f elf hello.asm  # this will produce hello.o ELF object file
			//$ ld -s -o hello hello.o # this will produce hello executable

			printf("PHASE: call NASM\n");

			string arg1="nasm -f elf ";
			arg1.append(asm_file_name);

			string arg2="ld -melf_i386 -s -o hello ";
			arg2.append(filearg);
			arg2.append(".o");

			system(arg1.c_str());
			system(arg2.c_str());
         */

        return assembly_codes;
    }

    private static List<IVMInstr> parseVMInstrs(List<String> clean_vm_codes) {
        return clean_vm_codes.stream().map(VMInstr::new).collect(Collectors.toList());
    }

    private static void compile_iconst(IVMInstr instr,AssemblyWriter a) {
        a.mov(eax,Integer.parseInt(instr.getArg1().get()));
        a.push(eax);
    }

    private static void compile_fconst(IVMInstr instr,AssemblyWriter a) {
        a.mov(eax,Float.parseFloat(instr.getArg1().get()));
        a.push(eax);
    }

    private static List<String> vm_codes_to_assembly(List<IVMInstr> vmcodes) throws Exception {
        //receives only clean VM Codes
        //TODO
        AssemblyWriter a=new AssemblyWriter();

        a.section(".text","must be declared for linker (ld)");
        a.global("_start","");
        a.label("_start","tell linker entry point");

        //at the start of the code, jump to main
        a.jmp("main");

        for(IVMInstr instr : vmcodes){

            //uniqueness for labels
            long uniq = DracoVMCodeGenerator.unique();

            switch (instr.getCmd()){
                //stack related commands
                case "iconst":
                    compile_iconst(instr,a);
                    break;
                case "fconst":
                    compile_fconst(instr,a);
                    break;
                case "pop":
                    compile_pop(instr,a);
                    break;
                case "push":
                    compile_push(instr,a);
                    break;
                case "dup":
                    compile_dup(instr,a);
                    break;
                //subroutine related commands
                case "subroutine":
                    compile_subroutine(instr,a);
                    break;
                case "call":
                    compile_call(instr,a);
                    break;
                case "return":
                    a.writeReturn();
                    break;
                //arithmetic commands
                case "add":
                    compile_add(instr,a);
                    break;
                case "sub":
                    compile_sub(instr,a);
                    break;
                case "neg":
                    compile_neg(instr,a);
                    break;
                case "eq":
                    compile_eq(instr,a,uniq);
                    break;
                //control flow
                case "goto":
                    a.jmp(instr.getArg1().get());
                    break;
                case "if-goto":
                    compile_if_goto(instr,a,uniq);
                    break;
                case "label":
                    a.label(instr.getArg1().get());
                    break;
                case "malloc":
                    compile_malloc(instr,a);
                    break;
                case "free":
                    throw new Exception("free not yet implemented");
                default:
                    throw new Exception("unrecognized vm instr "+instr.getCmd());
            }
        }

        a.section(".data","");
        //a.any("achar db '0'","");

        a.section(".bss","");
        //a.any("buffer:	resb	64","");

        ////https://cs.lmu.edu/~ray/notes/nasmtutorial/
        ////https://stackoverflow.com/questions/8201613/printing-a-character-to-standard-output-in-assembly-x86
        return a.getAssemblyProgram();
    }

    private static void compile_malloc(IVMInstr instr, AssemblyWriter a) {
        //malloc receives as an argument the amount of DWORDs to allocate
        int amount = Integer.parseInt(instr.getArg1().get());

        a.mov(eax,192,"192 : mmap system call");
        a.xor(ebx,ebx,"addr=NULL");
        a.mov(ecx, amount,"size of segment to be allocated?");
        a.mov(edx,0x7,"prot = PROT_READ | PROT_WRITE | PROT_EXEC");
        a.mov(esi,0x22,"flags=MAP_PRIVATE | MAP_ANONYMOUS");
        a.mov(edi,-1,"fd=-1");

        //TODO: worry about ebp which might be needed for the stack
        //we should maybe save it somewhere
        a.xor(ebp,ebp,"offset=0");
        a.call_kernel();

        //eax should now contain
        //the address of the allocated memory segment

        //now we push that pointer on the stack
        a.push(eax);
    }

    private static void compile_if_goto(IVMInstr instr, AssemblyWriter a,long uniq) {

        String truelabel = "ifgoto_true"+uniq;
        String endlabel = "ifgoto_end"+uniq;

        a.pop(eax);
        a.mov(ebx,1);
        a.cmp(eax, ebx);
        a.je(truelabel);
        a.jmp(endlabel);

        //in case top of stack is 1 (true)
        a.label(truelabel);
        a.jmp(instr.getArg1().get());

        //otherwise, just continue execution
        a.label(endlabel);
    }

    private static void compile_eq(IVMInstr instr, AssemblyWriter a,long uniq) {
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

    private static void compile_neg(IVMInstr instr, AssemblyWriter a) {
        a.pop(eax);
        a.mov(ebx,-1);
        a.mul(eax, ebx);
        a.push(eax);
    }

    private static void compile_sub(IVMInstr instr, AssemblyWriter a) {
        a.pop(eax);
        a.pop(ebx);
        a.sub(eax, ebx);
        a.push(eax);
    }

    private static void compile_add(IVMInstr instr, AssemblyWriter a) {
        a.pop(eax);
        a.pop(ebx);
        a.add(eax, ebx);
        a.push(eax);
    }

    private static void compile_call(IVMInstr instr, AssemblyWriter a) throws Exception {
        String method = instr.getArg1().get();
        if(method.equals("putchar")){
            //https://stackoverflow.com/questions/8201613/printing-a-character-to-standard-output-in-assembly-x86

            //TODO: print top of stack
            //prog.pop("ecx");
            a.mov(eax,4,"sys_writ");
            a.mov(ebx,1,"std_out");

            //prog.mov("byte [ecx]","'*'");
            //prog.mov("[buffer]","'c'");
            //prog.mov("ecx","[buffer]");
            //prog.mov("ecx","achar"); //character

            //print the char on stack
            a.pop(ecx); //pop digit into ecx
            a.add(ecx,0x30); //add ascii offset
            a.push(ecx);  //push it back on stack
            a.mov(ecx, Register.esp);
            //prog.mov("ecx",65);
            //prog.mov("byte [achar]",65,"");

            //val length
            a.mov(Register.edx,1);
            a.call_kernel();
            //a.any("int 0x80"," call kernel");

            //pop from stack, as println is supposed to remove its argument
            a.pop(Register.edx);

            //push return value
            a.mov(Register.edx,0);
            a.push(Register.edx);
        }else if(method.equals("print")
                || method.equals("println")
                || method.equals("read")
                || method.equals("readln")
        ){
            throw new Exception("now yet implemented");
        }else {
            a.call(instr.getArg1().get());
        }
    }

    private static void compile_subroutine(IVMInstr instr, AssemblyWriter a) {
        a.label(instr.getArg1().get());
        //push the number of local variables on the stack
        //but that should already be done by the vm code generator
    }

    private static void compile_dup(IVMInstr instr, AssemblyWriter a) {
        //duplicates top of stack
        a.pop(eax);
        a.push(eax);
        a.push(eax);
    }

    private static void compile_push(IVMInstr instr, AssemblyWriter a) throws Exception {
        String segment1 = instr.getArg1().get();
        int index1 = Integer.parseInt(instr.getArg2().get());
        switch (segment1){
            case "ARG":
                a.pop(eax);
                //TODO: get base address of argument segment on stack
                //TODO
                break;
            case "LOCAL":
                a.pop(eax);
                //TODO: get base address of local segment on stack
                //TODO
                break;
            case "STATIC":
                throw new Exception("not yet implemented");
            default:
                throw new Exception("fatal");
        }
    }

    private static void compile_pop(IVMInstr instr, AssemblyWriter a) throws Exception{
        String segment = instr.getArg1().get();
        int index = Integer.parseInt(instr.getArg2().get());

        switch (segment){
            case "ARG":
                a.pop(eax);
                //TODO: get base address of argument segment on stack
                //TODO
                break;
            case "LOCAL":
                a.pop(eax);
                //TODO: get base address of local segment on stack
                //TODO
                break;
            case "STATIC":
                throw new Exception("not yet implemented");
            default:
                throw new Exception("fatal");
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
