package org.vanautrui.languages.vmcompiler.codegenerator;

import org.vanautrui.languages.vmcompiler.AssemblyWriter;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;

import static org.vanautrui.languages.vmcompiler.codegenerator.StackFocusedAssemblyCodeGenerator.compile_swap;
import static org.vanautrui.languages.vmcompiler.model.Register.*;

/**
 * this class should both contain the Assembly code generation for calling a subroutine, and for the subroutine itself.
 * this makes it easier to see what is happening, as both must work together
 */
public class SubroutineFocusedAssemblyCodeGenerator {

    //https://stackoverflow.com/questions/8201613/printing-a-character-to-standard-output-in-assembly-x86

    public static String compile_call_description(){
        //this method offers a description which can be included in higher level vm code
        return "pushes return address of current subroutine on the stack,jumps to subroutine,swaps returned value (on stack) with ebp, pops ebp";
    }
    public static void compile_call(VMInstr instr, AssemblyWriter a){
        String subroutineName = instr.arg1.get();

        a.call(subroutineName,instr.toString()); //puts the return address on the stack and jumps to label

        //swap return value with saved ebp
        compile_swap("swap return value with saved ebp",a);

        a.pop(ebp,"get our ebp back "+instr.toString());
    }

    public static void compile_callfromstack(VMInstr instr, AssemblyWriter a) {
        //this subroutine should be closely related to the compile_call subroutine

        a.pop(eax,instr.toString()); //pop our destination from stack

        a.call(eax,instr.toString()); //puts the return address on the stack and jumps to label

        //swap return value with saved ebp
        compile_swap("swap return value with saved ebp"+instr.toString(),a);

        a.pop(ebp,"get our ebp back "+instr.toString());
    }

    public static String compile_subroutine_description() {
        return "pushes ebp, swaps that with the return address already on stack";
    }
    public static void compile_subroutine(String subroutine_name, AssemblyWriter a) {
        final String comment = "subroutine "+subroutine_name;

        a.label(subroutine_name,comment);

        if(subroutine_name.equals("main")){
            //we need not save ebp of the caller, as there is no caller
            //our ebp is iconst 0, to reference local variables, we need an ebp
            a.mov(eax,0,"fake return address for main()"+comment);
            a.push(eax,"fake return address for main()"+comment);
        }else {
            //save the ebp of the caller
            //push ebp
            a.push(ebp, "push ebp to save ebp of caller "+comment);

            //swap with return address to provide correct order
            compile_swap(comment,a);
        }

        //to provide a base to reference arguments and local variables
        //ebp will point to the return address of the caller
        //above it there are the current subroutines arguments,
        //below it are its local variables
        a.mov(ebp,esp,comment+" anchor our ebp, with local variables below and saved ebp and args above");
    }

    public static void compile_return(AssemblyWriter a) {
        //
        a.ret();
    }
    public static String compile_return_description() {
        return "pops the return address in the caller subroutine off the stack, and continues there.";
    }


}
