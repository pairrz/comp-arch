#include "assembler.hpp"
#include <cctype>

void SymbolTable::add(const std::string& name, int address) {
    if (table.count(name)) throw std::runtime_error("Duplicate label: " + name);
    table[name] = address;
}

int SymbolTable::find(const std::string& name) const {
    auto it = table.find(name);
    if (it == table.end()) return -1;
    return it->second;
}

bool SymbolTable::isValidLabel(const std::string& name) {
    if (name.empty() || name.size() > 6) return false;
    if (!std::isalpha(static_cast<unsigned char>(name[0]))) return false;
    for (unsigned char c : name) if (!std::isalnum(c)) return false;
    return true;
}
