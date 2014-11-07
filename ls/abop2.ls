# Lparser example file

# Elaboration of example d on page 56
# with double the amount of branches

recursion 10
angle 30
thickness 20
shape 1
switch_yz 1

axiom c(12)FAL

rule A = F[&'(.7)!BL]>(137)[&'(.6)!BL]>(137)'(.9)!(.9)A
rule B = F[-'(.7)!(.9)$CL]'(.9)!(.9)C
rule C = F[+'(.7)!(.9)$BL]'(.9)!(.9)B

rule L = ~c(8){+(45)f(.1)-(45)f(.1)-(45)f(.1)+(45)|+(45)f(.1)-(45)f(.1)-(45)f(.1)}

