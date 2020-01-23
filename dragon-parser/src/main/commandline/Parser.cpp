//standard includes
include <vector> 
include <stdio>
include <map>

//project includes
include "../compiler/lexing/utils/TokenList.hpp"
include "../compiler/parsing/astnodes/nonterminal/upperscopes/NamespaceNode.hpp"

using namespace std;

int main(int argc, char** argv){


	//this project is to parse a Dragon AST
	//from Tokens written into .tokens files by Dragon-Lexer,
	//and store it in a .json file to be retrieved by the Dragon Compiler

	//for simplicity, we invoke with just one filename as argument.
	//such we can easily have
	//multiple parallel invocations of the parser in the compiler.

	const vector<string> options = createOptions();

	const List<String> flags = Arrays.stream(args)
			.filter(s->s.startsWith("-"))
			.collect(Collectors.toList());

	const List<String> filenames = Arrays.stream(args)
			.filter(s->!s.startsWith("-"))
			.collect(Collectors.toList());

	bool debug = true;

	try {

		debug = flags.contains(FLAG_DEBUG);
		if(debug){
			out.println("Parser::main");
		}

		bool help = flags.contains(FLAG_HELP);
		bool test = flags.contains(FLAG_TEST);

		if(help) {
			ParserCLIUtils.printHelp();
		}else if(test){
			ParserTest.test_all();
		}else{
			if(filenames.size() != 1){
				throw new Exception("expected exactly 1 filename argument.");
			}
			main_inner(Paths.get(filenames.get(0)).toFile(),debug);
		}

	} catch (Exception e) {
		if(debug) {
			e.printStackTrace(out);
		}

		out.println(e.getMessage());

		System.exit(1);
	}
}

void build_ast_json_file(File tokensFile, File astJsonFile, bool debug) throws Exception {
	if(debug){
		out.println("Parser::build_ast_json_file");
	}

	const TokenList tokens = ParserPhases.readTokensFromTokensFile(tokensFile,debug);

	//get just the namespace name from .FILENAME.dg.tokens
	const String tokenFileName = tokensFile.getName();
	const String namespaceName = tokenFileName.substring(1,tokenFileName.length()-(".dg.tokens").length());

	const NamespaceNode mynamespace = new NamespaceNode(tokens,namespaceName,debug);

	//write json file
	ObjectMapper mapper = new ObjectMapper(JsonFactory.builder().build());
	mapper.enable(SerializationFeature.INDENT_OUTPUT);
	mapper.registerModule(new Jdk8Module());

	const String str = mapper.writeValueAsString(mynamespace);


	if(debug){
		out.println("write to "+astJsonFile.toPath());
	}
	Files.writeString(astJsonFile.toPath(),str);
}

void main_inner(File tokensFile, bool debug) throws Exception {

	if(debug){
		out.println("Parser::main_inner");
		out.println("Tokens File to parse: "+tokensFile.toString());
	}

	if(!tokensFile.toString().endsWith(".tokens")){
		throw new Exception(tokensFile.toString()+" does not have .tokens extension. Exiting.");
	}

	//it should receive a .filename.dg.tokens file as path
	//and write a .filename.dg.json file to disk

	//in this approach it should be incremental. if the modified time
	//of the .json file is later than that of the .tokens file, the .json
	//file should not be rebuilt

	if(tokensFile.exists()) {
		final long tokensLastModified = tokensFile.lastModified();

		final String astJSONFilename = tokensFile.toString().substring(0,tokensFile.toString().length()-(".tokens").length())+".json";

		final File ast_json_file = (
				Paths.get(astJSONFilename)
		).toFile();

		if(ast_json_file.exists()){
			if(debug){
				out.println(ast_json_file+"  already exists.");
			}
			//see which is more recent
			final long ast_json_last_modified = ast_json_file.lastModified();

			if(ast_json_last_modified < tokensLastModified){
				build_ast_json_file(tokensFile,ast_json_file,debug);
			}else{
				if(debug) {
					out.println("the AST .json file was more recent than the tokens file. nothing needs to be done.");
				}
			}
		}else{
			//ast .json file does not exist
			build_ast_json_file(tokensFile,ast_json_file,debug);
		}
	}else {
		throw new Exception("argument file "+tokensFile.toString()+" does not exist.");
	}

}

void printHelp() {


	string header = "dragon-parser - a parser for the dragon programming language\n";

	string footer = "EXAMPLES\n" +
	                      "   dragon-parser .Main.dg.tokens\n" +
	                      "   dragon-parser -debug .Main.dg.tokens\n" +
	                      "\n" +
	                      "            GITHUB\n" +
	                      "   https://github.com/pointbazaar/dragon-parser/\n" +
	                      "\n" +
	                      "AUTHOR\n" +
	                      "alex23667@gmail.com\n" +
	                      "\n" +
	                      "            REPORTING BUGS\n" +
	                      "   https://github.com/pointbazaar/dragon/issues\n" +
	                      "\n";

	cout 
		<< "dragon-parser FILE" << endl
		<< header << endl
		<< endl
		<< String.join("\n", createOptions())
		<< endl
		<< footer << endl;
}

vector<string> createOptions() {
	vector<string> res;

	res.push_back(Parser.FLAG_DEBUG); //,false,"prints debug output"));

	res.push_back(Parser.FLAG_HELP); //false,"print help"));

	res.push_back(Parser.FLAG_TEST); //false, "run tests"));

	return res;
}

TokenList makeTokenListByCallingLexer(File file, bool debug) throws Exception {

	// - call dragon-lexer with the filename
	// - read its output file, the tokens, put them in a TokenList

	string call = ((debug)?"dragon-lexer-debug ":"dragon-lexer ") + file.toPath().toString();

	if (debug) {
		out.println(call);
	}

	final Process proc = Runtime.getRuntime().exec(call);
	proc.waitFor();

	if (debug) {
		out.println("stdout: ");
		BufferedReader stdInput = new BufferedReader(new InputStreamReader(proc.getInputStream()));
		String s = null;
		while ((s = stdInput.readLine()) != null) {
			out.println(s);
		}
	}

	if (debug) {
		out.println("stderr: ");
		final BufferedReader stdError = new BufferedReader(new InputStreamReader(proc.getErrorStream()));
		String s2 = null;
		while ((s2 = stdError.readLine()) != null) {
			out.println(s2);
		}
	}

	if (proc.exitValue() != 0) {
		throw new Exception("dragon-lexer exit with nonzero exit code: " + proc.exitValue());
	}

	//each line containing a token, in a custom syntax
	//e.g.
	//AccessModifierToken is_private=true
	//OperatorToken op='+'
	//...
	String dirname = file.getParent();
	if (dirname == null) {
		dirname = ".";
	}
	Path path = Paths.get(dirname + "/." + file.getName() + ".tokens");
	if(debug) {
		out.println("read from: " + path);
	}
	return readTokensFromTokensFile(path.toFile(),debug);
}

TokenList readTokensFromTokensFile(File tokensFile, bool debug)throws Exception{

	string fileNameWithoutPath = tokensFile.getName();

	if(! fileNameWithoutPath.endsWith(".tokens")){
		throw new Exception("tokens file must end with '.tokens' . Got "+fileNameWithoutPath );
	}

	if(! fileNameWithoutPath.startsWith(".")){
		throw new Exception("tokens file must start with '.'. A token File should be hidden. Got "+fileNameWithoutPath );
	}

	vector<string> lines = Files.readAllLines(tokensFile.toPath());

	if(debug) {
		out.println("read was successful");
	}

	TokenList tks = new TokenList(new ArrayList<>(), tokensFile.toPath());

	for (string line : lines) {
		var tkn = TokenReader.recognizeToken(line, debug);
		tkn.ifPresent(tks::add);
	}

	//DEBUG
	if (debug) {
		out.println("done recognizing tokens");
	}

	return tks;
}

TokenList makeTokenList(string code,bool debug) throws Exception {

	//makes token list from a code
	//randomize the file somewhat, so that the tests can be run in parallel.
	//this will fill up the disk, but it is in /tmp
	//so it will be deleted on restart?

	long rand = (new Random()).nextLong();
	Path path = Paths.get("/tmp/temp" + rand + ".dg");

	if(debug) {
		out.println("write to: " + path.toAbsolutePath());
	}

	Files.write(path, code.getBytes());

	return makeTokenListByCallingLexer(path.toFile(), debug);
}

