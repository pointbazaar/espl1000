package org.vanautrui.languages.editor;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class DragonEditorContextArea {

    private JTextArea available_completions;

    private DragonGUI_Editor master;

    public DragonEditorContextArea(DragonGUI_Editor master1){
        this.master=master1;
        this.available_completions=new JTextArea();
        this.available_completions.setMinimumSize(new Dimension(200,200));
        this.available_completions.add(new JLabel("completions will appear here"));
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
