# Lparser example file

recursion 10
angle 30
thickness 40
shape 1
switch_yz 1

axiom c(12)A

rule A = aaSaaSA
rule S = '(.8)!(.9)

rule a = tF[&'(.8)!LBL]>(137)[z&'(.7)!LBL]>(137)
rule B = tF[-'(.8)!(.9)$LCL]'!(.9)C
rule C = tF[+'(.8)!(.9)$LBL]'!(.9)B

rule L = [~c(8){+(30)f(.2)-(120)f(.2)-(120)f(.2)}]

