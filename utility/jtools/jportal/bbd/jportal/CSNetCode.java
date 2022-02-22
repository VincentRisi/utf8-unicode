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
/// $Date: 2004/10/18 13:45:43 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

import static bbd.jportal.Writer.*;

public class CSNetCode extends Generator
{
  protected static PrintWriter outLog;

  public static String description()
  {
    return "Generate C# Code for ADO.NET via IDbConnection";
  }

  public static String documentation()
  {
    return """
            Generate C# Code for ADO.NET via IDbConnection
            DATABASE name FLAGS flag
            - "mssql storedprocs" generate stored procedures for MSSql
            - "use generics" generate lists as generics
            - "use partials" generate classes as partials
            - "use yields" generate code for yields
            - "no datatables" do not generate code for datatables
            - "use C# 2.0" generate classes with above uses
            - "use C# 1.0" generate classes compatable with C# 1.0
            - "use separate" generate classes in separate files
            - "use notify" generate classes with INotifyPropertyChanged implemented"""
            ;
  }

  protected static Vector<Flag> flagsVector;
  static boolean mSSqlStoredProcs;
  static boolean hasStoredProcs;
  static boolean useGenerics;
  static boolean usePartials;
  static boolean useSeparate;
  static boolean noDatatables;
  static boolean useYields;
  static boolean useCSharp2;
  static boolean useCSharp1;
  static boolean useNotify;
  static boolean useFunc;
  static String version = "4.0.";
  static String runTimeVersion = "4.0.30319";

  private static void flagDefaults()
  {
    hasStoredProcs = false;
    mSSqlStoredProcs = false;
    useGenerics = false;
    usePartials = false;
    useYields = false;
    useSeparate = false;
    noDatatables = false;
    useCSharp1 = false;
    useCSharp2 = false;
    useNotify = false;
    useFunc = false;
  }

  public static Vector<Flag> flags()
  {
    if (flagsVector == null)
    {
      flagsVector = new Vector<>();
      flagDefaults();
      flagsVector.addElement(new Flag("mssql storedprocs", mSSqlStoredProcs, "Generate MSSql Stored Procedures"));
      flagsVector.addElement(new Flag("use generics", useGenerics, "Generate C# 2.0 Generics"));
      flagsVector.addElement(new Flag("use partials", usePartials, "Generate C# 2.0 Partials"));
      flagsVector.addElement(new Flag("use yields", useYields, "Generate C# 2.0 Yields"));
      flagsVector.addElement(new Flag("use separate", useSeparate, "Generate Separate Files"));
      flagsVector.addElement(new Flag("no datatables", noDatatables, "Do not Generate Datatables"));
      flagsVector.addElement(new Flag("use C#2.0", useCSharp2, "Generate for C#2.0"));
      flagsVector.addElement(new Flag("use C#1.0", useCSharp1, "Generate for C#1.0"));
      flagsVector.addElement(new Flag("use notify", useNotify, "Generate for INotifyPropertyChanged"));
      flagsVector.addElement(new Flag("use func", useFunc, "Generate Functions"));
    }
    return flagsVector;
  }

  /**
   * Sets generation flags.
   */
  static void setFlags(Database database)
  {
    if (flagsVector != null)
    {
      mSSqlStoredProcs = toBoolean((flagsVector.elementAt(0)).value);
      useGenerics = toBoolean((flagsVector.elementAt(1)).value);
      usePartials = toBoolean((flagsVector.elementAt(2)).value);
      useYields = toBoolean((flagsVector.elementAt(3)).value);
      useSeparate = toBoolean((flagsVector.elementAt(4)).value);
      noDatatables = toBoolean((flagsVector.elementAt(5)).value);
      useCSharp2 = toBoolean((flagsVector.elementAt(6)).value);
      useCSharp1 = toBoolean((flagsVector.elementAt(7)).value);
      useNotify = toBoolean((flagsVector.elementAt(8)).value);
      useFunc = toBoolean((flagsVector.elementAt(9)).value);
    } else
      flagDefaults();
    for (int i = 0; i < database.flags.size(); i++)
    {
      String flag = database.flags.elementAt(i);
      if (flag.equalsIgnoreCase("mssql storedprocs"))
        mSSqlStoredProcs = true;
      else if (flag.equalsIgnoreCase("use generics"))
        useGenerics = true;
      else if (flag.equalsIgnoreCase("use partials"))
        usePartials = true;
      else if (flag.equalsIgnoreCase("use yields"))
        useYields = true;
      else if (flag.equalsIgnoreCase("use separate"))
        useSeparate = true;
      else if (flag.equalsIgnoreCase("no datatables"))
        noDatatables = true;
      else if (flag.equalsIgnoreCase("use C#2.0"))
        useGenerics = usePartials = useYields = useCSharp2 = true;
      else if (flag.equalsIgnoreCase("use C#1.0"))
      {
        useGenerics = usePartials = useYields = useCSharp2 = false;
        useCSharp1 = true;
        runTimeVersion = "1.1";
      } else if (flag.equalsIgnoreCase("use notify"))
        useNotify = true;
      else if (flag.equalsIgnoreCase("use func"))
        useFunc = true;
    }
    if (mSSqlStoredProcs)
      outLog.println(" (mssql storedprocs)");
    if (useGenerics)
      outLog.println(" (use generics)");
    if (usePartials)
      outLog.println(" (use partials)");
    if (useYields)
      outLog.println(" (use yields)");
    if (useSeparate)
      outLog.println(" (use separate)");
    if (noDatatables)
      outLog.println(" (no datatables)");
    if (useNotify)
      outLog.println(" (use notify)");
    if (useFunc)
      outLog.println(" (use func)");
  }

  public static void generate(Database database, String output, PrintWriter outLog)
  {
    CSNetCode.outLog = outLog;
    setFlags(database);
    for (int i = 0; i < database.tables.size(); i++)
    {
      try
      {
        Table table = database.tables.elementAt(i);
        generate(table, output);
      }
      catch (Exception ex)
      {
        outLog.println(ex);
        ex.printStackTrace(outLog);
      }
    }
  }

  static OutputStream procFile;
  static PrintWriter procData;

  static void generate(Table table, String output) throws Exception
  {
    OutputStream outFile;
    String added = "";
    if (useSeparate == true)
      added = "Structs";
    outFile = openOutputStream(table, output, added);
    hasStoredProcs = mSSqlStoredProcs == false && isStoredProcs(table);
    if (mSSqlStoredProcs == true || hasStoredProcs == true)
    {
      outLog.println("DDL: " + output + table.useName() + ".sproc.sql");
      procFile = new FileOutputStream(output + table.name + ".sproc.sql");
      procData = new PrintWriter(procFile);
      procData.println("USE " + table.database.name);
      procData.println();
    }
    openWriterPuttingTop(table, outFile);
    generateStructs(table);
    if (!noDatatables)
    {
      if (useSeparate == true)
      {
        writeln("}");
        writer.flush();
        outFile.close();
        outFile = openOutputStream(table, output, "Tables");
        openWriterPuttingTop(table, outFile);
      }
      generateDataTables(table);
    }
    if (useSeparate == true)
    {
      writeln("}");
      writer.flush();
      outFile.close();
      outFile = openOutputStream(table, output, "");
      openWriterPuttingTop(table, outFile);
    }
    generateCode(table);
    writeln("}");
    writer.flush();
    if (mSSqlStoredProcs == true || hasStoredProcs == true)
    {
      procData.flush();
      procFile.close();
    }
  }

  private static void openWriterPuttingTop(Table table, OutputStream outFile)
  {
    writer = new PrintWriter(outFile);
    String packageName = table.database.packageName;
    if (packageName.length() == 0)
      packageName = "Bbd.JPortal";
    writeln("//------------------------------------------------------------------------------");
    writeln("// <auto-generated>");
    writeln("//     This code was generated by a tool.");
    writeln("//     Runtime Version: " + runTimeVersion);
    writeln("//");
    writeln("//     Changes to this file may cause incorrect behavior and will be lost if");
    writeln("//     the code is regenerated.");
    writeln("// </auto-generated>");
    writeln("//------------------------------------------------------------------------------");
    writeln("");
    writeln("// ");
    writeln("// This source code was auto-generated by jportal.jar, Version=" + version);
    writeln("// ");
    writeln("using System;");
    if (useGenerics)
      writeln("using System.Collections.Generic;");
    else
    {
      writeln("using System.Collections;");
      writeln("using System.Collections.Specialized;");
    }
    if (useNotify)
    {
      writeln("using System.ComponentModel;");
      writeln("using System.Runtime.CompilerServices;");
    }
    writeln("using System.Data;");
    if (table.database.portalName.length() == 0)
      writeln("using Bbd.Idl2.AnyDb;");
    else
      writeln("using " + table.database.portalName + ";");
    writeln("");
    writeln("namespace " + packageName);
    writeln("{");
    writer.flush();
  }

  private static OutputStream openOutputStream(Table table, String output, String added) throws IOException
  {
    OutputStream outFile;
    String outFileName = output + table.name + added + ".cs";
    outLog.println("Code: " + outFileName);
    outFile = new FileOutputStream(outFileName);
    return outFile;
  }

  private static void generateSelector(Field field)
  {
    writeln(2, "public class Nfpp" + field.useUpperName() + " // NO _______ PARAMETER PROPERTIES ");
    writeln(2, "{");
    writeln(3, "DataTable parent;");
    writeln(3, "public " + fieldCastNo(field) + " this[int row]");
    writeln(3, "{");
    writeln(4, "get");
    writeln(4, "{");
    if (field.isNull == true)
    {
      writeln(5, "if (parent.Rows[row][c" + field.useUpperName() + "] == DBNull.Value)");
      writeln(5, "{");
      if (useCSharp1)
      {
        writeln(6, "return " + validNull(field) + ";");
      } else
      {
        writeln(6, "return default(" + fieldCastNo(field) + ");");
      }
      writeln(5, "}");
      writeln(5, "else");
      writeln(5, "{");
      writeln(6, "return (" + fieldCastNo(field) + ")parent.Rows[row][c" + field.useUpperName() + "];");
      writeln(5, "}");
    } else
      writeln(5, "return (" + fieldCastNo(field) + ")parent.Rows[row][c" + field.useUpperName() + "];");
    writeln(4, "}");
    writeln(4, "set");
    writeln(4, "{");
    writeln(5, "parent.Rows[row][c" + field.useUpperName() + "] = value;");
    writeln(4, "}");
    writeln(3, "}");
    if (field.isNull == true)
    {
      writeln(3, "public bool IsNull(int row){ return parent.Rows[row].IsNull(c" + field.useUpperName() + "); }");
      writeln(3, "public void SetNull(int row){ parent.Rows[row][c" + field.useUpperName() + "] = DBNull.Value; }");
    }
    writeln(3, "public Nfpp" + field.useUpperName() + "(DataTable parent) { this.parent = parent; }");
    writeln(2, "}");
    writeln(2, "private Nfpp" + field.useUpperName() + " m" + field.useUpperName() + ";");
    writeln(2, "public Nfpp" + field.useUpperName() + " " + field.useUpperName());
    writeln(2, "{");
    writeln(3, "get { return m" + field.useUpperName() + "; }");
    writeln(3, "set { m" + field.useUpperName() + " = value; }");
    writeln(2, "}");
  }

  public static boolean isStoredProcs(Table table)
  {
    boolean hasStoredProcs = false;
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isSProc == true)
        return true;
      //if (proc.outputs.size() == 0 || proc.isSingle)
      //  continue;
    }
    return hasStoredProcs;
  }

  public static void generateDataTables(Table table)
  {
    boolean hasDataTables = false;
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.outputs.size() == 0 || proc.isSingle)
        continue;
      hasDataTables = true;
      writeln(1, "[Serializable()]");
      writeln(1, "public " + (usePartials ? "partial " : "") + "class " + table.useName() + proc.upperFirst() + "DataTable : DataTable");
      writeln(1, "{");
      int noInDataSet = 0;
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        writeln(2, "public const int c" + field.useUpperName() + " = " + noInDataSet + ";");
        noInDataSet++;
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        writeln(2, "public const int c" + field.useUpperName() + " = " + noInDataSet + ";");
        noInDataSet++;
      }
      writeln(2, "public static string ToString(int ordinal)");
      writeln(2, "{");
      writeln(3, "switch (ordinal)");
      writeln(3, "{");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        writeln(4, "case c" + field.useUpperName() + ": return \"" + field.useUpperName() + "\";");
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        writeln(4, "case c" + field.useUpperName() + ": return \"" + field.useUpperName() + "\";");
      }
      writeln(3, "}");
      writeln(3, "return \"<??\"+ordinal+\"??>\";");
      writeln(2, "}");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        generateSelector(field);
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        generateSelector(field);
      }
      String mainName = table.useName();
      if (proc.isStd == false)
        mainName = mainName + proc.upperFirst();
      writeln(2, "public class RowBag");
      writeln(2, "{");
      writeln(3, "public " + mainName + "Rec mRec;");
      writeln(3, "public object tag = null;");
      writeln(3, "public RowBag(" + mainName + "Rec aRec)");
      writeln(3, "{");
      writeln(4, "mRec = aRec;");
      writeln(3, "}");
      writeln(2, "}");
      if (useGenerics)
        writeln(2, "public Dictionary<DataRow, RowBag> dictionary;");
      else
        writeln(2, "public HybridDictionary dictionary;");
      if (useGenerics)
        writeln(2, "public " + table.useName() + proc.upperFirst() + "DataTable(List<" + mainName + "Rec> aList)");
      else
        writeln(2, "public " + table.useName() + proc.upperFirst() + "DataTable(ArrayList aList)");
      writeln(3, ": base(\"" + table.useName() + proc.upperFirst() + "\")");
      writeln(2, "{");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        writeln(3, "Columns.Add(new DataColumn(\"" + field.useUpperName() + "\", typeof(" + dataTableType(field) + ")));");
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        writeln(3, "Columns.Add(new DataColumn(\"" + field.useUpperName() + "\", typeof(" + dataTableType(field) + ")));");
      }
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        writeln(3, "m" + field.useUpperName() + " = new Nfpp" + field.useUpperName() + "(this);");
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        writeln(3, "m" + field.useUpperName() + " = new Nfpp" + field.useUpperName() + "(this);");
      }
      if (useGenerics)
        writeln(3, "dictionary = new Dictionary<DataRow, RowBag>();");
      else
        writeln(3, "dictionary = new HybridDictionary();");
      writeln(3, "foreach (" + mainName + "Rec wRec in aList)");
      writeln(3, "{");
      writeln(4, "DataRow wRow = NewRow();");
      writeln(4, "dictionary.Add(wRow, new RowBag(wRec));");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        if (field.isNull == true)
        {
          writeln(4, "if (wRec." + field.useUpperName() + "IsNull == true)");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = DBNull.Value;");
          writeln(4, "}");
          writeln(4, "else");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = wRec." + field.useUpperName() + ";");
          writeln(4, "}");
        } else
          writeln(4, "wRow[c" + field.useUpperName() + "] = wRec." + field.useUpperName() + ";");
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        if (field.isNull == true)
        {
          writeln(4, "if (wRec." + field.useUpperName() + "IsNull == true)");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = DBNull.Value;");
          writeln(4, "}");
          writeln(4, "else");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = wRec." + field.useUpperName() + ";");
          writeln(4, "}");
        } else
          writeln(4, "wRow[c" + field.useUpperName() + "] = wRec." + field.useUpperName() + ";");
      }
      writeln(4, "Rows.Add(wRow);");
      writeln(3, "}");
      writeln(2, "}");
      writeln(2, "public RowBag GetRowBag(int row)");
      writeln(2, "{");
      writeln(3, "DataRow wRow = Rows[row];");
      if (useGenerics)
        writeln(3, "return dictionary[wRow];");
      else
        writeln(3, "return (RowBag)dictionary[wRow];");
      writeln(2, "}");
      writeln(2, "public " + mainName + "Rec this[int row]");
      writeln(2, "{");
      writeln(3, "get");
      writeln(3, "{");
      writeln(4, "DataRow wRow = Rows[row];");
      writeln(4, mainName + "Rec wRec = new " + mainName + "Rec();");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        if (field.isNull == true)
        {
          writeln(4, "if (wRow.IsNull(c" + field.useUpperName() + "))");
          writeln(4, "{");
          writeln(5, "wRec." + field.useUpperName() + "IsNull = true;");
          writeln(4, "}");
          writeln(4, "else");
          writeln(4, "{");
          writeln(5, "wRec." + field.useUpperName() + " = " + fieldCast(field) + "wRow[c" + field.useUpperName() + "];");
          writeln(4, "}");
        } else
          writeln(5, "wRec." + field.useUpperName() + " = " + fieldCast(field) + "wRow[c" + field.useUpperName() + "];");
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        if (field.isNull == true)
        {
          writeln(4, "if (wRow.IsNull(c" + field.useUpperName() + "))");
          writeln(4, "{");
          writeln(5, "wRec." + field.useUpperName() + "IsNull = true;");
          writeln(4, "}");
          writeln(4, "else");
          writeln(4, "{");
          writeln(5, "wRec." + field.useUpperName() + " = " + fieldCast(field) + "wRow[c" + field.useUpperName() + "];");
          writeln(4, "}");
        } else
          writeln(4, "wRec." + field.useUpperName() + " = " + fieldCast(field) + "wRow[c" + field.useUpperName() + "];");
      }
      writeln(4, "return wRec;");
      writeln(3, "}");
      writeln(3, "set");
      writeln(3, "{");
      writeln(4, "DataRow wRow = Rows[row];");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        if (field.isNull == true)
        {
          writeln(4, "if (value." + field.useUpperName() + "IsNull == true)");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = DBNull.Value;");
          writeln(4, "}");
          writeln(4, "else");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = value." + field.useUpperName() + ";");
          writeln(4, "}");
        } else
          writeln(4, "wRow[c" + field.useUpperName() + "] = value." + field.useUpperName() + ";");
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name))
          continue;
        if (field.isNull == true)
        {
          writeln(4, "if (value." + field.useUpperName() + "IsNull == true)");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = DBNull.Value;");
          writeln(4, "}");
          writeln(4, "else");
          writeln(4, "{");
          writeln(5, "wRow[c" + field.useUpperName() + "] = value." + field.useUpperName() + ";");
          writeln(4, "}");
        } else
          writeln(4, "wRow[c" + field.useUpperName() + "] = value." + field.useUpperName() + ";");
      }
      writeln(3, "}");
      writeln(2, "}");
      writeln(1, "}");
    }
    if (hasDataTables == true && usePartials == true && useSeparate == true)
    {
      String mainName = table.useName();
      //writeln("    [Serializable()]");
      writeln(1, "public partial class " + mainName);
      writeln(1, "{");
      for (int i = 0; i < table.procs.size(); i++)
      {
        Proc proc = table.procs.elementAt(i);
        if (proc.isData == true || proc.isStd == false)
          continue;
        if (!noDatatables)
        {
          if (proc.outputs.size() > 0 && !proc.isSingle)
            generateFetchProcDataTables(proc, mainName);
        }
      }
      writeln(1, "}");
      for (int i = 0; i < table.procs.size(); i++)
      {
        Proc proc = table.procs.elementAt(i);
        if (proc.isData == true || proc.isStd == true)
          continue;
        if (proc.outputs.size() > 0 && !proc.isSingle)
        {
          mainName = table.useName() + proc.upperFirst();
          //writeln("    [Serializable()]");
          writeln(1, "public partial class " + mainName);
          writeln(1, "{");
          if (!noDatatables)
          {
            generateFetchProcDataTables(proc, mainName);
          }
          writeln(1, "}");
        }
      }
    }
  }

  public static void generateStructPairs(Proc proc, Table table, Vector<Field> fields, Vector<String> dynamics, String mainName, String tableName, boolean hasReturning)
  {
    writeln(1, "[Serializable()]");
    String inherit = "";
    if (proc != null && proc.extendsStd == true)
    {
      inherit = " : " + tableName + "Rec";
      writeln(1, "public " + (usePartials ? "partial " : "") + "class " + mainName + "Rec" + inherit);
      writeln(1, "{");

      writeln(2, "public " + mainName + "Rec()");
      writeln(2, "{");
      writeln(2, "}");

      writeln(2, "public " + mainName + "Rec(" + tableName + "Rec rec)");
      writeln(2, "{");
      for (int j = 0; j < table.fields.size(); j++)
      {
        Field field = table.fields.elementAt(j);
        writeln(3, "this." + field.name + " = rec." + field.name + ";");
      }

      writeln(2, "}");

    } else
    {
      inherit = useNotify ? " : INotifyPropertyChanged" : "";
      writeln(1, "public " + (usePartials ? "partial " : "") + "class " + mainName + "Rec" + inherit);
      writeln(1, "{");
      if (useNotify)
      {
        writeln(2, "#region INotifyPropertyChanged Members ");
        writeln("");
        writeln(2, "public event PropertyChangedEventHandler PropertyChanged;");
        writeln("");
        writeln(2, "public void NotifyPropertyChanged([CallerMemberName] string propertyName = null)");
        writeln(2, "{");
        writeln(3, "if (this.PropertyChanged != null)");
        writeln(3, "{");
        writeln(4, "this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));");
        writeln(3, "}");
        writeln(2, "}");
        writeln("");
        writeln(2, "#endregion");
      }
      writeln("");
    }

    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      String temp = "[Column(MaxLength=" + field.length;
      if (field.precision > 0)
      {
        temp = "[Column(MaxLength=" + field.precision + field.scale + 1;
      }

      if (field.isNull)
      {
        temp = temp + ", CanBeNull=true";
      }
      if (field.isPrimaryKey)
      {
        temp = temp + ", IsPrimaryKey=true";
      }
      if (field.isSequence)
      {
        temp = temp + ", IsSequence=true";
      }
      if (field.precision > 0)
      {
        temp = temp + ", Precision=" + field.precision + ", Scale=" + field.scale;
      }

      writeln(fieldDef(field, temp));
      if (field.isNull)
      {
        if (getDataType(field, new StringBuffer(), new StringBuffer()).equals("string"))
        {
          writeln(2, "private bool " + field.useLowerName() + "IsNull;");
          writeln(2, "public bool " + field.useUpperName() + "IsNull");
          writeln(2, "{");
          writeln(3, "get { return this." + field.useLowerName() + " == null ? true : " + field.useLowerName() + "IsNull; }");
          writeln(3, "set { this." + field.useLowerName() + "IsNull = value; }");
          writeln(2, "}");
        } else
        {
          writeln(2, "private bool " + field.useLowerName() + "IsNull;");
          writeln(2, "public bool " + field.useUpperName() + "IsNull");
          writeln(2, "{ ");
          writeln(3, "get { return this." + field.useLowerName() + "IsNull; }");
          writeln(3, "set { this." + field.useLowerName() + "IsNull = value; }");
          writeln(2, "}");
        }
      }
    }
    writeln(1, "}");
  }

  public static void generateEnumOrdinals(Table table)
  {
    for (int i = 0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      if (field.enums.size() > 0)
      {
        writeln(1, "public class " + table.useName() + field.useUpperName() + "Ord");
        writeln(1, "{");
        String datatype = "int";
        if (field.type == Field.ANSICHAR && field.length == 1)
          datatype = "string";
        for (int j = 0; j < field.enums.size(); j++)
        {
          Enum en = field.enums.elementAt(j);
          String evalue = "" + en.value;
          if (field.type == Field.ANSICHAR && field.length == 1)
            evalue = "\"" + (char) en.value + "\"";
          writeln(2, "public const " + datatype + " " + en.name + " = " + evalue + ";");
        }
        writeln(2, "public static string ToString(" + datatype + " ordinal)");
        writeln(2, "{");
        writeln(3, "switch (ordinal)");
        writeln(3, "{");
        for (int j = 0; j < field.enums.size(); j++)
        {
          Enum en = field.enums.elementAt(j);
          String evalue = "" + en.value;
          if (field.type == Field.ANSICHAR && field.length == 1)
            evalue = "\"" + (char) en.value + "\"";
          writeln(4, "case " + evalue + ": return \"" + en.name + "\";");
        }
        writeln(3, "}");
        writeln(3, "return \"unknown ordinal: \"+ordinal;");
        writeln(2, "}");
        writeln(1, "}");
      }
    }
  }

  public static void generateStructs(Table table)
  {
    if (table.fields.size() > 0)
    {
      if (table.comments.size() > 0)
      {
        writeln(1, "/// <summary>");
        for (int i = 0; i < table.comments.size(); i++)
        {
          String s = table.comments.elementAt(i);
          writeln(1, "/// " + s);
        }
        writeln(1, "/// </summary>");
      }
      boolean hasReturning = false;
      for (int i = 0; i < table.procs.size(); i++)
      {
        Proc proc = table.procs.elementAt(i);
        if (proc.hasReturning && proc.isStd)
        {
          hasReturning = true;
          break;
        }
      }
      generateStructPairs(null, table, table.fields, null, table.useName(), null, hasReturning);
      generateEnumOrdinals(table);
      for (int i = 0; i < table.procs.size(); i++)
      {
        Proc proc = table.procs.elementAt(i);
        if (proc.isData || proc.isStd || proc.hasNoData())
          continue;
        if (proc.comments.size() > 0)
        {
          writeln(1, "/// <summary>");
          for (int j = 0; j < proc.comments.size(); j++)
          {
            String s = proc.comments.elementAt(j);
            writeln(1, "/// " + s);
          }
          writeln(1, "/// </summary>");
        }
        Vector<Field> fields = new Vector<>();
        //if (!proc.extendsStd)
        for (int j = 0; j < proc.outputs.size(); j++)
        {
          Field field = proc.outputs.elementAt(j);
          if (field.isExtStd)
            fields.addElement(proc.outputs.elementAt(j));
          else if (!proc.extendsStd)
            fields.addElement(proc.outputs.elementAt(j));
        }
        for (int j = 0; j < proc.inputs.size(); j++)
        {
          Field field = proc.inputs.elementAt(j);
          if (proc.hasOutput(field.name) == false)
            if (field.isExtStd)
              fields.addElement(field);
            else if (!proc.extendsStd)
              fields.addElement(field);
        }
        for (int j = 0; j < proc.dynamics.size(); j++)
        {
          String s = proc.dynamics.elementAt(j);
          Integer n = proc.dynamicSizes.elementAt(j);
          Field field = new Field();
          field.name = s;
          field.type = Field.DYNAMIC;
          field.length = n;
          fields.addElement(field);
        }
        generateStructPairs(proc, table, fields, proc.dynamics, table.useName() + proc.upperFirst(), table.useName(), false);
      }
    }
  }

  public static void generateCode(Table table)
  {
    boolean firsttime = true;
    boolean isLoaded = true;
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData == true || proc.isStd == false)
        continue;
      generateStdCode(table, proc, firsttime, isLoaded);
      if (proc.outputs.size() > 0 && !proc.isSingle)
      {
        isLoaded = false;
      }
      firsttime = false;
    }
    if (firsttime == false)
      writeln(1, "}");
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData == true || proc.isStd == true)
        continue;
      generateCode(table, proc);
    }
  }

  static PlaceHolder placeHolder;

  static void generateStoredProc(Proc proc, String storedProcName, Vector<String> lines)
  {
    //procData.println("IF EXISTS (SELECT * FROM SYSOBJECTS WHERE ID = OBJECT_ID('dbo." + storedProcName + "') AND SYSSTAT & 0xf = 4)");
    procData.println("IF OBJECT_ID('dbo." + storedProcName + "','P') IS NOT NULL");
    procData.println(indent(1) + "DROP PROCEDURE dbo." + storedProcName);
    procData.println("GO");
    procData.println("");
    procData.println("CREATE PROCEDURE dbo." + storedProcName);
    String comma = "(";
    for (int i = 0; i < placeHolder.pairs.size(); i++)
    {
      PlaceHolderPairs pair = placeHolder.pairs.elementAt(i);
      Field field = pair.field;
      procData.println(comma + " @P" + i + " " + varType(field) + " -- " + field.name);
      comma = ",";
    }
    if (placeHolder.pairs.size() > 0)
      procData.println(")");
    procData.println("AS");
    for (int i = 0; i < lines.size(); i++)
    {
      String line = lines.elementAt(i);
      procData.println(line.substring(1, line.length() - 1));
    }
    if (proc.isInsert && proc.hasReturning && proc.table.hasIdentity)
    {
      procData.println("; SELECT CAST(SCOPE_IDENTITY() AS INT)");
    }
    procData.println("GO");
    procData.println("");
  }

  static void generateStoredProcCommand(Proc proc)
  {
    placeHolder = new PlaceHolder(proc, PlaceHolder.AT, "");
    String storedProcName = proc.table.useName() + proc.upperFirst();
    Vector lines = placeHolder.getLines();
    generateStoredProc(proc, storedProcName, lines);
    writeln(2, "public string Command" + proc.upperFirst() + "()");
    writeln(2, "{");
    for (int i = 0; i < lines.size(); i++)
    {
      String line = (String) lines.elementAt(i);
      writeln(3, "// " + line.substring(1, line.length() - 1));
    }
    writeln(3, "return \"" + storedProcName + "\";");
    writeln(2, "}");
  }

  static void generateCommand(Proc proc)
  {
    if (proc.hasReturning)
    {
      placeHolder = new PlaceHolder(proc, PlaceHolder.CURLY, "");
      writeln(2, "public string Command" + proc.upperFirst() + "(Connect aConnect, string aTable, string aField)");
    } else
    {
      placeHolder = new PlaceHolder(proc, PlaceHolder.CURLY, "Rec.");
      writeln(2, "public string Command" + proc.upperFirst() + "()");
    }
    Vector<String> lines = placeHolder.getLines();
    writeln(2, "{");
    if (proc.hasReturning)
      writeln(3, "Returning _ret = new Returning(aConnect.TypeOfVendor, aTable, aField);");
    writeln(3, "return ");
    String plus = indent(4);
    for (int i = 0; i < lines.size(); i++)
    {
      String line = lines.elementAt(i);
      writeln(plus + line);
      plus = indent(4) + "+ ";
    }
    writeln(4, ";");
    writeln(2, "}");
  }

  static void generateNonQueryProc(Proc proc, String mainName)
  {
    Field identity = null;
    for (int i = 0; i < proc.table.fields.size(); i++)
    {
      Field field = proc.table.fields.elementAt(i);
      if (field.isPrimaryKey)
      {
        identity = field;
        break;
      }
    }
    if (proc.hasReturning)
      writeln(2, "public bool " + proc.upperFirst() + "(Connect aConnect)");
    else
      writeln(2, "public void " + proc.upperFirst() + "(Connect aConnect)");
    writeln(2, "{");
    writeln(3, "using (Cursor wCursor = new Cursor(aConnect))");
    writeln(3, "{");
    if (doMSSqlStoredProcs(proc))
      writeln(4, "wCursor.Procedure(Command" + proc.upperFirst() + "());");
    else
    {
      if (placeHolder.pairs.size() > 0)
        writeln(4, "// format command to change {n} to ?, @Pn or :Pn placeholders depending on Vendor");
      writeln(4, "wCursor.Format(Command" + proc.upperFirst() + "(" + returning(proc) + "), " + placeHolder.pairs.size() + ");");
    }
    for (int i = 0; i < placeHolder.pairs.size(); i++)
    {
      PlaceHolderPairs pair = placeHolder.pairs.elementAt(i);
      Field field = pair.field;
      String member = "";
      if (field.type == Field.BLOB || field.type == Field.IMAGE)
        member = ".getBlob()";
      String tail = "";
      if (field.isNull)
        tail = ", mRec." + field.useUpperName() + "IsNull";
      if (proc.isInsert && field.isSequence)
      {
        writeln(4, "var " + field.useLowerName() + " = mRec." + field.useUpperName() + ";");
        writeln(4, "wCursor.Parameter(" + i + ", wCursor.GetSequence(\"" + proc.table.name + "\",\"" + field.name + "\", ref " + field.useLowerName() + "));");
        writeln(4, "mRec." + field.useUpperName() + " = " + field.useLowerName() + ";");
      } else if (field.type == Field.TIMESTAMP)
      {
        writeln(4, "var " + field.useLowerName() + i + " = mRec." + field.useUpperName() + ";");
        writeln(4, "wCursor.Parameter(" + i + ", wCursor.GetTimeStamp(ref " + field.useLowerName() + i + "));");
        writeln(4, "mRec." + field.useUpperName() + " = " + field.useLowerName() + i + ";");
      } else if (field.type == Field.USERSTAMP)
      {
        writeln(4, "var " + field.useLowerName() + i + " = mRec." + field.useUpperName() + ";");
        writeln(4, "wCursor.Parameter(" + i + ", wCursor.GetUserStamp(ref " + field.useLowerName() + i + "));");
        writeln(4, "mRec." + field.useUpperName() + " = " + field.useLowerName() + i + ";");
      } else
        writeln(4, "wCursor.Parameter(" + i + ", mRec." + field.useUpperName() + member + tail + ");");
    }
    if (proc.hasReturning)
    {
      writeln(4, "wCursor.Run();");
      writeln(4, "bool wResult = (wCursor.HasReader() && wCursor.Read());");
      writeln(4, "if (wResult == true)");
      writeln(4, "{");
      if (identity.isNull && getDataType(identity, new StringBuffer(), new StringBuffer()) == "string")
      {
        writeln(5, "var " + identity.useLowerName() + "IsNull = mRec." + identity.useUpperName() + "IsNull;");
        writeln(5, "mRec." + identity.useUpperName() + " = " + castOf(identity) + "wCursor.GetString(0, out " + identity.useLowerName() + "IsNull);");
        writeln(5, "mRec." + identity.useUpperName() + "IsNull = " + identity.useLowerName() + "IsNull;");
      } else if (identity.isNull)
      {
        writeln(5, "var " + identity.useLowerName() + "IsNull = mRec." + identity.useUpperName() + "IsNull;");
        writeln(5, "mRec." + identity.useUpperName() + " = " + castOf(identity) + "wCursor." + cursorGet(identity, 0) + ";");
        writeln(5, "mRec." + identity.useUpperName() + "IsNull = " + identity.useLowerName() + "IsNull;");
      } else
      {
        writeln(5, "mRec." + identity.useUpperName() + " = " + castOf(identity) + "wCursor." + cursorGet(identity, 0) + ";");
      }
      writeln(4, "}");
      writeln(4, "if (wCursor.HasReader())");
      writeln(4, "{");
      writeln(5, "wCursor.Close();");
      writeln(4, "}");
      writeln(4, "return wResult;");
      writeln(3, "}");
      writeln(2, "}");
    } else
    {
      writeln(4, "wCursor.Exec();");
      writeln(3, "}");
      writeln(2, "}");
    }
  }

  static void generateFunc(Proc proc, String mainName)
  {
    String line, input = "", postfix = "";
    boolean isLoaded = false;
    int ind = 0;
    String placeHolder = "";
    if (proc.outputs.size() > 0 && !proc.isSingle && !proc.isInsert)
    {
      isLoaded = true;
    }

    for (int i = 0; i < proc.inputs.size(); i++)
    {

      Field field = proc.inputs.elementAt(i);
      if (field.isPrimaryKey || proc.updateFields.contains(field.name))
      {
        StringBuffer maker = new StringBuffer();
        StringBuffer temp2 = new StringBuffer();
        String result = returnNullableDataType(field, maker, temp2);
        line = ", ";
        input = input + line + result + " " + field.useLowerName();
      }
    }

    for (int i = 0; i < proc.inputs.size(); i++)
    {
      Field field = proc.inputs.elementAt(i);
      if (field.isExtStdOut)
        continue;
      if (field.type == Field.TIMESTAMP || field.type == Field.USERSTAMP)
      {
        continue;
      }
      if (!(field.isPrimaryKey || proc.updateFields.contains(field.name)))
      {
        StringBuffer maker = new StringBuffer();
        StringBuffer temp2 = new StringBuffer();
        String result = returnNullableDataType(field, maker, temp2);
        line = ", ";
        input = input + line + result + " " + field.useLowerName();
      }
    }
    for (int j = 0; j < proc.dynamics.size(); j++)
    {
      String s = proc.dynamics.elementAt(j);
      Integer n = proc.dynamicSizes.elementAt(j);
      Field field = new Field();
      field.name = s;
      field.type = Field.DYNAMIC;
      field.length = n;
      line = ", ";
      input = input + line + "string " + field.useLowerName();
    }
    input = input + ")";
    if (input.equals(")"))
    {
      postfix = "Struct";
    }

    if (proc.isSingle)
      writeln(2, "public " + mainName + "Rec " + proc.upperFirst() + postfix + "(Connect aConnect" + input);
    else if (!isLoaded)
    {
      if (proc.isMultipleInput)
      {
        writeln(2, "public void " + proc.upperFirst() + postfix + "(Connect aConnect, List<" + mainName + "Rec> aList)");
      } else
      {
        writeln(2, "public void " + proc.upperFirst() + postfix + "(Connect aConnect" + input);
      }
    } else if (proc.hasReturning)
    {
      writeln(2, "public bool " + proc.upperFirst() + postfix + "(Connect aConnect" + input);
    } else
      writeln(2, "public List<" + mainName + "Rec> " + proc.upperFirst() + postfix + "(Connect aConnect" + input);
    writeln(2, "{");
    if (proc.isMultipleInput)
    {
      writeln(3, "foreach (var item in aList)");
      writeln(3, "{");
      placeHolder = "item.";
      ind = 1;
    }
    for (int i = 0; i < proc.inputs.size(); i++)
    {
      Field field = proc.inputs.elementAt(i);
      if (field.isExtStdOut)
        continue;
      if (field.type == Field.TIMESTAMP || field.type == Field.USERSTAMP)
      {
        continue;
      }
      placeHolder = field.useLowerName();
      if (proc.isMultipleInput)
      {
        placeHolder = "item." + field.useUpperName();
      }
      if (field.isNull && getNullableType(field))
      {

        writeln(3 + ind, "if (" + placeHolder + ".HasValue)");
        writeln(3 + ind, "{");
        writeln(4 + ind, "mRec." + field.useUpperName() + " = " + placeHolder + ".Value;");
        writeln(3 + ind, "}");
        writeln(3 + ind, "mRec." + field.useUpperName() + "IsNull = !" + placeHolder + ".HasValue;");

      } else
      {
        writeln(3 + ind, "mRec." + field.useUpperName() + " = " + placeHolder + ";");
      }
    }
    for (int j = 0; j < proc.dynamics.size(); j++)
    {
      String s = proc.dynamics.elementAt(j);
      Integer n = proc.dynamicSizes.elementAt(j);
      Field field = new Field();
      field.name = s;
      field.type = Field.DYNAMIC;
      field.length = n;
      line = ", ";
      if (proc.isMultipleInput)
        writeln(3 + ind, "mRec." + field.useUpperName() + " = item." + field.useUpperName() + ";");
      else
        writeln(3 + ind, "mRec." + field.useUpperName() + " = " + field.useLowerName() + ";");
    }
    if (proc.isSingle)
    {
      writeln(3, "if (" + proc.upperFirst() + "(aConnect))");
      writeln(3, "{");
      writeln(4, "return mRec;");
      writeln(3, "}");
      writeln(3, "else");
      writeln(3, "{");
      writeln(4, "return null;");
      writeln(3, "}");
      writeln(2, "}");
    } else if (!isLoaded)
    {
      writeln(3 + ind, proc.upperFirst() + "(aConnect);");
      if (proc.isMultipleInput)
      {
        writeln(3, "}");
      }
      writeln(2, "}");
    } else if (proc.hasReturning)
    {
      writeln(3, "return " + proc.upperFirst() + "(aConnect);");
      writeln(2, "}");
    } else
    {
      writeln(3, proc.upperFirst() + "Load(aConnect);");
      writeln(3, "return Loaded;");
      writeln(2, "}");
    }

  }

  static void generateReturningProc(Proc proc, String mainName)
  {
    Field identity = null;
    for (int i = 0; i < proc.table.fields.size(); i++)
    {
      Field field = proc.table.fields.elementAt(i);
      if (field.isPrimaryKey)
      {
        identity = field;
        break;
      }
    }
    if (identity == null)
    {
      generateNonQueryProc(proc, mainName);
      return;
    }
    writeln(2, "public bool " + proc.upperFirst() + "(Connect aConnect)");
    writeln(2, "{");
    writeln(3, "using (Cursor wCursor = new Cursor(aConnect))");
    writeln(3, "{");
    if (doMSSqlStoredProcs(proc))
      writeln(4, "wCursor.Procedure(Command" + proc.upperFirst() + "());");
    else
    {
      if (placeHolder.pairs.size() > 0)
        writeln(4, "// format command to change {n} to ?, @Pn or :Pn placeholders depending on Vendor");
      writeln(4, "wCursor.Format(Command" + proc.upperFirst() + "(" + returning(proc) + "), " + placeHolder.pairs.size() + ");");
    }
    for (int i = 0; i < placeHolder.pairs.size(); i++)
    {
      PlaceHolderPairs pair = placeHolder.pairs.elementAt(i);
      Field field = pair.field;
      String member = "";
      if (field.type == Field.BLOB || field.type == Field.IMAGE)
        member = ".getBlob()";
      String tail = "";
      if (field.isNull)
        tail = ", mRec." + field.useUpperName() + "IsNull";
      if (field.type == Field.TIMESTAMP)
      {
        writeln(4, "var " + field.useLowerName() + i + " = mRec." + field.useUpperName() + ";");
        writeln(4, "wCursor.Parameter(" + i + ", wCursor.GetTimeStamp(ref " + field.useLowerName() + i + "));");
        writeln(4, "mRec." + field.useUpperName() + " = " + field.useLowerName() + i + ";");
      } else if (field.type == Field.USERSTAMP)
      {
        writeln(4, "var " + field.useLowerName() + i + " = mRec." + field.useUpperName() + ";");
        writeln(4, "wCursor.Parameter(" + i + ", wCursor.GetUserStamp(ref " + field.useLowerName() + i + "));");
        writeln(4, "mRec." + field.useUpperName() + " = " + field.useLowerName() + i + ";");
      } else
        writeln(4, "wCursor.Parameter(" + i + ", mRec." + field.useUpperName() + member + tail + ");");
    }
    writeln(4, "wCursor.Run();");
    writeln(4, "bool wResult = (wCursor.HasReader() && wCursor.Read());");
    writeln(4, "if (wResult == true)");
    writeln(4, "{");
    if (identity.isNull && getDataType(identity, new StringBuffer(), new StringBuffer()).equals("string"))
    {
      writeln(5, "var " + identity.useLowerName() + "IsNull = mRec." + identity.useUpperName() + "IsNull;");
      writeln(5, "mRec." + identity.useUpperName() + " = " + castOf(identity) + "wCursor.GetString(0, out " + identity.useLowerName() + "IsNull);");
      writeln(5, "mRec." + identity.useUpperName() + "IsNull = " + identity.useLowerName() + "IsNull;");
    } else if (identity.isNull)
    {
      writeln(4, "var " + identity.useLowerName() + "IsNull = mRec." + identity.useUpperName() + "IsNull;");
      writeln(4, "mRec." + identity.useUpperName() + " = " + castOf(identity) + "wCursor." + cursorGet(identity, 0) + ";");
      writeln(4, "mRec." + identity.useUpperName() + "IsNull = " + identity.useLowerName() + "IsNull;");
    } else
    {
      writeln(5, "mRec." + identity.useUpperName() + " = " + castOf(identity) + "wCursor." + cursorGet(identity, 0) + ";");
    }
    writeln(4, "}");
    writeln(4, "if (wCursor.HasReader())");
    writeln(4, "{");
    writeln(5, "wCursor.Close();");
    writeln(4, "}");
    writeln(4, "return wResult;");
    writeln(3, "}");
    writeln(2, "}");
  }

  static void generateReadOneProc(Proc proc)
  {
    writeln(2, "public bool " + proc.upperFirst() + "(Connect aConnect)");
    writeln(2, "{");
    writeln(3, "using (Cursor wCursor = new Cursor(aConnect))");
    writeln(3, "{");
    if (doMSSqlStoredProcs(proc))
      writeln(4, "wCursor.Procedure(Command" + proc.upperFirst() + "());");
    else
    {
      if (placeHolder.pairs.size() > 0)
        writeln(4, "// format command to change {n} to ?, @Pn or :Pn placeholders depending on Vendor");
      writeln(4, "wCursor.Format(Command" + proc.upperFirst() + "(" + returning(proc) + "), " + placeHolder.pairs.size() + ");");
    }
    for (int i = 0; i < placeHolder.pairs.size(); i++)
    {
      PlaceHolderPairs pair = placeHolder.pairs.elementAt(i);
      Field field = pair.field;
      String member = "";
      if (field.type == Field.BLOB || field.type == Field.IMAGE)
        member = ".getBlob()";
      String tail = "";
      if (field.isNull)
        tail = ", mRec." + field.useUpperName() + "IsNull";
      if (field.type == Field.TIMESTAMP)
      {
        writeln(4, "var " + field.useLowerName() + i + " = mRec." + field.useUpperName() + ";");
        writeln(4, "wCursor.Parameter(" + i + ", wCursor.GetTimeStamp(ref " + field.useLowerName() + i + "));");
        writeln(4, "mRec." + field.useUpperName() + " = " + field.useLowerName() + i + ";");
      } else if (field.type == Field.USERSTAMP)
      {
        writeln(4, "var " + field.useLowerName() + i + " = mRec." + field.useUpperName() + ";");
        writeln(4, "wCursor.Parameter(" + i + ", wCursor.GetUserStamp(ref " + field.useLowerName() + i + "));");
        writeln(4, "mRec." + field.useUpperName() + " = " + field.useLowerName() + i + ";");
      } else
        writeln(4, "wCursor.Parameter(" + i + ", mRec." + field.useUpperName() + member + tail + ");");
    }
    writeln(4, "wCursor.Run();");
    writeln(4, "bool wResult = (wCursor.HasReader() && wCursor.Read());");
    writeln(4, "if (wResult == true)");
    writeln(4, "{");
    for (int i = 0; i < proc.outputs.size(); i++)
    {
      Field field = proc.outputs.elementAt(i);
      String member = "";
      if (field.type == Field.BLOB || field.type == Field.IMAGE)
        member = ".Buffer";
      if (field.isNull && getDataType(field, new StringBuffer(), new StringBuffer()).equals("string"))
      {
        writeln(5, "var " + field.useLowerName() + "IsNull = mRec." + field.useUpperName() + "IsNull;");
        writeln(5, "mRec." + field.useUpperName() + member + " = " + castOf(field) + "wCursor.GetString(" + i + ", out " + field.useLowerName() + "IsNull);");
        writeln(5, "mRec." + field.useUpperName() + "IsNull = " + field.useLowerName() + "IsNull;");
      } else if (field.isNull)
      {
        writeln(5, "var " + field.useLowerName() + "IsNull = mRec." + field.useUpperName() + "IsNull;");
        writeln(5, "mRec." + field.useUpperName() + member + " = " + castOf(field) + "wCursor." + cursorGet(field, i) + ";");
        writeln(5, "mRec." + field.useUpperName() + "IsNull = " + field.useLowerName() + "IsNull;");
      } else
      {
        writeln(5, "mRec." + field.useUpperName() + member + " = " + castOf(field) + "wCursor." + cursorGet(field, i) + ";");
      }
    }
    writeln(4, "}");
    writeln(4, "if (wCursor.HasReader())");
    writeln(4, "{");
    writeln(5, "wCursor.Close();");
    writeln(4, "}");
    writeln(4, "return wResult;");
    writeln(3, "}");
    writeln(2, "}");
  }

  static String returning(Proc proc)
  {
    if (proc.hasReturning == false)
      return "";
    String tableName = proc.table.useName();
    String fieldName = "";
    for (int i = 0; i < proc.table.fields.size(); i++)
    {
      Field field = proc.table.fields.elementAt(i);
      if (field.isSequence == true)
      {
        fieldName = field.useName();
        break;
      }
    }
    return "aConnect, \"" + tableName + "\", \"" + fieldName + "\"";
  }

  static void generateFetchProc(Proc proc, String mainName, boolean isLoaded)
  {
    writeln(2, "private void " + proc.upperFirst() + "(Connect aConnect)");
    writeln(2, "{");
    if (doMSSqlStoredProcs(proc))
      writeln(3, "mCursor.Procedure(Command" + proc.upperFirst() + "());");
    else
    {
      if (placeHolder.pairs.size() > 0)
        writeln(3, "// format command to change {n} to ?, @Pn or :Pn placeholders depending on Vendor");
      writeln(3, "mCursor.Format(Command" + proc.upperFirst() + "(" + returning(proc) + "), " + placeHolder.pairs.size() + ");");
    }
    for (int i = 0; i < placeHolder.pairs.size(); i++)
    {
      PlaceHolderPairs pair = placeHolder.pairs.elementAt(i);
      Field field = pair.field;
      String member = "";
      if (field.type == Field.BLOB || field.type == Field.IMAGE)
        member = ".getBlob()";
      String tail = "";
      if (field.isNull)
        tail = ", mRec." + field.useUpperName() + "IsNull";
      writeln(3, "mCursor.Parameter(" + i + ", mRec." + field.useUpperName() + member + tail + ");");
    }
    writeln(3, "mCursor.Run();");
    writeln(2, "}");
    writeln(2, "private bool " + proc.upperFirst() + "Fetch()");
    writeln(2, "{");
    writeln(3, "bool wResult = (mCursor.HasReader() && mCursor.Read());");
    writeln(3, "if (wResult == true)");
    writeln(3, "{");
    for (int i = 0; i < proc.outputs.size(); i++)
    {
      Field field = proc.outputs.elementAt(i);
      String member = "";
      if (field.type == Field.BLOB || field.type == Field.IMAGE)
        member = ".Buffer";
      if (field.isNull && getDataType(field, new StringBuffer(), new StringBuffer()) == "string")
      {
        writeln(4, "var " + field.useLowerName() + "IsNull = mRec." + field.useUpperName() + "IsNull;");
        writeln(4, "mRec." + field.useUpperName() + member + " = " + castOf(field) + "mCursor.GetString(" + i + ", out " + field.useLowerName() + "IsNull);");
        writeln(4, "mRec." + field.useUpperName() + "IsNull = " + field.useLowerName() + "IsNull;");
      } else if (field.isNull)
      {
        writeln(4, "var " + field.useLowerName() + "IsNull = mRec." + field.useUpperName() + "IsNull;");
        writeln(4, "mRec." + field.useUpperName() + member + " = " + castOf(field) + "mCursor." + cursorGet(field, i) + ";");
        writeln(4, "mRec." + field.useUpperName() + "IsNull = " + field.useLowerName() + "IsNull;");
      } else
      {
        writeln(4, "mRec." + field.useUpperName() + member + " = " + castOf(field) + "mCursor." + cursorGet(field, i) + ";");
      }
    }
    writeln(3, "}");
    writeln(3, "else if (mCursor.HasReader())");
    writeln(3, "{");
    writeln(4, "mCursor.Close();");
    writeln(3, "}");
    writeln(3, "return wResult;");
    writeln(2, "}");
    writeln(2, "public void " + proc.upperFirst() + "Load(Connect aConnect)");
    writeln(2, "{");
    writeln(3, "using (mCursor = new Cursor(aConnect))");
    writeln(3, "{");
    writeln(4, proc.upperFirst() + "(aConnect);");
    writeln(4, "while (" + proc.upperFirst() + "Fetch())");
    writeln(4, "{");
    writeln(5, "mList.Add(mRec);");
    writeln(5, "mRec = new " + mainName + "Rec();");
    writeln(4, "}");
    writeln(3, "}");
    writeln(2, "}");
    writeln(2, "public bool " + proc.upperFirst() + "First(Connect aConnect)");
    writeln(2, "{");
    writeln(3, "using (mCursor = new Cursor(aConnect))");
    writeln(3, "{");
    writeln(4, proc.upperFirst() + "(aConnect);");
    writeln(4, "if (" + proc.upperFirst() + "Fetch())");
    writeln(4, "{");
    writeln(5, "mCursor.Close();");
    writeln(5, "return true;");
    writeln(4, "}");
    writeln(4, "return false;");
    writeln(3, "}");
    writeln(2, "}");
    if (isLoaded)
    {
      if (useGenerics)
        writeln(2, "public List<" + mainName + "Rec> Loaded { get { return mList; } }");
      else
        writeln(2, "public ArrayList Loaded { get { return mList; } }");
    }
    writeln(2, "public class " + proc.upperFirst() + "Ord");
    writeln(2, "{");
    int noInDataSet = 0;
    for (int i = 0; i < proc.inputs.size(); i++)
    {
      Field field = proc.inputs.elementAt(i);
      writeln(3, "public const int " + field.useUpperName() + " = " + noInDataSet + ";");
      noInDataSet++;
    }
    for (int i = 0; i < proc.outputs.size(); i++)
    {
      Field field = proc.outputs.elementAt(i);
      if (proc.hasInput(field.name))
        continue;
      writeln(3, "public const int " + field.useUpperName() + " = " + noInDataSet + ";");
      noInDataSet++;
    }
    writeln(3, "public static string ToString(int ordinal)");
    writeln(3, "{");
    writeln(4, "switch (ordinal)");
    writeln(4, "{");
    noInDataSet = 0;
    for (int i = 0; i < proc.inputs.size(); i++)
    {
      Field field = proc.inputs.elementAt(i);
      writeln(5, "case " + noInDataSet + ": return \"" + field.useUpperName() + "\";");
      noInDataSet++;
    }
    for (int i = 0; i < proc.outputs.size(); i++)
    {
      Field field = proc.outputs.elementAt(i);
      if (proc.hasInput(field.name))
        continue;
      writeln(5, "case " + noInDataSet + ": return \"" + field.useUpperName() + "\";");
      noInDataSet++;
    }
    writeln(4, "}");
    writeln(4, "return \"<??\"+ordinal+\"??>\";");
    writeln(3, "}");
    writeln(2, "}");
    if (!noDatatables)
    {
      if (useSeparate == false && usePartials == false)
        generateFetchProcDataTables(proc, mainName);
    }
  }

  static void generateFetchProcDataTables(Proc proc, String mainName)
  {
    writeln(2, "public " + proc.table.useName() + proc.upperFirst() + "DataTable " + proc.upperFirst() + "DataTable()");
    writeln(2, "{");
    writeln(3, proc.table.useName() + proc.upperFirst() + "DataTable wResult = new " + proc.table.useName() + proc.upperFirst() + "DataTable(mList);");
    writeln(3, "return wResult;");
    writeln(2, "}");
    writeln(2, "public " + proc.table.useName() + proc.upperFirst() + "DataTable " + proc.upperFirst() + "DataTable(Connect aConnect)");
    writeln(2, "{");
    writeln(3, "" + proc.upperFirst() + "Load(aConnect);");
    writeln(3, "return " + proc.upperFirst() + "DataTable();");
    writeln(2, "}");
  }

  static void generateProcFunctions(Proc proc, String name, boolean isLoaded)
  {
    if (proc.outputs.size() > 0 && !proc.isSingle)
      generateFetchProc(proc, name, isLoaded);
    else if (proc.outputs.size() > 0)
      generateReadOneProc(proc);
    else if (proc.isInsert && proc.hasReturning)
      generateReturningProc(proc, name);
    else
      generateNonQueryProc(proc, name);
    if (useFunc)
      generateFunc(proc, name);
  }

  static void generateCClassTop(Proc proc, String mainName, boolean doCursor)
  {
    writeln(1, "[Serializable()]");
    writeln(1, "public " + (usePartials ? "partial " : "") + "class " + mainName);
    writeln(1, "{");
    if (doCursor == true || proc.hasNoData() == false)
    {
      writeln(2, "private " + mainName + "Rec mRec;");
      writeln(2, "public " + mainName + "Rec Rec { get { return mRec; } set { mRec = value; } }");

      if (doCursor == true || (proc.outputs.size() > 0 && !proc.isSingle))
      {
        if (useGenerics)
          writeln(2, "private List<" + mainName + "Rec> mList;");
        else
          writeln(2, "private ArrayList mList;");
        writeln(2, "public int Count { get { return mList.Count; } }");
        writeln(2, "public Cursor mCursor;");
        writeln(2, "public " + mainName + "Rec this[int i]");
        writeln(2, "{");
        writeln(3, "get");
        writeln(3, "{");
        writeln(4, "if (i >= 0 && i < mList.Count)");
        writeln(4, "{");
        if (useGenerics)
          writeln(5, "return mList[i];");
        else
          writeln(5, "return (" + mainName + "Rec)mList[i];");
        writeln(4, "}");
        writeln(4, "throw new JPortalException(\"" + mainName + " index out of range\");");
        writeln(3, "}");
        writeln(3, "set");
        writeln(3, "{");
        writeln(4, "if (i < mList.Count)");
        writeln(4, "{");
        writeln(5, "mList.RemoveAt(i);");
        writeln(4, "}");
        writeln(4, "mList.Insert(i, value);");
        writeln(3, "}");
        writeln(2, "}");
        if (useYields)
        {
          if (useGenerics)
            writeln(2, "public IEnumerable<" + mainName + "Rec> Yielded()");
          else
            writeln(2, "public IEnumerable Yielded()");
          writeln(2, "{");
          writeln(3, "for (int i=0; i<Count; i++)");
          writeln(4, "yield return this[i];");
          writeln(2, "}");
        }
      }
      writeln(2, "public void Clear()");
      writeln(2, "{");
      if (doCursor == true || (proc.outputs.size() > 0 && !proc.isSingle))
        if (useGenerics)
          writeln(3, "mList = new List<" + mainName + "Rec>();");
        else
          writeln(3, "mList = new ArrayList();");
      writeln(3, "mRec = new " + mainName + "Rec();");
      writeln(2, "}");
      writeln(2, "public " + mainName + "()");
      writeln(2, "{");
      writeln(3, "Clear();");
      writeln(2, "}");
    }
  }

  static boolean doMSSqlStoredProcs(Proc proc)
  {
    return (mSSqlStoredProcs == true && proc.dynamics.size() == 0) || (proc.isSProc == true && proc.dynamics.size() == 0);
  }

  static void generateCode(Table table, Proc proc)
  {
    if (proc.comments.size() > 0)
    {
      writeln(1, "/// <summary>");
      for (int i = 0; i < proc.comments.size(); i++)
      {
        String comment = proc.comments.elementAt(i);
        writeln(1, "/// " + comment);
      }
      writeln(1, "/// </summary>");
    }

    generateCClassTop(proc, table.useName() + proc.upperFirst(), false);
    if (doMSSqlStoredProcs(proc) == true)
      generateStoredProcCommand(proc);
    else
      generateCommand(proc);
    generateProcFunctions(proc, table.useName() + proc.upperFirst(), true);
    writeln(1, "}");
  }

  static void generateStdCode(Table table, Proc proc, boolean firsttime, boolean isLoaded)
  {
    if (firsttime == true)
      generateCClassTop(proc, table.useName(), table.hasCursorStdProc());
    if (proc.comments.size() > 0)
    {
      writeln(2, "/// <summary>");
      for (int i = 0; i < proc.comments.size(); i++)
      {
        String comment = proc.comments.elementAt(i);
        writeln(2, "/// " + comment);
      }
      writeln(2, "/// </summary>");
    }
    if (doMSSqlStoredProcs(proc) == true)
      generateStoredProcCommand(proc);
    else
      generateCommand(proc);
    generateProcFunctions(proc, table.useName(), isLoaded);
  }

  static String castOf(Field field)
  {
    return switch (field.type)
            {
              case Field.BYTE -> "(byte)";
              case Field.SHORT -> "(short)";
              default -> "";
            };
  }

  static String validNull(Field field)
  {
    return switch (field.type)
            {
              case Field.DATE, Field.DATETIME, Field.TIMESTAMP, Field.TIME -> "DateTime.MinValue";
              case Field.BOOLEAN -> "false";
              case Field.BYTE, Field.DOUBLE, Field.FLOAT, Field.IDENTITY, Field.INT, Field.LONG, Field.SEQUENCE
                      , Field.BIGIDENTITY, Field.BIGSEQUENCE, Field.SHORT, Field.MONEY, Field.STATUS -> "0";
              case Field.UID -> "Guid.Empty";
              default -> "null";
            };
  }

  static String cursorGet(Field field, int occurence)
  {
    String tail = ")";
    if (field.isNull)
      tail = ", out " + field.useLowerName() + "IsNull)";
    switch (field.type)
    {
      case Field.ANSICHAR:
      case Field.WANSICHAR:
      case Field.CHAR:
      case Field.WCHAR:
      case Field.DYNAMIC:
      case Field.TLOB:
      case Field.XML:
      case Field.BIGXML:
      case Field.USERSTAMP:
        return "GetString(" + occurence + tail;
      case Field.BLOB:
      case Field.IMAGE:
        return "GetBlob(" + occurence + ", " + field.length + tail;
      case Field.BOOLEAN:
        return "GetBoolean(" + occurence + tail;
      case Field.BYTE:
        return "GetByte(" + occurence + tail;
      case Field.DATE:
      case Field.DATETIME:
      case Field.TIME:
      case Field.TIMESTAMP:
        return "GetDateTime(" + occurence + tail;
      case Field.DOUBLE:
      case Field.FLOAT:
        if (field.precision > 15)
          return "GetDecimal(" + occurence + tail;
        return "GetDouble(" + occurence + tail;
      case Field.IDENTITY:
      case Field.INT:
      case Field.SEQUENCE:
        return "GetInt(" + occurence + tail;
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return "GetLong(" + occurence + tail;
      case Field.MONEY:
        return "GetDecimal(" + occurence + tail;
      case Field.SHORT:
        return "GetShort(" + occurence + tail;
      case Field.UID:
        return "GetGuid(" + occurence + tail;
      default:
        break;
    }
    return "Get(" + occurence + tail;
  }

  static String dataTableType(Field field)
  {
    switch (field.type)
    {
      case Field.ANSICHAR:
      case Field.WANSICHAR:
      case Field.CHAR:
      case Field.WCHAR:
      case Field.DYNAMIC:
      case Field.MONEY:
      case Field.USERSTAMP:
      case Field.XML:
      case Field.BIGXML:
      case Field.TLOB:
        return "String";
      case Field.BLOB:
      case Field.IMAGE:
        return "Byte[]";
      case Field.BOOLEAN:
        return "Boolean";
      case Field.BYTE:
        return "Byte";
      case Field.DATE:
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.TIME:
        return "DateTime";
      case Field.DOUBLE:
      case Field.FLOAT:
        if (field.precision > 15)
          return "String";
        return "Double";
      case Field.IDENTITY:
      case Field.INT:
      case Field.SEQUENCE:
        return "Int32";
      case Field.BIGIDENTITY:
      case Field.LONG:
      case Field.BIGSEQUENCE:
        return "Int64";
      case Field.SHORT:
        return "Int16";
      case Field.UID:
        return "Guid";
      default:
        break;
    }
    return "dataTableType";
  }

  static String fieldDef(Field field, String temp)
  {
    StringBuffer maker = new StringBuffer();
    StringBuffer temp2 = new StringBuffer(temp);
    String result = getDataType(field, maker, temp2);
    String set = indent(3) + "set { this." + field.useLowerName() + " = value; } \r\n";

    if (useNotify)
    {
      set = indent(3) + "set\r\n"
              + indent(4) + "{\r\n"
              + indent(5) + "this." + field.useLowerName() + " = value; \r\n"
              + indent(5) + "NotifyPropertyChanged();\r\n"
              + indent(4) + "}\r\n";

    }

    if (field.isNull && result == "string")
    {
      set = indent(3) + "set\r\n"
              + indent(3) + "{\r\n"
              + indent(4) + "this." + field.useLowerName() + " = value; \r\n"
              + indent(4) + "this." + field.useUpperName() + "IsNull = value == null ? true : false;\r\n"
              + (useNotify ? indent(4) + "NotifyPropertyChanged();\r\n" : "")
              + indent(3) + "}\r\n";
    }
    String ret = indent(2) + "private " + result + " " + field.useLowerName() + maker + ";\r\n"
            + indent(2) + temp2 + ")]\r\n"
            + indent(2) + "public " + result + " " + field.useUpperName() + "\r\n"
            + indent(2) + "{ \r\n"
            + indent(3) + "get { return this." + field.useLowerName() + ";}\r\n"
            + set
            + indent(2) + "}";
    return ret;
  }

  static String getDataType(Field field, StringBuffer maker, StringBuffer temp)
  {
    String result;
    switch (field.type)
    {
      case Field.ANSICHAR:
      case Field.CHAR:
      case Field.WANSICHAR:
      case Field.WCHAR:
      case Field.USERSTAMP:
      case Field.DYNAMIC:
      case Field.XML:
      case Field.BIGXML:
      case Field.TLOB:
        result = "string";
        break;
      case Field.MONEY:
        result = "decimal";
        break;
      case Field.BLOB:
      case Field.IMAGE:
        result = "JPBlob";
        maker.append(" = new JPBlob()");
        break;
      case Field.UID:
        result = "Guid";
        break;
      case Field.DATE:
      case Field.DATETIME:
      case Field.TIME:
      case Field.TIMESTAMP:
        result = "DateTime";
        temp.append(", IsVersion=true");
        break;
      case Field.BOOLEAN:
        result = "bool";
        break;
      case Field.BYTE:
        result = "byte";
        break;
      case Field.STATUS:
      case Field.SHORT:
        result = "short";
        break;
      case Field.DOUBLE:
      case Field.FLOAT:
        if (field.precision > 15)
          result = "decimal";
        else
          result = "double";
        break;
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        result = "int";
        break;
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        result = "long";
        break;
      default:
        result = "whoknows";
        break;
    }
    return result;
  }

  static String returnNullableDataType(Field field, StringBuffer maker, StringBuffer temp)
  {
    String datatype = getDataType(field, maker, temp);
    if (getNullableType(field) && field.isNull)
      datatype = datatype + "?";
    return datatype;
  }

  static boolean getNullableType(Field field)
  {
    boolean nullableType = switch (field.type)
            {
              case Field.MONEY, Field.DATE, Field.DATETIME, Field.TIME, Field.TIMESTAMP, Field.BOOLEAN, Field.BYTE, Field.STATUS, Field.DOUBLE, Field.FLOAT, Field.INT, Field.SEQUENCE, Field.IDENTITY, Field.LONG, Field.BIGSEQUENCE, Field.BIGIDENTITY, Field.SHORT -> true;
              default -> false;
            };
    return nullableType;
  }

  static String fieldCastNo(Field field)
  {
    String result;
    switch (field.type)
    {
      case Field.ANSICHAR:
      case Field.CHAR:
      case Field.WANSICHAR:
      case Field.WCHAR:
      case Field.USERSTAMP:
      case Field.DYNAMIC:
      case Field.XML:
      case Field.BIGXML:
      case Field.TLOB:
        result = "string";
        break;
      case Field.MONEY:
        result = "decimal";
        break;
      case Field.BLOB:
      case Field.IMAGE:
        result = "JPBlob";
        break;
      case Field.UID:
        result = "Guid";
        break;
      case Field.DATE:
      case Field.DATETIME:
      case Field.TIME:
      case Field.TIMESTAMP:
        result = "DateTime";
        break;
      case Field.BOOLEAN:
        result = "bool";
        break;
      case Field.BYTE:
        result = "byte";
        break;
      case Field.STATUS:
      case Field.SHORT:
        result = "short";
        break;
      case Field.DOUBLE:
      case Field.FLOAT:
        if (field.precision > 15)
          result = "decimal";
        else
          result = "double";
        break;
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        result = "int";
        break;
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        result = "long";
        break;
      default:
        result = "whoknows";
        break;
    }
    return result;
  }

  static String fieldCast(Field field)
  {
    return "(" + fieldCastNo(field) + ")";
  }

  /**
   * Translates field type to SQLServer SQL column types
   */
  static String varType(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
        return "bit";
      case Field.BYTE:
        return "tinyint";
      case Field.SHORT:
        return "smallint";
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return "integer";
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return "longint";
      case Field.CHAR:
        if (field.length > 8000)
        {
          return field.name + " varchar(MAX)";
        }
        return field.name + " varchar(" + field.length + ")";
      case Field.ANSICHAR:
      case Field.WANSICHAR:
        return "char(" + field.length + ")";
      case Field.WCHAR:
        if (field.length > 4000)
        {
          return field.name + " nvarchar(MAX)";
        }
        return field.name + " nvarchar(" + field.length + ")";
      case Field.DATE:
      case Field.TIMESTAMP:
      case Field.TIME:
      case Field.DATETIME:
        return "datetime";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "decimal";
        return "float";
      case Field.BLOB:
      case Field.IMAGE:
        return "image";
      case Field.TLOB:
        return "text";
      case Field.XML:
      case Field.BIGXML:
        return "xml";
      case Field.UID:
        return "uniqueidentifier";
      case Field.MONEY:
        return "money";
      case Field.USERSTAMP:
        return "varchar(50)";
      default:
        break;
    }
    return "unknown";
  }
}
