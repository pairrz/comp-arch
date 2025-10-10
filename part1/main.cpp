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
        Assembler a;
        a.parseFile(inputFile);
        a.buildSymbols();
        vector<int> words = a.encodeAll();

        // 1) พิมพ์ผลใน Terminal
        for (int w : words) cout << w << '\n';

        // 2) เขียนไฟล์ (โหมด trunc = เขียนทับของเก่า)
        ofstream output(outFile, ios::trunc);
        if (!output) {
            cerr << "Error: cannot open " << outFile << " for writing\n";
            return 1;
        }
        for (int w : words) output << w << '\n';
        output.close();

        cout << "Saved machine code to '" << outFile << "'\n";
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
/*คำสั่งรัน g++ -std=c++17 assembler.cpp symbol_table.cpp main.cpp -o assembler && ./assembler
 */