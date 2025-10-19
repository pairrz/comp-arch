lw 0 1 n //โหลดค่า n ลงใน reg[1]
lw 0 6 pos1 //โหลดค่า pos1 = 1 ลงใน reg[6]
lw 0 7 neg1 //โหลดค่า neg1 = -1 ลงใน reg[7]
add 0 6 3 //คัดลอกค่า reg[6] ลงใน reg[3]

beq 1 0 done //ตรวจสอบ ถ้า reg[1] == 0 กระโดดไป done
beq 1 6 done //ถ้า reg[1] == reg[6] กระโดดไป done

factLp add 3 0 4 //คัดลอกค่า reg[3] ลงใน reg[4] 
add 1 0 5 //คัดลอกค่า reg[1] ลงใน reg[5]
add 0 0 3 //เซ็ต reg[3] = 0  
mulLp beq 5 0 mDone //ถ้า reg[5] == 0 กระโดดไป mDone
add 3 4 3 //เพิ่มค่า reg[4] ลงใน reg[3]
add 5 7 5 //เพิ่มค่า reg[7] ลงใน reg[5]
beq 0 0 mulLp //ถ้า reg[0] == 0 กระโดดไป mulLp (บังคับกระโดดไป mulLp)
mDone add 1 7 1 //เพิ่มค่า reg[7] ลงใน reg[1]
beq 1 6 done //ถ้า reg[1] == reg[6] กระโดดไป done
beq 0 0 factLp //ถ้า reg[0] == 0 กระโดดไป factLp
done halt //หยุดการทำงาน 

n .fill 6  //กำหนดค่า n = 6
pos1 .fill 1 //กำหนดค่า pos1 = 1
neg1 .fill -1 //กำหนดค่า neg1 = -1

