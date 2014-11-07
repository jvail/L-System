# Lparser example file

recursion 5
angle 10
thickness 100
min_thickness 5
shape 1
switch_yz 1

axiom c(12)FFFFFFFFFF>(1)&(1)A

rule A = !(.75)t(.9)FB>(94)B>(132)B
rule B = [&"t(.9)!(.75)FL[|zL]$AL[|zL]]

rule L = [~c(8)f(.8)|{+(30)f(1)-(120)f(1)-(120)f(1)}]

