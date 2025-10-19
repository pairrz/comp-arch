#include "assembler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cctype>

/*isNumber(s): ตรวจว่าสตริง s เป็นจำนวนเต็มหรือไม่ (รองรับ + / - นำหน้า)
    ใช้เวลาต้องตัดสินใจว่า arg เป็นตัวเลขจริง หรือเป็น label */
bool Assembler::isNumber(const std::string& s) {
    if (s.empty()) return false; //สตริงว่าง
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i++; // ข้ามเครื่องหมายบวก/ลบได้
    for (; i < s.size(); ++i) if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;// เจอตัวที่ไม่ใช่ '0'..'9' → ไม่ใช่เลข
    return true;
}

/* opcodeOf(op):
แปลงชื่อคำสั่งเป็นรหัส opcode (0..7) ตาม LC-2K
คืน -1 ถ้าไม่ใช่ opcode ที่มี */
int Assembler::opcodeOf(const std::string& op) {
    if (op == "add") return 0;
    if (op == "nand") return 1;
    if (op == "lw") return 2;
    if (op == "sw") return 3;
    if (op == "beq") return 4;
    if (op == "jalr") return 5;
    if (op == "halt") return 6;
    if (op == "noop") return 7;
    return -1;
}
/* toOffset16Checked(x):
ตรวจให้แน่ใจว่าค่า x อยู่ในช่วง signed 16-bit (-32768..32767)
ถ้าเกินช่วง → throw
คืนค่าที่ mask ให้เหลือ 16 บิตล่าง (พร้อม sign ที่ hardware จะตีความ) */
int Assembler::toOffset16Checked(long long x) {
    if (x < -32768 || x > 32767) throw std::runtime_error("offset out of range");
    return static_cast<int>(x & 0xFFFF);
}
/* parseFile(filename):
อ่านทีละบรรทัด แยกคำ (token) มากสุด 5 ตัว: label opcode arg0 arg1 arg2
ถ้าคำตัวแรกไม่ใช่ opcode และไม่ใช่ .fill → ถือว่าเป็น label
เก็บเป็นโครง Line แล้วโยนเข้าลิสต์ program */
void Assembler::parseFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) throw std::runtime_error("cannot open file: " + filename);
    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::string tok[5];  // label/opcode/arg0/arg1/arg2 (สูงสุด 5 โทเคน)
        int count = 0;
        //ต้องแน่ใจว่าไม่มี token เกิน 5 ตัวใน 1 บรรทัด
        while (iss >> tok[count] && count < 5) count++;
        if (count == 0) continue; // บรรทัดว่าง → ข้าม
        
        Line L;
        // ถ้า tok[0] ไม่ใช่ opcode และไม่ใช่ .fill → ถือว่าเป็น label
        if (opcodeOf(tok[0]) == -1 && tok[0] != ".fill") {
            L.hasLabel = true;
            if (!SymbolTable::isValidLabel(tok[0])) throw std::runtime_error("invalid label: " + tok[0]);
            //label ไม่ผ่านเงื่อนไข
            L.label = tok[0];
            if (count < 2) throw std::runtime_error("missing opcode");
            //มี label แต่ไม่มี opcode ตามมา
            L.opcode = tok[1];
            if (count > 2) L.arg0 = tok[2];
            if (count > 3) L.arg1 = tok[3];
            if (count > 4) L.arg2 = tok[4];
        } else {
            L.opcode = tok[0];
            if (count > 1) L.arg0 = tok[1];
            if (count > 2) L.arg1 = tok[2];
            if (count > 3) L.arg2 = tok[3];
        }
        program.push_back(L); // เก็บบรรทัดนี้ลงโปรแกรม
    }
}
/* buildSymbols():
เดินทุกบรรทัดใน program → ถ้าบรรทัดมี label ให้บันทึก:*/
void Assembler::buildSymbols() {
    for (int i = 0; i < (int)program.size(); i++) {
        if (program[i].hasLabel) symbols.add(program[i].label, i);
    }
}

/* encodeLine(L, pc):
เข้ารหัส 1 บรรทัดเป็นคำสั่ง 32 บิต (ส่งคืนเป็น int)
เคสต่าง ๆ:
    - .fill:
    arg0 เป็นเลข → คืนเลข
    arg0 เป็น label → คืน address ของ label นั้น
    - R: add/nand
        word = (op<<22) | (regA<<19) | (regB<<16) | dest
    - I: lw/sw/beq
        ถ้า arg2 เป็นเลข → ใช้ตรง ๆ
        ถ้า arg2 เป็น label:
            * beq: offset = labelAddr - (pc + 1)  (PC-relative)
            * lw/sw: offset = labelAddr           (absolute)
        แล้วตรวจช่วง 16 บิตด้วย toOffset16Checked()
    - J: jalr(regA, regB)
        word = (op<<22) | (regA<<19) | (regB<<16)
    - O: halt/noop
        word = (op<<22)
*/
int Assembler::encodeLine(const Line& L, int pc) const {
    // เคส pseudo-op: .fill (วางค่า/ที่อยู่ลง memory ตำแหน่งนี้)
    if (L.opcode == ".fill") {
        if (isNumber(L.arg0)) return std::stoi(L.arg0); // ตัวเลข → วางค่าตรง ๆ
        int addr = symbols.find(L.arg0);  // label → หา address
        if (addr == -1) throw std::runtime_error("undefined label: " + L.arg0);
        return addr;
    }
    // เคส opcode ปกติ
    int op = opcodeOf(L.opcode);

    // R-type: add(0), nand(1)
    if (op < 0) throw std::runtime_error("invalid opcode: " + L.opcode);
    if (op == 0 || op == 1) {
        int regA = std::stoi(L.arg0); // arg0 = regA
        int regB = std::stoi(L.arg1); // arg1 = regB
        int dest = std::stoi(L.arg2); // arg2 = destReg
        return (op << 22) | (regA << 19) | (regB << 16) | dest;
    }
    // I-type: lw(2), sw(3), beq(4)
    if (op == 2 || op == 3 || op == 4) {
        int regA = std::stoi(L.arg0); // arg0 = regA
        int regB = std::stoi(L.arg1); // arg1 = regB
        long long off = 0; // arg2 = offset/label
        if (isNumber(L.arg2)) {
            off = std::stoll(L.arg2); // immediate เป็นตัวเลข
        } else {
            int addr = symbols.find(L.arg2); // immediate เป็น label → แปลงเป็น address
            if (addr == -1) throw std::runtime_error("undefined label: " + L.arg2);
            off = (op == 4) ? addr - (pc + 1) // beq = PC-relative
                            : addr;  // lw/sw = absolute
        }
        // แพ็ค field พร้อมตรวจช่วง 16-bit signed
        return (op << 22) | (regA << 19) | (regB << 16) | toOffset16Checked(off);
    }
    // J-type: jalr(5)  (regA = target register, regB = link register)
    if (op == 5) {
        int regA = std::stoi(L.arg0);
        int regB = std::stoi(L.arg1);
        return (op << 22) | (regA << 19) | (regB << 16);
    }
    // O-type: halt(6), noop(7)
    if (op == 6 || op == 7) return (op << 22);
    // เผื่อในอนาคตมี opcode ใหม่ที่ยังไม่รองรับ
    throw std::runtime_error("unhandled opcode");
}

/* encodeAll():
เดินทุกบรรทัดใน program ตามลำดับ pc (คือ index)
แล้วเรียก encodeLine() เพื่อสร้างลิสต์คำสั่ง 32 บิต (int) */
std::vector<int> Assembler::encodeAll() const {
    std::vector<int> out;
    for (int pc = 0; pc < (int)program.size(); pc++) out.push_back(encodeLine(program[pc], pc));
    return out;
}
