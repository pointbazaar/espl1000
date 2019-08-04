package org.vanautrui.languages.editor;

import javax.swing.*;
import java.awt.*;

public class MyImagePanel extends JPanel {

    //https://stackoverflow.com/questions/299495/how-to-add-an-image-to-a-jpanel

    private Image img;

    public MyImagePanel(Image img){
        this.img=img;
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);
        g.drawImage(img,0,0,this);
    }
}
