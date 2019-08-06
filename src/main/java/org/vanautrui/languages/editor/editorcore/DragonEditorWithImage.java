package org.vanautrui.languages.editor.editorcore;

import org.vanautrui.languages.editor.DragonGUI_Editor;
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
    private final DragonEditorCore core;

    public static final int charSize=15;
    public static final int max_columns_per_line = 80;

    public DragonEditorWithImage(DragonGUI_Editor master1) {
        this.master = master1;
        this.panel = new JPanel();
        this.core=new DragonEditorCore();

        //this.updateEditorImage();

        this.panel.setBorder(BorderFactory.createLineBorder(Color.white));

        //setting layout manager and preferred size correctly makes it
        //appear in the correct size.
        this.panel.setLayout(new BoxLayout(this.panel,BoxLayout.Y_AXIS));

        this.panel.setBackground(DragonGUI_Editor.backgroundColor);
        this.panel.setMinimumSize(new Dimension(600, DragonGUI_Editor.middle_row_height));
        this.panel.setPreferredSize(new Dimension(600, DragonGUI_Editor.middle_row_height));

        this.updateEditorImage();
    }

    public int getLineCount(){ return this.core.getLineCount();}

    public int getCursorLineDisplay(){
        return this.core.getCursorLineDisplay();
    }

    public int getCursorColDisplay(){
        return this.core.getCursorColDisplay();
    }

    private void updateEditorImage(){

        long start = System.currentTimeMillis();
        this.panel.removeAll();
        this.panel.add(
                new JLabel(new ImageIcon(EditorImageService.getEditorImage(this.core.getLinesInEditor(),this.core.getCursorLine(),this.core.getCursorCol(),this.panel)))
        );
        long end = System.currentTimeMillis();
        System.out.println("complete editor draw took: "+(end-start)+" ms");

        this.panel.updateUI();
        this.panel.invalidate();
        this.panel.repaint();
    }

    public void arrowLeftMoveCursor(){
        this.core.arrowLeftMoveCursor();
        this.updateEditorImage();
    }

    public void arrowRightMoveCursor(){
        this.core.arrowRightMoveCursor();
        this.updateEditorImage();
    }

    public void arrowUpChangeLine(){
        this.core.arrowUpChangeLine();
        this.updateEditorImage();
    }
    public void arrowDownChangeLine(){
        this.core.arrowDownChangeLine();
        this.updateEditorImage();
    }

    public void writeCharcter(char c) throws Exception{
        this.core.writeCharcter(c);
        this.updateEditorImage();
    }



    public JPanel getImage(){
        return this.panel;
    }

    public void pressEnter() {
        this.core.pressEnter();
        this.updateEditorImage();
    }

    public void pressBackSpace() {
        this.core.pressBackSpace();
        this.updateEditorImage();
    }

    public void pressTab() {
        //if the cursor is just after a character,
        //it should insert a completion if there is just 1 available
        //otherwise, it should insert a tab
        this.core.pressTab();
        this.updateEditorImage();
    }
}
