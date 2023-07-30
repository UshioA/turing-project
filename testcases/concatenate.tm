; Write your Turing machine program here!
; Syntax: <current state> <current symbol> <new symbol> <direction> <new state>
; ';' starts a comment.
; '*' is a wildcard: it matches any symbol/state when used in the current symbol/state field;
; it means 'same as current' when used in new symbol/new state field.
; '_' represents the blank (space) symbol.
; Symbols must be a single non-whitespace character except ';'.
; States can be any word, not only numbers.
;
; This example program concatenates the first string of '1's to the end of the second.

; the finite set of states
#Q = {0,1,2,3,4,5,6,halt}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_,x}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 1

0 0 0 r 0
0 1 x r 1
1 1 1 r 1
1 0 0 r 2
2 0 0 r 2
2 1 1 r 3
3 1 1 r 3
3 0 1 l 4
3 _ 1 l 4
4 1 1 l 4
4 0 0 l 5
5 0 0 l 5
5 1 1 l 6
5 x x * halt
6 1 1 l 6
6 x x r 0
6 0 0 r 0
