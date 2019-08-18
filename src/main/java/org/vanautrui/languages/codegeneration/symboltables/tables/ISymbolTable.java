package org.vanautrui.languages.codegeneration.symboltables.tables;

import org.vanautrui.languages.codegeneration.symboltables.rows.ISymbolTableRow;

import java.util.List;
import java.util.stream.Collectors;

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
