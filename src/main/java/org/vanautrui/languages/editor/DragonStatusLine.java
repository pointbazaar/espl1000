package org.vanautrui.languages.editor;

import javax.swing.*;
import java.awt.*;

public class DragonStatusLine {

    private JPanel panel;
    private DragonGUI_Editor master;


    private JLabel line_number_label;

    public DragonStatusLine(DragonGUI_Editor master1){
        //we use it in constructor,
        //because it depends on the editor area
        //and needs to know about it, but not the other way around
        this.master=master1;

        this.panel = new JPanel();
        this.line_number_label=new JLabel("TODO: linu number");
        panel.add(new JLabel("TODO: status bar"));
        panel.add(this.line_number_label);
    }

    public void setCursorPos(int pos){
        this.line_number_label.setText("position: "+pos);
    }

    public Component statusBar(){

        return panel;
    }


}
