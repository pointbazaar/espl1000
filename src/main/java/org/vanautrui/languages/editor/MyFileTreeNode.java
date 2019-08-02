package org.vanautrui.languages.editor;

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreeNode;
import java.util.Enumeration;

public class MyFileTreeNode implements TreeNode,MutableTreeNode {

    //extra info
    private boolean isDirectory;

    public boolean isDirectory(){
        return isDirectory;
    }

    public void add(MyFileTreeNode child){
        this.value.add(child.value);
    }

    public MyFileTreeNode(boolean isDir,DefaultMutableTreeNode newValue){
        this.value=newValue;
        this.isDirectory=isDir;
    }

    private DefaultMutableTreeNode value;

    @Override
    public TreeNode getChildAt(int childIndex) {
        return value.getChildAt(childIndex);
    }

    @Override
    public int getChildCount() {
        return value.getChildCount();
    }

    @Override
    public TreeNode getParent() {
        return value.getParent();
    }

    @Override
    public int getIndex(TreeNode node) {
        return node.getIndex(node);
    }

    @Override
    public boolean getAllowsChildren() {
        return getAllowsChildren();
    }

    @Override
    public boolean isLeaf() {
        return value.isLeaf();
    }

    @Override
    public Enumeration children() {
        return value.children();
    }

    @Override
    public void insert(MutableTreeNode child, int index) {
        value.insert(child,index);
    }

    @Override
    public void remove(int index) {
        value.remove(index);
    }

    @Override
    public void remove(MutableTreeNode node) {
        value.remove(node);
    }

    @Override
    public void setUserObject(Object object) {
        value.setUserObject(object);
    }

    @Override
    public void removeFromParent() {
        value.removeFromParent();
    }

    @Override
    public void setParent(MutableTreeNode newParent) {
        value.setParent(newParent);
    }
}
