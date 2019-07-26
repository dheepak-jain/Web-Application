package Final;
import java.sql.*;
import java.util.*;


public class demo{
   
    
 public native void channel(String cname);

 public  void load(String name){
  
  System.load("S:\\Tomcat\\jni.dll");
  System.out.println(name);
  new demo().channel(name); 
  System.out.println("done");
 }
 public static demo getInstance(){
     return new demo();
 }

 
 public List<values> display(String cname)
 {
        List<values> av =new ArrayList<values>();
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
               String Query="SELECT * FROM loginfo WHERE channel=?;";
               conn = DriverManager.getConnection(url, user, password);
               pst = conn.prepareStatement(Query);
               pst.setString(1,cname);
               rs=pst.executeQuery();
               while ( rs.next() ) {
                    String  pname = rs.getString("provider");
                    String eid  = rs.getString("eventid");
                    String  level = rs.getString("level");
                    String date=rs.getString("createdtime");
                    String rid=rs.getString("recordid");
                    String channel=rs.getString("channel");
                    av.add(new values(pname,eid,level,date,rid,channel));
                    
                    /*System.out.println( "Provier = " + pname );
                    System.out.println("Level = "+level);
                    System.out.println( "Event ID = " + eid );
                    System.out.println( "Created Time = " + date );
                    System.out.println( "Record ID = " + rid );
                    System.out.println( "Channel = " + channel );
                    System.out.println();*/
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
               String Query="INSERT INTO loginfo(provider,eventid,level,createdtime,recordid,channel) VALUES(?,?,?,?,?,?);";
               conn = DriverManager.getConnection(url, user, password);
               pst = conn.prepareStatement(Query);
              for(int i=0;i<6;i++){
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