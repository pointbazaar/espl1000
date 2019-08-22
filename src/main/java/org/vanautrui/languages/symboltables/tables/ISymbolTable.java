package org.vanautrui.languages.symboltables.tables;

import org.vanautrui.languages.symboltables.rows.ISymbolTableRow;

//intentionally only package visibility to discourage implementation
//in other locations
interface ISymbolTable {

    public void add(ISymbolTableRow row);

    public boolean containsVariable(String varName);

    public int getIndexOfVariable(String varName);

    public String toString();

    public String getTypeOfVariable(String varName);

    public int size();

}
