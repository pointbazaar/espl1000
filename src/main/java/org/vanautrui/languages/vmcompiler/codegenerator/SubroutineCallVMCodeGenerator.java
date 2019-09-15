package org.vanautrui.languages.vmcompiler.codegenerator;

import org.vanautrui.languages.vmcompiler.AssemblyWriter;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;
import org.vanautrui.languages.vmcompiler.model.Register;

import static org.vanautrui.languages.vmcompiler.model.Register.*;

public class SubroutineCallVMCodeGenerator {
    private static void compile_putchar(VMInstr instr, AssemblyWriter a){
        //https://stackoverflow.com/questions/8201613/printing-a-character-to-standard-output-in-assembly-x86

        //prints top of stack as ascii char to stdout

        a.mov(eax,4,"sys_writ");
        a.mov(ebx,1,"std_out");

        //print the char on stack
        a.pop(ecx); //pop digit into ecx
        a.add(ecx,0x30); //add ascii offset
        a.push(ecx);  //push it back on stack
        a.mov(ecx, Register.esp);

        //val length
        a.mov(Register.edx,1,"value length");
        a.call_kernel();

        //pop from stack, as putchar is supposed to remove its argument
        a.pop(Register.edx);

        //push return value
        a.mov(Register.edx,0);
        a.push(Register.edx);
    }

    private static void compile_readint(VMInstr instr, AssemblyWriter a){
        //TODO
    }

    public static void compile_call(VMInstr instr, AssemblyWriter a) throws Exception {
        String method = instr.arg1.get();
        switch (method){
            case "putchar":
                compile_putchar(instr,a);
                break;
            case "readint":
                compile_readint(instr,a);
                break;
            case "int2char":
                compile_int2char(instr,a);
                break;
            case "int2float":
                //TODO
                break;
            case "float2int":
                //TODO
                break;
            default:
                a.call(instr.arg1.get());
                break;
        }
    }

    private static void compile_int2char(VMInstr instr, AssemblyWriter a) {

    }
}
