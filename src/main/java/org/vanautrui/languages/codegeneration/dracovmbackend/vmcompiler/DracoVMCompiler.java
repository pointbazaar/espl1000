package org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler;

import org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler.instructions.IVMInstr;
import org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler.instructions.VMInstr;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.codegeneration.dracovmbackend.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler.model.Register.*;
import static org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler.model.Register.eax;

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

    private static List<String> vm_codes_to_assembly(List<IVMInstr> vmcodes) throws Exception {
        //receives only clean VM Codes
        //TODO
        AssemblyWriter a=new AssemblyWriter();

        a.section(".text","must be declared for linker (ld)");
        a.global("_start","");
        a.label("_start","tell linker entry point");

        //at the start of the code, jump to main
        a.jmp("main");

        String current_subroutine;

        for(IVMInstr instr : vmcodes){
            //uniqueness for labels
            long uniq = unique();

            switch (instr.getCmd()){
                //stack related
                case "ipush":
                case "fpush":
                    a.mov(eax,Float.parseFloat(instr.getArg1().get()));
                    a.push(eax);
                    break;
                case "pop":
                case "push":
                    throw new Exception("not yet implemented");
                case "dup":
                    //duplicates top of stack
                    a.pop(eax);
                    a.push(eax);
                    a.push(eax);
                    break;


                //subroutine calls
                case "subroutine":
                    current_subroutine=instr.getArg1().get();
                    a.label(instr.getArg1().get());
                    //push the number of local variables on the stack
                    //but that should already be done by the vm code generator
                    break;
                case "call":
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
                        a.mov(ecx,esp);
                        //prog.mov("ecx",65);
                        //prog.mov("byte [achar]",65,"");

                        //val length
                        a.mov(edx,1);
                        a.call_kernel();
                        //a.any("int 0x80"," call kernel");

                        //pop from stack, as println is supposed to remove its argument
                        a.pop(edx);

                        //push return value
                        a.mov(edx,0);
                        a.push(edx);
                    }else if(method.equals("print")
                        || method.equals("println")
                        || method.equals("read")
                        || method.equals("readln")
                    ){
                        throw new Exception("now yet implemented");
                    }else {
                        a.call(instr.getArg1().get());
                    }
                    break;
                case "return":
                    a.writeReturn();
                    break;

                //arithmetic
                case "add":
                    a.pop(eax);
                    a.pop(ebx);
                    a.add(eax,ebx);
                    a.push(eax);
                    break;
                case "sub":
                    a.pop(eax);
                    a.pop(ebx);
                    a.sub(eax,ebx);
                    a.push(eax);
                    break;
                case "neg":
                    a.pop(eax);
                    a.mov(ebx,-1);
                    a.mul(eax,ebx);
                    a.push(eax);
                    break;

                case "eq":

                    String labeltrue="eq_push1"+uniq;
                    String labelend="eq_end"+uniq;

                    a.pop(eax);
                    a.pop(ebx);
                    a.cmp(eax,ebx);
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

                    break;

                case "goto":
                    a.jmp(instr.getArg1().get());
                    break;
                case "if-goto":
                    String truelabel = "ifgoto_true"+uniq;
                    String endlabel = "ifgoto_end"+uniq;

                    a.pop(eax);
                    a.mov(ebx,1);
                    a.cmp(eax,ebx);
                    a.je(truelabel);
                    a.jmp(endlabel);

                    //in case top of stack is 1 (true)
                    a.label(truelabel);
                    a.jmp(instr.getArg1().get());

                    //otherwise, just continue execution
                    a.label(endlabel);
                    break;
                case "label":
                    a.label(instr.getArg1().get());
                    break;
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
        return a.finalizeAndReturnAssemblyProgram();
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
