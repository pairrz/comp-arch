#include "assembler.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int main() {
    // ตั้งไฟล์อินพุตไว้ตายตัว (เปลี่ยนชื่อได้)
    const string inputFile = "test.asm";
    const string outFile   = "machinecode.txt";

    try {
        Assembler a; // ตัวแปลง assembly → machine code
        a.parseFile(inputFile); // 1) อ่าน test.asm แล้วแตกบรรทัดเป็น (label/opcode/args)
        a.buildSymbols(); // 2) สร้าง symbol table (label → เลขบรรทัด) สำหรับอ้างอิงตอนเข้ารหัส
        vector<int> words = a.encodeAll(); // 3) เข้ารหัสทุกบรรทัดเป็นคำสั่ง 32-bit (เก็บเป็น int)

        // 1) พิมพ์ผลใน Terminal
        for (int w : words) cout << w << '\n';

        // 2) เขียนไฟล์ (โหมด trunc = เขียนทับของเก่า)
        ofstream output(outFile, ios::trunc);
        if (!output) {
            cerr << "Error: cannot open " << outFile << " for writing\n";
            return 1;
        }
        for (int w : words) output << w << '\n';
        output.close(); // ปิดไฟล์ให้เรียบร้อย (แฟลชข้อมูลลงดิสก์)

        cout << "Saved machine code to '" << outFile << "'\n";
    }
    catch (const exception& e) { // ดักทุก error ที่ Assembler โยนมา (เช่น label ไม่เจอ / offset เกินช่วง
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
/*คำสั่งรัน g++ -std=c++17 assembler.cpp symbol_table.cpp main.cpp -o assembler && ./assembler
 */