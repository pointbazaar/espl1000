package org.vanautrui.languages.codegeneration.symboltables.nameconversions;

import java.util.List;
import java.util.stream.Collectors;

public class TypeNameToJVMInternalTypeNameConverter {

    public static String convertTypeNameToJVMInternalTypeName(String type){
        switch (type){
            case "Int":
                return "I";
            case "String":
                return "java/lang/String";
            case "Char":
                return "C";
            default:
                return type;
        }
    }

    private static String convertTypeToFieldType(String type){
        return "L"+convertTypeNameToJVMInternalTypeName(type)+";";
    }

    public static String convertSubroutineName(String returnType, List<String> argumentTypes){
        //TODO: check if it is correct
        String parameterDescriptor="";
        String returnDescriptor="";
        if(returnType.equals("Void")){
            returnDescriptor="V";
        }else{
            returnDescriptor = convertTypeToFieldType(returnType);
        }

        parameterDescriptor=argumentTypes.stream().map(arg->convertTypeToFieldType(arg)).collect(Collectors.joining(""));

        return "("+parameterDescriptor+")"+returnDescriptor;
    }
}
