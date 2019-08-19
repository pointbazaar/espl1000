package org.vanautrui.languages.commandline.compilerphases;

import org.fusesource.jansi.Ansi;
import org.simpleframework.xml.strategy.Strategy;
import org.simpleframework.xml.strategy.Type;
import org.simpleframework.xml.strategy.Visitor;
import org.simpleframework.xml.strategy.VisitorStrategy;
import org.simpleframework.xml.stream.InputNode;
import org.simpleframework.xml.stream.NodeMap;
import org.simpleframework.xml.stream.OutputNode;
import org.vanautrui.languages.TerminalUtil;

public class CompilerPhaseUtils {
    public static void printBeginPhase(String phaseName) {

        TerminalUtil.print(String.format("%-18s",phaseName),Ansi.Color.GREEN);
    }


    public static void printDuration(long start,long end){
        long duration=end-start;
        System.out.println("Duration: "+duration+" ms");
    }

    public static final Strategy getPreferredXMLSerializationStrategyHumanReadable(){
        Strategy strategy = new VisitorStrategy(new Visitor() {
            @Override
            public void read(Type type, NodeMap<InputNode> nodeMap) throws Exception {

            }

            @Override
            public void write(Type type, NodeMap<OutputNode> nodeMap) throws Exception {
                nodeMap.remove("class");
            }
        });
        return strategy;
    };

    public static void printDurationFeedback(long duration /*milliseconds*/){
        String str = duration + " ms";
        if(duration>500) {
            TerminalUtil.println("☠ "+str+" Compilation took too long. This needs to be fixed. Please file an Issue on GitHub.", Ansi.Color.RED);
        }else if(duration>200) {
            TerminalUtil.println("☠ "+str+" we are truly sorry for the delay :(", Ansi.Color.RED);
        }else if(duration>100){
            TerminalUtil.println("✝ "+str+" sorry it took so long!", Ansi.Color.YELLOW);
        }else {
            TerminalUtil.println("☕ " + str, Ansi.Color.GREEN);
        }
    }
}
