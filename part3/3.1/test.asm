lw 0 2 mcand         ; R2 ← memory[mcand]   โหลดตัวเลขตัวแรก (ตัวคูณ)
lw 0 3 mplier        ; R3 ← memory[mplier]  โหลดตัวเลขตัวที่สอง (ตัวคูณอีกตัว)
add 0 0 1
lw 0 4 one           ; R4 ← 1               ใช้เป็นตัวนับ +1
lw 0 7 neg1          ; R7 ← -1              ใช้สำหรับลบ (-1)
lw 0 5 fiftn         ; R5 ← 15              ใช้เก็บค่าชั่วคราว
loop nand 3 4 6      ; R6 ← ~(R3 & R4)      ตรวจบิตของ R3 เพื่อตัดสินใจคูณ (ใช้แทนการเช็กบิต)
nand 6 6 6
beq 6 0 skip         ; ถ้า R6 == 0 → ข้ามการบวก (ไม่บวก R2 เข้า R1)
add 1 2 1            ; R1 ← R1 + R2         บวกค่าตัวคูณเข้ากับผลรวม (result)
skip add 2 2 2       ; R2 ← R2 + R2         shift left 1 bit (คูณ 2)
add 4 4 4            ; R4 ← R4 + R4         เพิ่มค่าตัวนับ (2, 4, 8,...)
add 5 7 5            ; R5 ← R5 + R7         ลดค่า fiftn ลง 1 (15 → 14 → 13 → ...)
beq 5 0 done         ; ถ้า R5 == 0 → กระโดดไป done (จบการคำนวณ)
beq 0 0 loop         ; กระโดดกลับไป loop (ทำซ้ำ)
done halt            ; หยุดโปรแกรม
mcand .fill 32766    ; ตัวเลขตัวแรก (multiplicand)
mplier .fill 10383   ; ตัวเลขตัวที่สอง (multiplier)
one .fill 1          ; ค่าคงที่ 1
neg1 .fill -1        ; ค่าคงที่ -1
fiftn .fill 15       ; ค่าคงที่ 15 (ตัวนับรอบ)
