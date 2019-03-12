package org.vanautrui.languages;

import org.junit.Test;
import org.vanautrui.languages.interpreting.AmandaInterpreter;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.junit.Assert.assertEquals;

public class MainMethodBasicOutputTest {

    @Test
    public void can_do_basic_output_in_main_method() throws IOException {
        InputStream in = new InputStream() {
            @Override
            public int read() throws IOException {
                return 0;
            }
        };
        final String[] target = {""};

        OutputStream outputStream = new OutputStream() {
            @Override
            public void write(int i) throws IOException {
                target[0] +=((char)i)+"";
            }
        };
        (new AmandaInterpreter()).execute(String.valueOf(Files.readAllBytes(Paths.get("amanda-files/main_method.amanda"))), in, outputStream);

        assertEquals("Hello World\n", target[0]);
    }
}
