package org.vanautrui.languages.editor;

import org.apache.commons.io.FilenameUtils;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class DragonGUI_Editor {

    //https://www.java-tutorial.org/flowlayout.html
    //https://www.guru99.com/java-swing-gui.html

    public static final int min_width =1200;
    public static final int min_height =700;

    private JFrame frame;

    //https://docs.oracle.com/javase/tutorial/uiswing/components/tree.html

    //public Optional<DragonEditorArea> editorArea;
    public Optional<DragonStatusLine> statusBar;
    public Optional<DragonEditorContextArea> contextArea;
    public Optional<DragonEditorWithImage> editorWithImage;

    private void make_splash(){
        JLabel splash = splashScreen();
        frame.getContentPane().add(splash);
        frame.setVisible(true);
        try{
            Thread.sleep(800);
        }catch (Exception e){
            e.printStackTrace();
        }
        frame.getContentPane().remove(splash);
    }

    public DragonGUI_Editor(){

        set_dark_ui();

        this.frame = new JFrame("Dragon Editor");
        this.frame.setBackground(Color.BLACK);

        /*
        KeyEventDispatcher keyEventDispatcher = new KeyEventDispatcher() {
            @Override
            public boolean dispatchKeyEvent(KeyEvent e) {
                //prevent event from being processed
                return true;
            }
        };

         */

        //KeyboardFocusManager.getCurrentKeyboardFocusManager().addKeyEventDispatcher(keyEventDispatcher);


        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //frame.setSize(default_width,default_height);
        frame.setMinimumSize(new Dimension(min_width, min_height));
        //frame.setResizable(false);
        Point p = new Point(300,300);
        frame.setLocation(p);


        JPanel vertical_panel = new JPanel();
        vertical_panel.setBackground(Color.BLACK);

        //https://stackoverflow.com/questions/13510641/add-controls-vertically-instead-of-horizontally-using-flow-layout
        //https://stackoverflow.com/questions/761341/boxlayout-cant-be-shared-error
        vertical_panel.setLayout(new BoxLayout(vertical_panel,BoxLayout.Y_AXIS));

        frame.add(vertical_panel);

        vertical_panel.add(menuBar());

        FlowLayout flowLayoutHorizontal = new FlowLayout();
        JPanel horizontal_panel = new JPanel();
        horizontal_panel.setBackground(Color.BLACK);
        horizontal_panel.setLayout(flowLayoutHorizontal);
        vertical_panel.add(horizontal_panel);

        horizontal_panel.add(projectArea());

        //this.editorArea=Optional.of(new DragonEditorArea(this));
        //horizontal_panel.add(this.editorArea.get().editorArea());

        this.editorWithImage=Optional.of(new DragonEditorWithImage(this));
        try {
            horizontal_panel.add(this.editorWithImage.get().getImage());
        }catch (Exception e){
            e.printStackTrace();
        }

        this.statusBar=Optional.of(new DragonStatusLine(this));
        vertical_panel.add(statusBar.get().statusBar());

        this.contextArea=Optional.of(new DragonEditorContextArea(this));
        horizontal_panel.add(this.contextArea.get().make());

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

    //https://docs.oracle.com/javase/tutorial/uiswing/components/textarea.html

    private JScrollPane projectArea(){
        DefaultMutableTreeNode treeNode = new DefaultMutableTreeNode();



        DefaultMutableTreeNode child1 = new DefaultMutableTreeNode(
                "node 1"
        );

        DefaultMutableTreeNode child1_1 = new DefaultMutableTreeNode(
                "node 1 . 1"
        );



        treeNode.add(child1);
        child1.add(child1_1);

        MyFileTreeNode root;

        try {

            System.out.println(Paths.get(".").getFileName().toString());

            root = populateFileTree(Paths.get("."),12);
            //root.setUserObject(Paths.get(".").getFileName().toString());
            JTree tree = new JTree(root);
            tree.setMinimumSize(new Dimension(200,200));
            tree.setSize(20,20);

            JScrollPane scrollPane = new JScrollPane(tree);
            return scrollPane;
        }catch (Exception e){
            e.printStackTrace();
            System.exit(1);
        }

        return new JScrollPane();
    }

    private boolean deepContainsNoFiles(File file)throws Exception{

        if(file.isFile()){return false;}

        if(file.isDirectory()){

            List<File> files = Files.list(file.toPath()).map(
                    Path::toFile
            ).collect(Collectors.toList());

            if(files.size()==0 ){
                return true;
            }else{
                boolean contains_no_files=true;

                for(File f : files){
                    contains_no_files &= deepContainsNoFiles(f);
                }
                return contains_no_files;
            }
        }

        return false;
    }

    private MyFileTreeNode populateFileTree(Path path, int recursion_depth)throws Exception{

        if(Files.isDirectory(path)){

            List<String> left_out_directories= Arrays.asList(
                    "out","target",".git","build",".gradle",".idea"
            );

            if(left_out_directories.contains(FilenameUtils.getName(path.toString()))){
                throw new Exception("do not want to have that directory in the tree");
            }

            //if directory is empty, do not add it
            if(deepContainsNoFiles(path.toFile())){
                throw new Exception("path "+path.toString()+" is empty");
            }

            //put all the files/directories as children
            MyFileTreeNode node1 = new MyFileTreeNode(true,new DefaultMutableTreeNode(path.getFileName()));


            Stream<Path> list = Files.list(path);
            for(Path p : list.collect(Collectors.toList())){
                if(recursion_depth>0) {
                    try {
                        node1.add(populateFileTree(p, recursion_depth - 1));
                    }catch (Exception e){
                        //pass
                    }
                }
            }
            return node1;
        }else{
            //put the file as a child
            List<String> left_out_file_extensions= Arrays.asList(
                    "class","jar","out","log","iml"
            );

            if(!left_out_file_extensions.contains(FilenameUtils.getExtension(path.getFileName().toString()))){
                MyFileTreeNode node1 = new MyFileTreeNode(false,new DefaultMutableTreeNode(path.getFileName().toString()));
                return node1;
            }else{
                throw new Exception("do not want to have this file in the tree");
            }

        }
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
