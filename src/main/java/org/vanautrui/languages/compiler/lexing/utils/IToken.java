package org.vanautrui.languages.compiler.lexing.utils;

public interface IToken {

    /**
     * @return returns the string contents of the token.
     * must be the same as the original source string from which
     * the token was parsed.
     */
    public String getContents();

    /**
     * @return returns the line number in the source code where the token originated
     * this can be used to give very precise error messages
     */
    public long getLineNumber();

    public default boolean tokenEquals(IToken other){
        boolean b1= this.getClass().getName().equals(other.getClass().getName());
        boolean b2=this.getContents().equals(other.getContents());
        return b1 && b2;
    }
}
