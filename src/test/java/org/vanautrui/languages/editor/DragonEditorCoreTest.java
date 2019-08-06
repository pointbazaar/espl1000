package org.vanautrui.languages.editor;

import org.junit.Assert;
import org.junit.Test;
import org.omg.CORBA.PUBLIC_MEMBER;
import org.vanautrui.languages.editor.editorcore.DragonEditorCore;

public class DragonEditorCoreTest {

    @Test
    public void test_getLineCount(){

        DragonEditorCore core = new DragonEditorCore();

        Assert.assertEquals(1,core.getLineCount());
    }

    @Test
    public void test_getCursorLine(){
        DragonEditorCore core = new DragonEditorCore();

        Assert.assertEquals(0,core.getCursorLine());
    }

    @Test
    public void test_getCursorLineDisplay(){
        DragonEditorCore core = new DragonEditorCore();

        Assert.assertEquals(core.getCursorLine()+1,core.getCursorLineDisplay());
    }

    @Test
    public void test_arrowDownChangeLine_test_adjustCursorColumnToBeContainedInLine() throws Exception{
        DragonEditorCore core = new DragonEditorCore();

        core.writeCharcter('a');
        core.pressEnter();

        core.writeCharcter('b');
        core.writeCharcter('c');

        core.arrowUpChangeLine();
        core.arrowDownChangeLine();

        Assert.assertEquals(1,core.getCursorCol());
        Assert.assertEquals(1,core.getCursorLine());
    }

    @Test
    public void test_stringBeforeCursor()throws Exception{
        DragonEditorCore core = new DragonEditorCore();

        Assert.assertEquals("",core.stringBeforeCursor());

        core.writeCharcter('a');

        Assert.assertEquals("a",core.stringBeforeCursor());
    }

    @Test
    public void test_stringAfterCursor()throws Exception{
        DragonEditorCore core = new DragonEditorCore();

        Assert.assertEquals("",core.stringAfterCursor());

        core.writeCharcter('a');
        core.arrowLeftMoveCursor();

        Assert.assertEquals("a",core.stringAfterCursor());
    }

    @Test
    public void test_writeCharacter()throws Exception{
        //this test should test that not everything gets accepted as text
        //to be written to the lines

        DragonEditorCore core = new DragonEditorCore();

        try{
            core.writeCharcter('\b');
            Assert.fail();
        }catch (Exception e){
            //pass
        }

        try{
            //this is a spaces only editor
            //because thats easier to implement for drawing purposes
            core.writeCharcter('\t');
            Assert.fail();
        }catch (Exception e){
            //pass
        }

        //TODO: other test cases
    }
}
