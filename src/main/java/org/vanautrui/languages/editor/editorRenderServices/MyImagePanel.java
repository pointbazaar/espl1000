package org.vanautrui.languages.editor.editorRenderServices;

import org.vanautrui.languages.editor.editorcore.DragonEditorWithImage;
import org.vanautrui.languages.editor.gui.DragonGUI_Editor;

import javax.swing.*;
import java.awt.*;

public class MyImagePanel extends JPanel {

    //https://stackoverflow.com/questions/299495/how-to-add-an-image-to-a-jpanel

    private Image img;

    public MyImagePanel(Image img){
        this.img=img;
        Dimension dim = new Dimension(DragonEditorWithImage.max_columns_per_line *DragonEditorWithImage.charSize,DragonEditorWithImage.charSize);
        this.setMinimumSize(dim);
        this.setPreferredSize(dim);
        this.setBackground(DragonGUI_Editor.backgroundColor);
        this.setMaximumSize(new Dimension(1000,30));
    }

    public void setImg(Image img1){
        this.img=img1;
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);
        g.drawImage(img,0,0,this);
    }
}
