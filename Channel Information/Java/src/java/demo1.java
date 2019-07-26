package Final1;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class demo1 {
    public native void channelinfo();

 public  void load(){
  System.out.println("Trying to load JNI");   
     try {
         System.load("S:\\Tomcat\\jni_info.dll");
     } catch (Exception e) {
     e.printStackTrace();
     }
     new demo1().clear();
     new demo1().channelinfo();
  System.out.println("JNI loaded");
 }
 public static demo1 getInstance(){
     return new demo1();
 }

 
 public List<values1> display()
 {
        List<values1> av =new ArrayList<values1>();
        System.out.println("loaded");
        Connection conn=null;
        PreparedStatement pst=null;
        ResultSet rs=null;
        String url="jdbc:postgresql://localhost:5432/logDetails";
        String user="postgres";
        String password="root";
        try {
               Class.forName("org.postgresql.Driver");
           } catch (ClassNotFoundException e) {
           e.getMessage();
           }
       try {
               String Query="SELECT * FROM channelinfo;";
               conn = DriverManager.getConnection(url, user, password);
               pst = conn.prepareStatement(Query);
               rs=pst.executeQuery();
               while ( rs.next() ) {
                    String  cname = rs.getString("channel");
                    String  lsize = rs.getString("logsize");
                    String  nor = rs.getString("noofrecord");
                    String wf=rs.getString("workflow");
                    av.add(new values1(cname,lsize,nor,wf));          
               }
           } catch (SQLException e) {
           e.getMessage();
           } 
        try {
            if(conn!=null)
			conn.close();
            if(rs!=null)
			rs.close();
	if(pst!=null)
            pst.close();
        } catch (SQLException e) {
            e.getMessage();
        }
        return av;
     } 
 public void clear(){
    Connection conn=null;
        PreparedStatement pst=null;
        ResultSet rs=null;
        String url="jdbc:postgresql://localhost:5432/logDetails";
        String user="postgres";
        String password="root"; 
        try {
               Class.forName("org.postgresql.Driver");
           } catch (ClassNotFoundException e) {
           e.getMessage();
           }
       try {
           conn = DriverManager.getConnection(url, user, password);
           String Query="TRUNCATE TABLE channelinfo;";
               pst = conn.prepareStatement(Query);
               pst.executeUpdate();
       }catch(SQLException e){
        e.printStackTrace();       
       }   
 }
           
 public static void insert(String [] val){
        Connection conn=null;
        PreparedStatement pst=null;
        ResultSet rs=null;
        String url="jdbc:postgresql://localhost:5432/logDetails";
        String user="postgres";
        String password="root";
        try {
               Class.forName("org.postgresql.Driver");
           } catch (ClassNotFoundException e) {
           e.getMessage();
           }
       try {
           conn = DriverManager.getConnection(url, user, password);    
           
               
             String Query="INSERT INTO channelinfo(channel,logsize,noofrecord,workflow) VALUES(?,?,?,?);";
               conn = DriverManager.getConnection(url, user, password);
               pst = conn.prepareStatement(Query);
              for(int i=0;i<4;i++){
               System.out.println(val[i]);
               pst.setString(i+1,val[i]);
               }
               pst.executeUpdate();
              
           } catch (SQLException e) {
           e.getMessage();
           } 
        try {
            if(conn!=null)
                conn.close();
            if(rs!=null)
                rs.close();
            if(pst!=null)
            pst.close();
        } catch (SQLException e) {
            e.getMessage();
        }
}
}
