package org.vanautrui.languages.editor.gui;

import org.vanautrui.languages.editor.editorcore.DragonEditorWithImage;
import org.vanautrui.languages.editor.keyEventHandling.DragonKeyEventHandler;

import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Optional;

public class DragonGUI_Editor {

    //https://www.java-tutorial.org/flowlayout.html
    //https://www.guru99.com/java-swing-gui.html

    public static final int min_width =700;
    public static final int min_height =700;

    public static final int middle_row_height=600;

    public static final Color backgroundColor = new Color(18,30,49,255);

    private JFrame frame;


    //https://docs.oracle.com/javase/tutorial/uiswing/components/tree.html

    //public Optional<DragonEditorArea> editorArea;
    public Optional<DragonStatusLine> statusBar;
    public Optional<DragonEditorWithImage> editorWithImage;

    public Optional<DragonKeyEventHandler> keyEventHandler;

    //is the help window open?
    public Optional<CustomHelpWindow> helpWindow=Optional.empty();

    public Optional<Path> currentFilePath=Optional.empty();

    public DragonGUI_Editor(Optional<Path> filePath){

        set_dark_ui();
        this.frame = new JFrame("Dragon Editor");

        this.frame.setBackground(backgroundColor);

        this.keyEventHandler=Optional.of(new DragonKeyEventHandler(this));


        KeyboardFocusManager.getCurrentKeyboardFocusManager().addKeyEventDispatcher(this.keyEventHandler.get().getKeyEventDispatcherForKeyboardShortcuts());
        KeyboardFocusManager.getCurrentKeyboardFocusManager().addKeyEventDispatcher(this.keyEventHandler.get().getKeyEventDispatcher());

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //frame.setSize(default_width,default_height);
        frame.setMinimumSize(new Dimension(min_width, min_height));
        //frame.setResizable(false);
        Point p = new Point(300,300);
        frame.setLocation(p);


        JPanel vertical_panel = new JPanel();
        vertical_panel.setBackground(backgroundColor);

        //https://stackoverflow.com/questions/13510641/add-controls-vertically-instead-of-horizontally-using-flow-layout
        //https://stackoverflow.com/questions/761341/boxlayout-cant-be-shared-error
        vertical_panel.setLayout(new BoxLayout(vertical_panel,BoxLayout.Y_AXIS));

        frame.add(vertical_panel);

        //we could add a menu bar or something, but then the UI would be less minimalistic

        FlowLayout flowLayoutHorizontal = new FlowLayout();
        JPanel horizontal_panel = new JPanel();
        horizontal_panel.setBackground(backgroundColor);
        horizontal_panel.setLayout(flowLayoutHorizontal);
        vertical_panel.add(horizontal_panel);

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

        //maybe set the content to the file
        if(filePath.isPresent() && this.editorWithImage.isPresent()){
            try {
                this.currentFilePath = filePath;
                this.editorWithImage.get().loadFile(filePath.get().toFile());
            }catch (Exception e){
                e.printStackTrace();
                System.err.println("something went wrong reading the file");
                System.exit(1);
            }
        }

        frame.setVisible(true);
    }

    //https://docs.oracle.com/javase/tutorial/uiswing/components/textarea.html

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

    public void tryOpenFile(){
        //TODO: make sure that it is actually a
        //file for a good programming language we are opening, not some PHP or Python or Javascript.

        String[] supported_extensions = new String[]{
                ".java",".cpp","hpp",".c",".h",".dragon",".dg"
        };

        final JFileChooser  fileChooser = new JFileChooser();

        //https://docs.oracle.com/javase/tutorial/uiswing/components/filechooser.html
        //disable just selecting any file
        fileChooser.setAcceptAllFileFilterUsed(false);

        fileChooser.setFileFilter(new FileFilter() {
            @Override
            public boolean accept(File f) {
                if(f.isDirectory()){
                    return true;
                }

                if(
                        Arrays.asList(supported_extensions).stream().filter(str->f.getName().endsWith(str)).findAny().isPresent()
                ){
                    if(f.isFile()) {
                        return true;
                    }
                }
                return false;
            }

            @Override
            public String getDescription() {
                return null;
            }
        });

        fileChooser.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                File selectedFile = fileChooser.getSelectedFile();
                System.out.println("file selected:");
                System.out.println(selectedFile.getAbsolutePath());

                maybeShutdownDependingOnFileExtension(selectedFile);

                if(editorWithImage.isPresent()){
                    try {
                        editorWithImage.get().loadFile(selectedFile);

                        currentFilePath=Optional.of(selectedFile.toPath());
                    }catch (Exception ee){
                        ee.printStackTrace();
                    }
                }
            }
        });

        int returnValue = fileChooser.showOpenDialog(this.frame);
    }

    private void maybeShutdownDependingOnFileExtension(File selectedFile) {
        //if it is a python, javascript or other weakly typed source file
        //we should shut down the computer to prevent further mistakes
        String[] wrong_extensions = new String[]{
                ".py",".js",".php"
        };
        for(String extension : Arrays.asList(wrong_extensions)){

            if(selectedFile.getName().endsWith(extension)){
                try {
                    Process p = new ProcessBuilder().command("shutdown -h now").start();
                    p.waitFor();
                    
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }
    }

    public void trySaveFile(){
        //try to save file in the location specified when opening
        //the file
        if(currentFilePath.isPresent()){
            try {
                Files.write(currentFilePath.get(), editorWithImage.get().getCurrentBufferAsString().getBytes());
            }catch (Exception e){
                e.printStackTrace();
            }
        }else{
            System.err.println("no file currently opened. cannot save.");
        }
    }

}
