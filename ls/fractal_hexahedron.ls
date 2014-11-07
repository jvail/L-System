# Lparser example file

recursion 7
angle 90
thickness 100

color  1 255   0   0
color  2 255  64   0
color  3 255 128   0
color  4 255 191   0

color 10   0 255 255
color 11   0 191 255
color 12   0 128 255
color 13   0  64 255

axiom c(1)R ffffff c(10)'(2)T

rule R = [|z|F]'(.5)ca<a<a<a|['(.5)z]a>a>a>a
rule a = [f&f-fR]

rule T = [|z|F]'(.5)c[b][|b][&b][^b][+b][-b]  
rule b = fzT  
