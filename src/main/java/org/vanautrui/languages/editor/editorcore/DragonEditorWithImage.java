package org.vanautrui.languages.editor.editorcore;

import org.vanautrui.languages.editor.gui.DragonGUI_Editor;
import org.vanautrui.languages.editor.editorRenderServices.EditorImageService;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class DragonEditorWithImage {

    //TODO: add some unit tests, for the text editing code
    //so that new text editing functionality
    //can be added in confidence of not breaking anything

    private JPanel panel;

    private DragonGUI_Editor master;
    private DragonEditorCore core;

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

        //TODO: add an alternative, that only
        //updates one line. so that not everything has to be drawn

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

    public void pressHome() {
        this.core.pressHome();
        this.updateEditorImage();
    }

    public void pressEnd() {
        this.core.pressEnd();
        this.updateEditorImage();
    }

    public void loadFile(File selectedFile)throws Exception{
        List<String> lines = Files.readAllLines(selectedFile.toPath());

        this.core=new DragonEditorCore(lines);
        this.updateEditorImage();
    }

    public void loadString(String line) {
        this.core=new DragonEditorCore(Arrays.asList(line.split("\n")));
        this.updateEditorImage();
    }

    public String getCurrentLine() {
        return this.core.getLinesInEditor().get(this.core.getCursorLine());
    }

    public String getCurrentBufferAsString() {
        return this.core.getLinesInEditor().stream().collect(Collectors.joining("\n"));
    }
}
