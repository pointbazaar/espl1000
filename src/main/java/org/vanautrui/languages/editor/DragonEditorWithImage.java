package org.vanautrui.languages.editor;

import com.sun.javafx.iio.ImageStorage;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.nio.file.Paths;

public class DragonEditorWithImage {

    private JLabel picLabel;
    private DragonGUI_Editor master;


    private static final Dimension dim = new Dimension(400,400);

    public DragonEditorWithImage(DragonGUI_Editor master1){
        this.master=master1;

    }

    public JLabel getImage()throws Exception{

        BufferedImage image=new BufferedImage(500,20, BufferedImage.TYPE_INT_ARGB);
        BufferedImage read = ImageIO.read(Paths.get("test.ppm").toFile());
        this.picLabel=new JLabel(new ImageIcon(read));

        return this.picLabel;
    }
}
