#include "stdafx.h"
#include "Errors.h"

/*
NAME

    Errors Class

SYNOPSIS

    Constructor:

        No constructor avaliable

DESCRIPTION

    The Errors class which manages error reporting. Currently, this class initializes a vector object
    which we can then record a error to. It also supports displaying all error messages. All class 
    members (functions & variables) are static to allow access anywhere.
    
*/

vector<string> Errors::m_ErrorMsgs;

/*
NAME

    InitErrorReporting() - Clears vector containing error messages

SYNOPSIS

    void InitErrorReporting();
            - no argouments avaliable

DESCRIPTION

    Clears m_ErrorMsgs string vector.

*/
void Errors::InitErrorReporting()
{
    m_ErrorMsgs.clear();
}

/*
NAME

    SetVectorSize - reserves a_num strings in memory for m_ErrorMsgs

SYNOPSIS

    void SetVectorSize(int a_num);
        a_num       -> intended size of the vector

DESCRIPTION

    This function uses the .reserve method from the vector class to efficiently
    use memory

*/
void Errors::SetVectorSize(int a_num)
{
    m_ErrorMsgs.reserve(a_num);
}

/*
NAME

    RecordError - Records an error

SYNOPSIS

    void RecordError(int lineNumber, string a_emsg);
        lineNumber      ->  Line in file which contains the error
        a_emsg          ->  Error message

DESCRIPTION

    This function takes in two parameters, a line number and a error message. It pushes
    a error message which uses the format as follows:

        LINE 5 - ERROR: assmembly instruction "END" not detected

*/

void Errors::RecordError(int lineNumber, string a_emsg)
{
    auto line = to_string(lineNumber);
    string errorMessage = "\tLINE " + line + " - ERROR: " + a_emsg;
    m_ErrorMsgs.emplace_back(errorMessage);
}

/*
NAME

    DisplayErrors - Displays all errors

SYNOPSIS

    void DisplayErrors();
            - no argouments avaliable

DESCRIPTION

    This function iterates through the m_ErrorMsgs vector and outputs all elements.

*/
void Errors::DisplayErrors()
{
    for (string i : m_ErrorMsgs)
        cout << i << endl;
    cout << endl;
}
