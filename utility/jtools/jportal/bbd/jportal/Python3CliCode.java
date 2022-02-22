/// ------------------------------------------------------------------
/// Copyright (c) 1996, 2013 Vincent Risi in Association 
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
/// ------------------------------------------------------------------
package bbd.jportal;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

import static bbd.jportal.Writer.*;

/**
 * 
 */
public class Python3CliCode extends Generator
{
  /**
   * Reads input from stored repository
   */
  public static String description()
  {
    return "Generates CLI Python3 Code";
  }
  public static String documentation()
  {
    return "Generates CLI Python3 Code";
  }
  protected static Vector flagsVector;
  static String pymodName;
  static String pymodFront;
  static boolean dontQualify;
  static boolean useUTF8;
  static boolean useLatin1;
  private static void flagDefaults()
  {
    pymodName = "";
    pymodFront = "";
    dontQualify = false;
    useUTF8 = false;
    useLatin1 = false;
  }
  public static Vector flags()
  {
    if (flagsVector == null)
    {
      flagDefaults();
      flagsVector = new Vector();
      flagsVector.addElement(new Flag("use pymod", pymodName, "Use pymod"));
      flagsVector.addElement(new Flag("dont qualify", dontQualify, "Dont Qualify"));
      flagsVector.addElement(new Flag("utf-8", useUTF8, "use utf-8"));
      flagsVector.addElement(new Flag("iso-8859-1", useLatin1, "use iso-8859-1"));
    }
    return flagsVector;
  }
  /**
  * Sets generation flags.
  */
  static void setFlags(Database database, PrintWriter outLog)
  {
    if (flagsVector != null)
    {
      pymodName = (String)((Flag)flagsVector.elementAt(0)).value;
      dontQualify = toBoolean(((Flag)flagsVector.elementAt(1)).value);
      useUTF8 = toBoolean(((Flag)flagsVector.elementAt(2)).value);
      useLatin1 = toBoolean(((Flag)flagsVector.elementAt(3)).value);
    }
    else
      flagDefaults();
    for (int i = 0; i < database.flags.size(); i++)
    {
      String flag = database.flags.elementAt(i);
      if (flag.length() > 6 && flag.substring(0, 6).equalsIgnoreCase("pymod="))
        pymodName = flag.substring(6);
      else if (flag.equalsIgnoreCase("dont qualify"))
        dontQualify = true;
      else if (flag.equalsIgnoreCase("utf-8") || flag.equalsIgnoreCase("utf-8"))
      {
        useUTF8 = true;
        useLatin1 = false;
      }
      else if (flag.equalsIgnoreCase("iso-8859-1") || flag.equalsIgnoreCase("latin1"))
      {
        useLatin1 = true;
        useUTF8 = false;
      }
    }
    if (pymodName.length() > 0)
    {
      outLog.println(" (pymod=" + pymodName + ")");
      pymodFront = pymodName + ".";
      if (dontQualify == false)
        pymodFront += pymodName.substring(0, 1).toUpperCase() + pymodName.substring(1) + "_";
    }
    if (dontQualify == true)
      outLog.println(" (dont qualify)");
    if (useUTF8 == true)
      outLog.println(" (utf-8)");
    if (useLatin1 == true)
      outLog.println(" (iso-8859-1)");
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
  public static void generate(Database database, String output,
      PrintWriter outLog)
  {
    setFlags(database, outLog);
    for (int i = 0; i < database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      generateTable(table, output, outLog);
    }
  }
  /**
   * Build of standard and user defined procedures
   */
  static void generateTable(Table table, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + "DB_" + table.useName().toUpperCase() + ".py");
      OutputStream outFile = new FileOutputStream(output + "DB_" + table.useName().toUpperCase() + ".py");
      try
      {
        writer = new PrintWriter(outFile);
        indent_size = 4;
        try
        {
          if (useUTF8 == true)
            writeln("# -*- coding: utf-8 -*-");
          else if (useLatin1 == true)
            writeln("# -*- coding: iso-8859-1 -*-");
          writeln("# This code was generated, do not modify it, modify it at source and regenerate it.");
          writeln("# " + table.useName().toUpperCase() + ".py");
          writeln();
          if (pymodName.length() > 0)
            writeln("import "+pymodName);
          writeln();
          writeln("_BLOB = 1;_BOOLEAN = 2;_BYTE = 3;_CHAR = 4;_DATE = 5;_DATETIME = 6");
          writeln("_DOUBLE = 7;_DYNAMIC = 8;_FLOAT = 9;_IDENTITY = 10;_INT = 11;_LONG = 12");
          writeln("_MONEY = 13;_SEQUENCE = 14;_SHORT = 15;_STATUS = 16;_TIME = 17");
          writeln("_TIMESTAMP = 18;_TLOB = 19;_USERSTAMP = 20;_ANSICHAR = 21;_UID = 22;_XML = 23");
          writeln("_BIGSEQUENCE = 24;_BIGIDENTITY = 25");
          writeln();
          writeln("# =i=n=d=e=n=t=a=t=i=o=n===b=y===f=o=u=r======");
          writeln("# s    : value as a string");
          writeln("# attr : (type, length, scale, precision)");
          writeln("# name : name of field for reporting");
          writeln("# =i=s===a===p=a=i=n==========================");
          writeln();
          writeln("def _validate(s, attr, name):");
          writeln(1, "if attr[0] in (_CHAR, _ANSICHAR, _DATE, _DATETIME, _TIME, _TIMESTAMP, _USERSTAMP, _XML):");
          writeln(2, "if isinstance(s, bytes): s = s.decode()");
          writeln(2, "if len(s) > attr[1]: raise AssertionError('%s:Length exceeds %d' % (name, attr[1]))");
          writeln(1, "elif attr[0] in (_DOUBLE, _FLOAT) and attr[2] > 15:");
          writeln(2, "if len(s) > attr[2]+2: raise AssertionError('%s:Length exceeds %d' % (name, attr[2]+2))");
          writeln(1, "elif attr[0] == _MONEY:");
          writeln(2, "if isinstance(s, bytes): s = s.decode()");
          writeln(2, "if len(s) > 20: raise AssertionError('%s:Length exceeds %d' % (name, 20))");
          writeln(1, "return s");
          writeln();
          writeln("def _str(s, attr, name):");
          writeln(1, "if s == None:");
          writeln(2, "return None");
          if (useUTF8 == true)
          {
            writeln(1, "elif isinstance(s, str):");
            writeln(2, "fix = s.encode('utf-8')");
            writeln(2, "return _validate(fix, attr, name)");
          }
          else if (useLatin1 == true)
          {
            writeln(1, "elif isinstance(s, str):");
            writeln(2, "fix = s.encode('iso-8859-1')");
            writeln(2, "return _validate(fix, attr, name)");
          }
          else
          {
            writeln(1, "elif isinstance(s, bytes):");
            writeln(2, "return _validate(s, attr, name)");
          }
          
          writeln(1, "elif isinstance(s, float):");
          writeln(2, "return '%0.15g' % (s)");
          writeln(1, "return _validate(s, attr, name)");
          writeln();
          generateEnums(table);
          if (table.hasStdProcs)
            generateStdOutputRec(table);
          generateUserOutputRecs(table);
          writer.flush();
        }
        finally
        {
          writer.flush();
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
  static void generateDataFields(Vector fields, String superName, String className,
      String tableName)
  {
    write("    __slots__ = [");
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      if (i != 0)
      {
        writeln(",");
        write("        ");
      }
      write("'" + field.useName() + "'");
    }
    writeln("]");
    write("    __attrs__ = [");
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      if (i != 0)
      {
        writeln(",");
        write("        ");
      }
      write("(" + field.type + ", " + field.length + ", " + field.precision + ", " + field.scale + ")");
    }
    writeln("]");
    writeln(1, "def __init__(self):");
    if (superName.length() > 0)
      writeln(2, "" + superName + ".__init__(self) ## \\field see:" + superName);
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      if (isNull(field) == true)
        writeln(2, "self." + field.useName() + " = None ## \\field " + field.useName() + ":" + varName(field) + " nullable");
      else
        writeln(2, "self." + field.useName() + " = '' ## \\field " + field.useName() + ":" + varName(field));
    }
    writeln(1, "def _fromList(self, result):");
    String no = "";
    if (superName.length() > 0)
    {
      writeln(2, "no = " + superName + "._fromList(self, result)");
      no = "no+";
    }
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      switch (field.type) {
        case Field.LONG:
        case Field.BIGSEQUENCE:
        case Field.BIGIDENTITY:
          if (isNull(field) == true)
            writeln(2, "self." + field.useName() + " = None if result[" + no + i + "] == None else int(result[" + no + i + "])");
          else
            writeln(2, "self." + field.useName() + " = int(result[" + no + i + "])");
          break;
        case Field.ANSICHAR:
        case Field.BIGXML:
        case Field.CHAR:
        case Field.DATE:
        case Field.DATETIME:
        case Field.AUTOTIMESTAMP:
        case Field.MONEY:
        case Field.TIME:
        case Field.TIMESTAMP:
        case Field.TLOB:
        case Field.UID:
        case Field.USERSTAMP:
        case Field.UTF8:
        case Field.WANSICHAR:
        case Field.WCHAR:
          writeln(2, "self." + field.useName() + " = str(result[" + no + i + "])");
          break;
        default:
          writeln(2, "self." + field.useName() + " = result[" + no + i + "]");
          break;
      }
    }
    writeln(2, "return " + no + fields.size());
    writeln(1, "def _toList(self):");
    writeln(2, "names = " + className + ".__slots__");
    writeln(2, "attrs = " + className + ".__attrs__");
    if (superName.length() > 0)
      write("        result = " + superName + "._toList(self) + [");
    else
      write("        result = [");
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      if (i != 0)
      {
        writeln(",");
        write("            ");
      }
      write("_str(self." + field.useName() + ", attrs[" + i + "], names[" + i + "])");
    }
    writeln("]");
    writeln(2, "return result");
    writeln(1, "def _display(self):");
    write  ("        names = ");
    if (superName.length() > 0)
      write  (superName + ".__slots__ + ");
    writeln(className + ".__slots__");
    writeln(2, "data = self._toList()");
    writeln(2, "for i in range(len(data)):");
    writeln(3, "print('%s: %s' % (names[i], data[i]))");
  }
  /**
   * Build of output data rec for standard procedures
   */
  static void generateStdOutputRec(Table table)
  {
    for (int i = 0; i < table.comments.size(); i++)
    {
      String s = table.comments.elementAt(i);
      writeln("## " + s);
    }
    writeln("## \\class D" + table.useName());
    writeln("class D" + table.useName() + "(object):");
    writeln(1, "def _make(self): return D" + table.useName() + "()");
    writeln(1, "def _name(self): return ('D" + table.useName() + "','O" + table.useName() + "')");
    generateDataFields(table.fields, "", "D"+table.useName(), table.useName());
    writeln();
    writeln("## \\class O" + table.useName());
    writeln("## \\field see:D" + table.useName());
    writeln("O" + table.useName() + " = D" + table.useName());
    writeln();
    if (pymodName.length() > 0)
    {
      writeln("class " + table.useName() + "(D" + table.useName() + "):");
      writeln(1, "def __init__(self): D" + table.useName() + ".__init__(self)");
      coverFunctions = new Vector();
      for (int i = 0; i < table.procs.size(); i++)
      {
        Proc proc = table.procs.elementAt(i);
        if (proc.isData == true || (proc.isStd == false && proc.isStdExtended() == false))
          continue;
        if (proc.isMultipleInput)
          generateBulkAction(table, proc);
        else if (proc.isInsert && proc.hasReturning)
          generateAction(table, proc);
        else if (proc.outputs.size() > 0)
          if (proc.isSingle)
            generateSingle(table, proc);
          else
            generateMultiple(table, proc);
        else
          generateAction(table, proc);
      }
      writeln();
      for (int i = 0; i < coverFunctions.size(); i++)
        writeln((String)coverFunctions.elementAt(i));
    }
  }
  private static Vector coverFunctions;
  private static void generateInput(String tableName, String dataStruct, String procName, String parms)
  {
    writeln(1, "def _" + procName + "_dict(self, parms):");
    writeln(2, "'low level call with dictionary input'");
    writeln(2, "for parm in parms: setattr(self, parm, parms[parm])");
    writeln(2, "return self." + procName + "()");
    writeln(1, "def " + procName + "_with(self" + parms + "):");
    writeln(2, "'with method - it is suggested for maintenance named parameters are used'");
    writeln(2, "return self._" + procName + "_dict(vars())");
    coverFunctions.addElement("def " + tableName + "_" + procName + "(" + parms.substring(2) + "):");
    coverFunctions.addElement("    'It is suggested for maintenance named parameters are used'");
    coverFunctions.addElement("    return " + dataStruct + "()._" + procName + "_dict(vars())");
    coverFunctions.addElement("");
  }
  private static void generateCover(String tableName, String dataStruct, String procName)
  {
    coverFunctions.addElement("def " + tableName + "_" + procName + "():");
    coverFunctions.addElement("    return " + dataStruct + "()." + procName + "()");
    coverFunctions.addElement("");
  }
  private static void generateMultiple(Table table, Proc proc)
  {
    String parameters="", comma="";
    String dataStruct;
    if (proc.isStd || proc.isStdExtended())
      dataStruct = table.useName();
    else
      dataStruct = table.useName() + proc.upperFirst();
    boolean hasInput = (proc.inputs.size() > 0 || proc.dynamics.size() > 0);
    String firstParm = "";
    if (hasInput == true)
      firstParm = "self, ";
    writeln(1, "def " + proc.name + "(self):");
    writeln(2, "''' Multiple returns count and recs");
    if (hasInput == true)
    {
      writeln(2, "Input:");
      for (int f = 0; f < proc.inputs.size(); f++)
      {
        Field field = proc.inputs.elementAt(f);
        writeln(3, "" + field.useName());
        parameters += ", " + field.useName() + "=" + defValue(field);
        comma = ", ";
      }
      for (int f = 0; f < proc.dynamics.size(); f++)
      {
        String field = proc.dynamics.elementAt(f);
        writeln(3, "" + field);
        parameters += ", " + field + "=''";
        comma = ", ";
      }
    }
    writeln(2, "Output:");
    for (int f = 0; f < proc.outputs.size(); f++)
    {
      Field field = proc.outputs.elementAt(f);
      writeln(3, "" + field.useName());
    }
    writeln(2, "'''");
    writeln(2, "return " + pymodFront + table.useName() + proc.upperFirst() + "(" + firstParm + "0, O" + dataStruct + "())");
    if (hasInput == true)
      generateInput(table.useName(), dataStruct, proc.name, parameters);
    else
      generateCover(table.useName(), dataStruct, proc.name);
  }
  private static void generateSingle(Table table, Proc proc)
  {
    String dataStruct;
    if (proc.isStd || proc.isStdExtended())
      dataStruct = table.useName();
    else
      dataStruct = table.useName() + proc.upperFirst();
    String parameters = "", comma = "";
    boolean hasInput = (proc.inputs.size() > 0 || proc.dynamics.size() > 0);
    writeln(1, "def " + proc.name + "(self):");
    writeln(2, "''' Single returns boolean and record");
    if (hasInput == true)
    {
      writeln(2, "Input:");
      for (int f = 0; f < proc.inputs.size(); f++)
      {
        Field field = proc.inputs.elementAt(f);
        writeln(3, "" + field.useName());
        parameters += ", " + field.useName() + "=" + defValue(field);
        comma = ", ";
      }
      for (int f = 0; f < proc.dynamics.size(); f++)
      {
        String field = proc.dynamics.elementAt(f);
        writeln(3, "" + field);
        parameters += ", " + field + "=''";
      }
    }
    writeln(2, "Output:");
    for (int f = 0; f < proc.outputs.size(); f++)
    {
      Field field = proc.outputs.elementAt(f);
      writeln(3, "" + field.useName());
    }
    writeln(2, "'''");
    writeln(2, "return " + pymodFront + table.useName() + proc.upperFirst() + "(self)");
    if (hasInput == true)
      generateInput(table.useName(), dataStruct, proc.name, parameters);
    else
      generateCover(table.useName(), dataStruct, proc.name);
  }
  private static void generateAction(Table table, Proc proc)
  {
    String dataStruct;
    if (proc.isStd || proc.isStdExtended())
      dataStruct = table.useName();
    else
      dataStruct = table.useName() + proc.upperFirst();
    String parameters = "", comma = "";
    writeln(1, "def " + proc.name + "(self):");
    writeln(2, "''' Action");
    writeln(2, "Input:");
    for (int f = 0; f < proc.inputs.size(); f++)
    {
      Field field = proc.inputs.elementAt(f);
      writeln(3, "" + field.useName());
      parameters += ", " + field.useName() + "=" + defValue(field);
      comma = ", ";
    }
    for (int f = 0; f < proc.dynamics.size(); f++)
    {
      String field = proc.dynamics.elementAt(f);
      writeln(3, "" + field);
      parameters += ", " + field + "=''";
      comma = ", ";
    }
    writeln(2, "'''");
    writeln(2, "return " + pymodFront + table.useName() + proc.upperFirst() + "(self)");
    generateInput(table.useName(), dataStruct, proc.name, parameters);
  }
  private static void generateBulkAction(Table table, Proc proc)
  {
    writeln(1, "def " + proc.name + "(self, recs):");
    writeln(2, "''' Bulk Action");
    writeln(2, "Input:");
    for (int f = 0; f < proc.inputs.size(); f++)
    {
      Field field = proc.inputs.elementAt(f);
      writeln(3, "" + field.useName());
    }
    for (int f = 0; f < proc.dynamics.size(); f++)
    {
      String field = proc.dynamics.elementAt(f);
      writeln(3, "" + field);
    }
    writeln(2, "'''");
    writeln(2, "" + pymodFront + table.useName() + proc.upperFirst() + "(len(recs), recs)");
  }
  /**
   * Build of output data rec for user procedures
   */
  static void generateUserOutputRecs(Table table)
  {
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData || proc.isStd || proc.hasNoData())
        continue;
      if (proc.isStdExtended())
        continue;
      String work = "(object)";
      String superName = "";
      if (proc.outputs.size() > 0)
      {
        for (int j = 0; j < proc.comments.size(); j++)
          writeln("##" + proc.comments.elementAt(j));
        String typeChar = "D";
        if (proc.hasDiscreteInput())
          typeChar = "O";
        work = "(" + typeChar + table.useName() + proc.upperFirst() + ")";
        superName = typeChar + table.useName() + proc.upperFirst();
        writeln("## \\class " + typeChar + table.useName() + proc.upperFirst());
        writeln("class " + typeChar + table.useName() + proc.upperFirst() + "(object):");
        writeln(1, "def _make(self): return " + typeChar + table.useName() + proc.upperFirst() + "()");
        if (proc.hasDiscreteInput())
          writeln(1, "def _name(self): return ('" + typeChar + table.useName() + proc.upperFirst() + "')");
        else
          writeln(1, "def _name(self): return ('D" + table.useName() + proc.upperFirst() + "', 'O" + table.useName() + proc.upperFirst() + "')");
        generateDataFields(proc.outputs, "", typeChar + table.useName() + proc.upperFirst(), table.useName());
        writeln();
      }
      if (proc.hasDiscreteInput())
      {
        writeln("## \\class D" + table.useName() + proc.upperFirst());
        writeln("class D" + table.useName() + proc.upperFirst() + work + ":");
        writeln(1, "def _make(self): return D" + table.useName() + proc.upperFirst() + "()");
        writeln(1, "def _name(self): return ('D" + table.useName() + proc.upperFirst() + "')");
        Vector discreteInputs = new Vector();
        for (int j = 0; j < proc.inputs.size(); j++)
        {
          Field field = proc.inputs.elementAt(j);
          if (!proc.hasOutput(field.name))
            discreteInputs.addElement(field);
        }

        for (int j = 0; j < proc.dynamics.size(); j++)
        {
          Field f = new Field();
          Integer length = proc.dynamicSizes.elementAt(j);
          f.name = proc.dynamics.elementAt(j);
          f.type = Field.CHAR;
          f.length = length.intValue();
          discreteInputs.addElement(f);
        }
        generateDataFields(discreteInputs, superName, "D" + table.useName() + proc.upperFirst(), table.useName());
        writeln();
      }
      else if (proc.outputs.size() > 0)
      {
        writeln("## \\class O" + table.useName() + proc.upperFirst());
        writeln("## \\field see:O" + table.useName() + proc.upperFirst());
        writeln("O" + table.useName() + proc.upperFirst() + " = D" + table.useName() + proc.upperFirst());
        writeln();
      }
      if (pymodName.length() > 0)
      {
        coverFunctions = new Vector();
        writeln("class " + table.useName() + proc.upperFirst() + "(D" + table.useName() + proc.upperFirst() + "):");
        writeln(1, "def __init__(self): D" + table.useName()  + proc.upperFirst() + ".__init__(self)");
        if (proc.isMultipleInput)
          generateBulkAction(table, proc);
        else if (proc.isInsert && proc.hasReturning)
          generateAction(table, proc);
        else if (proc.outputs.size() > 0)
          if (proc.isSingle)
            generateSingle(table, proc);
          else
            generateMultiple(table, proc);
        else
          generateAction(table, proc);
        writeln();
        for (int j = 0; j < coverFunctions.size(); j++)
          writeln((String)coverFunctions.elementAt(j));
      }
    }
  }
  /**
   * Build of enums
   */
  static void generateEnums(Table table)
  {
    for (int i = 0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      generateEnums(table.useName() + field.useName(), field);
    }
  }
  static void generateEnums(String baseName, Field field)
  {
    if (field.enums.size() > 0)
    {
      writeln(baseName + " = {}");
      for (int j = 0; j < field.enums.size(); j++)
      {
        Enum entry = field.enums.elementAt(j);
        writeln(baseName + "['" + entry.name + "'] = " + entry.value);
      }
      for (int j = 0; j < field.enums.size(); j++)
      {
        Enum entry = field.enums.elementAt(j);
        writeln(baseName + "[" + entry.value + "] = '" + entry.name + "'");
      }
      writeln();
    }
    else if (field.valueList.size() > 0)
    {
      writeln("class " + baseName + "():");
      for (int j = 0; j < field.valueList.size(); j++)
      {
        String entry = field.valueList.elementAt(j);
        writeln(1, "" + entry + " = " + j);
      }
      write(1, "lookup = (");
      for (int j = 0; j < field.valueList.size(); j++)
      {
        String entry = field.valueList.elementAt(j);
        write((j == 0 ? "" : ", ") + "'" + entry + "'");
      }
      writeln(")");
      writeln();
    }
  }
  static String varName(Field field)
  {
    switch (field.type)
    {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return "int";
    case Field.LONG:
    case Field.BIGSEQUENCE:
    case Field.BIGIDENTITY:
      return "long";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.TLOB:
    case Field.XML:
    case Field.USERSTAMP:
      return "char";
    case Field.BLOB:
      return "BLOB";
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "char";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "double";
    }
    return "<unsupported>";
  }
  static String defValue(Field field)
  {
    if (isNull(field) == true)
      return "None";
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return "'0'";
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return "'0'";
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.TLOB:
      case Field.XML:
      case Field.USERSTAMP:
        return "''";
      case Field.BLOB:
        return "'BLOB'";
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
        return "''";
      case Field.FLOAT:
      case Field.DOUBLE:
      case Field.MONEY:
        return "'0.0'";
    }
    return "<unsupported>";
  }
  static boolean isNull(Field field)
  {
    if (field.isNull == false)
      return false;
    switch (field.type)
    {
    case Field.BOOLEAN:
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
    case Field.IDENTITY:
    case Field.SEQUENCE:
    case Field.BIGIDENTITY:
    case Field.BIGSEQUENCE:
    case Field.BLOB:
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIMESTAMP:
    case Field.TIME:
      return true;
    }
    return false;
  }
}
