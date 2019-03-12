package org.vanautrui.languages.base;
import org.junit.Test;
import org.vanautrui.languages.interpreting.AmandaInterpreter;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.junit.Assert.assertEquals;

public class BaseTest {

    public static InputStream getEmptyInputStream(){
        return new InputStream() {
            @Override
            public int read() throws IOException {
                return 0;
            }
        };
    }
}
