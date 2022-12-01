//
//		Symbol table class.
//
#pragma once

class SymbolTable {

public:
    
    const int multiplyDefinedSymbol = -999;

    // Add a new symbol to the symbol table.
    void AddSymbol( const string &a_symbol, int a_loc );

    // Display the symbol table.
    void DisplaySymbolTable();

    // Lookup a symbol in the symbol table.
    bool LookupSymbol(const string& a_symbol);

    // get location of a symbol in symbol table.
    int GetLocation(const string& a_symbol);

private:

    // symbol is the key to the map. value is the location
    // NOTE: Using <map> instead of <unordered_map>, thus, map will be ordered by key value
    map<string, int> m_symbolTable;
};
