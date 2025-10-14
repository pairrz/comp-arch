#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const int NUMMEMORY = 65536;
const int NUMREGS   = 8;

struct State {
    int pc;
    vector<int> mem;
    int reg[NUMREGS];
};

int convertNum(int num) {
    if (num & (1 << 15))
        num -= (1 << 16);
    return num;
}

void printState(const State& state) {
    cout << "\n@@@\nstate:\n";
    cout << "\tpc " << state.pc << "\n";
    cout << "\tmemory:\n";
    for (size_t i = 0; i < state.mem.size(); i++) {
        cout << "\t\tmem[" << setw(3) << i << "] " << state.mem[i] << "\n";
    }
    cout << "\tregisters:\n";
    for (int i = 0; i < NUMREGS; i++) {
        cout << "\t\treg[" << i << "] " << state.reg[i] << "\n";
    }
    cout << "end state\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <machine-code file>\n";
        return 1;
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cerr << "Error: cannot open " << argv[1] << "\n";
        return 1;
    }

    State state;
    state.pc = 0;
    state.mem.clear();
    for (int i = 0; i < NUMREGS; i++) state.reg[i] = 0;

    int val;
    while (infile >> val) {
        state.mem.push_back(val);
    }

    int instrCount = 0;
    bool halted = false;

    while (!halted) {
        printState(state);

        int pc = state.pc;
        if (pc < 0 || pc >= (int)state.mem.size()) {
            cerr << "Error: pc out of range: " << pc << "\n";
            return 1;
        }

        int instr = state.mem[pc];
        unsigned int uinstr = static_cast<unsigned int>(instr);
        int opcode = uinstr >> 22;
        int regA   = (uinstr >> 19) & 0x7;
        int regB   = (uinstr >> 16) & 0x7;
        int dest   = uinstr & 0x7;
        int off16  = uinstr & 0xFFFF;
        int off32  = convertNum(off16);

        int nextPC = pc + 1;
        instrCount++;

        switch (opcode) {
            case 0:{ //add
                state.reg[dest] = state.reg[regA] + state.reg[regB];
                state.pc = nextPC;
                break;
            }
            case 1:{ //nand
                state.reg[dest] = ~(state.reg[regA] & state.reg[regB]);
                state.pc = nextPC;
                break;
            }
            case 2:{ //lw
                int addr = state.reg[regA] + off32;
                if (addr < 0 || addr >= NUMMEMORY) {
                    cerr << "Error: lw address out of range\n";
                    return 1;
                }
                state.reg[regB] = state.mem[addr];
                state.pc = nextPC;
                break;
            }
            case 3:{//sw
                int addr = state.reg[regA] + off32;
                if (addr < 0 || addr >= NUMMEMORY) {
                    cerr << "Error: sw address out of range\n";
                    return 1;
                }
                if (addr >= (int)state.mem.size()) state.mem.resize(addr + 1, 0);
                state.mem[addr] = state.reg[regB];
                state.pc = nextPC;
                break;
            }
            case 4: {//beq
                if (state.reg[regA] == state.reg[regB])
                    state.pc = nextPC + off32;
                else
                    state.pc = nextPC;
                break;
            }
            case 5: {//jalr
                int next = nextPC;
                int target = state.reg[regA];
                state.pc = target;
                state.reg[regB] = next;
                break;
            }
            case 6: {//halt
                state.pc = nextPC;
                cout << "machine halted\n";
                cout << "total of " << instrCount << " instructions executed\n";
                cout << "final state of machine:\n";
                printState(state);
                halted = true;
                break;
            }
            case 7:{ //noop
                state.pc = nextPC;
                break;
            }
            default:
                cerr << "Error: unknown opcode " << opcode << "\n";
                return 1;
        }

        state.reg[0] = 0;
    }

    return 0;
}

