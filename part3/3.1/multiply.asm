; multiply.asm — ใช้ NASM (x86-64)
; ฟังก์ชันคูณเลขสองตัวแบบง่ายสุด
; long long mul2();

section .text
global mul2

mul2:
    mov rax, 5      ; rax = 5
    mov rbx, 6      ; rbx = 6
    imul rax, rbx   ; rax = rax * rbx
    ret              ; return rax (ได้ 30)

;cd part3 
;nasm -f win64 multiply.asm -o multiply.obj; g++ -std=c++17 main.cpp multiply.obj -o mul.exe; ./mul.exe
