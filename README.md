This is the README file for MAPIROPS compiler


About MAPIROPS
==============

mapirops is a compiler that builds a stack designed to pack and unpack
MAPI operations (Rops) and MAPI content. This technology is the core
of OpenChange protocols implementation and is a candidate to the
replacement of the existing MAPI buffering implementation relying on
Samba. The mapirops compiler aims at providing a similar approach but
with different outcomes.

First it makes use of its own description language while implementing
a grammar similar to Samba IDL files and in particular to the existing
OpenChange MAPI IDL. Secondly, it implements a specific set of
(compatible) keywords and primitive types chosen for this OpenChange
language offering an easy migration path and reduces the overall
effort.

Finally the compiler is responsible for generating code that
automatically builds and processes MAPI buffers. The immediate benefit
in having MAPI content as a buffer is the introduction of a semantic
and physical distinction between content and transport. This offers an
immediate solution working around existing NDR limitations with MAPI,
such as:

- deep recursion issues
- MAPI is not NDR encoded
- stateless and context agnostic
- string encoding depending on context (and specified codepage)

Note that many of the mapirops library files responsible for
pushing/pulling content are derivated from existing Samba dcerpc client
library code, but current headers do not acknowledge that.  This will be
fixed in forthcoming commits!


Requirements
============

* check
* python-pyparsing
* libpopt-dev
* libtalloc-dev


Structure
=========

- bin/		This directory contains embedded waf
- build/	This directory contains the auto-generated source and object files
- build/mr/	This directory contains C files auto-generated from *.mr IDL files
- mr/		This directory contains all mapirops IDL files
- mapirops/	This directory contains the mapirops compiler

mapirops/mapirops/MAPIRopsParser.py is the file where the BNF is implemented.


Build Instructions
==================

        ./bin/waf configure build


Run Testsuite
=============

        export LD_LIBRARY_PATH=./build/lib
        ./build/mapirops_testsuite


Testing the compiler
====================

        ./mapirops/mapirops.py --file=mr/oxcstor.mr --outputdir=/tmp --mapi-gen
        Generating /tmp/oxcstor.h
        Generating /tmp/oxcstor.c

        less /tmp/oxcstor.{c,h}

When run with the `--debug` option, mapirops will display the content of
the .mr file after it is processed and parsed.

