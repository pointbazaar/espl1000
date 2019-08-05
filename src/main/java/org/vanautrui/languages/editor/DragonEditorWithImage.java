package org.vanautrui.languages.editor;

import org.vanautrui.languages.editor.lineImageServices.LineImageService;
import org.vanautrui.languages.editor.lineImageServices.MyImagePanel;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class DragonEditorWithImage {

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

        this.addLine(0,"");

        this.panel.setBorder(BorderFactory.createLineBorder(Color.white));

        //setting layout manager and preferred size correctly makes it
        //appear in the correct size.
        this.panel.setLayout(new BoxLayout(this.panel,BoxLayout.Y_AXIS));

        this.panel.setBackground(DragonGUI_Editor.backgroundColor);
        this.panel.setMinimumSize(new Dimension(600, DragonGUI_Editor.middle_row_height));
        this.panel.setPreferredSize(new Dimension(600, DragonGUI_Editor.middle_row_height));

    }

    //-----------
    //to manage the components and lines in editor at the same time
    private void addLine(int beforeIndex,String text){
        this.lines_in_editor.add(beforeIndex,text);
        try{
            this.panel.add(LineImageService.makeImageForLine(text,beforeIndex,this.cursor_line,this.cursor_col),0);
        }catch (Exception e){
            e.printStackTrace();
        }
    }
    private void removeLine(int index){
        this.lines_in_editor.remove(index);
        this.panel.remove(index);
    }
    //--------------


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
        try{
            this.updateJLabelOnLine(this.cursor_line);
            //this.updateCompletely();
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void arrowRightMoveCursor(){
        //this feature behaves differently than in other text editors
        this.cursor_col++;
        this.adjustCursorColumnToBeContainedInLine();

        try{
            this.updateJLabelOnLine(this.cursor_line);
            //this.updateCompletely();
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void arrowUpChangeLine(){
        if(this.cursor_line>0){
            this.cursor_line--;
            this.adjustCursorColumnToBeContainedInLine();

            try {
                this.updateJLabelOnLine(this.cursor_line);
                this.updateJLabelOnLine(this.cursor_line + 1);
                //this.updateCompletely();
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }
    public void arrowDownChangeLine(){
        if(this.cursor_line < this.lines_in_editor.size()-1){
            this.cursor_line++;
            this.adjustCursorColumnToBeContainedInLine();

            try {
                this.updateJLabelOnLine(this.cursor_line-1);
                this.updateJLabelOnLine(this.cursor_line);
                //this.updateCompletely();
            }catch (Exception e){
                e.printStackTrace();
            }
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

        updateJLabelOnLine(this.cursor_line);
        //this.updateCompletely();
    }

    private void updateCompletely()throws Exception{
        long start = System.currentTimeMillis();

        this.panel.removeAll();
        for(int i=0;i<this.lines_in_editor.size();i++){
            String s = this.lines_in_editor.get(i);
            this.panel.add(LineImageService.makeImageForLine(s,i,this.cursor_line,this.cursor_col));
        }
        this.panel.updateUI();

        long end=System.currentTimeMillis();

        System.out.println("update took: "+(end-start)+" ms");
    }

    private void updateJLabelOnLine(int line) throws Exception{

        MyImagePanel panel = (MyImagePanel)this.panel.getComponent(line);
        panel.setImg(
                LineImageService.makeImageForLineInner(this.lines_in_editor.get(line),line,cursor_line,cursor_col)
        );

        /*
        if(this.lines_in_editor.size()>line) {
            this.panel.add(LineImageService.makeImageForLine(this.lines_in_editor.get(line), line,cursor_line,cursor_col));
            try {
                this.panel.remove(line+1);
            }catch (Exception e){
                //
            }
        }

         */

        this.panel.updateUI();

        this.panel.invalidate();
        this.panel.repaint();
    }

    public void appendLineTest(){
        try {
            Runtime rt = Runtime.getRuntime();
            String line="A";

            Process pr = rt.exec("./CodeRenderer/crend -r "+line+" -l 1");
            pr.waitFor();

            InputStream out = pr.getInputStream();
            //String image_in_ppm_format = IOUtils.toString(out);

            //System.out.println(image_in_ppm_format);

            BufferedImage read = ImageIO.read(out);
            //BufferedImage read = new BufferedImage(30,30, ImageStorage.ImageType.RGB);
            Image read2 = read.getScaledInstance(20,20,Image.SCALE_DEFAULT);

            //TODO: figure out why this is not visible
            JLabel line_img = new JLabel(new ImageIcon(read2));
            line_img.setMinimumSize(new Dimension(20,20));

            //this.panel.add(line_img);
            this.panel.add(new MyImagePanel(read2));

            //this.panel.setVisible(true);
            this.panel.updateUI();

            System.out.println("try to append line");

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

            //TODO: fix the bug where making a newline doesnt draw anew the lines thereafter

            //when a new line is inserted, a naive approach would be to then
            //update the drawings of all the subsequent lines

            //another approach would be to cache the stuff from the cpp program and
            //re-render the whole thing, but as most of it is cached it would not be a problem...

            //another approach would be to manually manage the images of the lines
            //when new lines get inserted. that would probably be most performant
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

        //update the Images of the respective lines to reflect the changes
        try {
            updateJLabelOnLine(this.cursor_line - 1);
            updateJLabelOnLine(this.cursor_line);
            //this.updateCompletely();
        }catch (Exception e){
            e.printStackTrace();
        }
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
                //this.lines_in_editor.remove(this.cursor_line);

                this.cursor_line--;
                this.cursor_col=this.lines_in_editor.get(this.cursor_line).length();

                try{
                    updateJLabelOnLine(this.cursor_line);
                    //updateJLabelOnLine(this.cursor_line+1);
                }catch (Exception e){
                    e.printStackTrace();
                }
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

            try{
                updateJLabelOnLine(this.cursor_line);
            }catch (Exception e){
                e.printStackTrace();
            }
        }
        try{
            //this.updateCompletely();
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
