# Lparser example file 

recursion 20
angle 10
thickness 80
min_thickness 5
shape 1
switch_yz 1

axiom [P]|[P]

rule P = [A]>>>>>>>>>[cB]>>>>>>>>>[ccC]>>>>>>>>>[cccD]
rule A = F+;'A
rule B = F!+F+;'B
rule C = F!^+F^+;'C
rule D = F!>^+F>^+;'D


