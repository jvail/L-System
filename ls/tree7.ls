# Lparser example

recursion 11
angle 5
thickness 30
shape 1
switch_yz 1

axiom c(12)T

rule T = [F~(5)FSd]
rule S = ~(5)FRR[Ba]d>(30)~(5)FRR[Ba]d>(30)!(.8)S
rule R = [Ba]d>(120)

rule a = ~(10)$F[Cxd]~(10)$Fd!(.8)b
rule b = ~(10)$F[Dyd]~(10)$Fd!(.8)a
rule d = [g(3)g(.5)Lg(.5)Lg(.5)Lg(.5)Lg(.5)Lg(.5)L]

rule B = &B
rule C = +C
rule D = -D

rule x = a
rule y = b

rule F = '(1.25)F'(.8)

rule L = [~f(.5)c(8){+(30)f(2)-(120)f(2)-(120)f(2)}]
rule f = z
rule z = _

