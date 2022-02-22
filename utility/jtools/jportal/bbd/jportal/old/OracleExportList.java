/// ------------------------------------------------------------------
/// Copyright (c) 1996, 2004 Vincent Risi in Association 
///                          with Barone Budge and Dominick 
/// All rights reserved. 
/// This program and the accompanying materials are made available 
/// under the terms of the Common Public License v1.0 
/// which accompanies this distribution and is available at 
/// http://www.eclipse.org/legal/cpl-v10.html 
/// Contributors:
///    Vincent Risi
/// ------------------------------------------------------------------
/// System : JPortal
/// $Date: 2004/10/18 13:45:48 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal.old;

import bbd.jportal.Database;
import bbd.jportal.Generator;
import bbd.jportal.Table;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;

public class OracleExportList extends Generator
{
  public static String description()
  {
    return "Generate Oracle Export List";
  }
  public static String documentation()
  {
    return "Generate Oracle Export List";
  }
  /**
  * Generates the SQL for ORACLE Table creation.
  */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("EXP: " + output + database.name + ".lst");
      OutputStream outFile = new FileOutputStream(output + database.name + ".Lst");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        String w1 = "TABLES = (";
        String w2 = ",";
        for (int i=0; i < database.tables.size(); i++)
        {
          Table table = database.tables.elementAt(i);
          if (i == database.tables.size()-1)
            w2 = ")";
          outData.println(w1 + table.name + w2);
          w1 = "          ";
        }
        outData.println("DIRECT=Y");
        outData.println("INDEXES=N");
        outData.println("GRANTS=N");
        outData.println("CONSTRAINTS=N");
        outData.println("LOG="+database.name+".log");
        outData.println("RECORDLENGTH=40000");
        outData.flush();
      }
      finally
      {
        outFile.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Oracle SQL IO Error");
    }
  }
}
