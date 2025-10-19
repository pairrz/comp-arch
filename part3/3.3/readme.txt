# Part 3.3 - Factorial (Iterative)
# ================================

# 🧱 1) คอมไพล์ Assembler
cd /d/comp-arch/part1
g++ -std=c++17 assembler.cpp symbol_table.cpp main.cpp -o assembler

# 🧱 2) คอมไพล์ Simulator
cd /d/comp-arch/comp-arch/part2
g++ -std=c++17 simulator.cpp -o simulator

# ⚙️ 3) รันโปรแกรม Factorial
cd /d/comp-arch/comp-arch/part3/3.3
/d/comp-arch/comp-arch/part1/assembler

/d/comp-arch/comp-arch/part2/simulator machinecode.txt \
| sed -n '/machine halted/,$p' > result_output.txt




# 📊 4) เปิดดูผลลัพธ์
cat result_output.txt
# เลื่อนไปท้ายไฟล์ ดูค่า reg[3] = ผลลัพธ์ factorial

