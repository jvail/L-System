# Lparser example file

# Lychnis structure taken from ABOP on page 84

recursion 45
angle 18
thickness 100
min_thickness 7
shape 1
switch_yz 1

axiom c(12)&(20)N

rule N = FII[&(60)rY]>(90)[&(45)'(0.8)rA]>(90)[&(60)rY]>(90)[&(45)'(0.8)rD]!FIK

# leaves
rule Y = [c(4){++l.--l.--l.++|++l.--l.--l.}]
rule l = g(.2)l 	# leaves grow in size with recursion level

# flowers
rule K = [!c(2)FF>w>(72)w>(72)w>(72)w>(72)w]
rule w = [c(2)^!F][c(5)&(72){-(54)f(3)+(54)f(3)|-(54)f(3)+(54)f(3)}]
rule f = _  		# removes all flower leaves except of the last recursion

# 7 step delay for A(i)
rule A = B
rule B = C
rule C = D
rule D = E
rule E = G
rule G = H
rule H = N

# 10 step delay for I(i)
rule I = FoO
rule O = FoP
rule P = FoQ
rule Q = FoR
rule R = FoS
rule S = FoT
rule T = FoU
rule U = FoV
rule V = FoW
rule W = FoX
rule X = _

# Changes
rule o = $t(-0.03)
rule r = ~(30)
