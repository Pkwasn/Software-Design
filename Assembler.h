#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII( );

    // Display the symbols in the symbol table
    void DisplaySymbolTable( ) 
    { 
        m_symtab.DisplaySymbolTable();
        cout << "__________________________________________________________\n" << endl;
        Wait();
    }
    
    // Run emulator on the translation.
    void RunProgramInEmulator( );

    // Assembler halts until enter key is pressed
    void Wait( );

    // Warning message
    void ConfirmErrorMessage( );

    // Translates Machine Language to decimal representation
    void TranslateMLInstruction(int a_Location);

    // Translates Assembly Language to decimal representation
    void TranslateALInstruction(int a_Location);

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
};

