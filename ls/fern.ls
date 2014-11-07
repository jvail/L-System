# Lparser example file

color 1     90 138 32
color 2     90 164 33
color 3     89 212 66
color 4     87 229 57
color 5     72 238 53
color 6     58 247 49
color 7     75 248 67
color 8     92 249 86
color 9     97 249 90
color 10    101 249 95
color 11    105 250 99
color 12    110 250 104
color 13    118 251 114
color 14    127 252 124
color 15    127 252 125
color 16    128 253 126
color 17    128 254 127
color 18    128 254 127
color 19    158 254 158
color 20    158 254 158

recursion 15
angle 20                                   
thickness 15
min_thickness 5
shape 1
switch_yz 1
no_wait 1

axiom c(1)b>(60)b>(60)b>(60)b>(60)b>(60)b    

rule b = [&(30)A]

rule A = ~(7)$t(.1)F[+(40)C][-(40)C]!(.95)~(7)t(.1)FcA
rule C = ~(10)$tF[+(60)L][-(60)L]C

rule L = [~(15){-f+f+f-|-f+f+f}]

rule F = '(1.3)F'(.77)
rule f = '(1.3)f'(.77)

#rule ~ = _             # uncomment to remove random efx

