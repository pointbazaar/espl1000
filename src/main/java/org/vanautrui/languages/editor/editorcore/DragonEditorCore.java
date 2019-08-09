package org.vanautrui.languages.editor.editorcore;

import org.apache.commons.lang3.StringUtils;

import java.util.ArrayList;
import java.util.List;

public class DragonEditorCore {

    //TODO: make this editor support atomic operations
    //so that multiple threads may call subroutines on it

    private List<String> lines_in_editor=new ArrayList<>();

    private int cursor_line =0;
    private int cursor_col=0;

    private static final int max_columns_per_line = 80;

    public DragonEditorCore() {
        this.addLine(0,"");
    }

    public List<String> getLinesInEditor(){
        return new ArrayList<>(this.lines_in_editor);
    }

    private void addLine(int beforeIndex,String text){
        this.lines_in_editor.add(beforeIndex,text);
    }
    private void removeLine(int index){
        this.lines_in_editor.remove(index);
    }

    public int getLineCount(){return this.lines_in_editor.size();}

    public int getCursorLine(){
        return this.cursor_line;
    }

    public int getCursorCol(){
        return this.cursor_col;
    }

    public int getCursorLineDisplay(){
        return this.cursor_line+1;
    }

    public int getCursorColDisplay(){
        return this.cursor_col+1;
    }

    private void adjustCursorColumnToBeContainedInLine(){
        if(this.cursor_col>this.lines_in_editor.get(this.cursor_line).length()){
            this.cursor_col=this.lines_in_editor.get(this.cursor_line).length();
        }
    }

    public void arrowLeftMoveCursor(){
        //this feature behaves differently than in other text editors
        if(this.cursor_col>0){
            this.cursor_col--;
        }
    }

    public void arrowRightMoveCursor(){
        //this feature behaves differently than in other text editors
        this.cursor_col++;
        this.adjustCursorColumnToBeContainedInLine();

    }

    public void arrowUpChangeLine(){
        if(this.cursor_line>0){
            this.cursor_line--;
            this.adjustCursorColumnToBeContainedInLine();
        }
    }
    public void arrowDownChangeLine(){
        if(this.cursor_line < this.lines_in_editor.size()-1){
            this.cursor_line++;
            this.adjustCursorColumnToBeContainedInLine();
        }
    }

    public String stringBeforeCursor(){
        //returns the string on the currently selected line, before ther cursor
        String result="";
        if(this.cursor_col>=1) {
            return this.lines_in_editor.get(this.cursor_line).substring(0, this.cursor_col);
        }
        return result;
    }

    public String stringAfterCursor(){
        //returns the string on the currently selected line, after the cursor
        return this.lines_in_editor.get(this.cursor_line).substring(this.cursor_col);
    }

    private synchronized void writeString(String s) throws Exception{
        String currentLine = this.lines_in_editor.get(this.cursor_line);

        if(currentLine.length()+s.length()>max_columns_per_line){
            throw new Exception("line too long already");
        }

        if(!isStringAllowed(s)){
            throw new Exception("this string is not allowd");
        }

        this.lines_in_editor.set(
                this.cursor_line,
                this.stringBeforeCursor()+s+this.stringAfterCursor()
        );
        this.cursor_col+=s.length();
    }

    private synchronized boolean isStringAllowed(String s){
        for(char c : s.toCharArray()){
            if(!isCharAllowed(c)){
                return false;
            }
        }
        return true;
    }

    private synchronized boolean isCharAllowed(char c){
        return (c>=' ' && c<='~');
    }

    public synchronized void writeCharcter(char c) throws Exception{
        if(this.lines_in_editor.get(this.cursor_line).length()==max_columns_per_line){
            throw new Exception("line too long already");
        }

        if(!isCharAllowed(c)){
            throw new Exception("character not allowed");
        }

        this.lines_in_editor.set(
                this.cursor_line,
                this.stringBeforeCursor()+c+this.stringAfterCursor()
        );
        this.cursor_col++;
    }

    public void pressEnter() {

        if(this.cursor_col==0){
            //if at beginning of line, insert new line, increment cursor
            //this.lines_in_editor.add(this.cursor_line,"");
            this.addLine(this.cursor_line,"");
            this.cursor_line++;


        }else if(this.cursor_col==this.lines_in_editor.get(this.cursor_line).length()){
            //cursor at end of line
            this.addLine(this.cursor_line+1,"");
            //this.lines_in_editor.add(this.cursor_line+1,"");
            this.cursor_line++;
            this.cursor_col=0;

        }else{
            //cursor is in the middle of a line
            //if in the middle of a line, split that line, increment cursor

            String beforeCursor = this.stringBeforeCursor();
            String afterCursor = this.stringAfterCursor();

            //maybe this call should also be for both the component and the string
            this.lines_in_editor.set(this.cursor_line,beforeCursor);

            //this.lines_in_editor.add(this.cursor_line+1,afterCursor);
            this.addLine(this.cursor_line+1,afterCursor);

            this.cursor_col=0;
            this.cursor_line++;
        }
    }

    public void pressBackSpace() {

        //TODO: if there are 4 spaces before the cursor, and  (cursorCol%4)==0  then delete the 4 spaces which are considered a tab

        if(this.cursor_col==0 ){
            if(this.cursor_line==0) {
                return;
            }else{
                //cursor_line !=0 , cursor_col==0
                //cursor_line >0 ~> previous line exists
                String previous_line = this.lines_in_editor.get(this.cursor_line-1);

                String current_line = this.lines_in_editor.get(this.cursor_line);

                this.lines_in_editor.set(this.cursor_line-1,previous_line+current_line);

                this.removeLine(this.cursor_line);

                this.cursor_line--;
                this.cursor_col=previous_line.length();
            }
        }else{
            //cursor_col !=0    should mean   cursor_col > 0
            String current_line = this.lines_in_editor.get(this.cursor_line);

            String before_cursor_init = "";
            if(this.cursor_col>=2){
                before_cursor_init = current_line.substring(0,this.cursor_col-1);
            }

            //delete until the next tab
            int dist_to_tab_increment = cursor_col%4;
            if(dist_to_tab_increment==0){
                dist_to_tab_increment=4;
            }
            String after_cursor = this.stringAfterCursor();

            if(this.stringBeforeCursor().endsWith(StringUtils.repeat(' ',dist_to_tab_increment))){
                before_cursor_init=this.stringBeforeCursor().substring(0,this.stringBeforeCursor().length()-dist_to_tab_increment);
                this.cursor_col-=dist_to_tab_increment;
            }else{
                this.cursor_col--;
            }


            String edited_line = before_cursor_init+after_cursor;
            this.lines_in_editor.set(this.cursor_line,edited_line);

            //System.out.println("line after backspace :");
            //System.out.println("'"+this.lines_in_editor.get(this.cursor_line)+"'");
        }
    }

    private final String aTabIs4Spaces = "    ";

    public void pressTab() {

        //do the tabs in 4-spaces increments

        //so a tab should bring the cursor up to the next increment, or
        //if it is already at an  increment, to the next increment

        //this editor is a spaces only editor
        //because it makes for easier display
        //and easier coding, since you do not have to consider so many different cases

        if(this.stringBeforeCursor().endsWith(" ") || this.stringBeforeCursor().equals("")){
            try {
                int above_icrement = this.cursor_col%4;
                if(above_icrement==0) {
                    this.writeString(aTabIs4Spaces);
                }else {
                    this.writeString(StringUtils.repeat(' ',4-above_icrement));
                }
            }catch (Exception e){
                e.printStackTrace();
            }
        }else{
            //TODO: use some kind of completion service
            //to find out a completion if there is just 1 available option
        }
    }
}
