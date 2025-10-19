#include "assembler.hpp"
#include <cctype>

void SymbolTable::add(const std::string& name, int address) {
    // ถ้า label นี้มีอยู่แล้วใน table → ไม่อนุญาตให้ซ้ำ
    if (table.count(name)) throw std::runtime_error("Duplicate label: " + name);
    
    // ถ้าไม่ซ้ำ ให้เพิ่มคู่ (label → address) ลงในแผนที่
    table[name] = address;
}
/*
 * ฟังก์ชัน: find
 * หน้าที่ : ค้นหาว่า label ที่กำหนดอยู่ในตารางไหม
 * ถ้าเจอจะคืนค่า address, ถ้าไม่เจอจะคืนค่า -1
 */
int SymbolTable::find(const std::string& name) const {
    auto it = table.find(name);// ใช้ iterator หาค่าใน unordered_map
    if (it == table.end()) return -1;// ถ้าไม่เจอเลย คืน -1
    return it->second;// ถ้าเจอ คืนค่าหมายเลข address
}
/*
 * ฟังก์ชัน: isValidLabel
 * หน้าที่ : ตรวจสอบว่า label ถูกต้องตามกติกาหรือไม่
 * เงื่อนไข:
 *   1) ต้องไม่ว่างและยาวไม่เกิน 6 ตัวอักษร
 *   2) ตัวอักษรตัวแรกต้องเป็น A–Z หรือ a–z
 *   3) ตัวที่เหลือเป็นได้แค่ A–Z, a–z, หรือ 0–9 เท่านั้น
 */
bool SymbolTable::isValidLabel(const std::string& name) {
    // ถ้าว่าง หรือยาวเกิน 6 ตัว → ไม่ถูกต้อง
    if (name.empty() || name.size() > 6) return false;
    // ตัวแรกต้องเป็นตัวอักษร (ห้ามเป็นตัวเลข)
    if (!std::isalpha(static_cast<unsigned char>(name[0]))) return false;
    // ตรวจทุกตัวว่ามีแต่ตัวอักษรหรือเลขเท่านั้น
    for (unsigned char c : name) if (!std::isalnum(c)) return false;
     // ถ้าเจออักขระพิเศษ → ไม่ผ่าน
    return true;
    // ถ้าผ่านครบทุกเงื่อนไข → label ถูกต้อง
}
