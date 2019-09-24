package org.vanautrui.languages.vmcompiler.codegenerator;

import org.vanautrui.languages.vmcompiler.AssemblyWriter;
import org.vanautrui.languages.vmcompiler.instructions.VMInstr;

import java.util.Optional;

import static org.vanautrui.languages.vmcompiler.model.Register.*;
import static org.vanautrui.languages.vmcompiler.model.Register.ebx;

public class StackFocusedAssemblyCodeGenerator {


  public static void compile_dup(VMInstr instr, AssemblyWriter a) {
    //duplicates top of stack
    a.mov(eax,"[esp]",instr.toString());
    //a.pop(eax,instr.toString());
    //a.push(eax,instr.toString());
    a.push(eax,instr.toString());
  }

  /**
   * swaps the 2 topmost items on the stack
   * @param a
   */
  public static void compile_swap(String mycomment, AssemblyWriter a) {
    String comment=" swap ";

    if(!mycomment.isEmpty()){
      comment=mycomment;
    }

    a.pop(eax,comment);
    a.pop(ebx,comment);

    a.push(eax,comment);
    a.push(ebx,comment);
  }

  public static void compile_push(String segment,int index, AssemblyWriter a) throws Exception {
    String comment="";

    switch (segment){
      case "ARG":
        comment="push ARG "+index;
        //arguments are on stack in reverse order
        //ebp is on the caller's return address, with the arguments above

        //eax = ebp + index + 2 * (4 (byte))
        //the + 2 * (4 (byte)) is because there is the saved ebp on stack also,
        //and the variables are indexed starting with 0
        a.mov(eax,2*4,comment); //4 bytes make up a 32 bit item on stack
        a.add(eax,ebp,comment);
        a.add(eax,index*4,comment); //4 bytes make up a 32 bit item on stack

        // a.dereference(eax) -> mov eax,[eax]
        a.dereference(eax,comment);

        //push eax
        a.push(eax,comment);
        break;
      case "LOCAL":
        comment="push LOCAL "+index;
        //locals are on stack in order
        //ebp is on the caller's return address, with the local variables below

        //eax = ebp - index - 1 //the +1 is because there is the saved ebp on stack also

        //eax=ebp
        a.mov(eax,ebp,comment);

        //eax -= 1
        a.mov(ebx,1*4,comment); //4 bytes make up a 32 bit item on stack
        a.sub(eax,ebx,comment);

        //eax -= index
        a.mov(ebx,index*4,comment); //4 bytes make up a 32 bit item on stack
        a.sub(eax,ebx,comment);

        // a.dereference(eax) -> mov eax,[eax]
        a.dereference(eax,comment);

        //push eax
        a.push(eax,comment);
        break;
      case "STATIC":
        throw new Exception("not yet implemented");
      default:
        throw new Exception("fatal");
    }
  }

  /**
   * @param mysegment the Optional segment to pop into
   * @param a the AssemblyWriter we are using
   * @throws Exception if we are pop - ing from a nonexisting segment
   */
  public static void compile_pop(Optional<String> mysegment, Optional<Integer> myindex, AssemblyWriter a) throws Exception{

    if(!mysegment.isPresent()){
      a.pop(eax,"pop");
    }else {

      String segment = mysegment.get();

      int index= myindex.get();
      String comment="";

      switch (segment) {
        case "ARG":
          comment = "pop ARG "+index;

          //arguments are on stack in reverse order
          //ebp is on the caller's return address, with the arguments above

          //eax = ebp + index + 2 //the +2 is because there is the saved ebp on stack also,
          //and the variables are indexed starting with 0
          a.mov(eax,2*4,comment); //4 bytes make up a 32 bit item on stack
          a.add(eax,ebp,comment);
          a.add(eax,index*4,comment);//4 bytes make up a 32 bit item on stack

          //get the value we want to pop
          a.pop(ebx,comment);

          //get that value into the address pointed to by eax
          a.store_second_into_memory_location_pointed_to_by_first(eax,ebx,comment);
          break;
        case "LOCAL":
          comment = "pop LOCAL "+index;

          //locals are on stack in order
          //ebp is on the caller's return address, with the local variables below

          //eax = ebp - index - 1 //the +1 is because there is the saved ebp on stack also

          //eax=ebp
          a.mov(eax,ebp,comment);

          //eax -= 1
          a.mov(ebx,1 * 4,comment); //4 bytes make up a 32 bit item on stack
          a.sub(eax,ebx,comment);

          //eax -= index
          a.mov(ebx,index*4,comment); //4 bytes make up a 32 bit item on stack
          a.sub(eax,ebx,comment);

          //get the value we want to pop
          a.pop(ebx,comment);

          a.store_second_into_memory_location_pointed_to_by_first(eax,ebx,comment);
          break;
        case "STATIC":
          throw new Exception("not yet implemented");
        default:
          throw new Exception("fatal");
      }
    }
  }
}
