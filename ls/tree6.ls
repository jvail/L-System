# Lparser example file

recursion 10
angle 60
thickness 20
shape 2
switch_yz 1

axiom c(15)T

rule T = CCA
rule A = CBD>(94)CBD>(132)BD
rule B = [&CDCD$A]
rule D = [g(1)Lg(1)Lg(1)Lg(1)Lg(1)Lg(1)L]
  
rule C = !(.95)~(5)tF

rule F = '(1.25)F'(.8)

rule L = [~f(2)c(8){+(30)f(2)-(120)f(2)-(120)f(2)}]
rule f = z
rule z = _

