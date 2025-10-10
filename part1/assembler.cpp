#include "assembler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cctype>

bool Assembler::isNumber(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i++;
    for (; i < s.size(); ++i) if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

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

int Assembler::toOffset16Checked(long long x) {
    if (x < -32768 || x > 32767) throw std::runtime_error("offset out of range");
    return static_cast<int>(x & 0xFFFF);
}

void Assembler::parseFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) throw std::runtime_error("cannot open file: " + filename);
    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::string tok[5];
        int count = 0;
        while (iss >> tok[count] && count < 5) count++;
        if (count == 0) continue;
        Line L;
        if (opcodeOf(tok[0]) == -1 && tok[0] != ".fill") {
            L.hasLabel = true;
            if (!SymbolTable::isValidLabel(tok[0])) throw std::runtime_error("invalid label: " + tok[0]);
            L.label = tok[0];
            if (count < 2) throw std::runtime_error("missing opcode");
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
        program.push_back(L);
    }
}

void Assembler::buildSymbols() {
    for (int i = 0; i < (int)program.size(); i++) {
        if (program[i].hasLabel) symbols.add(program[i].label, i);
    }
}

int Assembler::encodeLine(const Line& L, int pc) const {
    if (L.opcode == ".fill") {
        if (isNumber(L.arg0)) return std::stoi(L.arg0);
        int addr = symbols.find(L.arg0);
        if (addr == -1) throw std::runtime_error("undefined label: " + L.arg0);
        return addr;
    }
    int op = opcodeOf(L.opcode);
    if (op < 0) throw std::runtime_error("invalid opcode: " + L.opcode);
    if (op == 0 || op == 1) {
        int regA = std::stoi(L.arg0);
        int regB = std::stoi(L.arg1);
        int dest = std::stoi(L.arg2);
        return (op << 22) | (regA << 19) | (regB << 16) | dest;
    }
    if (op == 2 || op == 3 || op == 4) {
        int regA = std::stoi(L.arg0);
        int regB = std::stoi(L.arg1);
        long long off = 0;
        if (isNumber(L.arg2)) {
            off = std::stoll(L.arg2);
        } else {
            int addr = symbols.find(L.arg2);
            if (addr == -1) throw std::runtime_error("undefined label: " + L.arg2);
            off = (op == 4) ? addr - (pc + 1) : addr;
        }
        return (op << 22) | (regA << 19) | (regB << 16) | toOffset16Checked(off);
    }
    if (op == 5) {
        int regA = std::stoi(L.arg0);
        int regB = std::stoi(L.arg1);
        return (op << 22) | (regA << 19) | (regB << 16);
    }
    if (op == 6 || op == 7) return (op << 22);
    throw std::runtime_error("unhandled opcode");
}

std::vector<int> Assembler::encodeAll() const {
    std::vector<int> out;
    for (int pc = 0; pc < (int)program.size(); pc++) out.push_back(encodeLine(program[pc], pc));
    return out;
}
