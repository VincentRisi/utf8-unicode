///
/// System : Crackle
/// $Source: /main/nedcor/cvsroot/eclipse/workspace/crackle/bbd/crackle/Generator.java,v $
/// $Author: vince $
/// $Date: 2004/08/02 10:10:07 $
/// $Revision: 401.2 $
///

package bbd.jportal;
import java.io.PrintWriter;

public class Writer
{
  public static PrintWriter writer;
  public static String format(String fmt, Object... objects)
  {
    return String.format(fmt,  objects);
  }
  public static void write(String value)
  {
    writer.print(value);
  }
  public static void write(int no, String value)
  {
    writer.print(indent(no)+value);
  }
  public static void writeln(int no, String value)
  {
    writer.println(indent(no)+value);
  }
  public static void writeln(String value)
  {
    writeln(0, value);
  }
  public static void writeln()
  {
    writer.println();
  }
  public static String indent_string = "                                                                                             ";
  public static int indent_size = 2;
  public static String indent(int no)
  {
     int max = indent_string.length();
     int to = no * indent_size;
     if (to > max)
       to = max;
     return indent_string.substring(0,  to);
  }
  public static PrintWriter logger;
  
  public static void logln(String line)
  {
    logger.println(line);
  }
  
}
