# Lparser example file

recursion 8
angle 30
thickness 40
min_thickness 4
shape 1
switch_yz 1

axiom c(12)A

rule A = aaSaaSaaSaaSaaSaaSaaSaaSaaSaaB
rule S = '(.9)!(.9)

rule a = tF[&'(.8)!LBL|zL]>(137)[z&'(.7)!LBL|zL]>(137)

rule B = tFL[-'(.8)!(.9)$LCL|zL]'(.9)!(.9)C
rule C = tFL[+'(.8)!(.9)$LBL|zL]'(.9)!(.9)B

rule L = [~c(8){+(30)f(.4)-(120)f(.4)-(120)f(.4)}]
