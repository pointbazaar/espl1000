package org.vanautrui.languages.editor;

import javafx.scene.input.KeyCode;
import sun.awt.ExtendedKeyCodes;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class DragonEditorArea {

    private TextArea textArea;

    private static final Dimension dim = new Dimension(400,400);

    public DragonEditorArea(){}

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

                //System.out.println(e.toString());
                //System.out.println("pressed "+e.getKeyChar());
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
