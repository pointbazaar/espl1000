
//standard headers
#include <vector>
#include <set>

//project headers
#include "SubroutineTypeNode.hpp"

public final class SubroutineTypeNode implements IBasicAndWrappedTypeNode  {

	public final TypeNode returnType;

	public final boolean hasSideEffects;

	public final List<TypeNode> argumentTypes = new ArrayList<>();

	public SubroutineTypeNode(final TypeNode return_type, final boolean hasSideEffects) {
		this.returnType = return_type;
		this.hasSideEffects = hasSideEffects;
	}

	public SubroutineTypeNode(final TokenList tokens) throws Exception {

		final TokenList copy = tokens.copy();

		copy.expectAndConsumeOtherWiseThrowException(new LParensToken());

		boolean sucess_argument_types = true;
		try {
			this.argumentTypes.add(new TypeNode(copy));
		} catch (Exception e) {
			sucess_argument_types = false;
		}
		while (sucess_argument_types) {
			try {
				TokenList copy2 = copy.copy();

				copy2.expectAndConsumeOtherWiseThrowException(new CommaToken());
				this.argumentTypes.add(new TypeNode(copy2));

				copy.set(copy2);
			} catch (Exception e) {
				sucess_argument_types = false;
			}
		}

		copy.expectAndConsumeOtherWiseThrowException(new RParensToken());

		if (copy.head() instanceof ArrowToken) {
			ArrowToken arrow = (ArrowToken) copy.head();
			this.hasSideEffects = !arrow.is_functional;
			copy.consume(1);
		} else {
			throw new Exception("expected an arrow token here");
		}

		this.returnType = new TypeNode(copy);

		tokens.set(copy);
	}

}
