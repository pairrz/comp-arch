lw 0 2 mcand
lw 0 3 mplier
add 0 0 1
lw 0 4 one
lw 0 7 neg1
lw 0 5 fiftn
loop nand 3 4 6
nand 6 6 6
beq 6 0 skip
add 1 2 1
skip add 2 2 2
add 4 4 4
add 5 7 5
beq 5 0 done
beq 0 0 loop
done halt
mcand .fill 13
mplier .fill 9
one .fill 1
neg1 .fill -1
fiftn .fill 15
