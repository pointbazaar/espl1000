
//standard headers
#include <vector>
#include <set>

//project headers
#include "AST_Whole_Program.hpp"

public final class AST_Whole_Program implements IASTNode {

	//this contains all namespace nodes for the whole program

	public final Set<NamespaceNode> namespaceNodes = new HashSet<>();

	public AST_Whole_Program() {
	}

	public AST_Whole_Program(final NamespaceNode myNamespace) {
		//utility method, to facilitate creating AST_Whole_Program from a single namespace node
		this.namespaceNodes.add(myNamespace);
	}

	public AST_Whole_Program(final TokenList tokens, final String namespace, boolean debug) throws Exception {

		//use this constructor as a shortcut to create an AST
		//if there is just a single namespace

		if (debug) {
			System.out.println("parse AST_Whole_Program");
		}

		this.namespaceNodes.add(new NamespaceNode(tokens, namespace, debug));
	}

}
