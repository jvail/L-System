# Lparser example file

recursion 38
angle 10
thickness 10
min_thickness 1
shape 1

axiom PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP

rule P = S>(20)gg'
rule S = [&(90)[A]+(20)^(90)gg'&(90)[B][cD]]

rule D = ^FD                           # show stalks

rule A = [^gA{.].                      # put polygons on them
rule B = B^.g.}


