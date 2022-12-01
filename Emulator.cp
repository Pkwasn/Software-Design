#include "stdafx.h"
#include "Emulator.h"


/*
NAME

	insertMemory - Inserts memory at the location

SYNOPSIS

	void :insertMemory(int a_location, long long a_contents)
		a_location		-> location of instruction in memory
		a_contents		-> decimal representation of instruction

DESCRIPTION

	Inserts memory into m_memory vector at specified location
	
RETURNS

	True	-> if memory was able to be inserted
	False	-> if memory was not able to be inserted (some error)
*/
bool emulator::insertMemory(int a_location, long long a_contents)
{
	if (a_location >= MEMSZ || a_contents > 180'000'000 || m_memory[a_location] != 0)
	{
		return false;
	}
	m_memory[a_location] = a_contents;  
	return true;
}

/*
NAME

	runProgram - Runs emulator

SYNOPSIS

	bool runProgram()
		- no argouments avaliable

DESCRIPTION

	Runs emulator. Takes translated machine/assembly language and runs in 1,000'000 locations of long long memory

RETURNS

	True	-> if assembler was terminated using HALT machine language instruction
	False	-> if assembler went through all indexs and never found halt instruction
*/
bool emulator::runProgram()
{
	int curAddress = 0;
	int register1 = 0;
	int register2 = 0;
	long long instruction = 0;
	int input = 0;

	for (int index = 0; index < MEMSZ; index++)
	{
		if (m_memory[index] != 0)
		{
			instruction = m_memory[index] / 10'000'000;
			curAddress = m_memory[index] % 1'000'000;
			register1 = (m_memory[index] % 10'000'000) / 1'000'000;
			register2 = (m_memory[index] % 1'000'000) / 100'000;
			//cout << instruction << " " << curAddress << " " << register1 << endl;

			switch (instruction)
			{
			case 1:
				// Reg <-- c(Reg) + c(ADDR)
				registers[register1] += m_memory[curAddress];
				break;
			case 2:
				// Reg <-- c(Reg) - c(ADDR)
				registers[register1] -= m_memory[curAddress];
				break;
			case 3:
				// Reg <-- c(Reg) * c(ADDR)
				registers[register1] *= m_memory[curAddress];
				break;
			case 4:
				// Reg < --c(Reg) / c(ADDR)
				registers[register1] /= m_memory[curAddress];
				break;
			case 5:
				// Reg <-- c(ADDR)
				registers[register1] = m_memory[curAddress];
				break;
			case 6:
				// ADDR <-- c(Reg)
				m_memory[curAddress] = registers[register1];
				break;
			case 7:
				// REG1 <--c(REG1) + c(REG2)
				registers[register1] += registers[register2];
				break;
			case 8:
				// REG1 <--c(REG1) - c(REG2)  
				registers[register1] -= registers[register2];
				break;
			case 9:
				// REG1 <--c(REG1) * c(REG2) 
				registers[register1] *= registers[register2];
				break;
			case 10:
				// REG1 <--c(REG1) / c(REG2)  
				registers[register1] /= registers[register2];
				break;
			case 11:
				// A line is read in and the number found there is recorded in the specified memory address
				cout << "? ";
				cin >> input;
				m_memory[curAddress] = input % 1'000'000;;
				break;
			case 12:
				// c(ADDR) is displayed
				cout << m_memory[curAddress] << endl;
				break;
			case 13:
				// go to ADDR for next instruction
				index = curAddress -1;
				break;
			case 14:
				// go to ADDR if c(Reg) < 0
				if (registers[register1] < 0)
				{
					index = curAddress -1;
				}
				break;
			case 15:
				// go to ADDR if c(Reg) = 0
				if (registers[register1] == 0)
				{
					index = curAddress -1;
				}
				break;
			case 16:
				// go to ADDR if c(Reg) > 0
				if (registers[register1] > 0)
				{
					index = curAddress -1;
				}
				break;
			case 17:
				// terminate execution
				return true;
			}
		}
	}
	return false;
}