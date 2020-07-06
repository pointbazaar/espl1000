package org.vanautrui.languages.commandline;

import com.fasterxml.jackson.datatype.jdk8.Jdk8Module;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

import java.io.File;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.stream.Collectors;

import static java.lang.System.currentTimeMillis;
import static java.lang.System.out;
import static org.vanautrui.languages.commandline.ASTReader.parseNamespaceFromASTFile;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printBuildConclusion;
import static org.vanautrui.languages.commandline.CompilerPhaseUtils.printDurationFeedback;

public final class DragonCompiler {

	//prevents instance creation
	private DragonCompiler(){}

	//this should be the compiler

	//Assembly debugging:
	//(they are missing a stack display, but it be ok)
	//https://carlosrafaelgn.com.br/Asm86/

	//general having fun and trying different programming languages:
	//https://tio.run/#
	//https://www.tutorialspoint.com/codingground.htm

	//https://asmtutor.com/
	//https://asmtutor.com/#lesson1

	//https://www.youtube.com/watch?v=ubXXmQzzNGo

	public static void main(String[] args) {

		try {
			DragonCompiler.compile_main(Arrays.asList(args));
			System.exit(0);
		}catch (Exception e){
			out.println(e.getMessage());
			out.println("-help     for information about command line arguments");
			System.exit(1);
		}
	}

	//one thing to note would be that
	//we can include multiple stages of tokenization
	//and parsing.
	//parsing can include multiple passes and also
	//the AST and related data could undergo multiple representations
	//before reaching the final representation
	//from which code can be generated

	public static void compile_main(final List<String> args) throws Exception {

		final List<String> options = createOptions();

		//get all flags and remove prefixed '-'
		final List<String> flags = args
				.stream()
				.filter(s->s.startsWith("-"))
				.map(s->s.substring(1))
				.collect(Collectors.toList());

		//as no option currently has an argument,
		//this simplifies the usage of the compiler
		//also, everything that doesnt start with '-' is either a source file or directory

		if(flags.contains("help")){
			printHelp();
		}else {

			if(flags.contains("clean")){
				if(flags.contains("debug")){
					out.println("clearing the cache");
				}
				final String cache_dir=System.getProperty("user.home")+"/dragoncache";
				FileUtils.deleteDirectory(Paths.get(cache_dir).toFile());
			}

			List<String> fileArgs = args.stream().filter(str -> !str.startsWith("-")).collect(Collectors.toList());

			compile_main_inner(getAllDragonFilesRecursively(fileArgs), flags);
		}
	}

	private static List<File> getAllDragonFilesRecursively(final List<String> fileArgs) {
		//from dgc options, this can either be files or directories
		//which must be compiled together.
		final List<File> results=new ArrayList<>();
		for(final String s : fileArgs) {
			final Path path = Paths.get(s);
			if(path.toFile().isDirectory()) {
				//add all the files recursively
				final Collection<File> files = FileUtils.listFiles(
						path.toFile(),
						new String[]{"dg"},
						true
				);
				results.addAll(files);
			}else {
				if (s.endsWith(".dg")) {
					results.add(path.toFile());
				}
			}
		}
		return results;
	}

	private static void printHelp(){

		StringBuilder sbh = new StringBuilder("");
		StringBuilder sbf = new StringBuilder("");

		sbh.append("\ndraco - a compiler for the dragon language\n");

		sbf.append("\n");
		sbf.append("EXAMPLES\n\n");
		sbf.append("    draco Main.dg\n");
		sbf.append("    draco -targetATMEL Main.dg\n");
		sbf.append("    draco -debug Main.dg\n");
		sbf.append("\n");


		sbf.append("GITHUB");
		sbf.append("    https://github.com/pointbazaar/dragon/\n");
		sbf.append("\n");

		sbf.append("AUTHOR\n\n");
		sbf.append(
				String.format("    %-20s\n","alex23667@gmail.com")+
						String.format("    %-20s\n","David.Milosevic@web.de")
		);
		sbf.append("\n");

		sbf.append("REPORTING BUGS");
		sbf.append("    https://github.com/pointbazaar/dragon/issues\n\n");

		String header=sbh.toString();
		String footer=sbf.toString();

		out.println("draco FILE...");
		out.println(header);
		out.println(createOptions().stream().collect(Collectors.joining("\n")));
		out.println(footer);
	}

	//declaring these identifiers so that we can change the actual names of the options
	//without having to comb through all the code
	public static final String FLAG_DEBUG="debug";
	public static final String FLAG_TIMED="timed";
	public static final String FLAG_TARGET_ATMEL="targetATMEL";

	public static final String FLAG_PRINT_SYMBOLTABLES="symboltables";
	public static final String FLAG_PRINT_HELP="help";

	public static final String FLAG_CLEAN="clean";

	private static List<String> createOptions(){

		ArrayList<String> arr = new ArrayList<String>();

		arr.add(FLAG_DEBUG);//,false,"prints debug output"));

		arr.add(FLAG_TIMED);//,false,"how long did the build take?"));
		arr.add(FLAG_TARGET_ATMEL);//,false,"generate .asm files for ATMEL Microcontrollers ?"));

		arr.add(FLAG_PRINT_SYMBOLTABLES);//,false,"print symbol tables"));
		arr.add(FLAG_PRINT_HELP);//,false,"print help"));

		arr.add(FLAG_CLEAN);//,false,"clear cache"));

		return arr;
	}

	public static Path makePathHiddenWithCustomExtension(final File filename, final String fullextension){

		//provide a Path for a file, which places a '.' in front of the filename and adds a custom extension

		final String filenameWithoutPath = filename.getName();
		final String directoryPath = filename.getParent(); //could be null

		final String correctFilename = ((directoryPath==null)?".":directoryPath)+"/."+filenameWithoutPath+fullextension;

		return Paths.get(correctFilename);
	}

	private static void compile_main_inner(final List<File> sourceFiles, final List<String> flags){

		final boolean debug=flags.contains(FLAG_DEBUG);
		final boolean timed=flags.contains(FLAG_TIMED);
		final boolean targetATMEL = flags.contains(FLAG_TARGET_ATMEL);

		final long start_time_ms = currentTimeMillis();

		try {

			//PHASE LEXING
			for(final File sourceFile : sourceFiles) {
				invokeDragonLexer(sourceFile,debug);
			}

			//PHASE PARSING
			for(final File sourceFile : sourceFiles) {

				final Path correctTokenFilename = makePathHiddenWithCustomExtension(sourceFile,".tokens");

				invokeDragonParser(correctTokenFilename.toFile(),debug);
			}

			final List<File> jsonFiles = sourceFiles
					.stream()
					.map(f-> makePathHiddenWithCustomExtension(f,".json").toFile())
					.collect(Collectors.toList());

			final AST_Whole_Program ast = parseASTFromASTFiles(jsonFiles,debug);

			//PHASE: TYPE CHECKING
			CompilerPhases.phase_typecheck(ast,debug);

			//PHASE: SIMPLIFY
			CompilerPhases.phase_simplify(ast,debug);

			//PHASE CODE GENERATION, returns a list of paths where the files for the subroutines are
			final List<Path> javaFiles = CompilerPhases.phase_java_codegeneration(ast, flags.contains(FLAG_PRINT_SYMBOLTABLES),debug);

			//PHASE VM CODE COMPILATION, PHASE GENERATE EXECUTABLE
			//this phase depends on 'dracovm'
			//which can be obtained here: https://github.com/pointbazaar/dracovm-compiler
			//for each subroutine in vm code, make a NAME.subroutine.dracovm file
			invokeJavaCompiler(javaFiles,debug);

			if(timed) {
				final long end_time_ms = currentTimeMillis();
				final long duration = end_time_ms-start_time_ms;
				printDurationFeedback(duration);
			}

			printBuildConclusion(true);

		} catch (final Exception e) {

			out.println(e.getMessage());

			if(debug) {
				e.printStackTrace(out);
			}

			printBuildConclusion(false);
		}
	}

	public static void invokeDragonLexer(final File filename, final boolean debug)throws Exception{
		final String call = ((debug)?"dragon-lexer-debug ":"dragon-lexer ")+filename;
		if(debug){
			out.println(call);
		}
		Process p = Runtime.getRuntime().exec(call);
		p.waitFor();

		if(debug){
			final InputStream is = p.getInputStream();
			final String output = IOUtils.toString(is);
			final List<String> outputlist = Arrays.stream(output.split("\n")).map(s -> "\t" + s).collect(Collectors.toList());
			outputlist.forEach(out::println);
		}

		if(p.exitValue()==0 && debug) {
			out.println("... exit successfully");
		}

		if(p.exitValue()!=0){
			throw new Exception("dragon-lexer exit with nonzero exit code");
		}
	}

	public static void invokeJavaCompiler(final List<Path> javaFiles, final boolean debug ) throws Exception {

		if(debug){
			out.println(String.format("invokeJavaCompiler(%s,%b)",javaFiles.stream().map(Path::toString).collect(Collectors.joining(",")),debug));
		}

		final String call =
				"javac "
				+javaFiles
				.stream()
				.map(Path::toString)
				.collect(Collectors.joining(" "));

		if(debug) { out.println(call); }

		final Process process = Runtime.getRuntime().exec(call);
		process.waitFor();

		if(debug) {
			final var is = process.getInputStream();
			final String output = IOUtils.toString(is);
			final List<String> outputlist = Arrays.stream(output.split("\n")).map(s -> "\t" + s).collect(Collectors.toList());
			outputlist.forEach(out::println);

			final var es = process.getErrorStream();
			final String eo = IOUtils.toString(es);
			out.println(eo);
		}

		if(process.exitValue() != 0 ){
			throw new Exception("javac exited with nonzero exit value.");
		}else{
			if(debug) { out.println("... exit successfully"); }
		}
	}

	public static void invokeDragonParser(final File tokensFile, final boolean debug) throws Exception {
		final String call = "dragon-parser "+((debug)?" -debug ":" ")+tokensFile.toString();
		if(debug){
			out.println(call);
		}
		final Process p = Runtime.getRuntime().exec(call);
		p.waitFor();
		if(debug){
			final InputStream is = p.getInputStream();
			final String output = IOUtils.toString(is);
			final List<String> outputlist = Arrays.stream(output.split("\n")).map(s -> "\t" + s).collect(Collectors.toList());
			outputlist.forEach(out::println);
		}

		if(p.exitValue() != 0){
			throw new Exception("dragon-parser exited with nonzero exit value.");
		}else{
			if(debug) {
				out.println("... exit successfully");
			}
		}
	}


	public static AST_Whole_Program parseASTFromASTFiles(final List<File> astFiles, final boolean debug) throws Exception {

		final AST_Whole_Program ast = new AST_Whole_Program();
		for(File file : astFiles){
			ast.namespaceNodeList.add(parseNamespaceFromASTFile(file,debug));
		}
		return ast;
	}
}
