# Lparser example file
# taken from ABOP pag 129

recursion 10
angle 5
thickness 20
min_thickness 5
switch_yz 1

axiom &(90)+(90)FFF[-(120)'(.6)x][-(60)'(.8)x][+(120)'(.6)x][+(60)'(.8)x]x

rule a = F[cdx][cex]F!(.9)a
rule x = a

rule d = +d
rule e = -e

rule F = '(1.25)F'(.8)

