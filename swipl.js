/*  $Id$

    node.js addon for SWI-Prolog

    Author:        Tom Klonikowski
    E-mail:        klonik_t@informatik.haw-hamburg.de
    Copyright (C): 2012, Tom Klonikowski

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

var swipl = require("./build/Release/libswipl");

function Module(mod) {
	this.module_name = mod;
}

Module.prototype = {
	module : function(mod) {
		return new Module(mod);
	},

	term_type : function(term) {
		var r = swipl.term_type(term);
		if(this.DEBUG) console.log("[libswipl] term_type: " + r + " " + term);
		return r;
	},

	open_query : function(predicate, args) {
		return new swipl.Query(predicate, args, this.module_name);
	},

	call_predicate : function(name, args) {
		var query = new swipl.Query(name, args, this.module_name);
		var result = false;
		var r = query.next_solution();
		if (r) {
			result = r;
			if(this.DEBUG) console.log("[libswipl] call_predicate: "
					+ JSON.stringify(r));
		} else {
			var ex = query.exception();
			if (ex) {
				throw new Error(ex.exc);
			} else {
				if(this.DEBUG) console.log("[libswipl] call_predicate: " + r);
			}
		}
		query.close();
		return result;
	},

	assert : function(term) {
		if(this.DEBUG) console.log("[libswipl] assert: " + term);
		return this.call_predicate("assert", [ term ]);
	}
};

exports.module = function(mod) {
	return new Module(mod);
};

exports.initialise = function(prog) {
	if (!prog) {
		prog = 'node';
	}
	return swipl.initialise(prog);
};

exports.cleanup = function() {
	return swipl.cleanup();
};
