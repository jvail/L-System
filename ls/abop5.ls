# Lparser example file 
# taken from ABOP pag 129

recursion 29
angle 5
thickness 20
min_thickness 2
switch_yz 1
shape 1

color  1 255   0   0
color  2 255  64   0
color  3 255 128   0
color  4 255 191   0
color  5 191 255   0
color  6 128 255   0
color  7  64 255   0
color  8   0 255   0
color  9   0 255  64
color 10   0 255 128
color 11   0 255 191
color 12   0 255 255
color 13   0 191 255
color 14   0 128 255
color 15   0  64 255
color 16   0   0 255
color 17  64   0 255
color 18 128   0 255
color 19 191   0 255
color 20 255   0 255
color 21 255   0 191
color 22 255   0 128
color 23 255   0  64
color 24 255   0   0
color 25 255  64   0
color 26 255 128   0
color 27 255 191   0
color 28 191 255   0
color 29 128 255   0
color 30  64 255   0

axiom &(90)+(90)a

rule a = F[+(45)l][-(45)l]^;ca

# Apical delay
rule l = j
rule j = h
rule h = s
rule s = d
rule d = x
rule x = a

rule F = '(1.17)F'(.855)	# internode elongation rate

