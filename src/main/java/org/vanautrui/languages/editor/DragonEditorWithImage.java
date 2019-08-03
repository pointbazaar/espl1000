package org.vanautrui.languages.editor;

import com.sun.javafx.iio.ImageStorage;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.nio.file.Paths;

public class DragonEditorWithImage {

    private JPanel panel;
    private JLabel picLabel;
    private DragonGUI_Editor master;


    private static final Dimension dim = new Dimension(400,400);

    public DragonEditorWithImage(DragonGUI_Editor master1) {
        this.master = master1;
        this.panel = new JPanel();

        this.panel.setBorder(BorderFactory.createLineBorder(Color.white));

        //setting layout manager and preferred size correctly makes it
        //appear in the correct size.
        this.panel.setLayout(new FlowLayout());

        this.panel.setBackground(DragonGUI_Editor.backgroundColor);
        this.panel.setMinimumSize(new Dimension(600, DragonGUI_Editor.middle_row_height));
        this.panel.setPreferredSize(new Dimension(600, DragonGUI_Editor.middle_row_height));


        try {
            BufferedImage image = new BufferedImage(500, 20, BufferedImage.TYPE_INT_ARGB);
            BufferedImage read = ImageIO.read(Paths.get("test.ppm").toFile());
            this.picLabel = new JLabel(new ImageIcon(read));
        }catch (Exception e){
            e.printStackTrace();
        }

        this.panel.add(this.picLabel);

        this.panel.addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) {
                System.out.println("Key typed:"+e.getKeyChar());
            }

            @Override
            public void keyPressed(KeyEvent e) {

            }

            @Override
            public void keyReleased(KeyEvent e) {

            }
        });
    }

    public JPanel getImage(){
        return this.panel;
    }
}
