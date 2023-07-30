#Q = {cp,0,homesweethome,erasel,eraser,incresel,increser,acceptandhaveafuck,die,true,true1,true2,true3,false,false0,false1,false2,false3}
#F = {acceptandhaveafuck}
#G = {1,_,t,r,u,e,f,a,l,s,e}
#B = _
#N = 3
#q0 = 0
#S = {1}

0 ___ ___ *** true
0 *__ *__ *** cp
cp 1__ 11_ rr* cp
cp ___ ___ ll* homesweethome
homesweethome **_ _*_ ll* homesweethome
homesweethome ___ __1 rr* erasel 
erasel _11 __1 *rr erasel
erasel __1 __1 *** false
erasel _1_ _11 **r incresel
erasel ___ ___ *** true
incresel _1_ _11 *** eraser
eraser _11 __1 *rl eraser
eraser __1 __1 *** false
eraser _1_ _11 **l increser
eraser ___ ___ *** true
increser _1_ _11 *** erasel

true ___ t__ r** true1
true1 ___ r__ r** true2
true2 ___ u__ r** true3
true3 ___ e__ r** acceptandhaveafuck

false __1 f_1 r** false0
false0 __1 a_1 r** false1
false1 __1 l_1 r** false2
false2 __1 s_1 r** false3
false3 __1 e_1 r** die