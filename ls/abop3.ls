# Lparser example file

# Ternary skeleton from ABOP on page 60

recursion 13
angle 30
thickness 20
min_thickness 7
shape 1
switch_yz 1

axiom c(12)FA

rule A = !(.9)t(.4)FB>(94)B>(132)B
rule B = [&t(.4)F$A]
rule F = '(1.25)F'(.8)

