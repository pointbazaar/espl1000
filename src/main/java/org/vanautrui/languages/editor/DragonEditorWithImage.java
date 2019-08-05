package org.vanautrui.languages.editor;

import org.apache.commons.io.IOUtils;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

public class DragonEditorWithImage {

    private JPanel panel;

    private DragonGUI_Editor master;

    private List<String> lines_in_editor=new ArrayList<>();

    private int cursor_line =0;
    private int cursor_col=0;


    public static final int charSize=15;
    public static final int max_columns_per_line = 40;

    public DragonEditorWithImage(DragonGUI_Editor master1) {
        this.master = master1;
        this.panel = new JPanel();
        this.lines_in_editor.add("");

        this.panel.setBorder(BorderFactory.createLineBorder(Color.white));

        //setting layout manager and preferred size correctly makes it
        //appear in the correct size.
        this.panel.setLayout(new BoxLayout(this.panel,BoxLayout.Y_AXIS));

        this.panel.setBackground(DragonGUI_Editor.backgroundColor);
        this.panel.setMinimumSize(new Dimension(600, DragonGUI_Editor.middle_row_height));
        this.panel.setPreferredSize(new Dimension(600, DragonGUI_Editor.middle_row_height));
        /*
        JLabel picLabel;

        try {
            BufferedImage image = new BufferedImage(500, 20, BufferedImage.TYPE_INT_ARGB);

            BufferedImage read = ImageIO.read(Paths.get("test.ppm").toFile());
            Image read2 = read.getScaledInstance(20,20,Image.SCALE_DEFAULT);


            picLabel = new JLabel(new ImageIcon(read2));
        }catch (Exception e){
            e.printStackTrace();
        }

        this.panel.add(this.picLabel);

         */

        this.panel.addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) {
                System.out.println("Key :"+e.getKeyChar());
            }

            @Override
            public void keyPressed(KeyEvent e) {
                System.out.println("Key :"+e.getKeyChar());
            }

            @Override
            public void keyReleased(KeyEvent e) {
                System.out.println("Key :"+e.getKeyChar());
            }
        });
    }

    public void arrowUpChangeLine(){
        //TODO
    }
    public void arrowDownChangeLine(){
        //TODO
    }
    public void writeCharcter(char c) throws Exception{
        if(this.lines_in_editor.get(this.cursor_line).length()==max_columns_per_line){
            throw new Exception("line too long already");
        }

        this.cursor_col++;
        this.lines_in_editor.set(
                this.cursor_line,
                this.lines_in_editor.get(cursor_line)+c
        );
        updateJLabelOnLine(this.cursor_line);
    }
    private void updateJLabelOnLine(int line) throws Exception{
        try {
            this.panel.remove(line);
        }catch (Exception e){
            //
        }
        this.panel.add(makeImageForLine(this.lines_in_editor.get(line),line), line);
        this.panel.updateUI();
    }

    private MyImagePanel makeImageForLine(String line, int line_index) throws Exception{

        Runtime rt = Runtime.getRuntime();
        int cursor_position_argument = (this.cursor_line==line_index)?this.cursor_col:-1;
        Process pr = rt.exec("./CodeRenderer/crend -l 1 --cursor-position "+cursor_position_argument);
        OutputStream in = pr.getOutputStream();
        in.write((line+"\n").getBytes());
        in.flush();
        pr.waitFor();

        int exit_value = pr.exitValue();
        if(exit_value!=0){
            System.out.println("error in makeImageForLine, crend exited with nonzero exit value");
            System.out.println(IOUtils.toString(pr.getInputStream()));
            System.exit(1);
        }

        InputStream out = pr.getInputStream();
        //String s = IOUtils.toString(out);

        BufferedImage read = ImageIO.read(out);
        //BufferedImage read = ImageIO.

        Image read2 = read.getScaledInstance(charSize* max_columns_per_line,charSize,Image.SCALE_DEFAULT);

        return new MyImagePanel(read2);
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
            this.lines_in_editor.add(this.cursor_line,"");
            this.cursor_line++;


        }else if(this.cursor_col==this.lines_in_editor.get(this.cursor_line).length()){
            //cursor at end of line
            this.lines_in_editor.add(this.cursor_line+1,"");
            this.cursor_line++;
            this.cursor_col=0;


        }else{
            //cursor is in the middle of a line
            //if in the middle of a line, split that line, increment cursor

            String beforeCursor = this.lines_in_editor.get(this.cursor_line).substring(this.cursor_col);
            String afterCursor = this.lines_in_editor.get(this.cursor_line).substring(this.cursor_col);
            this.lines_in_editor.set(this.cursor_line,beforeCursor);
            this.lines_in_editor.add(this.cursor_line+1,afterCursor);

            this.cursor_col=0;
            this.cursor_line++;
        }

        //update the Images of the respective lines to reflect the changes
        try {
            updateJLabelOnLine(this.cursor_line - 1);
            updateJLabelOnLine(this.cursor_line);
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
                this.lines_in_editor.remove(this.cursor_line);

                this.cursor_line--;
                this.cursor_col=this.lines_in_editor.get(this.cursor_line).length();
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
    }
}
