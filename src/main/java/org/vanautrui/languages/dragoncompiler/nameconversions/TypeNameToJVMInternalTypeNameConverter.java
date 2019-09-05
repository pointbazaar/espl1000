package org.vanautrui.languages.dragoncompiler.nameconversions;

import java.util.List;
import java.util.stream.Collectors;

public class TypeNameToJVMInternalTypeNameConverter {

    public static String convertTypeNameToJVMInternalTypeName(String type){

        //all non primitive types go like 'L' + actualType + ';'

        //TODO: write tests

        switch (type){
            case "Int":
                return "I";
	    case "Float":
		    return "F";
        case "String":
            return "Ljava/lang/String;";
        case "Char":
            return "C";
        default:
            if(type.startsWith("[")){
                //array type
                return "["+convertTypeNameToJVMInternalTypeName(type.substring(1,type.length()-1));
            }else {
                //object type
                return "L" + type + ";";
            }
        }
    }

    /*
    private static String convertTypeToFieldType(String type){
        return "L"+convertTypeNameToJVMInternalTypeName(type)+";";
    }
     */

    public static String convertSubroutineName(String returnType, List<String> argumentTypes,boolean debug){
        //TODO: check if it is correct
        String parameterDescriptor="";
        String returnDescriptor="";
        if(returnType.equals("Void")){
            returnDescriptor="V";
        }else{
            returnDescriptor = convertTypeNameToJVMInternalTypeName(returnType);
        }

        parameterDescriptor=argumentTypes.stream().map(arg->convertTypeNameToJVMInternalTypeName(arg)).collect(Collectors.joining(""));

        String result = "("+parameterDescriptor+")"+returnDescriptor;

        if(debug){
            System.out.println("converted subroutine name: "+result);
        }
        return result;
    }
}
