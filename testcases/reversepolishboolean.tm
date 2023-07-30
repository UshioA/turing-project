; This is a simple stack language interpreter.
; It evaluates Boolean expressions entered in reverse Polish notation.
; Input: a Boolean expression in reverse Polish.
; Use 1,0,&,|,~ for true, false, and, or not, respectively, without spaces.
; Example inputs:
;   11&    - returns 1  (= 1 and 1)
;   01~1|& - returns 0    (= (not 1 or 1) and 0)

; the finite set of states
#Q = {0,1,2,100,101,102,110,111,112,120,121,122,123,124,127,130,131,132,133,134,137,140,141,142,147,halt}

; the finite set of input symbols
#S = {!,&,0,1,x,|,~}

; the complete set of tape symbols
#G = {!,&,0,1,_,x,|,~}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 1

0 _ _ r 0
0 * * * 1
1 x x r 1
1 0 x l 100
1 1 x l 110
1 & x l 120
1 | x l 130
1 ~ x l 140
1 _ _ l 2
2 x _ l 2
2 _ _ l halt
100 x x l 100
100 _ _ l 101
101 _ 0 r 102
101 * * l 101
102 _ _ r 0
102 * * r 102
110 x x l 110
110 _ _ l 111
111 _ 1 r 112
111 * * l 111
112 _ _ r 0
112 * * r 112
120 x x l 120
120 _ _ l 121
121 _ _ r 122
121 * * l 121
122 0 _ r 123
122 1 _ r 124
122 _ ! * halt
123 0 0 r 127
123 1 0 r 127
124 0 0 r 127
124 1 1 r 127
124 * ! * halt
124 _ ! * halt
127 _ _ r 0
127 * * r 127
130 x x l 130
130 _ _ l 131
131 _ _ r 132
131 * * l 131
132 0 _ r 133
132 1 _ r 134
132 _ ! * halt
133 0 0 r 137
133 1 1 r 137
133 _ ! * halt
134 0 1 r 137
134 1 1 r 137
134 _ ! * halt
137 _ _ r 0
137 * * r 137
140 x x l 140
140 _ _ l 141
141 _ _ r 142
141 * * l 141
142 0 1 r 147
142 1 0 r 147
142 _ ! * halt
147 _ _ r 0
147 * * r 147