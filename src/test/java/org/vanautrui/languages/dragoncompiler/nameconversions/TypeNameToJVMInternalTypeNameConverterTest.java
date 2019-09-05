package org.vanautrui.languages.dragoncompiler.nameconversions;

import org.junit.Assert;
import org.junit.Test;

import java.util.Arrays;

public class TypeNameToJVMInternalTypeNameConverterTest {

    @Test
    public void test_convert_simple()throws Exception{
        String result = TypeNameToJVMInternalTypeNameConverter.convertTypeNameToJVMInternalTypeName("Int");
        Assert.assertEquals("I",result);
    }

    @Test
    public void test_convert_array()throws Exception{
        String result = TypeNameToJVMInternalTypeNameConverter.convertTypeNameToJVMInternalTypeName("[Int]");
        Assert.assertEquals("[I",result);
    }

    @Test
    public void test_convert_object()throws Exception{
        String result = TypeNameToJVMInternalTypeNameConverter.convertTypeNameToJVMInternalTypeName("MyObject");
        Assert.assertEquals("LMyObject;",result);
    }

    @Test
    public void test_convert_subroutine()throws Exception{
        String result = TypeNameToJVMInternalTypeNameConverter.convertSubroutineName("Int", Arrays.asList("Int"),true);
        Assert.assertEquals("(I)I",result);
    }
}
