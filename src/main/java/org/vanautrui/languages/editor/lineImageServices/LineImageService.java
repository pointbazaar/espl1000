package org.vanautrui.languages.editor.lineImageServices;

import org.apache.commons.io.IOUtils;
import org.vanautrui.languages.editor.DragonEditorWithImage;

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

    public synchronized static MyImagePanel makeImageForLine(String line, int line_index, int cursor_line, int cursor_col) throws Exception{

        //Image image = makeImageForLineInner(line,line_index,cursor_line,cursor_col);
        Image image = makeImageForLineInnerJavaAlternative(line,line_index,cursor_line,cursor_col);

        return new MyImagePanel(image);
    }

    public synchronized static Image makeImageForLineInner(String line, int line_index, int cursor_line,int cursor_col) throws Exception{
        return makeImageForLineInnerJavaAlternative(line,line_index,cursor_line,cursor_col);
    }

    private synchronized static Image makeImageForLineInner2(String line, int line_index, int cursor_line,int cursor_col) throws Exception{

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

    private synchronized static Image makeImageForLineInnerJavaAlternative(String line, int line_index, int cursor_line, int cursor_col){

        //TODO: expand functionality
        int height= charSize;
        int width = charSize*max_columns_per_line;

        int approximate_character_width_px=8;

        //monospace is important so we accurately know cursor position
        final Font font = new Font(Font.MONOSPACED,Font.PLAIN,14);

        int text_x_offset=20;

        BufferedImage img = new BufferedImage(width,height,BufferedImage.TYPE_INT_ARGB);

        Graphics2D g = img.createGraphics();

        //monospace is important, so that we can determine
        //cursor position accurately
        g.setFont(font);

        g.setColor(Color.ORANGE);

        //draw line number
        g.drawString(""+(line_index+1),2,10);

        //draw text
        g.drawString(line,text_x_offset,10);

        if(line_index==cursor_line) {
            //TODO: draw cursor
            //the -4 is avoid drawing it over a character
            g.drawString("|", text_x_offset+cursor_col*approximate_character_width_px-4, 10);
        }

        g.dispose();
        return img;
    }
}
