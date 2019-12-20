package org.vanautrui.languages.compiler.symboltables.structs;

import java.util.ArrayList;
import java.util.List;

public final class StructsSymbolTableRow {

    //this class should represent a struct, and its members.

    public final String structName;

    public final List<String> members;
    public final List<String> memberTypes;

    public StructsSymbolTableRow(final String myStructName){
        this.structName=myStructName;
        this.memberTypes=new ArrayList<>();
        this.members=new ArrayList<>();
    }

    public StructsSymbolTableRow(final String myStructName, final List<String> memberNames, final List<String> memberTypeNames){
        this.structName=myStructName;
        this.members=memberNames;
        this.memberTypes=memberTypeNames;
    }

    public final boolean containsMember(final String memberName){
        return this.members.stream().anyMatch(s->s.equals(memberName));
    }

    public final int getIndexOfMember(final String memberName)throws Exception{
        if(this.containsMember(memberName)){
            return this.members.indexOf(memberName);
        }
        throw new Exception("could not find struct member: "+memberName+" in struct: "+structName);
    }

    public final String getTypeOfMember(final String memberName) throws Exception{
        return this.memberTypes.get(getIndexOfMember(memberName));
    }
}
