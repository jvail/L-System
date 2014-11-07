# Lparser example file by C.J. van der Mark  
# Only uneven rec.levels will produce objects.  

recursion 9                          
angle 30

axiom C

rule A = {[+f++++f++++f]}
rule B = [[^(19.5)A^(70)A]>>&(19.5)A^(19.5)---&&&&f&&&&<(19.5)-A]["(.5)C]+++f---
rule C = [BB+++^^^^f---B'(0.82)f'(1.22)---^'(0.574)f'(1.74)+++>>>>>B]+++ff---

rule { = _
rule } = _

