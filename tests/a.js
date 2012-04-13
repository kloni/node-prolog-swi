var swipl = require('../swipl');
swipl.initialise();

var m = swipl.module("myown");
var m2 = swipl.module("mo2");

var r, s, q;

m.term_type("1");
m.term_type("'a'");
m.term_type("\"b\"");
m.term_type("Var");
m.term_type("add(1, 2).");
m.term_type("[1, A, bla(me)]");
m.term_type("pred(Var).");
m.term_type("2/7");
m.term_type("calc(Atom) :- term_to_atom(Expr, Atom), A is Expr, write(A), nl.");

console.log(":- calc(Atom, Result) :- term_to_atom(Expr, Atom), Result is Expr.");
r = m.assert("calc(Atom, Result) :- term_to_atom(Expr, Atom), Result is Expr.");
console.log("calc('1+2', Result).");
r = m.call_predicate("calc", ["'1+2'", "Result"]);
console.log(r);

console.log("calc('1+2', Result).");
q = m.open_query("calc", ["'1+2'", "Result"]);
console.log(q.next_solution());
console.log(q.exception());
q.close();

console.log(":- add(1, 2, 3).");
m.assert("add(1, 2, 3).");
console.log(":- add(1, 2, 4).");
m.assert("add(1, 2, 4).");
console.log("add(1, 2, C).");
r = m.call_predicate("add", [1, 2, "C"]);
console.log(r);
try {
	r = m2.call_predicate("add", [1, 2]);
	console.log("THIS SHOULD HAVE FAILED!");
} catch(exc) {
	console.log("expected: " + exc);
}

console.log("add(1, 2, C).");
q = m.open_query("add", [1, 2, "C"]);
r = q.next_solution();
while( r ) {
	console.log(r);
	r = q.next_solution();
}
q.close();
try {
	q.next_solution();
	console.log("THIS SHOULD HAVE FAILED!");
} catch(exc) {
	console.log("expected: " + exc);
}

swipl.cleanup();
