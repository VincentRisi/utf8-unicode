/// ------------------------------------------------------------------
/// Copyright (c) 1996, 2007 Vincent Risi in Association 
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
package bbd.jportal.old;

import bbd.jportal.*;

import java.io.*;
import java.util.Vector;
import static bbd.jportal.Writer.*;

public class OdbcCCode extends Generator
{
  public static String description()
  {
    return "Generate ODBC C++ Code";
  }
  public static String documentation()
  {
    return "Generate ODBC C++ Code";
  }
  static PlaceHolder placeHolder;
  /**
   * Generates the procedure classes for each table present.
   */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    for (int i = 0; i < database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      generate(table, output, outLog);
      generateSnips(table, output, outLog);
    }
  }
  /**
   * Build of standard and user defined procedures
   */
  static void generate(Table table, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + fileName(output, table.useName().toLowerCase(), ".sh"));
      OutputStream outFile = new FileOutputStream(fileName(output, table.useName().toLowerCase(), ".sh"));
      try
      {
        writer = new PrintWriter(outFile);
        indent_size = 4;
        try
        {
          writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
          writeln("#ifndef _" + table.useName().toLowerCase() + "SH");
          writeln("#define _" + table.useName().toLowerCase() + "SH");
          writeln();
          writeln("#include <stddef.h>");
          writeln("#include \"padgen.h\"");
          writeln("#include \"cliapi.h\"");
          writeln("#include \"swapbytes.h\"");
          writeln();
          if (table.hasStdProcs)
            generateStdOutputRec(table);
          generateUserOutputRecs(table);
          generateInterface(table);
          writeln("#endif");
          writer.flush();
        }
        finally
        {
          writer.flush();
        }
        outFile.close();
        outLog.println("Code: " + fileName(output, table.useName().toLowerCase(), ".cpp"));
        outFile = new FileOutputStream(
            fileName(output, table.useName().toLowerCase(), ".cpp"));
        writer = new PrintWriter(outFile);
        try
        {
          writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
          writeln();
          writeln("#include \"" + fileName("", table.useName().toLowerCase(), ".sh") + "\"");
          writeln();
          generateImplementation(table);
        }
        finally
        {
          writer.flush();
        }
      }
      finally
      {
        writer.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
  }
  private static void generateSnips(Table table, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + table.useName().toLowerCase() + "_snips.h");
      OutputStream outFile = new FileOutputStream(output + table.useName().toLowerCase() + "_snips.h");
      try
      {
        writer = new PrintWriter(outFile);
        try
        {
          writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
          writeln("#ifndef _" + table.useName().toUpperCase() + "_SNIPS_H_");
          writeln("#define _" + table.useName().toUpperCase() + "_SNIPS_H_");
          writeln();
          writeln("#include \"list.h\"");
          writeln("#include \"" + table.useName().toLowerCase() + ".sh\"");
          writeln();
          for (int i = 0; i < table.procs.size(); i++)
          {
            Proc proc = table.procs.elementAt(i);
            if (proc.isData)
              continue;
            if (proc.isMultipleInput)
              generateSnipsBulkAction(table, proc);
            else if (proc.isInsert && proc.hasReturning)
              generateSnipsAction(table, proc);
            else if (proc.outputs.size() > 0)
              if (proc.isSingle)
                generateSnipsSingle(table, proc);
              else
                generateSnipsMultiple(table, proc);
            else
              generateSnipsAction(table, proc);
            writeln();
          }
          writeln("#endif");
        }
        finally
        {
          writer.flush();
        }
      }
      finally
      {
        writer.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
  }
  private static void generateSnipsAction(Table table, Proc proc)
  {
    String dataStruct;
    if (proc.isStd || proc.isStdExtended())
      dataStruct = table.useName();
    else
      dataStruct = table.useName() + proc.upperFirst();
    boolean hasInput = (proc.inputs.size() > 0 || proc.dynamics.size() > 0);
    write("inline void " + table.useName() + proc.upperFirst() + "(TJConnector *connect");
    if (hasInput || proc.hasModifieds())
      write(", D" + dataStruct + " *rec");
    writeln(")");
    writeln("{");
    writeln(1, "T" + table.useName() + proc.upperFirst() + " q(*connect, JP_MARK);");
    if (hasInput || proc.hasModifieds())
      writeln(1, "q.Exec(*rec);");
    else
      writeln(1, "q.Exec();");
    if (proc.hasReturning)
    {
      writeln(1, "if (q.Fetch())");
      writeln(2, "*rec = *q.DRec();");
    }
    else if (proc.hasModifieds())
      writeln(1, "*rec = *q.DRec();");
    writeln("}");
  }
  private static void generateSnipsBulkAction(Table table, Proc proc)
  {
    String dataStruct;
    if (proc.isStd || proc.isStdExtended())
      dataStruct = table.useName();
    else
      dataStruct = table.useName() + proc.upperFirst();
    write("inline void " + table.useName() + proc.upperFirst() + "(TJConnector *connect");
    write(", int noOf, D" + dataStruct + " *recs");
    writeln(")");
    writeln("{");
    writeln(1, "T" + table.useName() + proc.upperFirst() + " q(*connect, JP_MARK);");
    writeln(1, "q.Exec(noOf, recs);");
    writeln("}");
  }
  private static void generateSnipsSingle(Table table, Proc proc)
  {
    String dataStruct;
    if (proc.isStd || proc.isStdExtended())
      dataStruct = table.useName();
    else
      dataStruct = table.useName() + proc.upperFirst();
    boolean hasInput = (proc.inputs.size() > 0 || proc.dynamics.size() > 0);
    writeln("inline bool " + table.useName() + proc.upperFirst()
                   + "(TJConnector *connect, D" + dataStruct + " *rec)");
    writeln("{");
    writeln(1, "T" + table.useName() + proc.upperFirst() + " q(*connect, JP_MARK);");
    if (hasInput)
      writeln(1, "q.Exec(*rec);");
    else
      writeln(1, "q.Exec();");
    writeln(1, "if (q.Fetch())");
    writeln(1, "{");
    writeln(2, "*rec = *q.DRec();");
    writeln(2, "return true;");
    writeln(1, "}");
    writeln(1, "return false;");
    writeln("}");
  }
  private static void generateSnipsMultiple(Table table, Proc proc)
  {
    String dataStruct;
    if (proc.isStd || proc.isStdExtended())
      dataStruct = table.useName();
    else
      dataStruct = table.useName() + proc.upperFirst();
    boolean hasInput = (proc.inputs.size() > 0 || proc.dynamics.size() > 0);
    write("inline void " + table.useName() + proc.upperFirst() + "(TJConnector *connect");
    if (hasInput)
      write(", D" + dataStruct + "* inRec");
    writeln(", int32* noOf, O" + dataStruct + "*& outRecs)");
    writeln("{");
    writeln(1, "T" + table.useName() + proc.upperFirst() + " q(*connect, JP_MARK);");
    if (hasInput)
      writeln(1, "q.Exec(*inRec);");
    else
      writeln(1, "q.Exec();");
    writeln(1, "while (q.Fetch())");
    writeln(2, "SnipAddList(outRecs, *noOf, *q.ORec(), (int32)q.NOROWS);");
    writeln("}");
  }
  /**
   * Build of output data rec for standard procedures
   */
  static Vector nullVector = new Vector();
  static String structName = "";
  static void generateStdOutputRec(Table table)
  {
    for (int i = 0; i < table.comments.size(); i++)
    {
      String s = table.comments.elementAt(i);
      writeln("//" + s);
    }
    int filler = 0;
    structName = "D" + table.useName();
    writeln("struct D" + table.useName());
    writeln("{");
    boolean canExtend = true;
    Vector fields = table.fields;
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field)fields.elementAt(i);
      if (field.type == Field.BLOB)
        canExtend = false;
      if (field.comments.size() > 0)
        for (int c = 0; c < field.comments.size(); c++)
        {
          String s = field.comments.elementAt(c);
          writeln(1, "//" + s);
        }
      writeln(1, "" + padder(cppVar(field) + ";", 48) + generatePadding(field, filler++));
      if (isNull(field))
      {
        writeln(1, "" + padder("int16  " + field.useName() + "IsNull;", 48) + generatePadding(filler++));
        filler++;
      }
    }
    writeln();
    headerSwaps("", fields, null);
    String useName = table.useName();
    if (canExtend == true)
      extendHeader("", fields, useName, nullVector, null);
    else
      extendDataBuildHeader("", fields, useName, nullVector, null);
    // we cannot put in a virtual destructor here as the data has to remaina POD for GNU
    //writeln(1, "virtual ~D" + table.useName() + "() {}");
    writeln("};");
    writeln();
    writeln("typedef D" + table.useName() + " O" + table.useName() + ";");
    writeln();
    generateEnumOrdinals(table);
  }
  static void generateUserOutputRecs(Table table)
  {
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData || proc.isStd || proc.hasNoData())
        continue;
      if (proc.isStdExtended())
        continue;
      String work = "";
      String baseClass = "";
      boolean canExtend = true;
      Vector fields = proc.outputs;
      for (int j = 0; j < fields.size(); j++)
      {
        Field field = (Field)fields.elementAt(j);
        if (field.type == Field.BLOB)
          canExtend = false;
      }
      fields = proc.inputs;
      for (int j = 0; j < fields.size(); j++)
      {
        Field field = (Field)fields.elementAt(j);
        if (field.type == Field.BLOB)
          canExtend = false;
      }
      fields = proc.outputs;
      if (fields.size() > 0)
      {
        for (int j = 0; j < proc.comments.size(); j++)
        {
          String comment = proc.comments.elementAt(j);
          writeln("//" + comment);
        }
        String typeChar = "D";
        if (proc.hasDiscreteInput())
          typeChar = "O";
        work = " : public " + typeChar + table.useName() + proc.upperFirst();
        baseClass = typeChar + table.useName() + proc.upperFirst();
        structName = typeChar + table.useName() + proc.upperFirst();
        writeln("struct " + typeChar + table.useName() + proc.upperFirst());
        writeln("{");
        int filler = 0;
        for (int j = 0; j < fields.size(); j++)
        {
          Field field = (Field)fields.elementAt(j);
          for (int c = 0; c < field.comments.size(); c++)
          {
            String s = field.comments.elementAt(c);
            writeln(1, "//" + s);
          }
          writeln(1, "" + padder(cppVar(field) + ";", 48) + generatePadding(field, filler++));
          if (isNull(field))
            writeln(1, "" + padder("int16  " + field.useName() + "IsNull;", 48) + generatePadding(filler++));
        }
        writeln();
        headerSwaps("", fields, null);
        String useName = table.useName() + proc.upperFirst();
        if (canExtend == true)
          extendHeader("", fields, useName, nullVector, null);
        else
          extendDataBuildHeader("", fields, useName, nullVector, null);
        // we cannot put in a virtual destructor here as the data has to remaina POD for GNU
        //writeln(1, "virtual ~" + typeChar + table.useName() + proc.upperFirst() + "() {}");
        writeln("};");
        writeln();
      }
      if (proc.hasDiscreteInput())
      {
        structName = "D" + table.useName() + proc.upperFirst();
        writeln("struct D" + table.useName() + proc.upperFirst() + work);
        writeln("{");
        int filler = 0;
        Vector inputs = proc.inputs;
        for (int j = 0; j < inputs.size(); j++)
        {
          Field field = (Field)inputs.elementAt(j);
          if (proc.hasOutput(field.name))
            continue;
          for (int c = 0; c < field.comments.size(); c++)
          {
            String s = field.comments.elementAt(c);
            writeln(1, "//" + s);
          }
          writeln(1, "" + padder(cppVar(field) + ";", 48) + generatePadding(field, filler++));
          if (isNull(field))
            writeln(1, "" + padder("int16  " + field.useName() + "IsNull;", 48) + generatePadding(filler++));
        }
        writeln();
        for (int j = 0; j < proc.dynamics.size(); j++)
        {
          String s = proc.dynamics.elementAt(j);
          Integer n = proc.dynamicSizes.elementAt(j);
          writeln(1, "" + padder("char " + s + "[" + (n.intValue() + 1) + "];", 48) + charPadding(n.intValue() + 1, filler++));
        }
        headerSwaps(baseClass, inputs, proc);
        String useName = table.useName() + proc.upperFirst();
        if (canExtend == true)
          extendHeader(baseClass, inputs, useName, proc.dynamics, proc);
        else
          extendDataBuildHeader(baseClass, inputs, useName, proc.dynamics, proc);
        // we cannot put in a virtual destructor here as the data has to remaina POD for GNU
        //writeln(1, "virtual ~D" + table.useName() + proc.upperFirst() + "() {}");
        writeln("};");
        writeln();
      }
      else if (fields.size() > 0)
      {
        writeln("typedef D" + table.useName() + proc.upperFirst() + " O" + table.useName() + proc.upperFirst() + ";");
        writeln();
      }
    }
  }
  private static void headerSwaps(String baseClass, Vector inputs, Proc proc)
  {
    writeln(1, "void Clear()");
    writeln(1, "{");
    if (baseClass.length() > 0)
      writeln(2, "" + baseClass + "::Clear();");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field)inputs.elementAt(j);
      if (proc != null && proc.hasOutput(field.name))
        continue;
      writeln(2, "" + cppInit(field));
    }
    if (proc != null)
      for (int j = 0; j < proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        writeln(2, "memset(" + s + ", 0, sizeof(" + s + "));");
      }
    writeln(1, "}");
    writeln(1, "" + structName + "() { Clear(); }");
    writeln(1, "#ifdef swapbytesH");
    writeln(1, "void Swaps()");
    writeln(1, "{");
    if (baseClass.length() > 0)
      writeln(2, "" + baseClass + "::Swaps();");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field)inputs.elementAt(j);
      if (proc != null && proc.hasOutput(field.name))
        continue;
      if (notString(field) == false)
        continue;
      if (isStruct(field) == false)
        writeln(2, "SwapBytes(" + field.useName() + ");");
      else
        writeln(2, "" + field.useName() + ".Swaps();");
      if (isNull(field))
        writeln(2, "SwapBytes(" + field.useName() + "IsNull);");
    }
    writeln(1, "}");
    writeln(1, "#endif");
  }
  private static void extendHeader(String baseClass, Vector inputs, String useName, Vector dynamics, Proc proc)
  {
    writeln(1, "#if defined(_TBUFFER_H_)");
    writeln(1, "void _toXML(TBAmp &XRec)");
    writeln(1, "{");
    if (baseClass.length() > 0)
      writeln(2, "" + baseClass + "::_toXML(XRec);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field)inputs.elementAt(j);
      if (proc != null && proc.hasOutput(field.name))
        continue;
      writeln(2, "" + toXMLFormat(field));
    }
    for (int j = 0; j < dynamics.size(); j++)
    {
      String str = (String)dynamics.elementAt(j);
      String front = "XRec.append(\"  <" + str + ">\");";
      String back = "XRec.append(\"</" + str + ">\");";
      writeln(2, "" + front + "XRec.ampappend(" + str + ");" + back);
    }
    writeln(1, "}");
    writeln(1, "void ToXML(TBAmp &XRec, char* Attr, char* Outer)");
    writeln(1, "{");
    writeln(2, "XRec.append(\"<\");XRec.append(Outer);if (Attr) XRec.append(Attr);XRec.append(\">\\n\");");
    writeln(2, "_toXML(XRec);");
    writeln(2, "XRec.append(\"</\");XRec.append(Outer);XRec.append(\">\\n\");");
    writeln(1, "}");
    writeln(1, "void ToXML(TBAmp &XRec, char* Attr) {ToXML(XRec, Attr, \"" + useName + "\");}");
    writeln(1, "void ToXML(TBAmp &XRec) {ToXML(XRec, 0);}");
    writeln(1, "#endif");
    writeln(1, "#if defined(__XMLRECORD_H__)");
    writeln(1, "void _fromXML(TBAmp &XRec, TXMLRecord &msg)");
    writeln(1, "{");
    writeln(2, "TBAmp work;");
    if (baseClass.length() > 0)
      writeln(2, "" + baseClass + "::_fromXML(XRec, msg);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field)inputs.elementAt(j);
      if (proc != null && proc.hasOutput(field.name))
        continue;
      write("    msg.GetValue(\"" + field.useName() + "\", work);");
      writeln(fromXMLFormat(field));
    }
    for (int j = 0; j < dynamics.size(); j++)
    {
      String str = (String)dynamics.elementAt(j);
      write("    msg.GetValue(\"" + str + "\", work);");
      writeln("memcpy(" + str + ", work.data, sizeof(" + str + ")-1);");
    }
    writeln(1, "}");
    writeln(1, "void FromXML(TBAmp &XRec)");
    writeln(1, "{");
    writeln(2, "TXMLRecord msg;");
    writeln(2, "msg.Load(XRec);");
    writeln(2, "memset(this, 0, sizeof(*this));");
    writeln(2, "_fromXML(XRec, msg);");
    writeln(1, "}");
    writeln(1, "#endif");
    extendDataBuildHeader(baseClass, inputs, useName, dynamics, proc);
  }
  private static String nullAdd(Field field)
  {
    if (isNull(field))
      return ", " + field.useName() + "IsNull";
    return "";
  }
  private static String nullSet(Field field)
  {
    if (isNull(field))
      return ", " + field.useName() + "IsNull";
    return "";
  }
  private static void extendDataBuildHeader(String baseClass, Vector inputs, String useName, Vector dynamics, Proc proc)
  {
    writeln(1, "#if defined(_DATABUILD_H_)");
    int inputNo = 0;
    if (baseClass.length() > 0)
    {
      for (int j = 0; j < inputs.size(); j++)
      {
        Field field = (Field)inputs.elementAt(j);
        if (proc != null && proc.hasOutput(field.name))
          continue;
        inputNo++;
      }
      writeln(1, "static int NoBuildFields() {return " + baseClass + "::NoBuildFields()+" + (inputNo + dynamics.size()) + ";}");
    }
    else
      writeln(1, "static int NoBuildFields() {return " + (inputs.size() + dynamics.size()) + ";}");
    writeln(1, "void _buildAdds(DataBuilder &dBuild)");
    writeln(1, "{");
    if (baseClass.length() > 0)
      writeln(2, "" + baseClass + "::_buildAdds(dBuild);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field)inputs.elementAt(j);
      if (proc != null && proc.hasOutput(field.name))
        continue;
      if (field.type == Field.BLOB)
        writeln(2, "dBuild.add(\"" + field.useName() + "\", " + field.useName() + ".len, " + field.useName() + ".data" + nullAdd(field) + ");");
      else
        writeln(2, "dBuild.add(\"" + field.useName() + "\", " + field.useName() + nullAdd(field) + ");");
    }
    for (int j = 0; j < dynamics.size(); j++)
    {
      String str = (String)dynamics.elementAt(j);
      writeln(2, "dBuild.add(\"" + str + "\", " + str + ");");
    }
    writeln(1, "}");
    writeln(1, "void BuildData(DataBuilder &dBuild, char *name)");
    writeln(1, "{");
    writeln(2, "dBuild.name(name);");
    writeln(2, "_buildAdds(dBuild);");
    writeln(1, "}");
    writeln(1, "void BuildData(DataBuilder &dBuild) {BuildData(dBuild, \"" + useName + "\");}");
    writeln(1, "void _buildSets(DataBuilder &dBuild)");
    writeln(1, "{");
    if (baseClass.length() > 0)
      writeln(2, "" + baseClass + "::_buildSets(dBuild);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field)inputs.elementAt(j);
      if (proc != null && proc.hasOutput(field.name))
        continue;
      if (field.type == Field.BLOB)
        writeln(2, "dBuild.set(\"" + field.useName() + "\", " + field.useName() + ".len, " + field.useName() + ".data, sizeof(" + field.useName() + ".data)" + nullSet(field) + ");");
      else
        writeln(2, "dBuild.set(\"" + field.useName() + "\", " + field.useName() + ", sizeof(" + field.useName() + ")" + nullSet(field) + ");");
    }
    for (int j = 0; j < dynamics.size(); j++)
    {
      String str = (String)dynamics.elementAt(j);
      writeln(2, "dBuild.set(\"" + str + "\", " + str + ", sizeof(" + str + "));");
    }
    writeln(1, "}");
    writeln(1, "void SetData(DataBuilder &dBuild, char *name)");
    writeln(1, "{");
    writeln(2, "dBuild.name(name);");
    writeln(2, "_buildSets(dBuild);");
    writeln(1, "}");
    writeln(1, "void SetData(DataBuilder &dBuild) {SetData(dBuild, \"" + useName + "\");}");
    writeln(1, "#endif");
  }
  /**
   * Build of output data rec for standard procedures
   */
  static void generateInterface(Table table)
  {
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      generateInterface(table, proc);
    }
  }
  /**
   * Emits class method for processing the database activity
   */
  static void generateInterface(Table table, Proc proc)
  {
    String dataStruct;
    if (proc.comments.size() > 0)
      for (int i = 0; i < proc.comments.size(); i++)
      {
        String comment = proc.comments.elementAt(i);
        writeln(1, "//" + comment);
      }
    if (proc.hasNoData())
    {
      writeln("struct T" + table.useName() + proc.upperFirst());
      writeln("{");
      writeln(1, "TJQuery q_;");
      writeln(1, "void Exec();");
      writeln(1, "T" + table.useName() + proc.upperFirst() + "(TJConnector &conn, char *aFile=__FILE__, long aLine=__LINE__)");
      writeln(1, ": q_(conn)");
      writeln(1, "{q_.FileAndLine(aFile,aLine);}");
      writeln("};");
      writeln();
    }
    else
    {
      if (proc.isStdExtended() || proc.isStd)
        dataStruct = "D" + table.useName();
      else
        dataStruct = "D" + table.useName() + proc.upperFirst();
      writeln("struct T" + table.useName() + proc.upperFirst() + " : public " + dataStruct);
      writeln("{");
      generateInterface(table, proc, dataStruct);
      writeln("};");
      writeln();
    }
  }
  /**
   * 
   */
  static void generateImplementation(Table table)
  {
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      if (proc.isMultipleInput)
        generateMultipleImplementation(table, proc);
      else
        generateImplementation(table, proc);
    }
  }
  static String useNull(Field field)
  {
    if (isNull(field) 
    || (field.type == Field.CHAR && field.isNull == true) 
|| (field.type == Field.ANSICHAR && field.isNull == true))
      return ", " + field.useName() + "IsNull);";
    return ");";
  }
  static void generateMultipleImplementation(Table table, Proc proc)
  {
    placeHolder = new PlaceHolder(proc, PlaceHolder.QUESTION, "");
    String dataStruct;
    if (proc.isStdExtended() || proc.isStd)
      dataStruct = "D" + table.useName();
    else
      dataStruct = "D" + table.useName() + proc.upperFirst();
    placeHolder = new PlaceHolder(proc, PlaceHolder.QUESTION, "");
    String fullName = table.useName() + proc.upperFirst();
    writeln("void T" + fullName + "::Exec(int32 noOf, " + dataStruct + " *Recs)");
    writeln("{");
    generateCommand(proc);
    writeln(1, "q_.OpenArray(q_.command, NOBINDS, NONULLS, noOf, ROWSIZE);");
    for (int i = 0, n = 0; i < proc.inputs.size(); i++)
    {
      Field field = proc.inputs.elementAt(i);
      writeln(1, "" + cppArrayPointer(field));
      if (isNull(field))
        writeln(1, "SQLINTEGER* " + field.useName() + "IsNull = &q_.indicators[noOf*" + n++ + "];");
      else if (field.type == Field.CHAR && field.isNull == true)
        writeln(1, "SQLINTEGER* " + field.useName() + "IsNull = &q_.indicators[noOf*" + n++ + "];");
      else if (field.type == Field.ANSICHAR && field.isNull == true)
        writeln(1, "SQLINTEGER* " + field.useName() + "IsNull = &q_.indicators[noOf*" + n++ + "];");

    }
    writeln(1, "for (int i=0; i<noOf; i++)");
    writeln(1, "{");
    for (int i = 0; i < proc.inputs.size(); i++)
    {
      Field field = proc.inputs.elementAt(i);
      writeln(2, "" + cppArrayCopy(field));
      if (isNull(field))
        writeln(2, "" + field.useName() + "IsNull[i] = Recs[i]." + field.useName() + "IsNull;");
      else if (field.type == Field.CHAR && field.isNull == true)
        writeln(2, "" + field.useName() + "IsNull[i] = strlen(Recs[i]." + field.useName() + ") == 0 ? JP_NULL : SQL_NTS;");
      else if (field.type == Field.ANSICHAR && field.isNull == true)
        writeln(2, "" + field.useName() + "IsNull[i] = strlen(Recs[i]." + field.useName() + ") == 0 ? JP_NULL : SQL_NTS;");
    }
    writeln(1, "}");
    for (int i = 0; i < placeHolder.pairs.size(); i++)
    {
      PlaceHolderPairs pair = placeHolder.pairs.elementAt(i);
      Field field = pair.field;
      String size = field.useName().toUpperCase() + "_SIZE";
      switch (field.type)
      {
        case Field.ANSICHAR:
          writeln(1, "q_.BindAnsiCharArray(" + i + ", " + field.useName() + ", " + size + useNull(field));
          break;
        case Field.CHAR:
        case Field.TLOB:
        case Field.XML:
        case Field.USERSTAMP:
          writeln(1, "q_.BindCharArray(" + i + ", " + field.useName() + ", " + size + useNull(field));
          break;
        //case Field.BIGXML:
        //  break;
        case Field.LONG:
        case Field.BIGSEQUENCE:
        case Field.BIGIDENTITY:
          writeln(1, "q_.BindInt64Array(" + i + ", " + field.useName() + useNull(field));
          break;
        case Field.INT:
        case Field.SEQUENCE:
        case Field.IDENTITY:
          writeln(1, "q_.BindInt32Array(" + i + ", " + field.useName() + useNull(field));
          break;
        case Field.BOOLEAN:
        case Field.BYTE:
        case Field.SHORT:
          writeln(1, "q_.BindInt16Array(" + i + ", " + field.useName() + useNull(field));
          break;
        case Field.FLOAT:
        case Field.DOUBLE:
          if (field.precision <= 15)
            writeln(1, "q_.BindDoubleArray(" + i + ", " + field.useName() + ", " + (field.precision) + ", " + (field.scale) + useNull(field));
          else
            writeln(1, "q_.BindMoneyArray(" + i + ", " + field.useName() + ", " + (field.precision) + ", " + (field.scale) + useNull(field));
          break;
        case Field.MONEY:
          writeln(1, "q_.BindMoneyArray(" + i + ", " + field.useName() + ", 18, 2" + useNull(field));
          break;
        case Field.DATE:
          writeln(1, "q_.BindDateArray(" + i + ", " + field.useName() + useNull(field));
          break;
        case Field.TIME:
          writeln(1, "q_.BindTimeArray(" + i + ", " + field.useName() + useNull(field));
          break;
        case Field.DATETIME:
        case Field.TIMESTAMP:
          writeln(1, "q_.BindDateTimeArray(" + i + ", " + field.useName() + useNull(field));
          break;
        case Field.AUTOTIMESTAMP:
          writeln(1, "//q_.BindDateTimeArray(" + i + ", " + field.useName() + useNull(field));
          break;
      }
    }
    writeln(1, "q_.Exec();");
    writeln("}");
    writeln();
  }
  /**
   * Emits class method for processing the database activity
   */
  static boolean isIdentity(Field field)
  {
    return field.type == Field.BIGIDENTITY || field.type == Field.IDENTITY;
  }
  static boolean isSequence(Field field)
  {
    return field.type == Field.BIGSEQUENCE || field.type == Field.SEQUENCE;
  }
  static void generateImplementation(Table table, Proc proc)
  {
    placeHolder = new PlaceHolder(proc, PlaceHolder.QUESTION, "");
    String fullName = table.useName() + proc.upperFirst();
    writeln("void T" + fullName + "::Exec()");
    writeln("{");
    generateCommand(proc);
    if (proc.outputs.size() > 0)
      writeln(1, "q_.Open(q_.command, NOBINDS, NODEFINES, NOROWS, ROWSIZE);");
    else if (proc.inputs.size() > 0)
      writeln(1, "q_.Open(q_.command, " + proc.inputs.size() + ");");
    else
      writeln(1, "q_.Open(q_.command);");
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      generateCppBind(field);
    }
    Vector blobs = new Vector();
    for (int j = 0; j < placeHolder.pairs.size(); j++)
    {
      PlaceHolderPairs pair = placeHolder.pairs.elementAt(j);
      Field field = pair.field;
      String tablename = table.tableName();
      String bind = "Bind";
      if (field.type == Field.BLOB) bind += "Blob";
      //else if (field.type == Field.BIGXML) bind += "BigXML";
      writeln(1, "q_." + bind + "(" + padder("" + j + ",", 4) + cppBind(field, tablename, proc.isInsert) + padder(", " + cppDirection(field), 4) + ((isNull(field)) ? ", &" + field.useName() + "IsNull" : "") + charFieldFlag(field) + ");");
      if (field.type == Field.BLOB)
        blobs.addElement(field);
    }
    for (int j = 0; j < proc.outputs.size(); j++)
    {
      Field field = proc.outputs.elementAt(j);
      String define = "Define";
      if (field.type == Field.BLOB) define += "Blob";
      //else if (field.type == Field.BIGXML) define += "BigXML";
      writeln(1, "q_." + define +"(" + padder("" + j + ",", 4) + cppDefine(field) + ");");
    }
    writeln(1, "q_.Exec();");
    for (int j = 0; j < blobs.size(); j++)
    {
      Field field = (Field)blobs.elementAt(j);
      writeln(1, "SwapBytes(" + field.useName() + ".len); // fixup len in data on intel type boxes");
    }
    writeln("}");
    writeln();
    boolean skipExecWithParms = false;
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if (field.type == Field.BLOB)// || field.type == Field.BIGXML)
      {
        skipExecWithParms = true;
        break;
      }
    }
    if (skipExecWithParms == false)
      if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
      {
        writeln("void T" + fullName + "::Exec(");
        generateWithParms(proc, "");
        writeln(")");
        writeln("{");
        for (int j = 0; j < proc.inputs.size(); j++)
        {
          Field field = proc.inputs.elementAt(j);
          if ((isSequence(field) && proc.isInsert)
          || isIdentity(field)
          || field.type == Field.TIMESTAMP
          || field.type == Field.AUTOTIMESTAMP
          || field.type == Field.USERSTAMP)
            continue;
          writeln(1, "" + cppCopy(field));
        }
        for (int j = 0; j < proc.dynamics.size(); j++)
        {
          String s = proc.dynamics.elementAt(j);
          writeln(1, "strncpy(" + s + ", a" + s + ", sizeof(" + s + ")-1);");
        }
        writeln(1, "Exec();");
        writeln("}");
        writeln();
      }
    if (proc.outputs.size() > 0)
    {
      writeln("bool T" + fullName + "::Fetch()");
      writeln("{");
      writeln(1, "if (q_.Fetch() == false)");
      writeln(2, "return false;");
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        writeln(1, "q_.Get(" + cppGet(field) + ");");
        if (isNull(field))
          writeln(1, "q_.GetNull(" + field.useName() + "IsNull, " + j + ");");
      }
      writeln(1, "return true;");
      writeln("}");
      writeln();
    }
  }
  static String check(String value)
  {
    return value;
  }
  static void generateCommand(Proc proc)
  {
    boolean isReturning = false;
    boolean isBulkSequence = false;
    String front = "", back = "", sequencer = "";
    Vector lines = placeHolder.getLines();
    int size = 1;
    if (proc.isInsert == true && proc.hasReturning == true && proc.outputs.size() == 1)
    {
      Field field = proc.outputs.elementAt(0);
      if (field.isSequence == true)
      {
        isReturning = true;
        front = "select " + field.useName() + " from new table(";
        back = ")";
        sequencer = "nextval for " + proc.table.tableName() + "seq";
        size += front.length();
        size += back.length();
        size += sequencer.length();
      }
    }
    if (proc.isMultipleInput == true && proc.isInsert == true)
    {
      isBulkSequence = true;
      sequencer = "nextval for " + proc.table.tableName() + "seq";
      size += sequencer.length();
    }
    for (int i = 0; i < lines.size(); i++)
    {
      String l = (String)lines.elementAt(i);
      if (l.charAt(0) == '"')
        size += (l.length() + 2);
      else
      {
        String var = l.trim();
        for (int j = 0; j < proc.dynamics.size(); j++)
        {
          String s = proc.dynamics.elementAt(j);
          if (var.compareTo(s) == 0)
          {
            Integer n = proc.dynamicSizes.elementAt(j);
            size += (n.intValue() + 2);
          }
        }
      }
    }
    writeln(1, "if (q_.command == 0)");
    writeln(2, "q_.command = new char [" + size + "];");
    writeln(1, "memset(q_.command, 0, " + size + ");");
    if (isReturning == true || isBulkSequence == true)
    {
      writeln(1, "struct cpp_ret");
      writeln(1, "{");
      writeln(2, "char* head; char *output; char *sequence; char* tail;");
      writeln(2, "cpp_ret()");
      writeln(2, "{");
      writeln(3, "head = output = sequence = tail = \"\";");
      writeln(2, "}");
      writeln(2, "char* checkUse(char* data)");
      writeln(2, "{");
      writeln(3, "return data;");
      writeln(2, "}");
      writeln(1, "} _ret;");
      writeln(1, "_ret.sequence = \"" + sequencer + ",\";");
    }
    if (isReturning == true)
    {
      writeln(1, "_ret.head = \"" + front + "\";");
      writeln(1, "_ret.tail = \"" + back + "\";");
    }
    String strcat = "strcat(q_.command, ";
    String terminate = "";
    if (lines.size() > 0)
    {
      for (int i = 0; i < lines.size(); i++)
      {
        String l = (String)lines.elementAt(i);
        if (l.charAt(0) != '"')
        {
          terminate = ");";
          strcat = "strcat(q_.command, ";
          if (i != 0)
            writeln(terminate);
        }
        else if (i != 0)
          writeln(terminate);
        if (l.charAt(0) != '"')
          write(1, strcat + check(l));
        else
          write(1, strcat + l);
        if (l.charAt(0) == '"')
        {
          terminate = "\"\\n\"";
          strcat = "    ";
        }
      }
      writeln(");");
    }
  }
  /**
   * generate Holding variables
   */
  static void generateCppBind(Field field)
  {
    switch (field.type)
    {
      case Field.DATE:
        writeln(1, "DATE_STRUCT " + field.useName() + "_CLIDate;");
        break;
      case Field.TIME:
        writeln(1, "TIME_STRUCT " + field.useName() + "_CLITime;");
        break;
      case Field.DATETIME:
        writeln(1, "TIMESTAMP_STRUCT " + field.useName() + "_CLIDateTime;");
        break;
      case Field.TIMESTAMP:
        writeln(1, "TIMESTAMP_STRUCT " + field.useName() + "_CLITimeStamp;");
        break;
      case Field.AUTOTIMESTAMP:
        writeln(1, "//TIMESTAMP_STRUCT " + field.useName() + "_CLITimeStamp;");
        break;
    }
  }
  static void generateWithParms(Proc proc, String pad)
  {
    String comma = "  ";
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if ((isSequence(field) && proc.isInsert) || isIdentity(field)
        || field.type == Field.TIMESTAMP || field.type == Field.AUTOTIMESTAMP || field.type == Field.USERSTAMP)
        continue;
      writeln(pad + comma + "const " + cppParm(field));
      comma = ", ";
    }
    for (int j = 0; j < proc.dynamics.size(); j++)
    {
      String s = proc.dynamics.elementAt(j);
      writeln(pad + comma + "const char*   a" + s);
      comma = ", ";
    }
  }
  static void generateInterface(Table table, Proc proc, String dataStruct)
  {
    placeHolder = new PlaceHolder(proc, PlaceHolder.QUESTION, "");
    String front = "  { ";
    boolean standardExec = true;
    if (proc.outputs.size() > 0)
    {
      writeln(1, "enum");
      Field field = proc.outputs.elementAt(0);
      String thisOne = field.useName().toUpperCase() + "_OFFSET";
      String lastOne = thisOne;
      String lastSize = cppLength(field);
      writeln(front + padder(thisOne, 24) + "= 0");
      front = "  , ";
      for (int j = 1; j < proc.outputs.size(); j++)
      {
        field = proc.outputs.elementAt(j);
        thisOne = field.useName().toUpperCase() + "_OFFSET";
        writeln(1, ", " + padder(thisOne, 24) + "= (" + lastOne + "+" + lastSize + ")");
        lastOne = thisOne;
        lastSize = cppLength(field);
      }
      writeln(1, ", " + padder("ROWSIZE", 24) + "= (" + lastOne + "+" + lastSize + ")");
      if (proc.isSingle)
        writeln(1, ", " + padder("NOROWS", 24) + "= 1");
      else if (proc.noRows > 0)
        writeln(1, ", " + padder("NOROWS", 24) + "= " + proc.noRows);
      else
        writeln(1, ", " + padder("NOROWS", 24) + "= (24*1024 / ROWSIZE) + 1");
      writeln(1, ", " + padder("NOBINDS", 24) + "= " + placeHolder.pairs.size());
      writeln(1, ", " + padder("NODEFINES", 24) + "= " + proc.outputs.size());
      field = proc.outputs.elementAt(0);
      thisOne = field.useName().toUpperCase();
      writeln(1, ", " + padder(thisOne + "_POS", 24) + "= 0");
      for (int j = 1; j < proc.outputs.size(); j++)
      {
        field = proc.outputs.elementAt(j);
        thisOne = field.useName().toUpperCase();
        writeln(1, ", " + padder(thisOne + "_POS", 24) + "= " + padder(thisOne + "_OFFSET", 24) + "* NOROWS");
      }
      writeln(1, "};");
    }
    else if (proc.isMultipleInput)
    {
      int noNulls = 0;
      standardExec = false;
      writeln(1, "enum");
      Field field = proc.inputs.elementAt(0);
      if (isNull(field) || (field.type == Field.CHAR && field.isNull == true) || (field.type == Field.ANSICHAR && field.isNull == true))
        noNulls++;
      String thisOne = field.useName().toUpperCase() + "_OFFSET";
      String lastOne = thisOne;
      String thisSize = field.useName().toUpperCase() + "_SIZE";
      String lastSize = thisSize;
      writeln(front + padder(thisOne, 24) + "= 0");
      front = "  , ";
      writeln(front + padder(thisSize, 24) + "= " + cppLength(field));
      for (int j = 1; j < proc.inputs.size(); j++)
      {
        field = proc.inputs.elementAt(j);
        if (isNull(field) || (field.type == Field.CHAR && field.isNull == true) || (field.type == Field.ANSICHAR && field.isNull == true))
          noNulls++;
        thisOne = field.useName().toUpperCase() + "_OFFSET";
        thisSize = field.useName().toUpperCase() + "_SIZE";
        writeln(1, ", " + padder(thisOne, 24) + "= (" + lastOne + "+" + lastSize + ")");
        writeln(1, ", " + padder(thisSize, 24) + "= " + cppLength(field));
        lastOne = thisOne;
        lastSize = thisSize;
      }
      writeln(1, ", " + padder("ROWSIZE", 24) + "= (" + lastOne + "+" + lastSize + ")");
      writeln(1, ", " + padder("NOBINDS", 24) + "= " + placeHolder.pairs.size());
      writeln(1, ", " + padder("NONULLS", 24) + "= " + noNulls);
      writeln(1, "};");
      writeln(1, "void Exec(int32 noOf, " + dataStruct + "* Recs);");
    }
    writeln(1, "TJQuery q_;");
    if (standardExec == true)
    {
      writeln(1, "void Exec();");
      writeln(1, "void Exec(" + dataStruct + "& Rec) {*DRec() = Rec;Exec();}");
      boolean skipExecWithParms = false;
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        if (field.type == Field.BLOB)// || field.type == Field.BIGXML)
        {
          skipExecWithParms = true;
          break;
        }
      }
      if (skipExecWithParms == false)
        if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
        {
          writeln(1, "void Exec(");
          generateWithParms(proc, "  ");
          writeln(1, ");");
        }
    }
    if (proc.outputs.size() > 0)
      writeln(1, "bool Fetch();");
    writeln(1, "T" + table.useName() + proc.upperFirst() + "(TJConnector &conn, char *aFile=__FILE__, long aLine=__LINE__)");
    writeln(1, ": q_(conn)");
    writeln(1, "{Clear();q_.FileAndLine(aFile,aLine);}");
    writeln(1, "" + dataStruct + "* DRec() {return this;}");
    if (proc.outputs.size() > 0)
      writeln(1, "O" + dataStruct.substring(1) + "* ORec() {return this;}");
    if (proc.isStdExtended() == false && proc.extendsStd == true)
    {
      writeln(1, "D" + table.useName() + "* DStd() {return (D" + table.useName() + "*)this;}");
      if (proc.outputs.size() > 0)
        writeln(1, "O" + table.useName() + "* OStd() {return (O" + table.useName() + "*)this;}");
    }
  }
  static String padder(String s, int length)
  {
    for (int i = s.length(); i < length - 1; i++)
      s = s + " ";
    return s + " ";
  }
  public static void generateEnumOrdinals(Table table)
  {
    for (int i = 0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      if (field.enums.size() > 0)
      {
        writeln("enum e" + table.useName() + field.useName());
        String start = "{";
        for (int j = 0; j < field.enums.size(); j++)
        {
          bbd.jportal.Enum element = field.enums.elementAt(j);
          String evalue = "" + element.value;
          if (field.type == Field.ANSICHAR && field.length == 1)
            evalue = "'" + (char)element.value + "'";
          writeln(start + " " + table.useName() + field.useName() + element.name + " = " + evalue);
          start = ",";
        }
        writeln("};");
        writeln();
        writeln("inline char *" + table.useName() + field.useName() + "Lookup(int no)");
        writeln("{");
        writeln(1, "switch(no)");
        writeln(1, "{");
        for (int j = 0; j < field.enums.size(); j++)
        {
          bbd.jportal.Enum element = field.enums.elementAt(j);
          String evalue = "" + element.value;
          if (field.type == Field.ANSICHAR && field.length == 1)
            evalue = "'" + (char)element.value + "'";
          writeln(1, "case " + evalue + ": return \"" + element.name + "\";");
        }
        writeln(1, "default: return \"<unknown value>\";");
        writeln(1, "}");
        writeln("}");
        writeln();
      }
      else if (field.valueList.size() > 0)
      {
        writeln("enum e" + table.useName() + field.useName());
        String start = "{";
        for (int j = 0; j < field.valueList.size(); j++)
        {
          String element = field.valueList.elementAt(j);
          writeln(start + " " + table.useName() + field.useName() + element);
          start = ",";
        }
        writeln("};");
        writeln();
        writeln("inline const char *" + table.useName() + field.useName() + "Lookup(int no)");
        writeln("{");
        writeln(1, "switch(no)");
        writeln(1, "{");
        for (int j = 0; j < field.valueList.size(); j++)
        {
          String element = field.valueList.elementAt(j);
          writeln(1, "case " + j + ": return \"" + element + "\";");
        }
        writeln(1, "default: return \"<unknown value>\";");
        writeln(1, "}");
        writeln("}");
        writeln();
      }
    }
  }
  static String fileName(String output, String node, String ext)
  {
    return output + node + ext;
  }
  private static String charPadding(int no, int fillerNo)
  {
    int n = 8 - (no % 8);
    if (n != 8)
      return "IDL2_CHAR_PAD(" + fillerNo + "," + n + ");";
    return "";
  }
  private static String generatePadding(Field field, int fillerNo)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
        return "IDL2_INT16_PAD(" + fillerNo + ");";
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return "IDL2_INT32_PAD(" + fillerNo + ");";
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.USERSTAMP:
      case Field.XML:
      case Field.TLOB:
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return charPadding(field.length + 1, fillerNo);
      case Field.DOUBLE:
      case Field.FLOAT:
        if (field.precision > 15)
          return charPadding(field.precision + 3, fillerNo);
        break;
      case Field.MONEY:
        return charPadding(21, fillerNo);
      //case Field.BIGXML:
      //  break;
    }
    return "";
  }
  public static String generatePadding(int fillerNo)
  {
    return "IDL2_INT16_PAD(" + fillerNo + ");";
  }
  static int getLength(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
        return 2;
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return 4;
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return 8;
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.USERSTAMP:
      case Field.TLOB:
      case Field.XML:
        return field.length + 1;
      //case Field.BIGXML:
      case Field.BLOB:
        return 8;
      case Field.DATE:
        return 9;
      case Field.TIME:
        return 7;
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return 15;
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return field.precision + 3; // allow for - . and null terminator
        return 8;
      case Field.MONEY:
        return 21;
    }
    return 4;
  }
  static String charFieldFlag(Field field)
  {
    if (field.type != Field.CHAR && field.type != Field.ANSICHAR && field.type != Field.TLOB && field.type != Field.XML)
      return "";
    if ((field.type == Field.CHAR || field.type == Field.TLOB || field.type == Field.XML) && field.isNull == true)
      return ", 0, 1";
    if (field.type == Field.ANSICHAR)
      if (field.isNull == true)
        return ", 1, 1";
      else
        return ", 1, 0";
    return ", 0, 0";
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
      case Field.AUTOTIMESTAMP:
      case Field.TIME:
      //case Field.XML:
        return true;
    }
    return false;
  }
  static boolean notString(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.LONG:
      case Field.IDENTITY:
      case Field.SEQUENCE:
      case Field.BIGIDENTITY:
      case Field.BIGSEQUENCE:
      case Field.BLOB:
        return true;
      case Field.FLOAT:
      case Field.DOUBLE:
        return field.precision <= 15;
    }
    return false;
  }
  static boolean isStruct(Field field)
  {
    return field.type == Field.BLOB;
  }
  static boolean isLob(Field field)
  {
    return field.type == Field.BLOB;
  }
  static String cppInit(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
      case Field.LONG:
        return field.useName() + " = 0;";
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.TLOB:
      case Field.XML:
      case Field.USERSTAMP:
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return "memset(" + field.useName() + ", 0, sizeof(" + field.useName() + "));";
      case Field.BLOB:
        return "memset(&" + field.useName() + ", 0, sizeof(" + field.useName() + "));";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision <= 15)
          return field.useName() + " = 0.0;";
        return "memset(" + field.useName() + ", 0, sizeof(" + field.useName() + "));";
      case Field.MONEY:
        return "memset(" + field.useName() + ", 0, sizeof(" + field.useName() + "));";
    }
    return field.useName() + " <unsupported>";
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppVar(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
        return "int16  " + field.useName();
      case Field.INT:
      case Field.IDENTITY:
      case Field.SEQUENCE:
        return "int32  " + field.useName();
      case Field.LONG:
      case Field.BIGIDENTITY:
      case Field.BIGSEQUENCE:
        return "int64  " + field.useName();
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.TLOB:
      case Field.XML:
        return "char   " + field.useName() + "[" + (field.length + 1) + "]";
      case Field.USERSTAMP:
        return "char   " + field.useName() + "[9]";
      case Field.BLOB:
        return "TJBlob<" + field.length + "> " + field.useName();
      case Field.DATE:
        return "char   " + field.useName() + "[9]";
      case Field.TIME:
        return "char   " + field.useName() + "[7]";
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return "char   " + field.useName() + "[15]";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "char   " + field.useName() + "[" + (field.precision + 3) + "]";
        return "double " + field.useName();
      case Field.MONEY:
        return "char   " + field.useName() + "[21]";
    }
    return field.useName() + " <unsupported>";
  }
  static String cppLength(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
        return "sizeof(int16)";
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return "sizeof(int32)";
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return "sizeof(int64)";
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.TLOB:
      case Field.XML:
        return "" + (field.length + 1);
      case Field.BLOB:
        return "sizeof(TJBlob<" + field.length + ">)";
      //case Field.BIGXML:
      //  return "sizeof(TJBigXML)";
      case Field.USERSTAMP:
        return "9";
      case Field.DATE:
        return "sizeof(DATE_STRUCT)";
      case Field.TIME:
        return "sizeof(TIME_STRUCT)";
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return "sizeof(TIMESTAMP_STRUCT)";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "" + (field.precision + 3);
        return "sizeof(double)";
      case Field.MONEY:
        return "21";
    }
    return "0";
  }
  static String cppDirection(Field field)
  {
    if (field.isIn && field.isOut)
      return "SQL_PARAM_INPUT_OUTPUT";
    if (field.isOut)
      return "SQL_PARAM_OUTPUT";
    return "SQL_PARAM_INPUT";
  }
  static String cppArrayPointer(Field field)
  {
    String offset = field.useName().toUpperCase() + "_OFFSET";
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
        return "int16 *" + field.useName() + " = (int16 *)(q_.data + " + offset + " * noOf);";
      case Field.INT:
        return "int32 *" + field.useName() + " = (int32 *)(q_.data + " + offset + " * noOf);";
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return "int64 *" + field.useName() + " = (int64 *)(q_.data + " + offset + " * noOf);";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "char *" + field.useName() + " = (char *)(q_.data + " + offset + " * noOf);";
        return "double *" + field.useName() + " = (double *)(q_.data + " + offset + " * noOf);";
      case Field.MONEY:
        return "char *" + field.useName() + " = (char *)(q_.data + " + offset + " * noOf);";
      case Field.SEQUENCE:
        return "int32 *" + field.useName() + " = (int32 *)(q_.data + " + offset + " * noOf);";
      case Field.TLOB:
      case Field.XML:
      case Field.CHAR:
      case Field.ANSICHAR:
        return "char *" + field.useName() + " = (char *)(q_.data + " + offset + " * noOf);";
      case Field.USERSTAMP:
        return "char *" + field.useName() + " = (char *)(q_.data + " + offset + " * noOf);";
      case Field.DATE:
        return "DATE_STRUCT* " + field.useName() + " = (DATE_STRUCT *)(q_.data + " + offset + " * noOf);";
      case Field.TIME:
        return "TIME_STRUCT* " + field.useName() + " = (TIME_STRUCT *)(q_.data + " + offset + " * noOf);";
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return "TIMESTAMP_STRUCT* " + field.useName() + " = (TIMESTAMP_STRUCT *)(q_.data + " + offset + " * noOf);";
      case Field.BLOB:
        return "// Blobs are not handled here";
      //case Field.BIGXML:
      //  return "// BigXMLs are not handled here";
    }
    return "// not handled here";
  }
  static String cppBind(Field field, String tableName, boolean isInsert)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.LONG:
        return field.useName();
      case Field.FLOAT:
      case Field.DOUBLE:
        return field.useName() + ", " + (field.precision) + ", " + (field.scale);
      case Field.MONEY:
        return field.useName() + ", 18, 2";
      case Field.SEQUENCE:
      case Field.BIGSEQUENCE:
        if (isInsert)
          return "q_.Sequence(" + field.useName() + ", \"" + tableName + "Seq\")";
        else
          return field.useName();
      case Field.TLOB:
      case Field.XML:
        return field.useName() + ", " + (field.length);
      case Field.CHAR:
        return field.useName() + ", " + (field.length);
      case Field.ANSICHAR:
        return field.useName() + ", " + (field.length);
      case Field.USERSTAMP:
        return "q_.UserStamp(" + field.useName() + "), 8";
      case Field.DATE:
        return "q_.Date(" + field.useName() + "_CLIDate, " + field.useName() + ")";
      case Field.TIME:
        return "q_.Time(" + field.useName() + "_CLITime, " + field.useName() + ")";
      case Field.DATETIME:
        return "q_.DateTime(" + field.useName() + "_CLIDateTime, " + field.useName() + ")";
      case Field.TIMESTAMP:
        return "q_.TimeStamp(" + field.useName() + "_CLITimeStamp, " + field.useName() + ")";
      case Field.AUTOTIMESTAMP:
        return "q_.TimeStamp(" + field.useName() + "_CLITimeStamp, " + field.useName() + ")";
      case Field.BLOB:
        return "(char*)&" + field.useName() + ", sizeof(" + field.useName() + ".data)";
      //case Field.BIGXML:
      //  return field.useName()+".data, " + field.useName( )+ ".length";
    }
    return field.useName() + ", <unsupported>";
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppDefine(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
        return "(int16*) (q_.data+" + field.useName().toUpperCase() + "_POS)";
      case Field.INT:
      case Field.IDENTITY:
      case Field.SEQUENCE:
        return "(int32*) (q_.data+" + field.useName().toUpperCase() + "_POS)";
      case Field.LONG:
      case Field.BIGIDENTITY:
      case Field.BIGSEQUENCE:
        return "(int64*) (q_.data+" + field.useName().toUpperCase() + "_POS)";
      case Field.CHAR:
      case Field.TLOB:
      case Field.XML:
        return "(char*)  (q_.data+" + field.useName().toUpperCase() + "_POS), " + (field.length + 1);
      case Field.ANSICHAR:
        return "(char*)  (q_.data+" + field.useName().toUpperCase() + "_POS), " + (field.length + 1) + ", 1";
      case Field.USERSTAMP:
        return "(char*)  (q_.data+" + field.useName().toUpperCase() + "_POS), 9";
      case Field.BLOB:
        return "(char*)  (q_.data+" + field.useName().toUpperCase() + "_POS), " + (field.length);
      //case Field.BIGXML:
      //  return "(char*)  (q_.data+" + field.useName().toUpperCase() + "_POS), sizeof(" + field.useName() + ")";
      case Field.DATE:
        return "(DATE_STRUCT*)(q_.data+" + field.useName().toUpperCase() + "_POS)";
      case Field.TIME:
        return "(TIME_STRUCT*)(q_.data+" + field.useName().toUpperCase() + "_POS)";
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return "(TIMESTAMP_STRUCT*)(q_.data+" + field.useName().toUpperCase() + "_POS)";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "(char*)  (q_.data+" + field.useName().toUpperCase() + "_POS), " + (field.precision + 3);
        return "(double*)(q_.data+" + field.useName().toUpperCase() + "_POS)";
      case Field.MONEY:
        return "(char*)  (q_.data+" + field.useName().toUpperCase() + "_POS), 21";
    }
    return field.useName() + " <unsupported>";
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppGet(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
      case Field.LONG:
        return padder(field.useName() + ",", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return padder(field.useName() + ",", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS, " + (field.precision + 3);
        return padder(field.useName() + ",", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS";
      case Field.MONEY:
        return padder(field.useName() + ",", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS, 21";
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.TLOB:
      case Field.XML:
        return padder(field.useName() + ",", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS, " + (field.length + 1);
      case Field.USERSTAMP:
        return padder(field.useName() + ",", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS, 9";
      case Field.BLOB:
        return padder(field.useName() + ".len, " + field.useName() + ".data,", 32) +
            " q_.data+" + field.useName().toUpperCase() + "_POS, sizeof(" + field.useName() + ")";
      //case Field.BIGXML:
      //  return field.useName() + ".setBigXML(" + field.useName().toUpperCase() + "_POS, " + field.length + ")";
      case Field.DATE:
        return padder("TJDate(" + field.useName() + "),", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS";
      case Field.TIME:
        return padder("TJTime(" + field.useName() + "),", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS";
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return padder("TJDateTime(" + field.useName() + "),", 32) + " q_.data+" + field.useName().toUpperCase() + "_POS";
    }
    return field.useName() + " <unsupported>";
  }
  static String cppCopy(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.LONG:
      case Field.SEQUENCE:
      case Field.BIGSEQUENCE:
        return field.useName() + " = a" + field.useName() + ";";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "strncpy(" + field.useName() + ", a" + field.useName() + ", sizeof(" + field.useName() + ")-1);";
        return field.useName() + " = a" + field.useName() + ";";
      case Field.MONEY:
        return "strncpy(" + field.useName() + ", a" + field.useName() + ", sizeof(" + field.useName() + ")-1);";
      case Field.CHAR:
      case Field.TLOB:
      case Field.XML:
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
        return "strncpy(" + field.useName() + ", a" + field.useName() + ", sizeof(" + field.useName() + ")-1);";
      case Field.ANSICHAR:
        return "memcpy(" + field.useName() + ", a" + field.useName() + ", sizeof(" + field.useName() + "));";
      case Field.BLOB:
        return field.useName() + " = a" + field.useName() + ";";
      case Field.USERSTAMP:
      case Field.IDENTITY:
      case Field.TIMESTAMP:
        return "// " + field.useName() + " -- generated";
      case Field.AUTOTIMESTAMP:
        return "// " + field.useName() + " -- generated";
    }
    return field.useName() + " <unsupported>";
  }
  static String cppArrayCopy(Field field)
  {
    String size = field.useName().toUpperCase() + "_SIZE";
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.LONG:
      case Field.SEQUENCE:
      //case Field.IDENTITY:
      case Field.BIGSEQUENCE:
        //case Field.BIGIDENTITY:
        return field.useName() + "[i] = Recs[i]." + field.useName() + ";";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "strncpy(&" + field.useName() + "[i*" + size + "], Recs[i]." + field.useName() + ", " + size + "-1);";
        return field.useName() + "[i] = Recs[i]." + field.useName() + ";";
      case Field.MONEY:
        return "strncpy(&" + field.useName() + "[i*" + size + "], Recs[i]." + field.useName() + ", " + size + "-1);";
      case Field.CHAR:
      case Field.TLOB:
      case Field.XML:
        return "strncpy(&" + field.useName() + "[i*" + size + "], Recs[i]." + field.useName() + ", " + size + "-1);";
      case Field.DATE:
        return "q_.Date(" + field.useName() + "[i], Recs[i]." + field.useName() + ");";
      case Field.TIME:
        return "q_.Time(" + field.useName() + "[i], Recs[i]." + field.useName() + ");";
      case Field.DATETIME:
        return "q_.DateTime(" + field.useName() + "[i], Recs[i]." + field.useName() + ");";
      case Field.ANSICHAR:
        return "memcpy(&" + field.useName() + "[i*" + size + "], a" + field.useName() + ", " + size + ");";
      case Field.BLOB:
        return field.useName() + "[i] = Recs[i]." + field.useName() + ";";
      case Field.USERSTAMP:
        return field.useName() + " -- generated";
      case Field.IDENTITY:
        return field.useName() + " -- generated";
      case Field.TIMESTAMP:
        return "q_.TimeStamp(" + field.useName() + "[i], Recs[i]." + field.useName() + ");";
      case Field.AUTOTIMESTAMP:
        return "// " + field.useName() + " -- generated";
    }
    return field.useName() + " <unsupported>";
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppParm(Field field)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
        return "int16  a" + field.useName();
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return "int32   a" + field.useName();
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return "int64  a" + field.useName();
      case Field.CHAR:
      case Field.TLOB:
      case Field.XML:
      case Field.ANSICHAR:
        return "char*  a" + field.useName();
      case Field.USERSTAMP:
        return "char*  a" + field.useName();
      case Field.DATE:
        return "char*  a" + field.useName();
      case Field.TIME:
        return "char*  a" + field.useName();
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return "char*  a" + field.useName();
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return "char*  a" + field.useName();
        return "double a" + field.useName();
      case Field.MONEY:
        return "char*  a" + field.useName();
    }
    return field.useName() + " <unsupported>";
  }
  static String fromXMLFormat(Field field)
  {
    String front = "";
    if (isNull(field))
      front = "if (strlen(work.data) == 0) " + field.useName() + "IsNull = true; else ";
    switch (field.type)
    {
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.USERSTAMP:
      case Field.TLOB:
      //case Field.XML: (xml is xml is bizarro)
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return front+"memcpy(" + field.useName() + ", work.data, sizeof(" + field.useName() + ")-1);";
      case Field.BOOLEAN:
      case Field.BYTE:
        return front + field.useName() + " = (int8)atol(work.data);";
      case Field.SHORT:
        return front + field.useName() + " = (int16)atol(work.data);";
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return front + field.useName() + " = (int32)atol(work.data);";
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return front + field.useName() + " = (int64)atoint64(work.data);";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return front + "memcpy(" + field.useName() + ", work.data, sizeof(" + field.useName() + ")-1);";
        return front + field.useName() + " = atof(work.data);";
      case Field.MONEY:
        return front + "memcpy(" + field.useName() + ", work.data, sizeof(" + field.useName() + ")-1);";
    }
    return "// " + field.useName() + " <unsupported>";
  }
  static String toXMLFormat(Field field)
  {
    String front = "XRec.append(\"  <" + field.useName() + ">\");";
    String back = "XRec.append(\"</" + field.useName() + ">\");";
    if (isNull(field))
      front += "if (" + field.useName() + "IsNull == false) ";
    switch (field.type)
    {
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.USERSTAMP:
      case Field.TLOB:
      //case Field.XML: (xml is xml is bizarro)
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.AUTOTIMESTAMP:
        return front + "XRec.ampappend(" + field.useName() + ");" + back;
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.SEQUENCE:
      case Field.IDENTITY:
        return front + "XRec.ampappend(JP_XML_FORMAT((int32)" + field.useName() + ").result);" + back;
      case Field.LONG:
      case Field.BIGSEQUENCE:
      case Field.BIGIDENTITY:
        return front + "XRec.ampappend(JP_XML_FORMAT((int64)" + field.useName() + ").result);" + back;
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return front + "XRec.ampappend(" + field.useName() + ");" + back;
        return front + "XRec.ampappend(JP_XML_FORMAT((double)" + field.useName() + ").result);" + back;
      case Field.MONEY:
        return front + "XRec.ampappend(" + field.useName() + ");" + back;
    }
    return "// " + field.useName() + " <unsupported>";
  }
}
