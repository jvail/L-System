# Lparser example file
# using spirals to make a boat shape

recursion 20
angle 10
thickness 75
min_thickness 7
shape 1         

axiom &(90)[c(2)s][%|[&(90)Be][^(90)Be]rs]

rule s = F[[&(90)Ae][^(90)Ae]]+'!se
rule A = +''!c(3)FpA

rule b = F[[&(90)Be][^(90)Be]]
rule B = +''!c(3)FpB

rule e = [c(6)FF]
rule r = bbbbb
rule p = [c(4)[&(90)F][^(90)F]]

