node-swipl

### A node.js interface to the SWI-Prolog library

For now, this addon supports the creation of rules and facts 'on the fly' and querying.

The SWI-Prolog library requires the SWI_HOME_DIR environment variable to be set (See 
http://www.swi-prolog.org/FAQ/FindResources.html). Thus an example call could be:

SWI_HOME_DIR=/usr/lib/swi-prolog node tests/a.js

Missing SWI_HOME_DIR will result in

    [FATAL ERROR:
        Could not find system resources]

Now builds with node-gyp (Thanks to Johny Jose): 

    $ node-gyp configure
    $ SWI_HOME_DIR=/usr/lib/swi-prolog node-gyp build 

After building, try running:

    $ SWI_HOME_DIR=/usr/lib/swi-prolog NODE_PATH=build/Release node tests/a.js


If you like it, use and/or fork it. Corrections and improvements are welcome.

Basic usage
-----------

Initialization

    var swipl = require('swipl');
    swipl.initialise();

Create module

    var m = swipl.module("mymod");

Facts and rules (shortcut for m.call_predicate("assert", [ term ]) )

    m.assert("likes(romeo, julia).");
    {}

Querying - Single solution

	m.call_predicate("likes", ["romeo", "X"]);
	{ X: 'julia' }

Querying - Query

	m.assert("likes(john, julia).");
	{}
	var q = m.open_query("likes", ["X", "julia"]);
	undefined
	q.next_solution();
	{ X: 'romeo' }
	q.next_solution();
	{ X: 'john' }
	q.next_solution();
	false
	q.close();
	true

Cleanup

    swipl.cleanup();

