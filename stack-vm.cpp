#include "stack-vm.h"

/*
    Instruction format 
    header: 2 bits / tells what type it is. 
    data: 30 bits 

    header format: 
    0 => positive int 
    1 => primitive instruction
    2 => negative int
    3 => undefined 
    Look at get type to understand how we access them

    Extracted Type (Decimal) | Binary | Meaning
    0 | 00 | Positive integer
    1 | 01 | Primitive instruction
    2 | 10 | Negative integer
    3 | 11 | Undefined

*/

StackVM::StackVM() 
{
    // We use reserve to avoid reallocation. This causes allocating new memory, coping all existing elements and freeing old memory, which can be expensive.  
    memory.reserve(1000000);
}
i32 StackVM::getType(i32 instruction)
{
    // 4 bytes -> 32 bits 
    // 0x -> hexa 
    // each hex digit represents 4 bits 
    // C0000000 ->  8 * 4
    // 0xC0000000 = 1100 0000 0000 0000 0000 0000 0000 0000
    //        = 32 bits 
    // top 8 bits (24-31) represent the type or opcode
    // lower 24 bits (0-23) are data or payload

    // This sets a bitmask which extracts the top 2 bits of any instruction. It isolates the bits you care about by zeroing out the rest. 
    i32 type = 0xc0000000;
    // shifted 30 bits to the right, the first two bits are moved to the right
    type = (type & instruction) >> 30;
    return type; 
}
i32 StackVM::getData(i32 instruction)
{
    // This bitmask in binary is 0011 1111 1111 1111 1111 1111 1111 1111
    // it's the opposite of 0xc0000000, masks out the top 2 bits. 
    i32 data = 0x3fffffff;
    // Performs bitwise and. It keeps only the bits in instrution that match 1s in the mask 
    // example with 0x8000000A instruction 
    // mask out first 2, then keep the rest of the 30 elements. 
    // 1000 0000 0000 0000 0000 0000 0000 1010
    // AND
    // 0011 1111 1111 1111 1111 1111 1111 1111
    // -------------------------------------
    // 0000 0000 0000 0000 0000 0000 0000 1010

    data = data & instruction; 
    return data; 
}

// Gets next instruction from memory. 
void StackVM::fetch()
{   
    // When we call the run function pc is -1, this makes sure the actual instruction is picked
    pc++; 
}

// Extracts the opcode/type usings masks. What's the instruction?
void StackVM::decode()
{
    typ = getType(memory[pc]); 
    dat = getData(memory[pc]);
}

// 
void StackVM::execute()
{
    if (typ == 0 || typ == 2) // It's an integer positive or negative. This of when we push to stack
    {
        sp++;
        memory[sp] = dat; // sets the top of stack to data. Stack starts at 0 and goes up
    } 
    else
    {
        doPrimitive();
    }
}

void StackVM::doPrimitive()
{
    switch (dat) 
    {
        case 0: // Operand is 0, data part of the primitive => 0x40000000
            std::cout << "halt" << std::endl;
            running = 0; 
            break;
        case 1: // add, operand is 1 => 0x40000001
            std::cout << "add " << memory[sp -1] << " " << memory[sp]<< std::endl;
            memory[sp - 1] = memory[sp - 1] + memory[sp]; // pop two things from stack, put back on the stack
            sp--;
            break; 
    }
}

void StackVM::run()
{
    pc -= 1; // first time we fetch 1 minus whatever it was before. Substracting 1 means fetch() will land on the actual instruction. See the fetch() above
    while (running == 1)
    {
        fetch();
        decode();
        execute();
        std::cout << "tos: " << memory[sp] << std::endl;
    }
}

void StackVM::loadProgram(std::vector<i32> prog)
{
    for(i32 i = 0; i < prog.size(); i++)
    {
        memory[pc + i] = prog[i];
    }
}

