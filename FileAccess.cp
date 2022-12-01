#include "stdafx.h"
#include "FileAccess.h"

/*
NAME

    FileAccess Class

SYNOPSIS

    Constructor:

        FileAccess( int argc, char *argv[] )
            argc        -> number of command line arguments
            *argv       -> pointer to char array which should contain the name of the file we are processing

     Initialized within the following file: Assembler.h

DESCRIPTION

    The FileAccess class contains all functions dealing with reading data from file specified through
    the command line arguments. Currently, this class supports opening a file with error detection,
    closing a file, determining whether a file has any more lines, and a function to go back to the 
    beginning of the file

*/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit(1);
    }
    // Open the file.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}

/*
NAME

    GetNextLine - Get the next line from the source file. Some error checking.

SYNOPSIS

    bool GetNextLine( string &a_line )
        - &a_line       ->  line from file.

DESCRIPTION

    This function retrieves the lines from the file. It returns a boolean depending on whether there are 
    lines left in the file.

RETURNS

    Boolean:
        True -> Function was able to grab next line
        False -> No more lines are left in the file

*/
bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    FileAccess::IncrementLineNumber();
    // Return indicating success.
    return true;
}

/*
NAME

    Rewind  - Go back to beginning of file. Reset m_lineNumber.

SYNOPSIS

    bool Rewind( )
            - no argouments avaliable

DESCRIPTION

    This function resets inputed file so Assembler can through the file again. Called in PassII().
    Calls the ResetLineNumber() function to reset the variable containing the current line.

*/
void FileAccess::Rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
    FileAccess::ResetLineNumber();
}
    
