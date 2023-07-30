; Binary addition - adds two binary numbers
; Input: two binary numbers, separated by a single #, eg '100+1110'

; the finite set of states
#Q = {0,1,2,3x,3y,4x,4y,5,6,7,8,9,halt}

; the finite set of input symbols
#S = {0,1,+}

; the complete set of tape symbols
#G = {0,1,_,+,x,y}

; the start state
#q0 = 8

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 1

8 _ _ * 0
8 0 0 r 8
8 1 1 r 8
8 + _ l 9
9 * * l 9
9 _ _ r 0

0 _ _ r 1
0 * * r 0
1 _ _ l 2
1 * * r 1
2 0 _ l 3x
2 1 _ l 3y
2 _ _ l 7
3x _ _ l 4x
3x * * l 3x
3y _ _ l 4y
3y * * l 3y
4x 0 x r 0
4x 1 y r 0
4x _ x r 0
4x * * l 4x    ; skip the x/y's
4y 0 1 * 5
4y 1 0 l 4y
4y _ 1 * 5
4y * * l 4y    ; skip the x/y's
5 x x l 6
5 y y l 6
5 _ _ l 6
5 * * r 5
6 0 x r 0
6 1 y r 0

7 x 0 l 7
7 y 1 l 7
7 _ _ r halt
7 * * l 7

