package org.vanautrui.languages.editor;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class DragonEditorContextArea {

    private JTextArea available_completions;

    private DragonGUI_Editor master;

    public DragonEditorContextArea(DragonGUI_Editor master1){

        //https://docs.oracle.com/javase/tutorial/uiswing/components/textarea.html

        this.master=master1;
        this.available_completions=new JTextArea();
        this.available_completions.setBackground(DragonGUI_Editor.backgroundColor);
        //this.available_completions.setMinimumSize(new Dimension(200,200));
        this.available_completions.add(new JLabel("completions will appear here"));


        //this.available_completions.setMaximumSize(new Dimension(200,100));
        this.available_completions.setLineWrap(false);
        this.available_completions.setEditable(false);

        this.available_completions.setRows(7);
        this.available_completions.setColumns(18);
    }

    public Component make(){

        return this.available_completions;
    }

    public void setAvailableCompletions(List<String> list){
        this.available_completions.setText("");
        for(String str : list) {
            this.available_completions.append(str+"\n");
        }
    }
}
