package org.vanautrui.languages.editor.gui;

import org.vanautrui.languages.editor.gui.DragonGUI_Editor;

import javax.swing.*;
import java.awt.*;

public class CustomHelpWindow {

    private DragonGUI_Editor master;

    private JFrame helpFrame;

    public CustomHelpWindow(DragonGUI_Editor master1) {
        //we use it in constructor,
        //because it depends on the editor area
        //and needs to know about it, but not the other way around
        this.master = master1;


        this.helpFrame = new JFrame();
        helpFrame.setMinimumSize(new Dimension(300, 500));
        helpFrame.setResizable(false);

        JPanel panel = new JPanel();
        panel.setBackground(DragonGUI_Editor.backgroundColor);

        BoxLayout layout = new BoxLayout(panel, BoxLayout.Y_AXIS);

        panel.setLayout(layout);
        panel.setMaximumSize(new Dimension(2000, 30));

        String[] help_lines = new String[]{
                "Ctrl + S : Save File",
                "Ctrl + O : Open File",
                "Ctrl + C : Quit",
                "TODO: Ctrl + B : Build  Project",
                "TODO: Ctrl + Enter : Jump to : (Method|Constructor) Declaration | variable initialization",

                "F1 : Toggle Help Window",
                "F2 : Share ('NetSlide') the current Buffer",
                "F3 : Receive File into current Buffer"
        };

        for(String s : help_lines){
            panel.add(new JLabel(s));
        }

        helpFrame.setTitle("Dragon Editor Help");
        helpFrame.add(panel);
        helpFrame.setVisible(true);
    }

    public void destroy() {
        this.helpFrame.setVisible(false);
        this.helpFrame.dispose();
    }

}
