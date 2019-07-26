
package Final1;

import com.google.gson.Gson;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

@WebServlet(name = "channelInfoServlet", urlPatterns = {"/channelInfoServlet"})
public class channelInfoServlet extends HttpServlet {
        
      @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
         System.out.println("Servlet is loaded");  
        response.setContentType("application/json");
        demo1 obj=demo1.getInstance();
        obj.load();
      	List<values1> arrlist=new ArrayList<values1>(); 
        arrlist = obj.display();
        new Gson().toJson(arrlist, response.getWriter());
    }
   
}