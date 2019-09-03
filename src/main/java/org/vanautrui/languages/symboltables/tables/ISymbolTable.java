package org.vanautrui.languages.symboltables.tables;

import org.vanautrui.languages.symboltables.rows.ISymbolTableRow;

import java.util.List;

//intentionally only package visibility to discourage implementation
//in other locations
interface ISymbolTable {

    public boolean containsVariable(String varName);

    public int getIndexOfVariable(String varName) throws Exception;

    public String toString();

    public String getTypeOfVariable(String varName) throws Exception;

    public int size();

    public List<ISymbolTableRow> getRows();

}
