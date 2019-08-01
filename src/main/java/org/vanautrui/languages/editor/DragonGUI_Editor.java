package org.vanautrui.languages.editor;

import javafx.geometry.Rectangle2D;
import javafx.stage.Screen;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.Optional;

public class DragonGUI_Editor {

    //https://www.guru99.com/java-swing-gui.html

    public static final int default_width=800;
    public static final int default_height=300;

    private JFrame frame;

    public Optional<DragonEditorArea> editorArea;
    public Optional<DragonStatusLine> statusBar;

    public DragonGUI_Editor(){

        set_dark_ui();

        this.frame = new JFrame("Dragon Editor");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //frame.setSize(default_width,default_height);
        frame.setMinimumSize(new Dimension(default_width,default_height));
        frame.setResizable(false);
        Point p = new Point(300,300);
        frame.setLocation(p);

        JLabel splash = splashScreen();
        frame.getContentPane().add(splash);
        frame.setVisible(true);
        try{
            Thread.sleep(800);
        }catch (Exception e){
            e.printStackTrace();
        }
        frame.getContentPane().remove(splash);

        frame.getContentPane().add(BorderLayout.NORTH, menuBar());
        frame.getContentPane().add(BorderLayout.WEST,projectArea());
        this.editorArea=Optional.of(new DragonEditorArea(this));
        frame.getContentPane().add(BorderLayout.CENTER,this.editorArea.get().editorArea());

        this.statusBar=Optional.of(new DragonStatusLine(this));
        frame.getContentPane().add(BorderLayout.SOUTH,statusBar.get().statusBar());

        frame.getContentPane().add(BorderLayout.EAST,new JButton("context information, file navigation"));

        frame.setVisible(true);

    }



    private JLabel splashScreen(){
        System.out.println("making splash screen");
        try {
            BufferedImage splash = ImageIO.read(new File("splashscreens/programming.jpeg"));
            JLabel picLabel = new JLabel(new ImageIcon(splash));
            return picLabel;
        }catch (Exception e){
            e.printStackTrace();
            return new JLabel("could not read image "+e.getMessage());
        }

    }

    private JMenuBar menuBar(){
        JMenuBar mb = new JMenuBar();
        JButton interpret = new JButton("Interpret Program");
        interpret.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("clicked on interpret. TODO: excute davids program");
            }
        });
        mb.add(interpret);

        return mb;
    }

    private JTree projectArea(){
        DefaultMutableTreeNode treeNode = new DefaultMutableTreeNode();

        DefaultMutableTreeNode child1 = new DefaultMutableTreeNode(
                "node 1"
        );

        DefaultMutableTreeNode child1_1 = new DefaultMutableTreeNode(
                "node 1 . 1"
        );



        treeNode.add(child1);
        child1.add(child1_1);

        JTree tree = new JTree(treeNode);

        tree.setMaximumSize(new Dimension(200,200));
        return tree;
    }



    private void set_dark_ui(){
        UIManager.put( "control", new Color( 128, 128, 128) );
        UIManager.put( "info", new Color(128,128,128) );
        UIManager.put( "nimbusBase", new Color( 18, 30, 49) );
        UIManager.put( "nimbusAlertYellow", new Color( 248, 187, 0) );
        UIManager.put( "nimbusDisabledText", new Color( 128, 128, 128) );
        UIManager.put( "nimbusFocus", new Color(115,164,209) );
        UIManager.put( "nimbusGreen", new Color(176,179,50) );
        UIManager.put( "nimbusInfoBlue", new Color( 66, 139, 221) );
        UIManager.put( "nimbusLightBackground", new Color( 18, 30, 49) );
        UIManager.put( "nimbusOrange", new Color(191,98,4) );
        UIManager.put( "nimbusRed", new Color(169,46,34) );
        UIManager.put( "nimbusSelectedText", new Color( 255, 255, 255) );
        UIManager.put( "nimbusSelectionBackground", new Color( 104, 93, 156) );
        UIManager.put( "text", new Color( 230, 230, 230) );
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (javax.swing.UnsupportedLookAndFeelException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
