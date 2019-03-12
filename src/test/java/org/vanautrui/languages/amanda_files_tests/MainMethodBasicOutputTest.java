package org.vanautrui.languages.amanda_files_tests;

import org.junit.Test;
import org.vanautrui.languages.base.BaseTest;
import org.vanautrui.languages.interpreting.AmandaInterpreter;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.junit.Assert.assertEquals;


public class MainMethodBasicOutputTest extends BaseTest {

    @Test
    public void can_do_basic_output_in_main_method() throws Exception {
        InputStream in = BaseTest.getEmptyInputStream();
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
