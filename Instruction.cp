#include "stdafx.h"
#include "Instruction.h"

/*
NAME

    Instruction Class

SYNOPSIS

    Constructor:

        No constructor avaliable

     Initialized within the following file: Assembler.h

DESCRIPTION

    The Instruction Class contains all functions dealing with parsing line input into labels,
    op codes, and operands. It also has functionality to determine whether a instruction is a 
    assembly language instruction or a machine language instruction with basic error checking.

*/

// parse an instruction and return its type.  Yes, I want a call by value.  Why??
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    // Record the original statement.  This will be needed in the sceond pass.
    m_instruction = a_line;

    // Delete any comment from the line.
    DeleteComment(a_line);

    // Record label, opcode, and operands.  Up to you to deal with formatting errors.
    bool isFormatError = RecordFields(a_line);

    // Check if this is a comment.
    if (m_Label.empty() && m_OpCode.empty())
    {
        return InstructionType::ST_Comment;
    }
    else if (IsCorrectMachineLanguage(m_OpCode))
    {
        return InstructionType::ST_MachineLanguage;
    }
    else if (IsEND(m_OpCode))
    {
        m_type = InstructionType::ST_End;
    }
    else if (IsCorrectAssemblyLanguage(m_OpCode))
    {
        m_type = InstructionType::ST_AssemblerInstr;
    }
    else {
        m_type = InstructionType::ST_Error;
    }
    // Return the instruction type.  This has to be handled in the code.
    return m_type;
}
// Record the fields that make up the instructions.
bool Instruction::RecordFields( const string &a_line )
{
    // Get the fields that make up the instruction.
    bool isFormatError = ! ParseLineIntoFields( a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);

    // if code was a comment, there is nothing to do.
    if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

    // Record whether the operands are numeric and their value if they are.
    m_IsNumericOperand1 = isStrNumber(m_Operand1);
    if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

    m_IsNumericOperand2 = isStrNumber(m_Operand2);
    if (m_IsNumericOperand2) m_Operand2NumericValue = stoi(m_Operand2);

    // For the sake of comparing, convert the op code to upper case.
    for (char& c : m_OpCode)
    {
        c = toupper(c);
    }
    return true;
}

// Parse the intruction into fields.
bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
{
    // Get rid of any commas from the line.
    replace(a_line.begin(), a_line.end(), ',', ' ');

    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    string endStr;
    a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
    istringstream ins(a_line);
    if (a_line[0] == ' ' || a_line[0] == '\t')
    {
        a_label = "";
        ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    else
    {
        ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    // If there is extra data, return false.
    return endStr.empty() ? true : false;
}

bool Instruction::isStrNumber(const string& a_str)
{
    if (a_str.empty()) return false;

    // If there is a - or a plus, make sure there are more characters.
    int ichar = 0;
    if (a_str[0] == '-' || a_str[0] == '+')
    {
        ichar++;
        if (a_str.length() < 2) return false;
    }
    // Make sure that the remaining characters are all digits
    for (; ichar < a_str.length(); ichar++)
    {
        if (!isdigit(a_str[ichar])) return false;
    }
    return true;
}

/*
NAME

    FetchOpCodeEquvalent - Determines the numeric OpCode

SYNOPSIS

    long long FetchOpCodeEquvalent(string a_OpCode)
        a_OpCode    -> machine language opCode (non numeric)

DESCRIPTION

    This function takes in an string OpCode and determines the numeric equivalent of it
    using the m_MLInstructions array which stores all OpCodes

RETURNS

    Equivalent Numeric OpCode for inputed OpCode
*/
long long Instruction::FetchOpCodeEquvalent(string a_OpCode)
{
    for (int index = 0; index < sizeof(m_MLInstructions); index++)
    {
        if (m_MLInstructions[index] == a_OpCode)
        {
            return (index + 1) * 10;
        }
    }
    // Represents error conditon
    return -1;
}


/*
NAME

    IgnoreRegisterCheck - Determines whether instruction register matters

SYNOPSIS

    long long FetchOpCodeEquvalent(string a_OpCode)
        a_OpCode    -> machine language opCode (non numeric)

DESCRIPTION

    This function takes an OpCode and determines whether the function registers matter.
    The instructions that can be seen as follows (READ, WRITE, B, HALT) aren't affected by
    the register value.

RETURNS

    Equivalent Numeric OpCode for inputed OpCode
*/
bool Instruction::IgnoreRegisterCheck(string a_OpCode)
{
    if (a_OpCode == "READ" || a_OpCode == "WRITE" || a_OpCode == "B" || a_OpCode == "HALT")
    {
        return true;
    }
    return false;
}

/*
NAME

    IsMachineLanguage - Determines if instruction is a AssemblyLanguage instruction and checks operands

SYNOPSIS

    bool IsCorrectAssemblyLanguage(string a_OpCode)
        a_OpCode    -> machine language opCode (non numeric)

DESCRIPTION

    This function checks if the inputed current OpCode is a Assembly instruction and checks both operand1 and operand2.
    Does basic checks for formatting errors

RETURNS

    Boolean:
        True -> If both operands are empty & opCode equals "END"
        False -> If one of the above conditions isn't met

*/
bool Instruction::IsCorrectMachineLanguage(string a_OpCode)
{
    for (const string& element : m_MLInstructions)
    {
        if (element == a_OpCode)
        {
            if (a_OpCode == "ADDR" || a_OpCode == "SUBR" || a_OpCode == "MULTR" || a_OpCode == "DIVR")
            {
                if (!m_IsNumericOperand1 || !m_IsNumericOperand2)
                {
                    return false;
                }
            }
            else if (a_OpCode != "READ" && a_OpCode != "WRITE" && a_OpCode != "B" && a_OpCode != "HALT")
            {
                if (!m_IsNumericOperand1 || m_IsNumericOperand2)
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

/*
NAME

    IsCorrectAssemblyLanguage - Determines if instruction is a AssemblyLanguage instruction and checks operands

SYNOPSIS

    bool IsCorrectAssemblyLanguage(string a_OpCode)
        a_OpCode    -> machine language opCode (non numeric)

DESCRIPTION

    This function checks if the inputed current OpCode is a Assembly instruction and checks both operand1 and operand2.
    Operand1 should always be an integer and have value. Operand2 should always be empty.

RETURNS

    Boolean:
        True -> If both operands are empty & opCode equals "END"
        False -> If one of the above conditions isn't met

*/
bool Instruction::IsCorrectAssemblyLanguage(string a_OpCode)
{
    for (const string& element : m_ALInstructions)
    {
        if (element == a_OpCode && !m_Operand1.empty() && isStrNumber(m_Operand1) && m_Operand2.empty())
        {
            return true;
        }
    }
    return false;
}

/*
NAME

    IsEND - Determines if instruction is of type Instruction::InstructionType::ST_END and checks parameters

SYNOPSIS

    bool IsEND(string a_OpCode)
        a_OpCode    -> machine language opCode (non numeric)

DESCRIPTION

    This function determines if the instruction is of type Instruction::InstructionType::ST_END
    and checks both variables, m_Operand1 and m_Operand2 which should both be empty. 

RETURNS

    Boolean:
        True -> If both operands are empty & opCode equals "END"
        False -> If one of the above conditions isn't met

*/
bool Instruction::IsEND(string a_OpCode)
{
    if (a_OpCode == "END" && m_Operand1.empty() && m_Operand2.empty())
        return true;
    return false;
}

/*
NAME

    LocationNextInstruction - Determine the location of the next instruction 

SYNOPSIS

    int LocationNextInstruction( int a_loc );
        a_loc		-> the location to be associated with the OpCode. 

DESCRIPTION

    This function determines the location of the next instruction based on the instruction
    type and current location.

 RETURNS

    Location of instruction
        - Assembly Language Instruction:
            * ORG (Origin)          -> returns integer equivalent of origin when inputed
            * DS (Define Storage)   -> returns a_loc + integer equivalent of DS operand
        - Machine Language Instruction:
            * All:                  -> returns a_loc + 1


    
*/
int Instruction::LocationNextInstruction( int a_loc )
{

    // std::stoi converts a std::string object to its equivalent integer.
    if (m_OpCode == "ORG" && isStrNumber(m_Operand1) )
    {
        return stoi( FetchOperand1() );
    }
    else if (m_OpCode == "DS" && isStrNumber(m_Operand1))
    {
        return stoi( FetchOperand1() ) + a_loc;
    }

    return a_loc + 1;
}
