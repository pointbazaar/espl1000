when considering PInt, NInt, NZInt,
there could be the possibility of variables,
when not being explicitly typed, to be of some of these types for only a certain amount of time.

consider:

PInt x= 1;
x=0;

in such statements, in the first case it is of type PInt and of type NZInt,
although only NZInt is declared.
we should probably allow it to be set to 0,
as mutable variables can be useful in some situations.

before the 2nd statement, with x being NZInt (nonzero integer),
there could be a division (z=10/x;) inserted.

so all this is to illustrate that primitive datatypes could have different types throughout their lifetime.

consider:

Integer x=10;
x=x*(-1);
subr(x);

(NInt y)~>PInt subr{return 0;}

x also has type PInt, as it is positive.
later on, x also has type NInt,
and could be passed into subr. It would be convenient
if the compiler could detect such things. it would
simplify placing additional restrictions on the primitive datatypes
used in dragon programs.
