These files comprise the basic building blocks for a Tcl Extension
Architecture (TEA) extension.  For more information on TEA see:

	http://www.tcl.tk/doc/tea/

This package is part of the Tcl project at SourceForge, but sources
and bug/patch database are hosted on fossil here:

	https://core.tcl-lang.org/tclconfig

This package is a freely available open source package.  You can do
virtually anything you like with it, such as modifying it, redistributing
it, and selling it either in whole or in part.

CONTENTS
========
The following is a short description of the files you will find in
the sample extension.

README.txt	This file

install-sh	Program used for copying binaries and script files
		to their install locations.

tcl.m4		Collection of Tcl autoconf macros.  Included by a package's
		aclocal.m4 to define TEA_* macros.
