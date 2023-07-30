#N = 3
#Q = {0,cp,shift,what,haltandhaveafuck}
#F = {haltandhaveafuck}
#G = {0,1,_}
#S = {0,1}
#q0 = 0
#B = _

0 ___ ___ *** haltandhaveafuck
0 *__ *__ *** cp

cp 1__ 11_ rr* cp
cp 0__ 00_ rr* cp
cp ___ ___ ll* what

what *0_ *00 *l* shift
what *1_ *11 *l* shift
shift *1_ 11_ ll* shift
shift *1* 11* ll* shift
shift *0* 00* ll* shift
shift *0_ 00_ ll* shift
shift *_0 0_0 *** haltandhaveafuck
shift *_1 1_1 *** haltandhaveafuck
