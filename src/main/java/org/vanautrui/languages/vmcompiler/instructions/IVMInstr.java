package org.vanautrui.languages.vmcompiler.instructions;

import java.util.Optional;

public interface IVMInstr {

    public String getCmd();

    public Optional<String> getArg1();
    public Optional<String> getArg2();
}
