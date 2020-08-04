#include <stdint.h>
#include <stdbool.h>

#ifndef TOKENS
#define TOKENS


struct Token{
	uint32_t kind;
	char* value;
	bool statically_allocated;
};

#endif

//https://de.wikipedia.org/wiki/Zugriffsmodifikator

	/*
		private  : can only use it within the same namespace
		public   : can use it everywhere
		
		shareup  : can use it in all files which are above in the directory tree
		sharedown: can use it in all files which are below in the directory tree 
		sharedir : can use it within the directory where it has been defined
		
		//comments
		private and public are old classics
		
		shareup is intended for utilities and 'lower' subroutines, 
		which are important for the part/module
		of the program which they are in, but are not supposed to call each other.
		"only their master can control them".
		these are your servant subroutines
		
		sharedown is intended for subroutines which need to be accessible in every part of 			a subdirectory of your source tree. for example an exception handling 
		subroutine which is very important to your networking subroutines.
		but it does not need to be visible from above or by its peers in the directory tree
		
		sharedir is intended for utilities which are supposed to be only used by subroutines
		declared in files of the same directory. in software you often have 
		classes / subroutines which have the same pattern, and instead of stuffing
		them all in one file and making the utility subroutines private, which can 
		lead to huge files, you can use 'sharedir' to share a subroutine within the same 
		directory.
	*/
