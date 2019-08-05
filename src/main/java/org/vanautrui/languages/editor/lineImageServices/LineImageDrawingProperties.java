package org.vanautrui.languages.editor.lineImageServices;

import java.util.Objects;

public class LineImageDrawingProperties {

    //contains the properties relevant for drawing

    public String text;

    public int line_index;

    public int cursor_line;
    public int cursor_col;

    public LineImageDrawingProperties(String nText,int nline_index, int ncursor_line,int ncursor_col){
        this.text=nText;
        this.line_index=nline_index;
        this.cursor_line=ncursor_line;
        this.cursor_col=ncursor_col;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof LineImageDrawingProperties)) return false;
        LineImageDrawingProperties that = (LineImageDrawingProperties) o;
        return line_index == that.line_index &&
                cursor_line == that.cursor_line &&
                cursor_col == that.cursor_col &&
                text.equals(that.text);
    }

    @Override
    public int hashCode() {
        return this.text.hashCode()
                +this.line_index*100000
                +this.cursor_line*1000
                +this.cursor_col;
    }
}
