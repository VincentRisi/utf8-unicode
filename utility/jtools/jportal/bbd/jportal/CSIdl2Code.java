package bbd.jportal;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;

public class CSIdl2Code extends Generator
{

  public static String description()
  {
    return "Generate C# IDL Code for 3 Tier Access";
  }

  public static String documentation()
  {
    return "Generate C# IDL Code for 3 Tier Access";
  }

  /**
   * Padder function
   */
  static String padder(String s, int length)
  {
    for (int i = s.length(); i < length - 1; i++)
      s = s + " ";
    return s + " ";
  }

  /**
   * Generates the procedure classes for each table present.
   */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    for (int i = 0; i < database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      genTable(table, output, outLog);
    }
  }

  static String packName;

  /**
   * Build of standard and user defined procedures
   */
  static void genTable(Table table, String output, PrintWriter outLog)
  {
    boolean noIdl = table.hasOption("noIdl");
    if (noIdl == true)
    {
      boolean needIdl = false;
      for (int i = 0; i < table.procs.size(); i++)
      {
        Proc proc = table.procs.elementAt(i);
        if (proc.hasOption("Idl") == true)
        {
          needIdl = true;
          break;
        }
      }
      if (needIdl == false)
        return;
    }
    if (table.database.packageName.length() > 0)
      packName = table.database.packageName;
    else
      packName = "Bbd.JPortal";
    try
    {
      outLog.println("Code: " + output + table.useName() + ".ii");
      OutputStream outFile = new FileOutputStream(output + table.useName() + ".ii");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        try
        {
          outData.println("// This code was generated, do not modify it, modify it at source and regenerate it.");
          outData.println("module  " + table.useName() + ";");
          outData.println();
          outData.println("struct " + table.useName() + " \"using " + packName + ";\"");
          if (table.hasStdProcs)
            outData.println("struct " + table.useName() + "Rec \"using " + packName + ";\"");
          for (int i = 0; i < table.procs.size(); i++)
          {
            Proc proc = table.procs.elementAt(i);
            if (noIdl == true && proc.hasOption("Idl") == false)
              continue;
            if (proc.isData || proc.isStdExtended() || proc.hasNoData())
              continue;
            String procName = table.useName() + proc.upperFirst() + "Rec";
            outData.println("struct " + procName + " \"using " + packName + ";\"");
          }
          outData.println();
          for (int i = 0; i < table.procs.size(); i++)
          {
            Proc proc = table.procs.elementAt(i);
            if (proc.isData)
              continue;
            if (noIdl == true && proc.hasOption("Idl") == false)
              continue;
            if (proc.isMultipleInput)
              generateBulkAction(table, proc, outData);
            else if (proc.isInsert && proc.hasReturning)
              generateAction(table, proc, outData);
            else if (proc.outputs.size() > 0)
              if (proc.isSingle)
                generateSingle(table, proc, outData);
              else
                generateMultiple(table, proc, outData);
            else
              generateAction(table, proc, outData);
          }
        } finally
        {
          outData.flush();
        }
      } finally
      {
        outFile.close();
      }
    } catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
  }
  private static void generateBulkAction(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    if (proc.isStdExtended())
      dataStruct = table.useName() + "Rec";
    else
    {
      dataStruct = table.useName() + proc.upperFirst() + "Rec";
    }
    outData.print("void " + table.useName() + proc.upperFirst() + "(");
    outData.print("int noOf, " + dataStruct + " *recs");
    outData.println(")");
    outData.println("{");
    outData.println("message: #");
      outData.println("input noOf; recs size(noOf);");
    outData.println("code");
    outData.println("  try");
    outData.println("  {");
    if (proc.isStdExtended())
      outData.println("    " + table.useName() + " table = new " + table.useName() + "();");
    else
      outData.println("    " + table.useName() + "." + proc.upperFirst() + " table = new " + table.useName() + "."
          + proc.upperFirst() + "();");
    outData.println("    table.mRecs.Clone(recs);");
    outData.println("    table." + proc.upperFirst() + "(connector);");
    outData.println("  }");
    outData.println("  catch(Exception ex)");
    outData.println("  {");
    outData.println("    connector.FlagRollback();");
    outData.println("    throw ex;");
    outData.println("  }");
    outData.println("endcode");
    outData.println("}");
    outData.println();
  }
  private static void generateAction(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    if (proc.isStdExtended())
      dataStruct = table.useName() + "Rec";
    else
    {
      dataStruct = table.useName() + proc.upperFirst() + "Rec";
    }
    boolean hasMods = proc.hasModifieds();
    boolean hasInput = (proc.inputs.size() > 0 || proc.dynamics.size() > 0);
    outData.print("void " + table.useName() + proc.upperFirst() + "(");
    if (hasInput || hasMods)
      outData.print(dataStruct + " *rec");
    outData.println(")");
    outData.println("{");
    outData.println("message: #");
    if (hasInput || hasMods)
      outData.println("input rec;");
    if (hasMods || proc.hasReturning)
      outData.println("output rec;");
    outData.println("code");
    outData.println("  try");
    outData.println("  {");
    if (proc.isStdExtended())
      outData.println("    " + table.useName() + " table = new " + table.useName() + "();");
    else
      outData.println("    " + table.useName() + "." + proc.upperFirst() + " table = new " + table.useName() + "."
          + proc.upperFirst() + "();");
    if (hasInput)
      outData.println("    table.mRec = rec;");
    outData.println("    table." + proc.upperFirst() + "(connector);");
    if (hasMods == true)
      outData.println("    rec = table.mRec;");
    outData.println("  }");
    outData.println("  catch(Exception ex)");
    outData.println("  {");
    outData.println("    connector.FlagRollback();");
    outData.println("    throw ex;");
    outData.println("  }");
    outData.println("endcode");
    outData.println("}");
    outData.println();
  }

  private static void generateMultiple(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    String tableName = table.useName();
    if (proc.isStdExtended())
      dataStruct = tableName + "Rec";
    else
      dataStruct = tableName + proc.upperFirst() + "Rec";
    boolean hasInput = (proc.inputs.size() > 0 || proc.dynamics.size() > 0);
    outData.print("void " + tableName + proc.upperFirst() + "(");
    if (hasInput)
      outData.print("" + dataStruct + "* rec, ");
    outData.println("int noOf," + dataStruct + "*& recs)");
    outData.println("{");
    outData.println("message: #");
    if (hasInput)
    {
      outData.println("input");
      outData.println("  rec;");
    }
    outData.println("output");
    outData.println("  noOf;");
    outData.println("  recs size(noOf);");
    outData.println("code");
    outData.println("  try");
    outData.println("  {");
    outData.println("    var table = new " + tableName + "();");
    if (hasInput)
      outData.println("    table.mRec = rec;");
    outData.println("    table." + proc.upperFirst() + "Load();");
    outData.println("    " + dataStruct + " recs = new " + dataStruct + "[table.Count];");
    outData.println("    table.mList.CopyTo(recs);");
    outData.println("  }");
    outData.println("  catch(Exception ex)");
    outData.println("  {");
    outData.println("    connector.FlagRollback();");
    outData.println("    throw ex;");
    outData.println("  }");
    outData.println("endcode");
    outData.println("}");
    outData.println();
  }

  private static void generateSingle(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    String tableName = table.useName();
    if (proc.isStdExtended())
      dataStruct = tableName + "Rec";
    else
      dataStruct = tableName + proc.upperFirst() + "Rec";
    outData.println("int " + table.useName() + proc.upperFirst() + "(" + dataStruct + " *rec)");
    outData.println("{");
    outData.println("message: #");
    outData.println("input");
    outData.println("  rec;");
    outData.println("output");
    outData.println("  rec;");
    outData.println("code");
    outData.println("  bool has = false;");
    outData.println("  try");
    outData.println("  {");
    outData.println("    var table = new " + tableName + "();");
    outData.println("    table.mRec = rec;");
    outData.println("    has = table." + proc.upperFirst() + "(connnect);");
    outData.println("    if (has == true)");
    outData.println("      rec = table.mRec;");
    outData.println("  }");
    outData.println("  catch(Exception ex)");
    outData.println("  {");
    outData.println("    connector.FlagRollback();");
    outData.println("    throw ex;");
    outData.println("  }");
    outData.println("  return has ? 1 : 0;");
    outData.println("endcode");
    outData.println("}");
    outData.println();
  }
}
