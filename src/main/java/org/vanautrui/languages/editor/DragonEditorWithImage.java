package org.vanautrui.languages.editor;

import org.vanautrui.languages.editor.editorRenderServices.EditorImageService;
import org.vanautrui.languages.editor.editorRenderServices.LineImageService;
import org.vanautrui.languages.editor.editorRenderServices.MyImagePanel;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class DragonEditorWithImage {

    //TODO: add some unit tests, for the text editing code
    //so that new text editing functionality
    //can be added in confidence of not breaking anything

    private JPanel panel;

    private DragonGUI_Editor master;

    private List<String> lines_in_editor=new ArrayList<>();

    private int cursor_line =0;
    private int cursor_col=0;


    public static final int charSize=15;
    public static final int max_columns_per_line = 80;

    public DragonEditorWithImage(DragonGUI_Editor master1) {
        this.master = master1;
        this.panel = new JPanel();


        addLine(0,"");
        this.updateEditorImage();

        this.panel.setBorder(BorderFactory.createLineBorder(Color.white));

        //setting layout manager and preferred size correctly makes it
        //appear in the correct size.
        this.panel.setLayout(new BoxLayout(this.panel,BoxLayout.Y_AXIS));

        this.panel.setBackground(DragonGUI_Editor.backgroundColor);
        this.panel.setMinimumSize(new Dimension(600, DragonGUI_Editor.middle_row_height));
        this.panel.setPreferredSize(new Dimension(600, DragonGUI_Editor.middle_row_height));

    }

    private void addLine(int beforeIndex,String text){
        this.lines_in_editor.add(beforeIndex,text);
    }
    private void removeLine(int index){
        this.lines_in_editor.remove(index);
    }

    public int getLineCount(){return this.lines_in_editor.size();}

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

        this.updateEditorImage();
    }

    public void arrowRightMoveCursor(){
        //this feature behaves differently than in other text editors
        this.cursor_col++;
        this.adjustCursorColumnToBeContainedInLine();

        this.updateEditorImage();
    }

    public void arrowUpChangeLine(){
        if(this.cursor_line>0){
            this.cursor_line--;
            this.adjustCursorColumnToBeContainedInLine();
            this.updateEditorImage();
        }
    }
    public void arrowDownChangeLine(){
        if(this.cursor_line < this.lines_in_editor.size()-1){
            this.cursor_line++;
            this.adjustCursorColumnToBeContainedInLine();
            this.updateEditorImage();
        }
    }

    private String stringBeforeCursor(){
        //returns the string on the currently selected line, before ther cursor
        String result="";
        if(this.cursor_col>=1) {
            return this.lines_in_editor.get(this.cursor_line).substring(0, this.cursor_col);
        }
        return result;
    }

    private String stringAfterCursor(){
        //returns the string on the currently selected line, after the cursor
        return this.lines_in_editor.get(this.cursor_line).substring(this.cursor_col);
    }

    public void writeCharcter(char c) throws Exception{
        if(this.lines_in_editor.get(this.cursor_line).length()==max_columns_per_line){
            throw new Exception("line too long already");
        }

        this.lines_in_editor.set(
                this.cursor_line,
                this.stringBeforeCursor()+c+this.stringAfterCursor()
        );
        this.cursor_col++;
        updateEditorImage();
    }

    private void updateEditorImage(){

        long start = System.currentTimeMillis();
        this.panel.removeAll();
        this.panel.add(
                new JLabel(new ImageIcon(EditorImageService.getEditorImage(lines_in_editor,cursor_line,cursor_col,this.panel)))
        );
        long end = System.currentTimeMillis();
        System.out.println("complete editor draw took: "+(end-start)+" ms");

        this.panel.updateUI();
        this.panel.invalidate();
        this.panel.repaint();
    }

    public void ppm_format_experiment(){
        try {
            Runtime rt = Runtime.getRuntime();
            String line="A";

            Process pr = rt.exec("./CodeRenderer/crend -r "+line+" -l 1");
            pr.waitFor();

            InputStream out = pr.getInputStream();
            //String image_in_ppm_format = IOUtils.toString(out);
            //System.out.println(image_in_ppm_format);

            BufferedImage read = ImageIO.read(out);
            Image read2 = read.getScaledInstance(20,20,Image.SCALE_DEFAULT);

            JLabel line_img = new JLabel(new ImageIcon(read2));
            line_img.setMinimumSize(new Dimension(20,20));

        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public JPanel getImage(){
        return this.panel;
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

        updateEditorImage();
    }

    public void pressBackSpace() {

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

            String after_cursor = current_line.substring(this.cursor_col);

            String edited_line = before_cursor_init+after_cursor;
            this.lines_in_editor.set(this.cursor_line,edited_line);

            this.cursor_col--;
        }
        this.updateEditorImage();
    }
}
