package org.vanautrui.languages.editor;

import org.apache.commons.io.FilenameUtils;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.*;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class DragonProjectArea {

    private DragonGUI_Editor master;

    public DragonProjectArea(DragonGUI_Editor master){
        this.master=master;
    }

    public JScrollPane projectArea(){
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

        JScrollPane result = new JScrollPane();
        result.setLayout(new BoxLayout(result,BoxLayout.Y_AXIS));
        result.setMinimumSize(new Dimension(400,DragonGUI_Editor.middle_row_height));
        result.setPreferredSize(new Dimension(400,DragonGUI_Editor.middle_row_height));
        return result;
    }

    private boolean deepContainsNoFiles(File file)throws Exception{

        if(file.isFile()){return false;}

        if(file.isDirectory()){

            java.util.List<File> files = Files.list(file.toPath()).map(
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

            java.util.List<String> left_out_directories= Arrays.asList(
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
}
