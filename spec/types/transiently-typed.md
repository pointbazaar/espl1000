when considering int, 
there could be the possibility of variables,
when not being explicitly typed, to be of some of these types for only a certain amount of time.

consider:

uint x= 1;
x=0;

in such statements, in the first case it is of type uint and has property ( != 0),
although only uint is declared.

we should probably allow it to be set to 0,
as mutable variables can be useful in some situations.

before the 2nd statement, with x being (nonzero integer),
there could be a division (z=10/x;) inserted.

so all this is to illustrate that primitive datatypes could have different types throughout their lifetime.

