#ifndef _FILEACCESS_H  // This is the way that multiple inclusions are defended against often used in UNIX
#define _FILEACCESS_H  // We use pragmas in Visual Studio and g++.  See other include files

#include <fstream>
#include <stdlib.h>
#include <string>

class FileAccess {

public:

    // Opens the file.
    FileAccess( int argc, char *argv[] );

    // Closes the file.
    ~FileAccess( );

    // Get the next line from the source file. Returns true if there was one.
    bool GetNextLine( string &a_line );

    // accessor for m_lineNumber
    inline int &FetchLineNumber()
    {
        return m_lineNumber;
    }

    // increments m_lineNumber
    inline void IncrementLineNumber()
    {
        m_lineNumber += 1;
    }
    // resets current line variable
    inline void ResetLineNumber()
    {
        m_lineNumber = 0;
    }

    // Put the file pointer back to the beginning of the file.
    void Rewind( );

private:

    ifstream m_sfile;		// Source file object.
    int m_lineNumber = 0;
};
#endif

