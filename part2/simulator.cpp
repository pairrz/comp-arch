#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const int NUMMEMORY = 65536; //ขนาดหน่วยความจำสูงสุด
const int NUMREGS   = 8; //จำนวน register

struct State {
    int pc; //program counter
    vector<int> mem; //เก็บคำสั่ง
    int reg[NUMREGS]; //registers 8 ตัว
    int numMemory; //จำนวนคำสั่งในหน่วยความจำ
};

int convertNum(int num) { //แปลงเลข 16 บิต เป็นเลข 32 บิต
    if (num & (1 << 15)) //ถ้าบิตที่ 15 เป็น 1
        num -= (1 << 16); //ลบด้วย 2^16 เพื่อแปลงเป็นลบ
    return num;
}

void printState(const State& state) { //ฟังก์ชันแสดงสถานะของเครื่อง
    cout << "\n@@@\nstate:\n";
    cout << "\tpc " << state.pc << "\n"; //แสดงค่า pc
    cout << "\tmemory:\n";
    for (int i = 0; i < state.numMemory; i++) { //วนค่าหน่วยความจำที่มีคำสั่ง
        cout << "\t\tmem[" << setw(3) << i << "] " << state.mem[i] << "\n"; //แสดงค่าคำสั่งในหน่วยความจำ
    }
    cout << "\tregisters:\n";
    for (int i = 0; i < NUMREGS; i++) { //วนทุกค่า register
        cout << "\t\treg[" << i << "] " << state.reg[i] << "\n"; //แสดงค่า register ที่ i
    }
    cout << "end state\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) { //ตรวจสอบจำนวนอาร์กิวเมนต์
        cerr << "Usage: " << argv[0] << " <machine-code file>\n";
        return 1;
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) { //ตรวจสอบการเปิดไฟล์
        cerr << "Error: cannot open " << argv[1] << "\n"; //แสดงข้อความ error ถ้าเปิดไฟล์ไม่ได้
        return 1;
    }

    State state; //สร้าง State
    state.pc = 0; //เริ่มต้น pc ที่ 0
    state.mem.clear(); //ล้างหน่วยความจำ
    for (int i = 0; i < NUMREGS; i++){
        state.reg[i] = 0; //ตั้งค่า register ทุกตัวเป็น 0
    }

    int val; //ตัวแปรชั่วคราว
    while (infile >> val) {
        state.mem.push_back(val); //อ่านคำสั่งจากไฟล์และเก็บในหน่วยความจำ
    }

    state.numMemory = state.mem.size(); //เก็บจำนวนคำสั่งในหน่วยความจำ
    int instrCount = 0; //นับจำนวนคำสั่งที่ประมวลผล
    bool halted = false; //สถานะหยุดการทำงาน

    while (!halted) { //ลูปจนกว่าจะหยุดการทำงาน
        printState(state);

        int pc = state.pc; //เก็บค่า pc ปัจจุบัน
        if (pc < 0 || pc >= (int)state.mem.size()) { //ตรวจสอบค่า pc ว่าอยู่ในขอบเขตมั้ย
            cerr << "Error: pc out of range: " << pc << "\n";
            return 1;
        }

        int instr = state.mem[pc]; //ดึงคำสั่งจากหน่วยความจำตามค่า pc
        unsigned int uinstr = static_cast<unsigned int>(instr); //แปลงคำสั่งเป็น unsigned int
        int opcode = uinstr >> 22; //ดึง opcode จากคำสั่ง
        int regA   = (uinstr >> 19) & 0x7; //ดึง register A
        int regB   = (uinstr >> 16) & 0x7; //ดึง register B
        int dest   = uinstr & 0x7; //ดึง register destination
        int off16  = uinstr & 0xFFFF; //ดึง offset 16 บิต
        int off32  = convertNum(off16); //แปลง offset เป็น 32 บิต

        int nextPC = pc + 1; //คำนวณค่า pc ถัดไป
        instrCount++; //เพิ่มจำนวนคำสั่งที่ประมวลผล

        switch (opcode) {
            case 0:{ //add
                state.reg[dest] = state.reg[regA] + state.reg[regB]; //บวกค่าใน register A กับ B แล้วเก็บใน dest
                state.pc = nextPC; //อัพเดต pc
                break;
            }
            case 1:{ //nand
                state.reg[dest] = ~(state.reg[regA] & state.reg[regB]); //ทำ nand ระหว่าง register A กับ B แล้วเก็บใน dest
                state.pc = nextPC; //อัพเดต pc
                break;
            }
            case 2:{ //lw
                int addr = state.reg[regA] + off32; //คำนวณที่อยู่โดยใช้ register A และ offset
                if (addr < 0 || addr >= NUMMEMORY) { //ตรวจสอบขอบเขต
                    cerr << "Error: lw address out of range\n";
                    return 1;
                }
                state.reg[regB] = state.mem[addr]; //โหลดค่าจากหน่วยความจำไปยัง register B
                state.pc = nextPC; //อัพเดต pc
                break;
            }
            case 3:{//sw
                int addr = state.reg[regA] + off32; //คำนวณที่อยู่โดยใช้ register A และ offset
                if (addr < 0 || addr >= NUMMEMORY) { //ตรวจสอบขอบเขต
                    cerr << "Error: sw address out of range\n";
                    return 1;
                }
                if (addr >= (int)state.mem.size()) state.mem.resize(addr + 1, 0); //ขยายขนาดหน่วยความจำ
                state.mem[addr] = state.reg[regB]; //เก็บค่าจาก register B ไปยังหน่วยความจำ
                state.pc = nextPC; //อัพเดต pc
                break;
            }
            case 4: {//beq
                if (state.reg[regA] == state.reg[regB]) //ตรวจสอบเงื่อนไข
                    state.pc = nextPC + off32; //ถ้าเท่ากันกระโดดไปที่ pc ใหม่
                else
                    state.pc = nextPC; //ถ้าไม่เท่ากันไปที่ pc ถัดไป
                break;
            }
            case 5: {//jalr
                int next = nextPC;
                int target = state.reg[regA]; //เก็บค่าจาก register A เป็น target pc
                state.pc = target; //อัพเดต pc เป็น target
                state.reg[regB] = next; //เก็บค่าของ next pc ใน register B
                break;
            }
            case 6: {//halt
                state.pc = nextPC; //อัพเดต pc เป็น pc ถัดไป
                cout << "machine halted\n";
                cout << "total of " << instrCount << " instructions executed\n"; //แสดงจำนวนคำสั่งที่ประมวลผล
                cout << "final state of machine:\n";
                printState(state); //แสดงสถานะตอนจบ
                halted = true; //หยุดการทำงาน
                break;
            }
            case 7:{ //noop
                state.pc = nextPC; //อัพเดต pc เป็น pc ถัดไป
                break;
            }
            default:
                cerr << "Error: unknown opcode " << opcode << "\n"; //แสดงข้อความ error ถ้าเจอ opcode ที่ไม่รู้จัก
                return 1;
        }

        state.reg[0] = 0; //ตั้งค่า register 0 เป็น 0
    }

    return 0;
}

//g++ simulator.cpp -o simulator.exe
//./simulator.exe D:\comp-arch\part1\machinecode.txt
//.\simulator.exe "D:\comp-arch\part1\machinecode.txt" > output.txt 
// คำสั่งรัน 

