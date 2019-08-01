package org.vanautrui.languages.editor;

import javax.swing.*;

public class DragonGUI_Editor {

    //TODO: make the editor

    public static final int default_width=600;
    public static final int default_height=300;

    public DragonGUI_Editor(){

        JFrame frame = new JFrame("Dragon Editor");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(default_width,default_height);

        frame.setResizable(false);

        JButton button = new JButton("Button");

        frame.getContentPane().add(button);

        frame.setVisible(true);
    }
}
