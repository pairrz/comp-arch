lw 0 1 n
lw 0 6 pos1
lw 0 7 neg1
add 0 6 3

beq 1 0 done
beq 1 6 done

factLp add 3 0 4
add 1 0 5
add 0 0 3
mulLp beq 5 0 mDone
add 3 4 3
add 5 7 5
beq 0 0 mulLp
mDone add 1 7 1
beq 1 6 done
beq 0 0 factLp
done halt

n .fill 8
pos1 .fill 1
neg1 .fill -1
