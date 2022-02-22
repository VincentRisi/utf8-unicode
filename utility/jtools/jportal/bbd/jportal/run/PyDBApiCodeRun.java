package bbd.jportal.run;

import bbd.jportal.Database;
import bbd.jportal.Table;

import java.io.PrintWriter;

public class PyDBApiCodeRun
{
  public static void generate(Database database, String output, PrintWriter outLog) throws Exception
  {
    var module = new bbd.jportal2.generators.PyDBApiCode();
    module.generate(database, output);
    for (int i=0; i < database.tables.size(); i++)
    {
      Table table = (Table)database.tables.elementAt(i);
      outLog.println(String.format("Code: %s%sDBApi.py", output, table.name));
    }
  }
}
