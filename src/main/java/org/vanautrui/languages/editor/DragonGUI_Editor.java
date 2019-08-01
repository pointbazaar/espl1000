package org.vanautrui.languages.editor;

import javax.swing.*;
import java.awt.*;

public class DragonGUI_Editor {

    //TODO: make the editor

    public static final int default_width=600;
    public static final int default_height=300;

    private JFrame frame;

    public DragonGUI_Editor(){

        this.frame = new JFrame("Dragon Editor");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(default_width,default_height);

        frame.setResizable(false);

        JButton button = new JButton("Button");

        //frame.getContentPane().add(button);

        frame.getContentPane().add(BorderLayout.NORTH,new JButton("tools panel"));

        frame.getContentPane().add(BorderLayout.WEST,new JButton("project tree"));

        frame.getContentPane().add(BorderLayout.SOUTH,new JButton("status bar"));

        frame.getContentPane().add(BorderLayout.CENTER,editorArea());

        frame.getContentPane().add(BorderLayout.EAST,new JButton("context information, file navigation"));

        frame.setVisible(true);


    }

    private Component editorArea(){
        TextArea textArea = new TextArea();
        textArea.setPreferredSize(new Dimension(400,500));
        return textArea;
    }

}
