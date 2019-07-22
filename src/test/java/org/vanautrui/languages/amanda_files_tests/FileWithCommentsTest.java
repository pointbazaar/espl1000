package org.vanautrui.languages.amanda_files_tests;

import org.junit.Test;
import org.vanautrui.languages.base.BaseTest;
import org.vanautrui.languages.interpreting.DragonInterpreter;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.junit.Assert.assertEquals;

public class FileWithCommentsTest extends BaseTest {

    @Test
    public void test() throws Exception{
        InputStream in = BaseTest.getEmptyInputStream();
        final String[] target = {""};

        OutputStream outputStream = new OutputStream() {
            @Override
            public void write(int i) throws IOException {
                target[0] +=((char)i)+"";
            }
        };

        PrintStream outStream = new PrintStream(outputStream);
        (new DragonInterpreter()).execute(String.valueOf(Files.readAllBytes(Paths.get("dragon-files/filewithcomments.dragon"))), in, outStream);

        assertEquals("", target[0]);
    }
}
