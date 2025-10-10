#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

struct Line {
    std::string label;
    std::string opcode;
    std::string arg0;
    std::string arg1;
    std::string arg2;
    bool hasLabel = false;
};

class SymbolTable {
public:
    void add(const std::string& name, int address);
    int find(const std::string& name) const;
    static bool isValidLabel(const std::string& name);
private:
    std::unordered_map<std::string, int> table;
};

class Assembler {
public:
    void parseFile(const std::string& filename);
    void buildSymbols();
    std::vector<int> encodeAll() const;
private:
    int encodeLine(const Line& L, int pc) const;
    static int toOffset16Checked(long long x);
    static int opcodeOf(const std::string& op);
    static bool isNumber(const std::string& s);
private:
    std::vector<Line> program;
    SymbolTable symbols;
};

#endif
