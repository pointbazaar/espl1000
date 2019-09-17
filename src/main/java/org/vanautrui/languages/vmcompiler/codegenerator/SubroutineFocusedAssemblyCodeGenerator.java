package org.vanautrui.languages.vmcompiler.codegenerator;

import org.vanautrui.languages.vmcompiler.AssemblyWriter;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;
import org.vanautrui.languages.vmcompiler.model.Register;

import static org.vanautrui.languages.vmcompiler.model.Register.*;

/**
 * this class should both contain the Assembly code generation for calling a subroutine, and for the subroutine itself.
 * this makes it easier to see what is happening, as both must work together
 */
public class SubroutineFocusedAssemblyCodeGenerator {



    private static void compile_putchar(VMInstr instr, AssemblyWriter a){
        //https://stackoverflow.com/questions/8201613/printing-a-character-to-standard-output-in-assembly-x86

        //prints top of stack as ascii char to stdout

        //we can spare this as putchar is not modifying ebp and thus has no need to save it
        //a.push(ebp,"putchar: save ebp");

        a.mov(eax,4,"putchar: sys_write");
        a.mov(ebx,1,"putchar: std_out");

        //print the char on stack
        a.mov(ecx, Register.esp,"putchar: print the char on the stack");

        //val length
        a.mov(Register.edx,1,"putchar: value length");
        a.call_kernel();

        //push return value
        a.mov(Register.edx,0,"putchar: push return value");
        a.push(Register.edx,"putchar: push return value");

        //we can spare this as putchar is not modifying ebp and thus has no need to save it
        //a.pop(ebp,"putchar: get our ebp back");
    }

    private static void compile_readint(VMInstr instr, AssemblyWriter a)throws Exception{
        throw new Exception("unhandled");
    }

    public static void compile_call(VMInstr instr, AssemblyWriter a) throws Exception {
        String method = instr.arg1.get();
        switch (method){
            case "putchar": compile_putchar(instr,a); break;
            case "readint": compile_readint(instr,a); break;
            case "int2char": compile_int2char(instr,a); break;
            case "int2float": compile_int2float(instr,a); break;
            case "float2int": compile_float2int(instr,a); break;
            default:
                a.push(ebp,"push ebp to save ebp of caller");
                a.call(instr.arg1.get()); //puts the return address on the stack and jumps to label
                a.pop(ebp,"get our ebp back");
                break;
        }
    }

    private static void compile_float2int(VMInstr instr, AssemblyWriter a) throws Exception{
        throw new Exception("unhandled");
    }

    private static void compile_int2float(VMInstr instr, AssemblyWriter a) throws Exception{
        throw new Exception("unhandled");
    }

    private static void compile_int2char(VMInstr instr, AssemblyWriter a) throws Exception{
        throw new Exception("unhandled");
    }

    public static void compile_subroutine(VMInstr instr, AssemblyWriter a) {
        a.label(instr.arg1.get(),instr.toString());

        //to provide a base to reference arguments and local variables
        //ebp will point to the return address of the caller
        //above it there are the current subroutines arguments,
        //below it are its local variables
        a.mov(ebp,esp);
    }
}
