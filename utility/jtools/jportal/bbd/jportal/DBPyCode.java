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
/// $Date: 2004/10/18 13:45:47 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

public class DBPyCode extends Generator
{
  public static String description()
  {
    return "Generate DB Python Code";
  }
  public static String documentation()
  {
    return "Generate DB Python Code";
  }
  /**
   * Padding function
   */
  static String padder(String s, int length)
  {
    for (int i = s.length(); i < length-1; i++)
      s = s + " ";
    return s + " ";
  }
  /**
  * Generates the procedure classes for each table present.
  */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    for (int i=0; i<database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      generatePython(table, output, outLog);
    }
  }
  private static void generatePython(Table table, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + table.useName() + "AnyDB.py");
      OutputStream outFile = new FileOutputStream(output + table.useName() + "AnyDB.py");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        try
        {
          outData.println("# This code was generated, do not modify it, modify it at source and regenerate it.");
          outData.println();
          outData.println("def load(idl2, procs):");
          outData.println("  Proc = idl2.Proc");
          outData.println("  SINGLE   = idl2.SINGLE");
          outData.println("  MULTIPLE = idl2.MULTIPLE");
          outData.println("  ACTION   = idl2.ACTION");
          for (int i = 0; i < table.procs.size(); i++)
          {
            Proc proc = table.procs.elementAt(i);
            if (proc.isData)
              continue;
            if (proc.outputs.size() > 0)
              if (proc.isSingle)
                generatePythonSingle(table, proc, outData);
              else
                generatePythonMultiple(table, proc, outData);
            else
              generatePythonAction(table, proc, outData);
          }
          outData.println("  return procs");
        }
        finally
        {
          outData.flush();
        }
      }
      finally
      {
        outFile.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
  }
  private static void generatePythonSingle(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    String extended = "True";
    if (proc.isStdExtended())
      dataStruct = table.useName();
    else
    {
      extended = "False";
      dataStruct = table.useName() + proc.upperFirst();
    }
    outData.println("  procs.append(Proc('" + proc.name + "', '" + dataStruct + "', '" + table.useName() + "', SINGLE, " + extended + ", True, True) )");
  }
  private static void generatePythonMultiple(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    String input = "False";
    if (proc.inputs.size() > 0 || proc.dynamics.size() > 0)
      input = "True";
    String extended = "True";
    if (proc.isStdExtended())
      dataStruct = table.useName();
    else
    {
      extended = "False";
      dataStruct = table.useName() + proc.upperFirst();
    }
    outData.println("  procs.append(Proc('" + proc.name + "', '" + dataStruct + "', '" + table.useName() + "', MULTIPLE, " + extended + ", " + input + ", True) )");
  }
  private static void generatePythonAction(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    String input = "False";
    if (proc.inputs.size() > 0 || proc.dynamics.size() > 0)
      input = "True";
    String output = "False";
    if (proc.hasModifieds())
      output = "True";
    String extended = "True";
    if (proc.isStdExtended())
      dataStruct = table.useName();
    else
    {
      extended = "False";
      dataStruct = table.useName() + proc.upperFirst();
    }
    outData.println("  procs.append(Proc('" + proc.name + "', '" + dataStruct + "', '" + table.useName() + "', ACTION, " + extended + ", " + input + ", " + output + ") )");
  }
}
