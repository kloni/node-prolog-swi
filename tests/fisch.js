// Beispiel von http://de.wikipedia.org/wiki/Prolog_%28Programmiersprache%29

var swipl = require('../swipl');
swipl.initialise();

var m = swipl.module("fisch");

m.assert("erstes(E,[E|_]).");
m.assert("mittleres(M,[_,_,M,_,_]).");
m.assert("links(A,B,[A,B|_]).");
m.assert("links(A,B,[_|R]) :- links(A,B,R).");
m.assert("neben(A,B,L) :- links(A,B,L);links(B,A,L).");

m.assert("ask(N) :-"+
  "X = [_,_,_,_,_],"+                                /* Es gibt (nebeneinander) 5 (noch unbekannte) Häuser */
  "member([rot,brite,_,_,_],X),"+                    /* Der Brite lebt im roten Haus */
  "member([_,schwede,_,_,hund],X),"+                 /* Der Schwede hält einen Hund */
  "member([_,daene,tee,_,_],X),"+                    /* Der Däne trinkt gern Tee */
  "links([gruen,_,_,_,_],[weiss,_,_,_,_],X),"+       /* Das grüne Haus steht links vom weißen Haus */
  "member([gruen,_,kaffee,_,_],X),"+                 /* Der Besitzer des grünen Hauses trinkt Kaffee */
  "member([_,_,_,pallmall,vogel],X),"+               /* Die Person, die Pall Mall raucht, hält einen Vogel */
  "mittleres([_,_,milch,_,_],X),"+                   /* Der Mann, der im mittleren Haus wohnt, trinkt Milch */
  "member([gelb,_,_,dunhill,_],X),"+                 /* Der Besitzer des gelben Hauses raucht Dunhill */
  "erstes([_,norweger,_,_,_],X),"+                   /* Der Norweger wohnt im 1. Haus */
  "neben([_,_,_,marlboro,_],[_,_,_,_,katze],X),"+    /* Der Marlboro-Raucher wohnt neben dem, der eine Katze hält */
  "neben([_,_,_,_,pferd],[_,_,_,dunhill,_],X),"+     /* Der Mann, der ein Pferd hält, wohnt neben dem, der Dunhill raucht */
  "member([_,_,bier,winfield,_],X),"+                /* Der Winfield-Raucher trinkt gern Bier */
  "neben([_,norweger,_,_,_],[blau,_,_,_,_],X),"+     /* Der Norweger wohnt neben dem blauen Haus */
  "member([_,deutsche,_,rothmans,_],X),"+            /* Der Deutsche raucht Rothmans */
  "neben([_,_,_,marlboro,_],[_,_,wasser,_,_],X),"+   /* Der Marlboro-Raucher hat einen Nachbarn, der Wasser trinkt */
  "member([_,N,_,_,fisch],X).");                      /* Der mit der Nationalität N hat einen Fisch */

console.log("Der " + m.call_predicate("ask", ["R"]).R + " hat einen Fisch.");

swipl.cleanup();
