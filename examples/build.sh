#this build script
#tests that all examples transpile

echo "Transpiling Example Programs"
echo "..."

sd booleans/Booleans.dg \
&& sd chars/Chars.dg \
&& sd cmd-args/CmdArgs.dg \
&& sd floats/Floats.dg \
&& sd for_atmel/LoopAdd1.dg \
&& sd for_atmel/Main.dg \
&& sd functional/Filter.dg \
&& sd ifstatement/IfStatement.dg \
&& sd input/Input.dg \
&& sd local_variables/Locals.dg \
&& sd local_variables/Locals2.dg \
&& sd loops/EmptyLoop.dg \
&& sd loops/WhileLoop.dg \
&& sd loops/Loop.dg \
&& sd mathematics/Faculty.dg \
&& sd mathematics/Fibonacci.dg \
&& sd methodCalls/MethodCall.dg \
&& sd other/Simple.dg \
&& sd structs/MyStructs.dg \
&& sd structs/Node.dg \
&& sd break/Break.dg

echo "done!"

# currently not working: functional/Twice.dg
