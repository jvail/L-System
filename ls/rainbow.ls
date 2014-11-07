# Lparser example file
# this file uses the color command to set up a palette
# making 23 steps in a color circle

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

recursion 23
no_wait 1

axiom c(1)A

rule A = F+(15.652)cA
