//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
// ^ * * * * * * * * * * * DELETE THIS EVENTUALLY
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>

class Errors {

public:

    // Initializes error reports.
    static void InitErrorReporting();

    // Sets the size of m_ErrorMsgs, so we can be as efficient as possible (we don't have to 
    // string elements from vector to vector if the error messages pop in
    static void SetVectorSize(int a_num);

    // Records an error message.
    static void RecordError(int lineNumber, string a_emsg);

    // Displays the collected error message.
    static void DisplayErrors();

    // Returns true when m_ErrorMsgs' size > 0 (there is an error)
    static bool HasError()
    {
        return !m_ErrorMsgs.empty();
    }

private:

    static vector<string> m_ErrorMsgs;  // This must be declared in the .cpp file.  Why?
};
#endif