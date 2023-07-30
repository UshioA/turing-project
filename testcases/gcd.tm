; Copyright 2021 StardustDL
; Calculate gcd(a,b)

; the finite set of states
#Q = {0,pre1,pre2,pback0,pback1,pback2,sback0,sback1,sback2,small,test1,testb1,test2,testb2,fail,final,minus}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {final}

; the number of tapes
#N = 3

; the transition functions

0 1__ 1__ *** pre1
0 1_* 1_* *** pre1
0 1__ 1__ *** pre1
0 1*_ 1*_ *** pre1
0 1** 1** *** pre1

; pre: split two integer a,b to tape 1,2

pre1 1__ 11_ rr* pre1
pre1 1_* 11* rr* pre1
pre1 1__ 11_ rr* pre1
pre1 1*_ 11_ rr* pre1
pre1 1** 11* rr* pre1
pre1 0__ 1__ rl* pre2
pre1 0_* 1_* rl* pre2
pre1 0__ 1__ rl* pre2
pre1 0*_ 1*_ rl* pre2
pre1 0** 1** rl* pre2
pre2 1__ 1_1 r*r pre2
pre2 1_* 1_1 r*r pre2
pre2 1__ 1_1 r*r pre2
pre2 1*_ 1*1 r*r pre2
pre2 1** 1*1 r*r pre2
pre2 ___ ___ l*l pback0
pre2 __* __* l*l pback0
pre2 ___ ___ l*l pback0
pre2 _*_ _*_ l*l pback0
pre2 _** _** l*l pback0
pre2 ___ ___ l*l pback0
pre2 __* __* l*l pback0
pre2 ___ ___ l*l pback0
pre2 *__ *__ l*l pback0
pre2 *_* *_* l*l pback0
pre2 ___ ___ l*l pback0
pre2 _*_ _*_ l*l pback0
pre2 ___ ___ l*l pback0
pre2 *__ *__ l*l pback0
pre2 **_ **_ l*l pback0
pre2 *** *** l*l pback0

; back: back to position 0
pback0 1__ ___ l** pback0
pback0 1_* __* l** pback0
pback0 1__ ___ l** pback0
pback0 1*_ _*_ l** pback0
pback0 1** _** l** pback0
pback0 ___ ___ r** pback1
pback0 __* __* r** pback1
pback0 ___ ___ r** pback1
pback0 _*_ _*_ r** pback1
pback0 _** _** r** pback1
pback1 _1_ _1_ *l* pback1
pback1 _1* _1* *l* pback1
pback1 _1_ _1_ *l* pback1
pback1 *1_ *1_ *l* pback1
pback1 *1* *1* *l* pback1
pback1 ___ ___ *r* pback2
pback1 __* __* *r* pback2
pback1 ___ ___ *r* pback2
pback1 *__ *__ *r* pback2
pback1 *_* *_* *r* pback2
pback2 __1 __1 **l pback2
pback2 _*1 _*1 **l pback2
pback2 __1 __1 **l pback2
pback2 *_1 *_1 **l pback2
pback2 **1 **1 **l pback2
pback2 ___ ___ **r small
pback2 _*_ _*_ **r small
pback2 ___ ___ **r small
pback2 *__ *__ **r small
pback2 **_ **_ **r small

sback0 1__ 1__ l** sback0
sback0 1_* 1_* l** sback0
sback0 1__ 1__ l** sback0
sback0 1*_ 1*_ l** sback0
sback0 1** 1** l** sback0
sback0 ___ ___ r** sback1
sback0 __* __* r** sback1
sback0 ___ ___ r** sback1
sback0 _*_ _*_ r** sback1
sback0 _** _** r** sback1
sback1 _1_ _1_ *l* sback1
sback1 _1* _1* *l* sback1
sback1 _1_ _1_ *l* sback1
sback1 *1_ *1_ *l* sback1
sback1 *1* *1* *l* sback1
sback1 ___ ___ *r* sback2
sback1 __* __* *r* sback2
sback1 ___ ___ *r* sback2
sback1 *__ *__ *r* sback2
sback1 *_* *_* *r* sback2
sback2 __1 __1 **l sback2
sback2 _*1 _*1 **l sback2
sback2 __1 __1 **l sback2
sback2 *_1 *_1 **l sback2
sback2 **1 **1 **l sback2
sback2 ___ ___ **r test1
sback2 _*_ _*_ **r test1
sback2 ___ ___ **r test1
sback2 *__ *__ **r test1
sback2 **_ **_ **r test1

; small: move min(a,b) to tape 0
small _11 111 rrr small
small *11 111 rrr small
small _1_ _1_ lll sback0
small *1_ *1_ lll sback0
small __1 __1 lll sback0
small *_1 *_1 lll sback0

; test: test d is divisor of a and b
test1 11_ 11_ rr* test1
test1 11* 11* rr* test1
test1 _1_ _1_ l** testb1
test1 _1* _1* l** testb1
test1 1__ 1__ *l* fail
test1 1_* 1_* *l* fail
test1 ___ ___ ll* testb2
test1 __* __* ll* testb2

test2 1_1 1_1 r*r test2
test2 1*1 1*1 r*r test2
test2 __1 __1 l** testb2
test2 _*1 _*1 l** testb2
test2 1__ 1__ **l fail
test2 1*_ 1*_ **l fail
test2 ___ ___ l*l final
test2 _*_ _*_ l*l final

testb1 1__ 1__ l** testb1
testb1 1_* 1_* l** testb1
testb1 1__ 1__ l** testb1
testb1 1*_ 1*_ l** testb1
testb1 1** 1** l** testb1
testb1 ___ ___ r** test1
testb1 __* __* r** test1
testb1 ___ ___ r** test1
testb1 _*_ _*_ r** test1
testb1 _** _** r** test1
testb2 1__ 1__ l** testb2
testb2 1_* 1_* l** testb2
testb2 1__ 1__ l** testb2
testb2 1*_ 1*_ l** testb2
testb2 1** 1** l** testb2
testb2 ___ ___ r** test2
testb2 __* __* r** test2
testb2 ___ ___ r** test2
testb2 _*_ _*_ r** test2
testb2 _** _** r** test2

; fail: test fail, decrease d and test again
fail 1__ 1__ r** fail
fail 1_* 1_* r** fail
fail 1__ 1__ r** fail
fail 1*_ 1*_ r** fail
fail 1** 1** r** fail
fail ___ ___ l** minus
fail __* __* l** minus
fail ___ ___ l** minus
fail _*_ _*_ l** minus
fail _** _** l** minus
minus 1__ ___ l** sback0
minus 1_* __* l** sback0
minus 1__ ___ l** sback0
minus 1*_ _*_ l** sback0
minus 1** _** l** sback0
