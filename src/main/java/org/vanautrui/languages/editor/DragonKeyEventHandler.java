package org.vanautrui.languages.editor;

import java.awt.*;
import java.awt.event.KeyEvent;

public class DragonKeyEventHandler {

    private DragonGUI_Editor master;

    public DragonKeyEventHandler(DragonGUI_Editor master){
        this.master=master;
    }

    public KeyEventDispatcher getKeyEventDispatcher(){
        KeyEventDispatcher keyEventDispatcher = new KeyEventDispatcher() {
            @Override
            public boolean dispatchKeyEvent(KeyEvent e) {
                //prevent event from being processed
                //so we can do our own global event processing
                //that does not depend on the individual key listeners
                //on all the individual components

                //System.out.println(e.toString());
                //System.out.println(e.paramString());
                String key_event_type = e.paramString().split(",")[0];

                switch (key_event_type){
                    case "KEY_PRESSED":
                        break;
                    case "KEY_RELEASED":
                        break;
                    case "KEY_TYPED":
                        break;
                }
                System.out.println(key_event_type);

                //System.out.println(e.getKeyCode());
                System.out.println("key dispatch: "+e.getKeyChar());

                return true;
            }
        };
        return keyEventDispatcher;
    }
}
