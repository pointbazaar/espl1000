package org.vanautrui.languages.editor;

import com.sun.javafx.iio.ImageStorage;
import org.apache.commons.io.IOUtils;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.stream.Collectors;

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
            Image read2 = read.getScaledInstance(20,20,Image.SCALE_DEFAULT);


            this.picLabel = new JLabel(new ImageIcon(read2));
        }catch (Exception e){
            e.printStackTrace();
        }

        this.panel.add(this.picLabel);

        this.panel.addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) {
                System.out.println("Key :"+e.getKeyChar());
            }

            @Override
            public void keyPressed(KeyEvent e) {
                System.out.println("Key :"+e.getKeyChar());
            }

            @Override
            public void keyReleased(KeyEvent e) {
                System.out.println("Key :"+e.getKeyChar());
            }
        });
    }

    public void appendLineTest(){
        try {
            Runtime rt = Runtime.getRuntime();
            String line="A";

            Process pr = rt.exec("./CodeRenderer/crend -r "+line+" -l 1");
            //wait for the renderer to terminate
            pr.waitFor();

            InputStream out = pr.getInputStream();
            //String image_in_ppm_format = IOUtils.toString(out);

            //System.out.println("crend gave us : ");
            //System.out.println(image_in_ppm_format);

            BufferedImage read = ImageIO.read(out);
            //BufferedImage read = new BufferedImage(30,30, ImageStorage.ImageType.RGB);
            Image read2 = read.getScaledInstance(20,20,Image.SCALE_DEFAULT);

            //TODO: figure out why this is not visible
            JLabel line_img = new JLabel(new ImageIcon(read2));
            line_img.setMinimumSize(new Dimension(20,20));

            this.panel.add(line_img);
            this.panel.add(new MyImagePanel(read2));

            //this.panel.setVisible(true);
            this.panel.updateUI();

            System.out.println("try to append line");

        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public JPanel getImage(){

        return this.panel;
    }
}
