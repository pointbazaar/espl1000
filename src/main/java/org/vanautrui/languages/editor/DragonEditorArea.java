package org.vanautrui.languages.editor;

import javafx.scene.input.KeyCode;
import sun.awt.ExtendedKeyCodes;
import sun.misc.IOUtils;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.OutputStream;

public class DragonEditorArea {

    private TextArea textArea;
    private DragonGUI_Editor master;

    private static final Dimension dim = new Dimension(400,400);

    public DragonEditorArea(DragonGUI_Editor master1){
        this.master=master1;
    }

    public Component editorArea(){
        this.textArea = new TextArea();

        this.textArea.setPreferredSize(dim);
        this.textArea.setMinimumSize(dim);
        this.textArea.setMaximumSize(dim);

        textArea.addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) {

            }

            @Override
            public void keyPressed(KeyEvent e) {

                if(e.isControlDown() && e.getKeyCode()==KeyEvent.VK_A){
                    //CTRL + A
                    //select all text
                    selectAll();
                }

                if(e.isControlDown() && e.getKeyCode()==KeyEvent.VK_C){
                    //CTRL + C
                    System.out.println("Goodbye");
                    System.exit(0);
                }

                if(e.getKeyCode()==KeyEvent.VK_TAB){
                    //invoke the autocomplete if
                    //the last word seems to be a keyword

                    String lastWord=  "publi";
                    try {
                        Runtime rt = Runtime.getRuntime();
                        Process pr = rt.exec("./Interpreter/dri -complete " + lastWord);
                        OutputStream out = pr.getOutputStream();

                    }catch (Exception ee){
                        ee.printStackTrace();
                    }
                }

                //System.out.println(e.toString());
                //System.out.println("pressed "+e.getKeyChar());

                //TODO : consume

                //inform the status bar
                if(master.statusBar.isPresent()){
                    master.statusBar.get().setCursorPos(
                            getCaretPosition()
                    );
                }

            }

            @Override
            public void keyReleased(KeyEvent e) {

            }
        });


        return textArea;
    }

    public void selectAll(){
        System.out.println("Select All");
        this.textArea.selectAll();
    }

    public int getCaretPosition(){
        return textArea.getCaretPosition();
    }
}
