# Lparser example file
# polyhedra in l-systems

recursion 3
angle 90
switch_yz 1
no_wait 1

axiom T cf H cf O cf(1.5) K

# Kepler solid
rule K = [a|b]'(0.41)f'(2.43)|<(60)[a|b]

# Tetrahedron
rule T = [a|b]
rule a = [d<(120)d<(120)d]
rule b = [^'(.2887)f'(3.4758)&&+z{.-(120)f-(120)f}]
rule d = [^'(.2887)f'(3.4758)&(109.5111)+z{.-(120)f-(120)f}]

# Hexahedron
rule H = F			# duh ...

# Octahedron
rule O = [e|e]
rule e = [h<h<h<h]
rule h = [^z&&(35.24444)+z{.-(120)f-(120)f}]
