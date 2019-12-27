package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.parsing.IASTNode;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class NamespaceNode implements IASTNode {

	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	public final Path srcPath;

	public String name;

	//structs must be declared before the subroutines
	public final List<StructDeclNode> structs = new ArrayList<>();

	public final List<MethodNode> methods = new ArrayList<>();

	public NamespaceNode(final String name){
		this.name=name;
		this.srcPath = Paths.get("/dev/null");
	}

	public NamespaceNode(final MethodNode method, final String namespacename) {
		//utility method, to create a NamespaceNode from a single method
		this.name = namespacename;
		this.srcPath = Paths.get("/dev/null");
		this.methods.add(method);
	}



	@Override
	public String toSourceCode() {
		String result = "";

		result += methods
				.stream()
				.map(node -> node.toSourceCode())
				.collect(Collectors.joining("\n"));

		return result;
	}
}
