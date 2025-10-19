lw 0 1 n
lw 0 2 r
lw 0 5 sp0
lw 0 4 cadr
jalr 4 7
halt

comb sw 5 7 0
lw 0 4 pos1
add 5 4 5
sw 5 1 0
add 5 4 5
sw 5 2 0
add 5 4 5
lw 0 6 neg1
beq 2 0 ret1
beq 1 2 ret1

add 1 6 1
lw 0 4 cadr
jalr 4 7            
sw 5 3 0           
add 5 4 5           


lw 5 1 -3          
lw 5 2 -2         

add 1 6 1           
add 2 6 2           
lw 0 4 cadr
jalr 4 7            


lw 5 4 -1           
add 5 6 5           
add 3 4 3           
beq 0 0 done

ret1 lw 0 3 pos1    

done lw 5 7 -3      
add 5 6 5           
add 5 6 5           
add 5 6 5           
jalr 7 2

n .fill 4
r .fill 2
pos1 .fill 1
neg1 .fill -1
sp0 .fill 1024
cadr .fill comb
