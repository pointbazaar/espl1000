package org.vanautrui.languages.vmcompiler.instructions;

import java.util.Optional;

/**
 * DAO for a single VM Instruction.
 * It stores the command and its arguments.
 */
public class VMInstr{

    public VMInstr(String s){
        String[] parts=s.split(" ");
        this.cmd=parts[0];
        if(parts.length>1){
            this.arg1=Optional.of(parts[1]);
        }else{
            this.arg1=Optional.empty();
        }
        if(parts.length>2){
            this.arg2=Optional.of(parts[2]);
        }else{
            this.arg2=Optional.empty();
        }
    }

    public String cmd;
    public final Optional<String> arg1;
    public final Optional<String> arg2;

    @Override
    public String toString(){
        return cmd+" "+arg1.orElse("")+arg2.orElse("");
    }

}
