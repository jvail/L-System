# Lparser example file

recursion 12
angle 5
thickness 30
shape 1
switch_yz 1

axiom c(4)FFS                                          # the actual tree

rule S = FFR>(60)R>(60)R>(60)R>(60)R>(60)R>(30)S       # six branches per segment
rule R = [Ba]                                          # a single branch

rule a = $tF[Cx]Fb                                    
rule b = $tF[Dy]Fa

rule B = &B                                            # angle increments
rule C = +C
rule D = -D

rule x = a                                             # apical elay
rule y = b

rule F = '(1.25)F'(.8)                                 # elongation rate

