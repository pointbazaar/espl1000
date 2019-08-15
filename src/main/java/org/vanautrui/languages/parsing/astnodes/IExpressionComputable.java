package org.vanautrui.languages.parsing.astnodes;

public interface IExpressionComputable {

    //every computable expression has a type
    //sometimes this needs to be type-checked at compile time
    //this is what this interface is for
    public String getType();
}
