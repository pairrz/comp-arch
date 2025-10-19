#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

struct Line {
    std::string label; // ชื่อ label (ถ้ามี) เช่น "loop"
    std::string opcode; // ชื่อคำสั่ง เช่น "add", "lw", ".fill"
    std::string arg0; // อาร์กิวเมนต์ตัวที่ 1 (มักเป็น regA)
    std::string arg1; // อาร์กิวเมนต์ตัวที่ 2 (มักเป็น regB)
    std::string arg2; // อาร์กิวเมนต์ตัวที่ 3 (dest หรือ offset/label)
    bool hasLabel = false; // true ถ้าบรรทัดนั้นเริ่มด้วย label
};
/*
ตารางสัญลักษณ์ (Symbol Table):
เก็บ mapping: ชื่อ label → เลขบรรทัด (address) ที่ label นั้นประกาศไว้
ใช้ตอนเข้ารหัสคำสั่งที่อ้าง label (เช่น beq/lw/sw หรือ .fill ชนิด label)
 */
class SymbolTable {
public:
    void add(const std::string& name, int address);
    //เพิ่มคู่ (name → address) ลงตาราง

    int find(const std::string& name) const;
    //คืน address ของ label ถ้าเจอ
    // ถ้าไม่เจอ ให้คืน -1 (ไว้ให้คนเรียกตัดสินใจว่าจะ throw หรือไม่)
    static bool isValidLabel(const std::string& name);
    // ตรวจรูปแบบ label ว่าใช้ได้มั้ย:
    // ตัวแรกต้องเป็นตัวอักษร
    // ความยาวไม่เกิน 6 ตัว
    // มีแต่ a–z/A–Z/0–9
private:
// โครงสร้างเก็บข้อมูลจริง: แผนที่จากชื่อ → address
    std::unordered_map<std::string, int> table;
};

/*
 * ตัว Assembler หลัก:
 * งานของคลาสนี้คือ:
 *  1) อ่านไฟล์ .asm แล้วแปลงเป็นรายการ Line (parseFile)
 *  2) สร้าง symbol table จาก label ทั้งหมด (buildSymbols)
 *  3) เข้ารหัสทุกบรรทัดเป็น machine code (encodeAll)
 */
class Assembler {
public:
// อ่านไฟล์ .asm แตกบรรทัดเป็น Line แล้วเก็บใน program
    void parseFile(const std::string& filename);
    // เดินดูทุก Line ถ้ามี label ให้บันทึกลง SymbolTable (label → address)
    void buildSymbols();
    // วนเรียก encodeLine ทีละบรรทัด → คืน vector<int> ของ machine words
    std::vector<int> encodeAll() const;
private:
// เข้ารหัส “บรรทัดเดียว” เป็นคำสั่ง 32 บิต (ฐานสิบ)
// pc คือเลขบรรทัดปัจจุบัน (ใช้คำนวณ beq แบบ PC-relative)
    int encodeLine(const Line& L, int pc) const;
     // ตรวจว่า offset ยัดลง 16-bit signed ได้ไหม (-32768..32767)
     // ถ้าไม่ได้ให้ throw เพื่อกัน bug ตั้งแต่ตอน assemble
    static int toOffset16Checked(long long x);
    // map ชื่อ opcode เป็นรหัสตัวเลข 0..7 ตามสเปก LC-2K
    // ถ้าไม่ใช่ opcode ที่รู้จัก ให้คืน -1 (ผู้เรียกจะตัดสินใจต่อ)
    static int opcodeOf(const std::string& op);
    // เช็กว่า string เป็นเลขจำนวนเต็ม (อนุญาต + / - นำหน้า)
    // ใช้แยกแยะว่า arg เป็น “เลขจริง” หรือ “ชื่อ label”
    static bool isNumber(const std::string& s);
private:
// เก็บ “โปรแกรมที่ parse แล้ว” เป็นลิสต์บรรทัด
    std::vector<Line> program;
    // ตาราง label → address ที่สร้างจาก buildSymbols()
    SymbolTable symbols;
};

#endif
