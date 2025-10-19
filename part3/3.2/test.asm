lw 0 1 pos1
lw 0 7 neg1
expLp lw 0 5 expo
beq 5 0 done
lw 0 3 base
add 1 0 2
lw 0 4 pos1
lw 0 5 fiftn
add 0 0 1
mLp nand 3 4 6
nand 6 6 6
beq 6 0 skp
add 1 2 1
skp add 2 2 2
add 4 4 4
add 5 7 5
beq 5 0 mDn
beq 0 0 mLp
mDn lw 0 6 expo
add 6 7 6
sw 0 6 expo
beq 0 0 expLp
done halt
base .fill 2
expo .fill 8
pos1 .fill 1
neg1 .fill -1
fiftn .fill 15
