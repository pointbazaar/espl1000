
//standard headers
#include <vector>
#include <set>

//project headers
#include "NamespaceNode.hpp"

public final class NamespaceNode implements IASTNode {

	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	public final Path srcPath;

	public final String name;

	//structs must be declared before the subroutines
	public final List<StructDeclNode> structs = new ArrayList<>();

	public final List<MethodNode> methods = new ArrayList<>();

	public NamespaceNode(
			final TokenList tokens,
			final String namespace,
			final boolean debug
	) throws Exception {

		if (debug) {
			System.out.println("try to parse " + this.getClass().getSimpleName() + " from " + tokens.toSourceCodeFragment());
		}

		this.srcPath = Paths.get("/dev/null");
		this.name = namespace;
		final TokenList copy = tokens.copy();

		if(copy.size()>0) {

			IToken next_struct = copy.get(0);

			while (next_struct instanceof StructToken) {
				this.structs.add(new StructDeclNode(copy, debug));

				if (copy.size() > 0) {
					next_struct = copy.get(0);
				} else {
					break;
				}
			}
		}

		//it is be nice to have prefix 'fn' before a function
		//to make parsing easier.
		//this does not add much boilerplate to the syntax
		//and would probably make the parser faster
		if (copy.size() > 0) {

			IToken next_subr = copy.get(0);

			while (next_subr instanceof FnToken) {
				this.methods.add(new MethodNode(copy, debug));

				if (copy.size() > 0) {
					next_subr = copy.get(0);
				} else {
					break;
				}
			}
		}

		tokens.set(copy);
	}

}
