#pragma once

class Instruction {

public:

    // Instruction Types
    enum class InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End,                 // end instruction.
        ST_Error                // Statement has an error.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    // To access the label
    inline string &GetLabel( ) {

        return m_Label;
    };
    // To determine if a label is blank.
    inline bool isLabel( ) {

        return ! m_Label.empty();
    };

    // To access the OpCode
    inline string &FetchOpCode()
    {
        return m_OpCode;
    }

    // To access Operand1
    inline string &FetchOperand1()
    {
        return m_Operand1;
    }

    // To access Operand2
    inline string &FetchOperand2()
    {
        return m_Operand2;
    }

    inline int &FetchCurrentRegister()
    {
        return m_CurrentRegister;
    }

    inline void SetCurrentRegister(int a_Value)
    {
        m_CurrentRegister = a_Value;
    }

    inline bool IsNumericOperand1()
    {
        return m_IsNumericOperand1;
    }

    inline bool IsNumericOperand2()
    {
        return m_IsNumericOperand2;
    }

    // outputs original instruction
    inline void DisplayOriginalInstruction()
    {
        cout << m_instruction << endl;
    }

    long long FetchOpCodeEquvalent(string a_OpCode);

    bool IgnoreRegisterCheck(string a_OpCode);

private:
    
    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand1;     // The first operand. 
    string m_Operand2;     // The second operand.

    string m_instruction;    // The original instruction.

    // List of all Machine Language instructions
    string m_MLInstructions[17] = { "ADD","SUB","MULT","DIV","LOAD","STORE","ADDR", "SUBR", "MULTR",
        "DIVR","READ","WRITE","B","BM","BZ","BP","HALT" };

    // List of all Assembly Language Instructions
    string m_ALInstructions[4] = { "DC","DS","ORG"};

    // Register for the purpose of translation
    int m_CurrentRegister = 9;

    // Derived values.
    int m_NumOpCode = 0;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type = InstructionType::ST_Error; // The type of instruction.

    bool m_IsNumericOperand1 = false;// == true if the operand 1 is numeric.
    int m_Operand1NumericValue = 0;   // The value of the operand 1 if it is numeric.
    
    bool m_IsNumericOperand2 = false;// == true if the operand 2 is numeric.
    int m_Operand2NumericValue = 0;   // The value of the operand 2 if it is numeric.

    // Delete any comments from the statement.
    void DeleteComment(string &a_line)
    {
        size_t isemi1 = a_line.find(';');
        if (isemi1 != string::npos)
        {
            a_line.erase(isemi1);
        }
    }
    // Record the fields of the instructions.
    bool RecordFields( const string &a_line );

    // Get the fields that make up the statement.  This function returns false if there
    // are extra fields.
    bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
        string& a_Operand1, string& a_Operand2);

    // Check if a string contains a number. 
    bool isStrNumber(const string& a_str);

    // Basic error checking & checks if OpCode is a machine language instruction
    bool IsCorrectMachineLanguage(string a_OpCode);
    
    // Basic error checking & checks if OpCode is a assembly language instruction
    bool IsCorrectAssemblyLanguage(string a_OpCode);

    bool IsEND(string a_OpCode);

};
