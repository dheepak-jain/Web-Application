package Final;

import com.google.gson.Gson;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.*;


@WebServlet(urlPatterns = {"/channelServlet"})
public class channelServlet extends HttpServlet {

   static String cname="Setup";
   
  
     
   public static demo d=new demo();
    @Override
    
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        response.setContentType("application/json");
        
        cname=request.getParameter("channel_name");
        System.out.println(cname);
        System.out.println("hello");
        
        demo obj=demo.getInstance();
        obj.load(cname);
        // System.out.println("hello");
	List<values> arrlist=new ArrayList<values>(); 
        arrlist = d.display(cname);
        new Gson().toJson(arrlist, response.getWriter());
        
        
    }
     
    
   
}

