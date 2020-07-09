package org.vanautrui.languages.commandline;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.simplify.Simplifier;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typechecking.TypeChecker;
import org.vanautrui.languages.compiler.codegenerator.JavaCodeGenerator;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator.createStructsSymbolTable;
import static org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator.createSubroutineSymbolTable;

public final class CompilerPhases {

    //prevents instance creation
    private CompilerPhases(){}

    public static void phase_typecheck(
            final AST_Whole_Program ast, final boolean debug
    )throws Exception{
        if(debug) {
            System.out.println("TYPE CHECKING");
        }

        //this should throw an exception, if it does not typecheck
        try {
            TypeChecker.doTypeCheck(ast,debug);

        }catch (Exception e){
            throw e;
        }
    }

    public static List<Path> phase_java_codegeneration(
            final AST_Whole_Program ast,
            final boolean printsymboltables,
            final boolean debug
    )throws Exception{

        if(debug) {
            System.out.println(String.format("CompilerPhases::phase_java_codegeneration(...,printsymboltables = %b, debug = %b)",printsymboltables,debug));
        }

        final SubroutineSymbolTable subTable = createSubroutineSymbolTable(ast,debug);
        final StructsSymbolTable structsTable = createStructsSymbolTable(ast,debug);
        final Map<String,List<String>> javaCodes = JavaCodeGenerator.generateJavaCode(ast, subTable,structsTable,debug,printsymboltables);

        final List<Path> paths = new ArrayList<>();

        for(Map.Entry<String,List<String>> subr : javaCodes.entrySet()){

            final String extension = ".java";
            final Path path = Paths.get(subr.getKey()+extension);

            if(debug) {
                System.out.println("write: " + path);
            }

            Files.write(
                    path,
                    (String.join("\n", subr.getValue()) +"\n").getBytes()
            );

            paths.add(path);
        }

        return paths;
    }
}
