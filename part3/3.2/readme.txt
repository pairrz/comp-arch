lw 0 1 n // โหลดค่า n = 4 ได้ r1 = 4
lw 0 2 r // โหลดค่า r = 2 ได้ r2 = 2
lw 0 5 sp0 // โหลดค่า stack pointer เริ่มต้น (sp0 = 1024) ดังนั้น r5 = 1024
lw 0 4 cadr // โหลด address ของฟังก์ชัน comb จะได้ r4 = address(comb)
jalr 4 7 //jump ไปเรียก comb() และเก็บ return address ไว้ใน r7
halt //หยุดการทำงานเมื่อ comb() เสร็จเเล้ว
//โปรแกรมจะกระโดดเข้าไปทำงานใน label comb ซึ่งจะใช้ stack (ผ่าน r5) ในการเก็บค่าชั่วคราวและ return address

comb sw 5 7 0 // เก็บ return address ไว้ที่ stack[0]
lw 0 4 pos1 // โหลดค่า +1 ทําให้ r4 = 1
add 5 4 5 // sp = sp + 1 ชี้ไปตำแหน่งใหม่
sw 5 1 0 // เก็บ n ลง stack[sp]
add 5 4 5 // sp = sp + 1
sw 5 2 0 // เก็บ r ลง stack[sp]
add 5 4 5 // sp = sp + 1  กันที่ว่าง 1 ช่องไว้เก็บผลของ call แรก
lw 0 6 neg1 // โหลด -1 ได้ r6 = -1
//ตอนนี้ stack จะเก็บข้อมูล (จากล่างขึ้นบน):[return address] [n] [r] ใช้ sp เพิ่มขึ้นเรื่อย ๆ เพื่อกันช่องสำหรับตัวแปรใหม่

beq 2 0 ret1 // ถ้า r == 0 return 1
beq 1 2 ret1 // ถ้า n == r return 1
//ตามนิยามของ combination:C(n, 0) = 1 และ C(n, n) = 1

// ----------Recursive call #1----------
add 1 6 1 // n = n - 1
lw 0 4 cadr
jalr 4 7 // เรียก comb(n-1, r-1)  
sw 5 3 0 // เก็บผลลัพธ์ C(n-1, r-1) ไว้ใน stack[sp]
add 5 4 5  // sp++          

// ----------เตรียมอาร์กิวเมนต์เดิมสำหรับ call #2----------
lw 5 1 -3 // ดึงค่า n เดิมกลับมา          
lw 5 2 -2 // ดึงค่า r เดิมกลับมา

// ----------ปรับเป็นพารามิเตอร์ของ call #2----------
add 1 6 1   // n = n - 1       
add 2 6 2   // r = r - 1 (สำหรับ call ถัดไป)       
lw 0 4 cadr
jalr 4 7   // เรียก comb(n-1, r-1) กลับมา r3 = ผลของ call #2 

// ----------รวมผลลัพธ์----------
lw 5 4 -1 // r4 = [sp-1] = [S+3] = ผลของ call #1
add 5 6 5  // sp = sp - 1 ได้ sp: S+4 → S+3        
add 3 4 3  // r3 = r3 + r4 → รวมผล: C(n-1, r-1) + C(n-1, r)         
beq 0 0 done // jump แบบไม่มีเงื่อนไข ไปส่วนที่คืนค่า

// ----------Base case return----------
ret1 lw 0 3 pos1 // r3 = 1   

done lw 5 7 -3  // r7 = [sp-3] = [S+0] → โหลด RA ของเฟรมนี้กลับมา    
add 5 6 5  // sp = sp - 1  → S+3 → S+2        
add 5 6 5   // sp = sp - 1  → S+2 → S+1        
add 5 6 5   // sp = sp - 1  → S+1 → S        
jalr 7 2  // return: r2 = PC+1, PC = r7

// ----------ข้อมูล----------
n .fill 4  // ค่าตั้งต้น n = 4
r .fill 2  // ค่าตั้งต้น r = 2
pos1 .fill 1  // ค่าคงที่ +1
neg1 .fill -1  // ค่าคงที่ -1
sp0 .fill 1024  // ค่าเริ่มของ stack pointer
cadr .fill comb // เก็บเลขที่อยู่ของ label comb ลงในหน่วยความจำ
