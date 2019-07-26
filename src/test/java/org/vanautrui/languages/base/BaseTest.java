package org.vanautrui.languages.base;

import java.io.IOException;
import java.io.InputStream;

public class BaseTest {

    public static InputStream getEmptyInputStream() {
        return new InputStream() {
            @Override
            public int read() throws IOException {
                return 0;
            }
        };
    }
}
