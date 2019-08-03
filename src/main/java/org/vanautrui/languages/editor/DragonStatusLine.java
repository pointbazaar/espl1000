package org.vanautrui.languages.editor;

import javax.swing.*;
import java.awt.*;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;

public class DragonStatusLine {

    private JPanel panel;
    private DragonGUI_Editor master;


    private JLabel line_number_label;
    private JLabel line_count_label;
    private JLabel my_ip_address;

    public DragonStatusLine(DragonGUI_Editor master1){
        //we use it in constructor,
        //because it depends on the editor area
        //and needs to know about it, but not the other way around
        this.master=master1;

        this.panel = new JPanel();
        this.panel.setBackground(DragonGUI_Editor.backgroundColor);
        BoxLayout layout=new BoxLayout(this.panel,BoxLayout.X_AXIS);

        this.panel.setLayout(layout);
        this.panel.setMaximumSize(new Dimension(2000,30));

        this.line_number_label=new JLabel("TODO: linu number");
        this.line_count_label=new JLabel("TODO: line count label");

        //https://stackoverflow.com/questions/8083479/java-getting-my-ip-address

        String my_ip_string="TODO: display my ip address";
        String ip="";
        try {
            Enumeration<NetworkInterface> networkInterfaces = NetworkInterface.getNetworkInterfaces();
            while (networkInterfaces.hasMoreElements()) {
                NetworkInterface networkInterface = networkInterfaces.nextElement();
                if(networkInterface.isLoopback() || !networkInterface.isUp()){
                    continue;
                }

                System.out.println(networkInterface.getDisplayName());
                System.out.println(networkInterface.getInetAddresses());
                System.out.println(networkInterface.getName());

                Enumeration<InetAddress> addresses = networkInterface.getInetAddresses();
                while(addresses.hasMoreElements()) {
                    InetAddress addr = addresses.nextElement();
                    ip = addr.getHostAddress();
                    System.out.println(networkInterface.getDisplayName() + " " + ip);
                }
            }

            my_ip_string=ip;
        }catch (Exception e){
            e.printStackTrace();
        }

        this.my_ip_address=new JLabel("    "+my_ip_string+"    ");

        panel.add(new JLabel("TODO: status bar"));
        panel.add(this.line_number_label);
        panel.add(this.line_count_label);

        panel.add(this.my_ip_address);
    }

    public void setCursorPos(int pos){
        this.line_number_label.setText("position: "+pos);
    }

    public Component statusBar(){
        return panel;
    }

    public void updateLineCount(int lines){
        this.line_count_label.setText(lines+" Lines");
    }
}
