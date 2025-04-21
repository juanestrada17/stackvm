#include <iostream> 
#include <vector> 
#include <cstdint>

// Type definitions 
// signed integer of 32 bits size -> everything will be 32 bits  
typedef int32_t i32;

class StackVM 
{
    // program counter at address 100. 100 means that the program beings executing at address 100, first 100 memory slots could be reserved for something else. 
    // like data or stack space. 
    i32 pc = 100;
    // stack pointer. Init at 0, grows from bottom of memory upwards. It keeps track of where the top of the stack is. 
    i32 sp = 0; 
    std::vector<i32> memory;
    i32 typ = 0; // type  
    i32 dat = 0; // data
    i32 running = 1; // is it still running 

    // private function 
    i32 getType(i32 instruction); // takes type a token/instruction
    i32 getData(i32 instruction);
    
    // cpu infinity loop 
    void fetch();
    void decode();
    void execute(); 
    void doPrimitive(); 
    
    public: 
    StackVM();
    void run(); 
    void loadProgram(std::vector<i32> prog); // initializes the memory . 


};

