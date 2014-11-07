# Lparser example file
 
recursion 18
angle 10	
thickness 15	
switch_yz 1
shape 1

axiom [|FFFFFFFFFFF]P>(90)'P>(90)'P>(90)'P	

rule P = [&(10)G[ccA][ccB][a][b]]
rule G = tFtFtFtFtFtFtFtFtFtFtFtFtF

rule A = [+A{.].C.}
rule B = [-B{.].C.}
rule C = tfC

rule a = [+a]d
rule b = [-b]d
rule d = tFd

