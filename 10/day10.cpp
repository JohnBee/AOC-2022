#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
using namespace std;


// class Instruction{
//     public:
//         Instruction(size_t cycles, string name, size_t nOperand){
//             this->cycles = cycles;
//             this->name = name;
//         }
//     private:
//         size_t cycles;
//         string name;
//     virtual void execute(CPU & cpu){

//     }
// };
class CPU;

class Instruction {
    public:
        size_t cycles;
        Instruction(void (*opcode)(CPU * cpu, vector<int> & args), vector<int> operands, size_t cycles){
            fncPtr = opcode;
            args = operands;
            this->cycles = cycles;
        }
        void execute(CPU * CPU){
            fncPtr(CPU, args);
        }
    private:
        void (*fncPtr)(CPU * cpu, vector<int> & args);
        vector<int> args; 
};

class CPU{
    public:
        vector<int> registers;
        size_t cycles = 0;
        size_t beforeExecutionCycle = 0;
        size_t afterExecutionCycle = 0;
        Instruction * toExecute = nullptr;
        CPU(vector<Instruction> program, size_t nRegisters=1) {
            registers = vector<int>(nRegisters, 1);
            this->program = program;
        }
        bool stepProgram(){            
            if(pc < program.size()){
                Instruction instr = program.at(pc);
                afterExecutionCycle = beforeExecutionCycle + instr.cycles;
                if(cycles == afterExecutionCycle){
                    instr.execute(this);
                    beforeExecutionCycle = cycles;
                    pc++;
                }
                cycles++;
                return true;
            }
            
            return false;
        }
        void runProgram(){
            while(pc < program.size()){
                stepProgram();
            }
        }
    private:
        size_t pc = 0;
        vector<Instruction> program;
    
};

void noop(CPU * cpu, vector<int> & args){
    return; // 1 cycle to complete
}

void addx(CPU * cpu, vector<int> & args){
    cpu->registers.at(0) += args.at(0);
    return; // two cycles
}


vector<string> SplitRegexString(string regExp, string s){
    regex s_regex(regExp);
    vector<string> foundStr(sregex_token_iterator(s.begin(), s.end(), s_regex, -1), sregex_token_iterator());
    return foundStr;
}
vector<Instruction> loadInput(string filename){
    fstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<Instruction> outInstructions;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            vector<string> instrStrings = SplitRegexString("\\s", line);
            if(instrStrings.at(0) == "noop"){
                outInstructions.push_back(Instruction(&noop, {}, 1));
            }
            if(instrStrings.at(0) == "addx"){
                vector<int> args;
                for(size_t i = 1; i < instrStrings.size(); i++){
                    args.push_back(stoi(instrStrings.at(i)));
                }
                outInstructions.push_back(Instruction(&addx, args, 2));
            }
        }
    }
    file.close();
    return outInstructions;
}
int part1(vector<Instruction> instr){
    CPU cpu(instr, 1);
    vector<int> signalStrength;
    vector<int> signalChecks = {20,60,100,140,180,220};
    while(cpu.stepProgram()){
        if(find(signalChecks.begin(), signalChecks.end(), cpu.cycles)!=signalChecks.end()){
            signalStrength.push_back(cpu.registers.at(0)*cpu.cycles);
        }
    }
    int sum = 0;
    for(int i : signalStrength){
        sum += i;
    }
    return sum;

}
void part2(vector<Instruction> instr){
    CPU cpu(instr, 1);
    int beamPos = 0;
    while(cpu.cycles < 240){
        cpu.stepProgram();
        int x = cpu.registers.at(0);
        vector<int> sprite = {x-1,x,x+1};
        
        if(find(sprite.begin(), sprite.end(), beamPos)!=sprite.end()){
            cout << '#';
        }
        else{
            cout << '.';
        }
        
        if(cpu.cycles % 40 == 0 && cpu.cycles > 0){
            cout << endl;
        }
        beamPos = (beamPos+1) % 40;
        
        
    }

    return;
}
    
int main(int argc, char const *argv[]){
    /* code */
    vector<Instruction> instr =  loadInput("./input");
    cout << "Part 1: " << part1(instr) << endl;
    part2(instr);
    return 0;
}
