#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    SymbolTable Class

SYNOPSIS

    Constructor:

        No constructor avaliable

     Initialized within the following file: Assembler.h

DESCRIPTION

    Symbol Table: Table with following data
        Label       --> Name associated with OpCode. Used as the key in the m_symboltable objet (ordered map) * * * * * * * Further Define
        Location    --> Location of the label represented as an integer. Used as the value in the m_symboltable object (ordered map)

    The SymbolTable class contains all functions dealing with the Symbol Table as define above. Currently,
    it supports adding a label to the symbol table (with duplicate checking), printing the symbol table in 
    the console, and looking up the 


*/

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol    -> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION  

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*
NAME

    DisplaySymbolTable - displays every symbol in symbol table.

SYNOPSIS

    void DisplaySymbolTable(); 
            - no argouments avaliable

DESCRIPTION

    This function will display every symbol from the symbol table object. This function
    will output symbols in Alphabetical order, with the initial symbol numbered as symbol 0

NOTE 

    
*/
void SymbolTable::DisplaySymbolTable()
{
    cout << "Symbol Table:\n" << endl;
    cout << "Symbol#\tSymbol\tLocation" << endl;

    int count = 0;
    //Loop through map and output each symbol#, key (label/symbol), and value (location)
    for (auto& [symbol, location] : m_symbolTable)
    {
        cout << count << "\t" << symbol << "\t" << location << endl;
        count++;
    }
}

/*
NAME

    LookupSymbol - displays every symbol in symbol table.

SYNOPSIS

    void LookupSymbol( const string &a_symbol, int &a_loc);
        a_symbol    -> name of the label/symbol

DESCRIPTION

    This function will iterate through the symbol table (m_symbolTable) and attempt to locate
    symbol based on 

RETURN

    
*/
bool SymbolTable::LookupSymbol(const string& a_symbol)
{
    // This check must come first, as the map will create a obj at a_symbol
    // with no value if the symbol doesn't actually exist.
    if (m_symbolTable.find(a_symbol) == m_symbolTable.end())
    {
        return false;
    }
    return true;
}

/*
NAME

    GetLocation - get location using key(symbol/label)

SYNOPSIS

    int GetLocation(const string& a_symbol);
        a_symbol    -> name of the label/symbol (key for m_symbolTable)

DESCRIPTION

    This function will return the location (value) from the m_symbolTable map, given the 
    symbol/label (key). I imagine this function was supposed to be redundant given the LookUpSymbol
    function, however I had significant errors while attempting to use it.

RETURN

    integer representing location of symbol

*/
int SymbolTable::GetLocation(const string& a_symbol)
{
    return m_symbolTable[a_symbol];
}
