# Lparser example file
# creating a trail of trees

recursion 13
angle 20
thickness 50
min_thickness 7
shape 1
switch_yz 1

axiom c(12)&(90)H

rule H = g(20)TH 

rule T = [^(90)CCA]
rule A = CBD>(94)CBD>(132)BD
rule B = [&CDCD$A]
rule D = [gLgLgLgLgLgL]

rule C = !(.95)~(5)tF
rule F = '(1.25)F'(.8)

rule L = [~f(2)c(8){+(30)f(2)-(120)f(2)-(120)f(2)}]
rule f = z
rule z = _

