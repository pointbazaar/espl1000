package org.vanautrui.languages.editor.lineImageServices;

import org.apache.commons.io.IOUtils;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map;

import static org.vanautrui.languages.editor.DragonEditorWithImage.charSize;
import static org.vanautrui.languages.editor.DragonEditorWithImage.max_columns_per_line;

public class LineImageService {

    private static Map<LineImageDrawingProperties,Image> cache = new HashMap<>();

    public synchronized static MyImagePanel makeImageForLine(String line, int line_index, int cursor_line, int cursor_col) throws Exception{


        LineImageDrawingProperties props = new LineImageDrawingProperties(line,line_index,cursor_line,cursor_col);

        if(cache.containsKey(props)){
            System.out.println("makeImageForLine CACHE HIT");
            return new MyImagePanel(cache.get(props));
        }


        Image image = makeImageForLineInner(line,line_index,cursor_line,cursor_col);
        cache.put(props,image);

        return new MyImagePanel(image);
    }

    private synchronized static Image makeImageForLineInner(String line, int line_index, int cursor_line,int cursor_col) throws Exception{

        Runtime rt = Runtime.getRuntime();
        int cursor_position_argument = (cursor_line==line_index)?cursor_col:-1;
        Process pr = rt.exec("./CodeRenderer/crend -l 1 --cursor-position "+cursor_position_argument);
        OutputStream in = pr.getOutputStream();
        in.write((line+"\n").getBytes());
        in.flush();
        pr.waitFor();

        int exit_value = pr.exitValue();
        if(exit_value!=0){
            System.out.println("error in makeImageForLine, crend exited with nonzero exit value");
            System.out.println(IOUtils.toString(pr.getInputStream()));
            System.exit(1);
        }

        InputStream out = pr.getInputStream();
        //String s = IOUtils.toString(out);

        BufferedImage read = ImageIO.read(out);
        //BufferedImage read = ImageIO.

        Image read2 = read.getScaledInstance(charSize* max_columns_per_line,charSize,Image.SCALE_DEFAULT);

        return read2;
    }
}
