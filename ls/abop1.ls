# Lparser example file

# Monopodial skeleton
#
# Here is how to read the formulas on page 56 into
# lparser syntax. The values can be input from page 57,
# the strings colapsed and used.
#
# A = F[ &( a0 ) '( r2 ) !( wr ) BL  ] >( c ) '( r1 ) !( wr ) A
# B = F[ -( a2 ) '( r2 ) !( wr ) $CL ]        '( r1 ) !( wr ) C
# C = F[ +( a2 ) '( r2 ) !( wr ) $BL ]        '( r1 ) !( wr ) B

recursion 10
angle 45
thickness 15
shape 1
switch_yz 1

axiom c(12)FFAL

rule A = F[&'(.8)!BL]>(137)'!(.9)A
rule B = F[-'(.8)!(.9)$CL]'!(.9)C
rule C = F[+'(.8)!(.9)$BL]'!(.9)B

rule L = ~c(8){+(30)f-(120)f-(120)f}

