var swipl = require('../swipl');

swipl.initialise();

var m = swipl.module("org");

m.assert("fuz(hit, 1.0).");
m.assert("fuz(unknown, 0.9).");
m.assert("fuz(miss, 0.1).");
m.assert("unknown(O, P) :- property(O, _, _), property(_, P, _), not(property(O, P, _)).");
m.assert("fitp(O, P, L) :- property(O, P, V), member(V, L).");
m.assert("fitp(O, P, _) :- unknown(O, P).");
m.assert("fits(O, [P,V|T]) :- fitp(O, P, V), fits(O, T).");
m.assert("fits(O, []) :- property(O, _, _).");
m.assert("fuzzyval(O, P, L, F) :- property(O, P, V), member(V, L), fuz(hit, F), !.");
m.assert("fuzzyval(O, P, _, F) :- unknown(O, P), fuz(unknown, F), !.");
m.assert("fuzzyval(_, _, _, F) :- fuz(miss, F).");
m.assert("fuzzyfits(O, F, [P,L|T]) :- fuzzyfits(O, F1, T), fuzzyval(O, P, L, F2), mul(F1, F2, F).");
m.assert("fuzzyfits(O, F, []) :- property(O, _, _), fuz(hit, F).");


m.assert("property(o1, p1, p1v1).");
m.assert("property(o1, p1, p1v2).");
m.assert("property(o2, p1, p1v2).");
m.assert("property(o1, p2, p2v1).");
m.assert("property(o2, p2, p2v2).");
m.assert("property(o3, p2, p2v2).");

console.log("property(o2, P, V).");
var q = m.open_query("property", ["o2", "P", "V"]);
var s = q.next_solution();
while(s) {
	console.log(s);
	s = q.next_solution();
}
q.close();

console.log("fits(O, [p1, [p1v1], p2, [p2v1, p2v2]]).");
q = m.open_query("fits", ["O", "[p1, [p1v1], p2, [p2v1, p2v2]]"]);
var s = q.next_solution();
while(s) {
	console.log(s);
	s = q.next_solution();
}
q.close();

swipl.cleanup();
