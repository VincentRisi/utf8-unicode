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

package bbd.jportal.old;

import bbd.jportal.*;
import bbd.jportal.Enum;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class Lite3PyCode extends Generator
{
  public static String description()
  {
    return "Generate Lite3 Code for Python";
  }
  public static String documentation()
  {
    return "Generate Lite3 Code for Python";
  }

  /**
  * Generates the procedure classes for each table present.
  */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    for (int i=0; i<database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      generateStructs(table, output, outLog);
    }
  }
  /**
   * Build of standard and user defined procedures
   */
  static void generateStructs(Table table, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + table.useName() + "Lite3.py");
      try (OutputStream outFile = new FileOutputStream(output + table.useName() + "Lite3.py"))
      {
        PrintWriter outData = new PrintWriter(outFile);
        try
        {
          outData.println("# This code was generated, do not modify it, modify it at source and regenerate it.");
          outData.println("# " + table.useName() + "Lite3.py");
          outData.println();
          outData.println("import types");
          outData.println();
          generateEnums(table, outData);
          //generateSql(table, outData);
          if (table.hasStdProcs)
            generateStdOutputRec(table, outData);
          generateUserOutputRecs(table, outData);
          generateCode(table, outData);
          outData.flush();
        }
        finally
        {
          outData.flush();
        }
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
  }
  static void generateDataFields(Vector<Field> fields, String superName,
                                 String tableName, PrintWriter outData)
  {
    outData.print("  __slots__ = [");
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = fields.elementAt(i);
      if (i != 0)
      {
        outData.println(",");
        outData.print("    ");
      }
      outData.print("'" + field.useName() + "'");
    }
    outData.println("]");
    outData.println("  def __init__(self):");
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = fields.elementAt(i);
      if (isNull(field) == true)
        outData.println("    self." + field.useName() + " = None");
      else
        outData.println("    self." + field.useName() + " = ''");
    }
    outData.println("  def _fromList(self, _result):");
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = fields.elementAt(i);
      outData.println("    self." + field.useName() + " = _result[" + i + "]");
    }
    outData.println("    return " + fields.size());
    outData.println("  def _str(self, s):");
    outData.println("    if s == None: return None");
    outData.println("    elif type(s) is types.FloatType: return '%0.15g' % (s)");
    outData.println("    return str(s)");
    outData.println("  def _toList(self):");
    if (superName.length() > 0)
      outData.print("    _result = " + superName + "._toList(self) + [");
    else
      outData.print("    _result = [");
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = fields.elementAt(i);
      if (i != 0)
      {
        outData.println(",");
        outData.print("      ");
      }
      outData.print("self._str(self." + field.useName() + ")");
    }
    outData.println("]");
    outData.println("    return _result");
  }
  static void generateStdOutputRec(Table table, PrintWriter outData)
  {
    outData.println("class " + table.useName() + "(object):");
    outData.println("  def _make(self): return " + table.useName() + "()");
    generateDataFields(table.fields, "", table.useName(), outData);
    outData.println();
  }
  static void generateUserOutputRecs(Table table, PrintWriter outData)
  {
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData || proc.isStd || proc.hasNoData())
        continue;
      if (proc.isStdExtended())
        continue;
      String superName;
      Vector<Field> procFields = new Vector<>();
      superName = table.useName() + proc.upperFirst();
      outData.println("class " + table.useName() + proc.upperFirst() + "(object):");
      outData.println("  def _make(self): return " + table.useName() + proc.upperFirst() + "()");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        procFields.addElement(field);
      }
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (proc.hasInput(field.name) == true)
          continue;
        procFields.addElement(field);
      }
      for (int j = 0; j < proc.dynamics.size(); j++)
      {
        Field f = new Field();
        f.name = proc.dynamics.elementAt(j);
        f.type = Field.CHAR;
        procFields.addElement(f);
      }
      generateDataFields(procFields, superName, table.useName(), outData);
      outData.println();
    }
  }
  static void generateEnums(Table table, PrintWriter outData)
  {
    for (int i = 0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      generateEnums(table.useName() + field.useName(), field, outData);
    }
  }
  static void generateEnums(String baseName, Field field, PrintWriter outData)
  {
    if (field.enums.size() > 0)
    {
      outData.println(baseName + " = {}");
      for (int j = 0; j < field.enums.size(); j++)
      {
        bbd.jportal.Enum entry = field.enums.elementAt(j);
        outData.println(baseName + "['" + entry.name + "'] = " + entry.value);
      }
      for (int j = 0; j < field.enums.size(); j++)
      {
        Enum entry = field.enums.elementAt(j);
        outData.println(baseName + "[" + entry.value + "] = '" + entry.name + "'");
      }
      outData.println();
    }
  }

  private static void generateCode(Table table, PrintWriter outData)
  {
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      PlaceHolder holder = new PlaceHolder(proc, PlaceHolder.QUESTION, "");
      Vector<PlaceHolderPairs> pairs = holder.getPairs();
      String parent;
      if (proc.hasNoData() == true)
        parent = "object";
      else if (proc.isStd == true || proc.isStdExtended() == true)
        parent = table.useName();
      else
      {
        parent = table.useName() + proc.upperFirst();
      }
      outData.println("class " + table.useName() + proc.upperFirst() + "(" + parent + "):");
      if (proc.hasNoData() == false)
      {
        outData.println("  def __init__(self):");
        outData.println("    " + parent + ".__init__(self)");
      }
      outData.println("  def " + proc.lowerFirst() + "(self, connect):");
      Vector<String> lines = holder.getLines();
      String command = "_command";
      generateString(proc, command, lines, outData);
      outData.println("    cursor = connect.cursor()");
      if (pairs.size() > 0)
      {
        outData.println("    cursor.execute(" + command + ", [");
        for (int j = 0; j < pairs.size(); j++)
        {
          if (j > 0)
            outData.println(",");
          PlaceHolderPairs pair = pairs.elementAt(j);
          Field field = pair.field;
          outData.print("      self." + field.useName());
        }
        outData.println("])");
      }
      else
      {
        outData.println("    cursor.execute(" + command + ")");
      }
      if (proc.outputs.size() > 0)
        if (proc.isSingle)
          generatePythonSingle(parent, outData);
        else
          generatePythonMultiple(parent, outData);
      else
        generatePythonAction(outData);
      outData.println();
    }
  }
  private static void generatePythonSingle(String parent, PrintWriter outData)
  {
    outData.println("    result = " + parent + "()");
    outData.println("    result._fromList(cursor.fetchone())");
    outData.println("    return result");
  }
  private static void generatePythonMultiple(String parent, PrintWriter outData)
  {
    outData.println("    result = []");
    outData.println("    for row in cursor:");
    outData.println("      record = " + parent + "()");
    outData.println("      record._fromList(row)");
    outData.println("      result.append(record)");
    outData.println("    return result");
  }
  private static void generatePythonAction(PrintWriter outData)
  {
    outData.println("    pass");
  }
  static void generateString(Proc proc, String name, Vector<String> strings, PrintWriter outData)
  {
    String added = "";
    outData.println("    "+name+" = '''\\");
    for (int i = 0; i < strings.size(); i++)
    {
      String string = strings.elementAt(i);
      if (string.charAt(0) == '"')
        outData.println(string.substring(1, string.length() - 1));
      else
      {
        if (added.length() == 0)
          added = added + " = (";
        else
          added = added + ", ";
        String l = string.trim();
        added = added + l;
        String quotes = "";
        if (proc.isStrung(l) == true)
          quotes = "'";
        outData.println(quotes + "%s" + quotes);
      }
    }
    if (added.length() > 0)
      added = added + ")";
    outData.println("'''"+added);
  }

  static boolean isNull(Field field)
  {
    if (field.isNull == false)
      return false;
    return switch (field.type)
            {
              case Field.BOOLEAN, Field.FLOAT, Field.DOUBLE, Field.MONEY, Field.BYTE, Field.SHORT, Field.INT, Field.LONG, Field.IDENTITY, Field.SEQUENCE, Field.BLOB, Field.DATE, Field.DATETIME, Field.TIMESTAMP, Field.TIME -> true;
              default -> false;
            };
  }
}
