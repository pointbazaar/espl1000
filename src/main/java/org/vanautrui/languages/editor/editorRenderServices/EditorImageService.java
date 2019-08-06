package org.vanautrui.languages.editor.editorRenderServices;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.List;

import static org.vanautrui.languages.editor.editorcore.DragonEditorWithImage.charSize;
import static org.vanautrui.languages.editor.editorcore.DragonEditorWithImage.max_columns_per_line;

public class EditorImageService {

    //this should draw an image for the whole editor

    public static synchronized Image getEditorImage(List<String> lines_in_editor,int cursor_line,int cursor_col,Component imageObserver){
        BufferedImage img = new BufferedImage(600,600, BufferedImage.TYPE_INT_ARGB);

        Graphics g = img.getGraphics();

        final int line_height=20;

        //draw the lines
        //g.drawImage()
        int line_index=0;

        for(String line : lines_in_editor){
            Image line_img = LineImageService.makeImageForLineInner(line,line_index,cursor_line,cursor_col);
            g.drawImage(line_img,0,line_index*line_height,charSize*max_columns_per_line,charSize,imageObserver);
            line_index++;
        }

        return img;
    }
}
