package org.vanautrui.languages.amanda_files_tests;

import org.vanautrui.languages.base.BaseTest;


public class MainMethodBasicOutputTest extends BaseTest {

    /*
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
        (new DragonInterpreter()).execute(String.valueOf(Files.readAllBytes(Paths.get("dragon-files/main.dragon"))), in, new PrintStream(outputStream));

        assertEquals("Hello World\n", target[0]);
    }

     */
}
