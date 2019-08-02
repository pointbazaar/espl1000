package org.vanautrui.languages.editor;

import javafx.scene.input.KeyCode;
import org.apache.commons.io.IOUtils;
import sun.awt.ExtendedKeyCodes;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Optional;

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

                    tryCompletion();
                    e.consume();
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

    public void tryCompletion(){
        System.out.println("try to autocomplete keywords...");

        Optional<String> maybeLastWord = this.getLastWord();

        if(!maybeLastWord.isPresent()){
            System.out.println("no last word present");
            return;
        }

        String lastWord=  "publi";
        lastWord=maybeLastWord.get();

        try {
            Runtime rt = Runtime.getRuntime();
            Process pr = rt.exec("./Interpreter/dri --complete " + lastWord);

            //wait for the completion to exit
            pr.waitFor();

            InputStream out = pr.getInputStream();
            String completed = IOUtils.toString(out);

            System.out.println("Copmleted: "+completed);

        }catch (Exception ee){
            ee.printStackTrace();
        }
    }

    public Optional<String> getLastWord(){
        //TODO: make this method more optimized
        String[] words = this.textArea.getText().split(" ");

        if (words.length>0){
            String last = words[words.length-1];
            if(last.matches("[a-zA-Z]+")){
                System.out.println("last word: '"+last+"'");
                return Optional.of(last);
            }
        }
        return Optional.empty();
    }

    public void selectAll(){
        System.out.println("Select All");
        this.textArea.selectAll();
    }

    public int getCaretPosition(){
        return textArea.getCaretPosition();
    }
}
