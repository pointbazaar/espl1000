#ifndef HALTS_H
#define HALTS_H

/*
 * for some subroutines it is desired that they
 * never terminate, except when the process is killed
 * externally (Ctrl-C), SIGKILL, ...
 * That would be the case for a web server main loop
 * in some cases.
 *
 * for other subroutines, it is desired that they will
 * terminate in all cases.
 *
 * for some subroutines, we cannot know their
 * halting behavior
 *
 * for other subroutines their halting behavior
 * depends on user input.
 *
 * This enum is designed to be able to represent those
 * c_types_util of (non-)termination behavior.
 */

enum HALTS {

	HALTS_ALWAYS,
	HALTS_NEVER,
	HALTS_UNKNOWN
};

#endif
