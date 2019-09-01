package org.vanautrui.languages.codegeneration.dracovmbackend.vmcompiler.instructions;

import java.util.Optional;

public class VMInstr implements IVMInstr {

    public VMInstr(String s){
        String[] parts=s.split(" ");
        this.cmd=parts[0];
        if(parts.length>1){
            this.arg1=Optional.of(parts[1]);
        }
        if(parts.length>2){
            this.arg1=Optional.of(parts[2]);
        }
    }

    private String cmd;
    private Optional<String> arg1=Optional.empty();
    private Optional<String> arg2=Optional.empty();

    @Override
    public String getCmd() {
        return this.cmd;
    }

    @Override
    public Optional<String> getArg1() {
        return this.arg1;
    }

    @Override
    public Optional<String> getArg2() {
        return this.arg2;
    }
}
