
# Part 3.1 - การคูณ
# ================================

# 🧱 1) คอมไพล์ Assembler
cd /d/comp-arch/part1
g++ -std=c++17 assembler.cpp symbol_table.cpp main.cpp -o assembler

# 🧱 2) คอมไพล์ Simulator
cd /d/comp-arch/comp-arch/part2
g++ -std=c++17 simulator.cpp -o simulator

# ⚙️ 3) รันโปรแกรม Factorial
cd /d/comp-arch/comp-arch/part3/3.1
/d/comp-arch/comp-arch/part1/assembler

/d/comp-arch/comp-arch/part2/simulator machinecode.txt \
| sed -n '/machine halted/,$p' > result_output.txt

/d/comp-arch/comp-arch/part2/simulator machinecode.txt

/d/comp-arch/comp-arch/part2/simulator machinecode.txt > result_output.txt

ทดสอบ part2
/d/comp-arch/comp-arch/part2/simulator /d/comp-arch/comp-arch/part1/machinecode.txt

