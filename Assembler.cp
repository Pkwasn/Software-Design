#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

/*
NAME

    Assembler Class

SYNOPSIS

    Constructor:
    
        Assembler( int argc, char *argv[] );
            argc        -> number of command line arguments
            *argv       -> pointer to char array which should contain the name of the file we are processing
     
     Initialized within the following file: Assembler.h

DESCRIPTION

    The Assembler class contains methods which develop the symbol table, translate the program into a decimal representation,
    and ultimately, runs the assembler with the translation
    
*/


// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor. 
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  

/*
NAME

    PassI - establishes the location of the symbols

SYNOPSIS

    void PassI();
        - no argouments avaliable

DESCRIPTION

    This function iterates through the m_facc object (FileAccess Object) until it reaches the "END" OpCode
    or there are no more lines ( Which will be reported as an error in PassII(). ). While iterating through,
    PassI will parse the line for a Label, OpCode, Operand1, and Operand2 in which it will pass the location
    of the Label alongside its respective address to the m_symtab object (SymbolTable Object).

*/

void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.
    int lineNumber = 0;

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line;
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End)
        {
            // Reserve lineNumber spots in vector (there can only be as many errors as there is lines [probably
            // not true but better than copying at 1,2,4,8,16... and losing efficiency])
            lineNumber = m_facc.FetchLineNumber();
            Errors::SetVectorSize(lineNumber);
            return;
        }

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::InstructionType::ST_Comment )  
        {
        	continue;
	    }
        // Handle the case where there is an error.
        
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

/*
NAME

    PassII - generates a translation

SYNOPSIS

    void PassII();
        - no argouments avaliable

DESCRIPTION

    PassII translates the machine & assembly language into a decimal representation. Does
    Error checking for simple cases, leaving the more complicated cases for the other translation
    functions which are called within.

*/

void Assembler::PassII()
{
    // Go to the beginning of the file
    m_facc.Rewind();

    cout << "Translation of Program:\n" << endl;
    cout << "Location\tContents\tOriginal Statement" << endl;

    int loc = 0;                // Tracks the location of the instructions to be generated.
    int lineNumber = 0;         // Tracks the current line in the file

    Errors::InitErrorReporting();

    // Successively process each line of source code.
    for (; ; ) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {
            Errors::RecordError((lineNumber), "Assmembly instruction \"END\" not detected or Incorrectly formated.");
            return;
        }

        lineNumber = m_facc.FetchLineNumber();

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        if (st == Instruction::InstructionType::ST_Error)
        {
            Errors::RecordError(lineNumber, "Line is neither an assembly or machine langauge instruction or has incorrect operands");
            cout << "\t\t\t\t";
            m_inst.DisplayOriginalInstruction();
        }
        else if (st == Instruction::InstructionType::ST_End)
        {
            if (m_facc.GetNextLine(line))
            {
                Errors::RecordError(lineNumber, "Instruction following END instruction");
            }
            cout << "\t\t\t\t";
            m_inst.DisplayOriginalInstruction(); // puts end really far out for some reason
            return;
        }
        else if (st == Instruction::InstructionType::ST_Comment)
        {
            cout << "\t\t\t\t";
            m_inst.DisplayOriginalInstruction();
            continue;
        }
        else if (st == Instruction::InstructionType::ST_AssemblerInstr)
        {
            TranslateALInstruction(loc);
        }
        else if (st == Instruction::InstructionType::ST_MachineLanguage)
        {
            TranslateMLInstruction(loc);
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}

/*
NAME

    TranslateALInstruction - translates Assembly Language Instruction

SYNOPSIS

    void TranslateALInstruction(int &a_Location)
        - a_location        -> current location of instruction

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void Assembler::TranslateMLInstruction(int a_Location)
{
    long long numericOpCode = m_inst.FetchOpCodeEquvalent(m_inst.FetchOpCode());
    string stringOpCode = m_inst.FetchOpCode();

    if (numericOpCode == -1)
        Errors::RecordError(m_facc.FetchLineNumber(), "Instruction has no numeric OpCode equivalent");

    int currentRegister = m_inst.FetchCurrentRegister(); // last register is the default register (10 registers)

    if (m_inst.IsNumericOperand1() && m_inst.IsNumericOperand2())
    {
        long long register1 = stol(m_inst.FetchOperand1(), nullptr, 10);
        long long register2 = stol(m_inst.FetchOperand2(), nullptr, 10);

        numericOpCode += register1;
        numericOpCode *= 10;
        numericOpCode += register2;
        numericOpCode *= 100'000;
    }
    else
    {
        if (m_inst.IsNumericOperand1() && !m_inst.IgnoreRegisterCheck(stringOpCode))
        {
            currentRegister = stol(m_inst.FetchOperand1(), nullptr, 10);

            if (currentRegister < 0 || currentRegister > 9)
            {
                Errors::RecordError(m_facc.FetchLineNumber(), "VC8000 Assembly only has 10 registers (0-9)");
            }

            m_inst.SetCurrentRegister(currentRegister);
        }

        numericOpCode += currentRegister;
        numericOpCode *= 1'000'000;

        if (!m_inst.IsNumericOperand1() && m_symtab.LookupSymbol(m_inst.FetchOperand1()))
        {
            numericOpCode += m_symtab.GetLocation(m_inst.FetchOperand1());
        }
        else if (!m_inst.IsNumericOperand2() && m_symtab.LookupSymbol(m_inst.FetchOperand2()))
        {
            numericOpCode += m_symtab.GetLocation(m_inst.FetchOperand2());
        }
    }

    if (!m_emul.insertMemory(a_Location, numericOpCode)) // send constant to memory location in emulator
    {
        Errors::RecordError(m_facc.FetchLineNumber(), "Unable to insert into emulator memory");
    }
    cout << a_Location << "\t\t" << setfill('0') << setw(9) << numericOpCode << "\t";
    m_inst.DisplayOriginalInstruction();
}

/*
NAME

    TranslateALInstruction - translates Assembly Language Instruction

SYNOPSIS

    void TranslateALInstruction(int a_Location)
        - a_location        -> current location of instruction

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void Assembler::TranslateALInstruction(int a_Location)
{
    string opCode = m_inst.FetchOpCode(); // Get OpCode

    if (opCode == "DS" || opCode == "ORG")
    {
        cout << a_Location << "\t\t\t\t";
    }
    else if (opCode == "DC")
    {
        long long contents = stol(m_inst.FetchOperand1(), nullptr, 10); // convert string to type long long
        if (!m_emul.insertMemory(a_Location, contents)) // send constant to memory location in emulator
        {
            Errors::RecordError(m_facc.FetchLineNumber(), "Unable to insert into emulator memory");
        }
        cout << a_Location << "\t\t" << setfill('0') << setw(9) << contents << "\t";
    }
    m_inst.DisplayOriginalInstruction();
}

void Assembler::RunProgramInEmulator()
{
    if (Errors::HasError())
    {
        ConfirmErrorMessage();
    }
    cout << "__________________________________________________________\n" << endl;
    Wait();
    cout << "Results from emulating program:\n" << endl;

    if (m_emul.runProgram())
    {
        cout << "\n End of emulation" << endl;
    }
}

/*
NAME

    ConfirmErrorMessage() - Warns user of error

SYNOPSIS

    void :ConfirmErrorMessage()
        - no argouments avaliable

DESCRIPTION

    This function displays a warning message to the user, so they can decide whether to terminate the
    program and check if their input file is correct.
*/
void Assembler::ConfirmErrorMessage()
{
    cout << "__________________________________________________________\n" << endl;
    cout << "Input file has the following errors:\n" << endl;

    Errors::DisplayErrors();

    cout << "Would you like to continue with translation? Terminate otherwise." << endl;

}

/*
NAME

    Wait() - Waits for user input

SYNOPSIS

    void Wait()
        - no argouments avaliable

DESCRIPTION

    This function waits for the user to press the enter key. Allows user to logically understand
    each section before continuing to the next.
*/
void Assembler::Wait()
{
    cout << "Press Enter to continue..." << endl;
    cin.ignore();
}