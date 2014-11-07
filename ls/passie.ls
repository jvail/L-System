# Lpaser example file by C.J.van der Mark jr.

recursion 10
angle 20
thickness 20
min_thickness 1
shape 1
switch_yz 1

# Axioms
#axiom o                                # the flower
#axiom L                                # a single leaf
#axiom &(60)C                           # the middle stem of the plant
#axiom k                                # middle flower branch
#axiom b                                # the side branches of the plant, with flower

axiom k"b>(55)"b>(60)'b>(65)b>(55)''b>(80)b   # the whole passieflora like plant

rule b = ['Ao]
rule k = [''''C^(90)o]
rule A = $t(.2)F''[+(55)$L][-(55)$L]t(-.1)FA
rule C = $F'[+(55)$L][-(55)$L]FC

# Plant Leaf
rule L = F&(30)&(10)cc[D][E]
rule D = [+D{.].X.}
rule E = [^(5)-E{.].X.}
rule X = ^(5)ggggX

# Flower leaf
rule d = [+d{.].x.}
rule h = [-h{.].x.}
rule x = gggx

# Whole flower
rule o = [&!ce>(90)w>(90)w>(90)w>(90)w>vy>vy>vy>vy>vy>vy>vy>vy>vy>yv>vy>vy>vy>vy>vy>vy>vy>vy]

# Flower stem
rule e = Z

# Spikes in the flower
rule v = [ZZc"""""&(90){'''F!!''F!!''F!!''F}]

# Flower female stem and male stems
rule w = [[sc??c'!!Ft(-.1)F<<''''!!^^^s]&(100)'(0.5)ZZZ[d][h]]

# Colored flower leafs
rule y = [[ZZcccc&(80)'''{-ff+ff|-ff+ff}][ZZccccc&(60)'''{-f+f|-f+f}][ZZcccccc&(35)''''{-f+f|-f+f}]]

# Singel male/female stem
rule s = [c^(8)!Ft(-.2)Ft(-.2)Ft(-.2)Fu]

# Pollen holder on male stem
rule u = [???F]

rule F = '(1.3)F'(.77)
rule f = '(1.3)f'(.77)

