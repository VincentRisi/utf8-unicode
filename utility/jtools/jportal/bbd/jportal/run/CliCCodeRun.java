package bbd.jportal.run;

import bbd.jportal.Database;
import bbd.jportal.Table;

import java.io.PrintWriter;

public class CliCCodeRun
{
  public static void generate(Database database, String output, PrintWriter outLog) throws Exception
  {
    var module = new bbd.jportal2.generators.CliCCode();
    module.generate(database, output);
    for (int i=0; i < database.tables.size(); i++)
    {
      Table table = (Table)database.tables.elementAt(i);
      outLog.println(String.format("Code: %s%s.sh", output, table.name));
      outLog.println(String.format("Code: %s%s.cpp", output, table.name));
      outLog.println(String.format("Code: %s%s_snips.h", output, table.name));
    }
  }
}
