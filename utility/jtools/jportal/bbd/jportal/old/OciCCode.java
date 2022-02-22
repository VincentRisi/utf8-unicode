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

public class OciCCode extends Generator
{
  public static String description()
  {
    return "Generate OCI C++ Code";
  }
  public static String documentation()
  {
    return "Generate OCI C++ Code";
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
  protected static Vector flagsVector;
  static boolean          aix;
  static boolean          lowercase;
  static boolean          xmlValue;
  private static void flagDefaults()
  {
    aix = false;
    lowercase = false;
    xmlValue = false;
  }
  public static Vector flags()
  {
    if (flagsVector == null)
    {
      flagsVector = new Vector();
      flagDefaults();
      flagsVector.addElement(new Flag("aix", aix,
          "Generate for AIX"));
      flagsVector.addElement(new Flag("lowercase", lowercase,
          "Generate lowercase"));
      flagsVector.addElement(new Flag("xmlValue", xmlValue,
          "Generate lowercase"));
  }
    return flagsVector;
  }
  static void setFlags(Database database, PrintWriter outLog)
  {
    if (flagsVector != null)
    {
      aix = toBoolean(flagsVector.elementAt(0));
      lowercase = toBoolean(flagsVector.elementAt(1));
    } else
      flagDefaults();
    for (int i = 0; i < database.flags.size(); i++)
    {
      String flag = database.flags.elementAt(i);
      if (flag.equalsIgnoreCase("lowercase"))
        lowercase = true;
      else if (flag.equalsIgnoreCase("aix"))
        aix = true;
      else if (flag.equalsIgnoreCase("xmlValue"))
        xmlValue = true;
    }
    if (xmlValue)
      outLog.println(" (xmlValue)");
    if (lowercase)
      outLog.println(" (lowercase)");
    if (aix)
      outLog.println(" (aix)");
  }
  /**
   * Generates the procedure classes for each table present.
   */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    setFlags(database, outLog);
    for (int i = 0; i < database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      generate(table, output, outLog);
    }
  }
  static String fileName(String output, String node, String ext)
  {
    int p = output.indexOf('\\');
    if (p == -1 && aix == true && ext.equals(".cpp"))
      ext = ".C";
    if (lowercase == true)
      node = node.toLowerCase();
    return output + node + ext;
  }
  /**
   * Build of standard and user defined procedures
   */
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
        writeln("inline const char *" + table.useName() + field.useName() + "Lookup(int no)");
        writeln("{");
        writeln("  switch(no)");
        writeln("  {");
        for (int j = 0; j < field.enums.size(); j++)
        {
          bbd.jportal.Enum element = field.enums.elementAt(j);
          String evalue = "" + element.value;
          if (field.type == Field.ANSICHAR && field.length == 1)
            evalue = "'" + (char)element.value + "'";
          writeln("  case " + evalue + ": return \"" + element.name + "\";");
        }
        writeln("  default: return \"<unknown value>\";");
        writeln("  }");
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
        for (int j = 0; j < field.valueList.size(); j++)
        {
          String element = field.valueList.elementAt(j);
          writeln(format("static const char* %s_%s_%s = \"%s\";", table.useName().toUpperCase(), field.useName().toUpperCase(), element, element));
        }
        writeln();
        writeln("inline const char *" + table.useName() + field.useName() + "Lookup(int no)");
        writeln("{");
        writeln("  switch(no)");
        writeln("  {");
        for (int j = 0; j < field.valueList.size(); j++)
        {
          String element = field.valueList.elementAt(j);
          writeln("  case " + j + ": return \"" + element + "\";");
        }
        writeln("  default: return \"<unknown value>\";");
        writeln("  }");
        writeln("}");
        writeln();
      }
    }
  }
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
          writeln("#include \"ociapi.h\"");
          writeln();
          generateEnumOrdinals(table);
          if (table.hasStdProcs)
            generateStdOutputRec(table);
          generateUserOutputRecs(table);
          generateInterface(table);
          writeln("#endif");
        } finally
        {
          writer.flush();
        }
        outFile.close();
        outLog.println("Code: " + fileName(output, table.useName().toLowerCase(), ".cpp"));
        outFile = new FileOutputStream(
            fileName(output, table.useName().toLowerCase(), ".cpp"));
        writer = new PrintWriter(outFile);
        indent_size = 4;
        try
        {
          writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
          writeln();
          writeln("#include \"" + fileName("", table.useName().toLowerCase(), ".sh")
              + "\"");
          writeln();
          generateImplementation(table);
        } finally
        {
          writer.flush();
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
  /**
   * Build of output data rec for standard procedures
   */
  static String structName = "";
  static String initName = "";
  static void generateStdOutputRec(Table table)
  {
    for (int i = 0; i < table.comments.size(); i++)
    {
      String s = table.comments.elementAt(i);
      writeln("//" + s);
    }
    int filler = 0;
    structName = "D" + table.useName();
    initName = format("__D%s__", table.useName());
    writeln("struct D" + table.useName());
    writeln("{");
    boolean canExtend = true;
    Vector fields = table.fields;
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      if (field.type == Field.BLOB)
        canExtend = false;
      if (field.comments.size() > 0)
      {
        for (int c = 0; c < field.comments.size(); c++)
        {
          String s = field.comments.elementAt(c);
          writeln("  //" + s);
        }
      }
      writeln("  " + padder(cppVar(field) + ";", 48)
          + generatePadding(field, filler++));
      if (isNull(field))
      {
        writeln("  "
            + padder("int16  " + field.useName() + "IsNull;", 48)
            + generatePadding(filler++));
        filler++;
      }
    }
    writeln();
    headerSwaps("", fields);
    String useName = table.useName();
    if (canExtend == true)
      extendHeader("", fields, useName);
    else
      extendDataBuildHeader("", fields, useName);
    writeln("};");
    clearTheStruct();
    writeln();
    writeln("typedef D" + table.useName() + " O" + table.useName()
        + ";");
    writeln();
  }
  private static void clearTheStruct()
  {
    writeln(format("static const %s %s;", structName, initName));
    writeln(format("inline void %s::Clear() {*this = %s;}", structName, initName));
  }
  private static void headerSwaps(String baseClass,
      Vector inputs)
  {
    writeln("  void Clear();");
    writeln("  " + structName + "() { Clear(); }");
    writeln("  #ifdef swapbytesH");
    writeln("  void Swaps()");
    writeln("  {");
    if (baseClass.length() > 0)
      writeln("    " + baseClass + "::Swaps();");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field) inputs.elementAt(j);
      if (notString(field) == false)
        continue;
      if (isStruct(field) == false)
        writeln("    SwapBytes(" + field.useName() + ");");
      else
        writeln("    " + field.useName() + ".Swaps();");
      if (isNull(field))
        writeln("    SwapBytes(" + field.useName() + "IsNull);");
    }
    writeln("  }");
    writeln("  #endif");
  }
  private static void extendHeader(String baseClass,
      Vector inputs, String useName)
  {
    writeln("  #if defined(_TBUFFER_H_)");
    writeln("  void _toXML(TBAmp &XRec)");
    writeln("  {");
    if (baseClass.length() > 0)
      writeln("    " + baseClass + "::_toXML(XRec);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field) inputs.elementAt(j);
      writeln("    " + toXMLFormat(field));
    }
    writeln("  }");
    writeln("  void ToXML(TBAmp &XRec, char* Attr=0, char* Outer=\""
        + useName + "\")");
    writeln("  {");
    writeln("    XRec.append(\"<\");XRec.append(Outer);if (Attr) XRec.append(Attr);XRec.append(\">\\n\");");
    writeln("    _toXML(XRec);");
    writeln("    XRec.append(\"</\");XRec.append(Outer);XRec.append(\">\\n\");");
    writeln("  }");
    writeln("  #endif");
    writeln("  #if defined(__XMLRECORD_H__)");
    writeln("  void _fromXML(TBAmp &XRec, TXMLRecord &msg)");
    writeln("  {");
    writeln("    TBAmp work;");
    if (baseClass.length() > 0)
      writeln("    " + baseClass + "::_fromXML(XRec, msg);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field) inputs.elementAt(j);
      if (xmlValue)
        write("    msg.GetValue(\"" + field.useName() + "/value\", work);");
      else
        write("    msg.GetValue(\"" + field.useName() + "\", work);");
      writeln(fromXMLFormat(field));
    }
    writeln("  }");
    writeln("  void FromXML(TBAmp &XRec)");
    writeln("  {");
    writeln("    TXMLRecord msg;");
    writeln("    msg.Load(XRec);");
    //writeln("    memset(this, 0, sizeof(*this));");
    writeln("    Clear();");
    writeln("    _fromXML(XRec, msg);");
    writeln("  }");
    writeln("  #endif");
    extendDataBuildHeader(baseClass, inputs, useName);
  }
  private static void extendDataBuildHeader(String baseClass, Vector inputs, String useName)
  {
    writeln("  #if defined(_DATABUILD_H_)");
    if (baseClass.length() > 0)
      writeln("  static int NoBuildFields() {return " + baseClass + "::NoBuildFields()+" + inputs.size() + ";}");
    else
      writeln("  static int NoBuildFields() {return " + inputs.size() + ";}");
    writeln("  void _buildAdds(DataBuilder &dBuild)");
    writeln("  {");
    if (baseClass.length() > 0)
      writeln("    " + baseClass + "::_buildAdds(dBuild);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field) inputs.elementAt(j);
      if (field.type == Field.BLOB)
        writeln("    dBuild.add(\"" + field.useName() + "\", "
            + field.useName() + ".len, " + field.useName() + ".data);");
      else
        writeln("    dBuild.add(\"" + field.useName() + "\", "
            + field.useName() + ");");

    }
    writeln("  }");
    writeln("  void BuildData(DataBuilder &dBuild, char *name=\""
        + useName + "\")");
    writeln("  {");
    writeln("    dBuild.name(name);");
    writeln("    _buildAdds(dBuild);");
    writeln("  }");
    writeln("  void _buildSets(DataBuilder &dBuild)");
    writeln("  {");
    if (baseClass.length() > 0)
      writeln("    " + baseClass + "::_buildSets(dBuild);");
    for (int j = 0; j < inputs.size(); j++)
    {
      Field field = (Field) inputs.elementAt(j);
      if (field.type == Field.BLOB)
        writeln("    dBuild.set(\"" + field.useName() + "\", "
            + field.useName() + ".len, " + field.useName() + ".data, sizeof("
            + field.useName() + ".data));");
      else
        writeln("    dBuild.set(\"" + field.useName() + "\", "
            + field.useName() + ", sizeof(" + field.useName() + "));");
    }
    writeln("  }");
    writeln("  void SetData(DataBuilder &dBuild, char *name=\""
        + useName + "\")");
    writeln("  {");
    writeln("    dBuild.name(name);");
    writeln("    _buildSets(dBuild);");
    writeln("  }");
    writeln("  #endif");
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
      String work = "";
      String work2 = "";
      boolean canExtend = true;
      Vector outputs = proc.outputs;
      for (int j = 0; j < outputs.size(); j++)
      {
        Field field = (Field) outputs.elementAt(j);
        if (field.type == Field.BLOB)
          canExtend = false;
      }
      Vector inputs = proc.inputs;
      for (int j = 0; j < inputs.size(); j++)
      {
        Field field = (Field) inputs.elementAt(j);
        if (field.type == Field.BLOB)
          canExtend = false;
      }
      if (proc.outputs.size() > 0)
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
        work2 = typeChar + table.useName() + proc.upperFirst();
        structName = typeChar + table.useName() + proc.upperFirst();
        initName = format("__%s%s%s__", typeChar, table.useName(), proc.upperFirst());
        //writeln(format("static %1$s%2$s __%1$s%2$s__;", typeChar, table.useName() + proc.upperFirst()));
        writeln("struct " + typeChar + table.useName()
            + proc.upperFirst());
        writeln("{");
        int filler = 0;
        for (int j = 0; j < outputs.size(); j++)
        {
          Field field = (Field) outputs.elementAt(j);
          for (int c = 0; c < field.comments.size(); c++)
          {
            String s = field.comments.elementAt(c);
            writeln("  //" + s);
          }
          writeln("  " + padder(cppVar(field) + ";", 48)
              + generatePadding(field, filler++));
          if (isNull(field))
            writeln("  "
                + padder("int16  " + field.useName() + "IsNull;", 48)
                + generatePadding(filler++));
        }
        writeln();
        headerSwaps("", outputs);
        String useName = table.useName() + proc.upperFirst();
        if (canExtend == true)
          extendHeader("", outputs, useName);
        else
          extendDataBuildHeader("", outputs, useName);
        writeln("};");
        clearTheStruct();
        writeln();
      }
      if (proc.hasDiscreteInput())
      {
        structName = "D" + table.useName() + proc.upperFirst();
        initName = format("__D%s%s__", table.useName(), proc.upperFirst());
        writeln("struct D" + table.useName() + proc.upperFirst() + work);
        writeln("{");
        int filler = 0;
        for (int j = 0; j < proc.inputs.size(); j++)
        {
          Field field = proc.inputs.elementAt(j);
          if (proc.hasOutput(field.name))
            continue;
          for (int c = 0; c < field.comments.size(); c++)
          {
            String s = field.comments.elementAt(c);
            writeln("  //" + s);
          }
          writeln("  " + padder(cppVar(field) + ";", 48)
              + generatePadding(field, filler++));
          if (isNull(field))
            writeln("  "
                + padder("int16  " + field.useName() + "IsNull;", 48)
                + generatePadding(field, filler++));
        }
        for (int j = 0; j < proc.dynamics.size(); j++)
        {
          String s = proc.dynamics.elementAt(j);
          Integer n = proc.dynamicSizes.elementAt(j);
          writeln("  char " + s + "[" + n.intValue() + "];");
        }
        writeln("  void Clear();");
        writeln("  D" + table.useName() + proc.upperFirst() + "() { Clear(); }");
        extendHeader(work2, proc.inputs, table.useName()
            + proc.upperFirst());
        writeln("};");
        clearTheStruct();
        writeln();
      } else if (proc.outputs.size() > 0)
      {
        writeln("typedef D" + table.useName() + proc.upperFirst()
            + " O" + table.useName() + proc.upperFirst() + ";");
        writeln();
      }
    }
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
    {
      for (int i = 0; i < proc.comments.size(); i++)
      {
        String comment = proc.comments.elementAt(i);
        writeln("  //" + comment);
      }
    }
    if (proc.hasNoData())
    {
      writeln("struct T" + table.useName() + proc.upperFirst());
      writeln("{");
      writeln("  TJQuery q_;");
      writeln("  void Exec();");
      writeln("  T" + table.useName() + proc.upperFirst()
          + "(TJConnector &conn, char *aFile=__FILE__, long aLine=__LINE__)");
      writeln("  : q_(conn)");
      writeln("  {q_.FileAndLine(aFile,aLine);}");
      writeln("};");
      writeln();
    } else
    {
      if (proc.isStd)
        dataStruct = "D" + table.useName();
      else
        dataStruct = "D" + table.useName() + proc.upperFirst();
      writeln("struct T" + table.useName() + proc.upperFirst()
          + " : public " + dataStruct);
      writeln("{");
      if (proc.isMultipleInput)
        generateArrayInterface(table, proc, dataStruct);
      else
        generateInterface(table, proc, dataStruct);
      writeln("};");
      writeln();
    }
  }
  static void generateArrayInterface(Table table, Proc proc, String dataStruct)
  {
    writeln("  enum");
    Field field = proc.inputs.elementAt(0);
    String thisOne = field.useName().toUpperCase() + "_OFFSET";
    String lastOne = thisOne;
    String lastSize = cppLength(field);
    writeln("  { " + padder(thisOne, 24) + "= 0");
    int lobNo = 0;
    for (int j = 1; j < proc.inputs.size(); j++)
    {
      field = proc.inputs.elementAt(j);
      if (isLob(field))
      {
        writeln("  , "
                + padder(field.useName().toUpperCase() + "_LOB", 24) + "= "
                + lobNo);
        writeln("  , "
            + padder(field.useName().toUpperCase() + "_LOB_TYPE", 24) + "= "
            + (field.type == Field.TLOB || field.type == Field.IMAGE ? "SQLT_CLOB" : "SQLT_BLOB"));
        lobNo++;
      }
      thisOne = field.useName().toUpperCase() + "_OFFSET";
      writeln("  , " + padder(thisOne, 24) + "= (" + lastOne + "+"
          + lastSize + ")");
      lastOne = thisOne;
      lastSize = cppLength(field);
    }
    writeln("  , " + padder("ROWSIZE", 24) + "= (" + lastOne + "+"
        + lastSize + ")");
    if (proc.noRows > 0)
      writeln("  , " + padder("NOROWS", 24) + "= " + proc.noRows);
    else
      writeln("  , " + padder("NOROWS", 24) + "= (24576 / ROWSIZE) + 1");
    writeln("  , " + padder("NOBINDS", 24) + "= " + proc.inputs.size());
    writeln("  , " + padder("NOLOBS", 24) + "= " + noOfLobs(proc));
    field = proc.inputs.elementAt(0);
    thisOne = field.useName().toUpperCase();
    writeln("  , " + padder(thisOne + "_POS", 24) + "= 0");
    for (int j = 1; j < proc.inputs.size(); j++)
    {
      field = proc.inputs.elementAt(j);
      thisOne = field.useName().toUpperCase();
      writeln("  , " + padder(thisOne + "_POS", 24) + "= "
          + padder(thisOne + "_OFFSET", 24) + "* NOROWS");
    }
    writeln("  };");
    writeln("  TJQuery q_;");
    //writeln("  void Clear() {memset(this, 0, sizeof(" + dataStruct
    //    + "));}");
    writeln("  void Init(int Commit=1); // Commit after each block inserted");
    writeln("  void Fill();");
    if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
    {
      writeln("  void Fill(" + dataStruct
          + "& Rec) {*DRec() = Rec;Fill();}");
      writeln("  void Fill(");
      generateWithArrayParms(proc, "  ");
      writeln("  );");
    }
    writeln("  void Done();");
    writeln("  T" + table.useName() + proc.upperFirst()
        + "(TJConnector &conn, char *aFile=__FILE__, long aLine=__LINE__)");
    writeln("  : q_(conn)");
    writeln("  {Clear();q_.FileAndLine(aFile,aLine);}");
    writeln("  " + dataStruct + "* DRec() {return this;}");
  }
  static int noOfInputLobs(Proc proc)
  {
    int result = 0;
    for (int i = 0; i < proc.inputs.size(); i++)
    {
      Field field = proc.inputs.elementAt(i);
      if (isLob(field))
        result++;
    }
    return result;
  }
  static int noOfOutputLobs(Proc proc)
  {
    int result = 0;
    for (int i = 0; i < proc.outputs.size(); i++)
    {
      Field field = proc.outputs.elementAt(i);
      if (isLob(field))
        result++;
    }
    return result;
  }
  static int noOfLobs(Proc proc)
  {
    return noOfInputLobs(proc) + noOfOutputLobs(proc);
  }
  static void generateInterface(Table table, Proc proc, String dataStruct)
  {
    int inputLobs = noOfInputLobs(proc);
    int outputLobs = noOfOutputLobs(proc);
    if (proc.outputs.size() > 0 || inputLobs + outputLobs > 0)
      writeln("  enum");
    String front = "  { ";
    int lobNo = 0;
    if (inputLobs > 0)
    {
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        if (isLob(field))
        {
          writeln(front
              + padder(field.useName().toUpperCase() + "_LOB", 24) + "= "
              + lobNo);
          writeln("  , "
              + padder(field.useName().toUpperCase() + "_LOB_TYPE", 24) + "= "
              + (field.type == Field.TLOB || field.type == Field.IMAGE ? "SQLT_CLOB" : "SQLT_BLOB"));
          lobNo++;
          front = "  , ";
        }
      }
    }
    if (outputLobs > 0)
    {
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (isLob(field))
        {
          writeln(front
              + padder(field.useName().toUpperCase() + "_LOB", 24) + "= "
              + lobNo);
          writeln("  , "
              + padder(field.useName().toUpperCase() + "_LOB_TYPE", 24) + "= "
              + (field.type == Field.TLOB || field.type == Field.IMAGE ? "SQLT_CLOB" : "SQLT_BLOB"));
          lobNo++;
          front = "  , ";
        }
      }
    }
    if (proc.outputs.size() > 0)
    {
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
        writeln("  , " + padder(thisOne, 24) + "= (" + lastOne + "+"
            + lastSize + ")");
        lastOne = thisOne;
        lastSize = cppLength(field);
      }
      writeln("  , " + padder("ROWSIZE", 24) + "= (" + lastOne + "+"
          + lastSize + ")");
      if (proc.isSingle)
        writeln("  , " + padder("NOROWS", 24) + "= 1");
      else if (proc.noRows > 0)
        writeln("  , " + padder("NOROWS", 24) + "= " + proc.noRows);
      else
        writeln("  , " + padder("NOROWS", 24)
            + "= (24576 / ROWSIZE) + 1");
      writeln("  , " + padder("NOBINDS", 24) + "= "
          + proc.inputs.size());
      writeln("  , " + padder("NODEFINES", 24) + "= "
          + proc.outputs.size());
      writeln("  , " + padder("NOLOBS", 24) + "= " + noOfLobs(proc));
      field = proc.outputs.elementAt(0);
      thisOne = field.useName().toUpperCase();
      writeln("  , " + padder(thisOne + "_POS", 24) + "= 0");
      for (int j = 1; j < proc.outputs.size(); j++)
      {
        field = proc.outputs.elementAt(j);
        thisOne = field.useName().toUpperCase();
        writeln("  , " + padder(thisOne + "_POS", 24) + "= "
            + padder(thisOne + "_OFFSET", 24) + "* NOROWS");
      }
    }
    if (proc.outputs.size() > 0 || inputLobs + outputLobs > 0)
      writeln("  };");
    writeln("  TJQuery q_;");
    //writeln("  void Clear() {memset(this, 0, sizeof(" + dataStruct
    //    + "));}");
    writeln("  void Exec();");
    if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
    {
      writeln("  void Exec(" + dataStruct
          + "& Rec) {*DRec() = Rec;Exec();}");
      writeln("  void Exec(");
      generateWithParms(proc, "  ");
      writeln("  );");
    }
    if (proc.outputs.size() > 0)
      writeln("  bool Fetch();");
    writeln("  T" + table.useName() + proc.upperFirst()
        + "(TJConnector &conn, char *aFile=__FILE__, long aLine=__LINE__)");
    writeln("  : q_(conn)");
    writeln("  {Clear();q_.FileAndLine(aFile,aLine);}");
    writeln("  " + dataStruct + "* DRec() {return this;}");
    if (proc.outputs.size() > 0)
      writeln("  O" + dataStruct.substring(1)
          + "* ORec() {return this;}");
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
        generateArrayImplementation(table, proc);
      else
        generateImplementation(table, proc);
    }
  }
  static void generateCommand(Proc proc)
  {
    int size = 1;
    questionsSeen = 0;
    for (int i = 0; i < proc.lines.size(); i++)
    {
      Line l = proc.lines.elementAt(i);
      if (l.isVar)
        size += 256;
      else
        size += question(proc, l.line).length();
    }
    writeln("  if (q_.command == 0)");
    writeln("    q_.command = new char [" + size + "];");
    writeln("  memset(q_.command, 0, sizeof(q_.command));");
    if (proc.lines.size() > 0)
    {
      String strcat = "  strcat(q_.command, ";
      String tail = "";
      questionsSeen = 0;
      for (int i = 0; i < proc.lines.size(); i++)
      {
        Line l = proc.lines.elementAt(i);
        if (l.isVar)
        {
          tail = ");";
          if (i != 0)
            writeln(tail);
          write("  strcat(q_.command, " + l.line + "");
          strcat = "  strcat(q_.command, ";
        } else
        {
          if (i != 0)
            writeln(tail);
          tail = "";
          write(strcat + "\"" + question(proc, l.line) + "\"");
          strcat = "                      ";
        }
      }
      writeln(");");
    }
  }
  /**
   * Emits class method for processing the database activity
   */
  static void generateArrayImplementation(Table table, Proc proc)
  {
    String fullName = table.useName() + proc.name;
    writeln("void T" + fullName + "::Init(int Commit)");
    writeln("{");
    generateCommand(proc);
    writeln("  q_.OpenArray(q_.command, NOBINDS, NOROWS, NOLOBS, ROWSIZE);");
    writeln("  q_.SetCommit(Commit);");
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      writeln("  q_.BindArray("
          + padder("\":" + field.name + "\",", 24) + padder("" + j + ",", 4)
          + cppBindArray(field, table.name) + ");");
    }
    writeln("}");
    writeln();
    writeln("void T" + fullName + "::Fill()");
    writeln("{");
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if (field.type == Field.SEQUENCE && proc.isInsert)
        writeln("  q_.Sequence(" + field.useName() + ", \""
            + table.name + "Seq\");");
      if (field.type == Field.TIMESTAMP)
        writeln("  q_.conn.TimeStamp(" + field.useName() + ");");
      if (field.type == Field.USERSTAMP)
        writeln("  q_.UserStamp(" + field.useName() + ");");
      writeln("  q_.Put(" + cppPut(field) + ");");
      if (isNull(field))
        writeln("  q_.PutNull(" + field.useName() + "IsNull, " + j
            + ");");
    }
    writeln("  q_.Deliver(0); // 0 indicates defer doing it if not full");
    writeln("}");
    writeln();
    writeln("void T" + fullName + "::Fill(");
    generateWithArrayParms(proc, "");
    writeln(")");
    writeln("{");
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if ((field.type == Field.SEQUENCE && proc.isInsert)
          || field.type == Field.IDENTITY || field.type == Field.TIMESTAMP
          || field.type == Field.USERSTAMP)
        continue;
      writeln("  " + cppCopy(field));
    }
    writeln("  Fill();");
    writeln("}");
    writeln();
    writeln("void T" + fullName + "::Done()");
    writeln("{");
    writeln("  q_.Deliver(1); // 1 indicates doit now");
    writeln("}");
    writeln();
  }
  /**
   * Emits class method for processing the database activity
   */
  static void generateImplementation(Table table, Proc proc)
  {
    String fullName = table.useName() + proc.name;
    writeln("void T" + fullName + "::Exec()");
    writeln("{");
    generateCommand(proc);
    if (proc.outputs.size() > 0)
      writeln("  q_.Open(q_.command, NOBINDS, NODEFINES, NOROWS, NOLOBS, ROWSIZE);");
    else if (proc.inputs.size() > 0)
      writeln("  q_.Open(q_.command, " + proc.inputs.size() + ");");
    else
      writeln("  q_.Open(q_.command);");
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      generateCppBind(field);
    }
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      writeln("  q_.Bind("
          + padder("\":" + field.name + "\",", 24)
          + padder("" + j + ",", 4)
          + cppBind(field, table.name, proc.isInsert)
          + ((isNull(field)) ? ", &" + field.useName()
              + "IsNull);" : ");"));
    }
    for (int j = 0; j < proc.outputs.size(); j++)
    {
      Field field = proc.outputs.elementAt(j);
      writeln("  q_.Define(" + padder("" + j + ",", 4)
          + cppDefine(field) + ");");
    }
    writeln("  q_.Exec();");
    writeln("}");
    writeln();
    if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
    {
      writeln("void T" + fullName + "::Exec(");
      generateWithParms(proc, "");
      writeln(")");
      writeln("{");
      for (int j = 0; j < proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        if ((field.type == Field.SEQUENCE && proc.isInsert)
            || field.type == Field.IDENTITY || field.type == Field.TIMESTAMP
            || field.type == Field.USERSTAMP)
          continue;
        writeln("  " + cppCopy(field));
      }
      for (int j = 0; j < proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        writeln("  strncpy(" + s + ", a" + s + ", sizeof(" + s
            + ")-1);");
      }
      writeln("  Exec();");
      writeln("}");
      writeln();
    }
    if (proc.outputs.size() > 0)
    {
      writeln("bool T" + fullName + "::Fetch()");
      writeln("{");
      writeln("  if (q_.Fetch() == false)");
      writeln("    return false;");
      for (int j = 0; j < proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        writeln("  q_.Get(" + cppGet(field) + ");");
        if (isNull(field))
          writeln("  q_.GetNull(" + field.useName() + "IsNull, " + j
              + ");");
      }
      writeln("  return true;");
      writeln("}");
      writeln();
    }
  }
  static void generateWithArrayParms(Proc proc, String pad)
  {
    String comma = "  ";
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if ((field.type == Field.SEQUENCE && proc.isInsert)
          || field.type == Field.IDENTITY || field.type == Field.TIMESTAMP
          || field.type == Field.USERSTAMP)
        continue;
      writeln(pad + comma + "const " + cppParm(field));
      comma = ", ";
    }
  }
  static void generateWithParms(Proc proc, String pad)
  {
    String comma = "  ";
    for (int j = 0; j < proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if ((field.type == Field.SEQUENCE && proc.isInsert)
          || field.type == Field.IDENTITY || field.type == Field.TIMESTAMP
          || field.type == Field.USERSTAMP)
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
  static int questionsSeen;
  static String question(Proc proc, String line)
  {
    String result = "";
    int p;
    while ((p = line.indexOf("?")) > -1)
    {
      if (p > 0)
      {
        result = result + line.substring(0, p);
        line = line.substring(p);
      }
      Field field = proc.inputs.elementAt(questionsSeen++);
      if (field.type == Field.IDENTITY && proc.isInsert)
        field = proc.inputs.elementAt(questionsSeen++);
      result = result + ":" + field.name;
      line = line.substring(1);
    }
    result = result + line;
    return result;
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppLength(Field field)
  {
    switch (field.type)
    {
    case Field.BOOLEAN:
      // return "sizeof(bool)";
    case Field.BYTE:
      // return "sizeof(signed char)";
    case Field.SHORT:
      return "sizeof(int16)";
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return "sizeof(int32)";
    case Field.LONG:
      return "sizeof(int32)";
    case Field.CHAR:
    case Field.ANSICHAR:
      return "" + (field.length + 1);
    case Field.USERSTAMP:
      return "51";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return "sizeof(OCILobLocator *)";
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "8";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "sizeof(double)";
    }
    return "0";
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
      case Field.LONG:
        return field.useName() + " = 0;";
      //return "long   " + field.useName();
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.TLOB:
      case Field.IMAGE:      
      case Field.USERSTAMP:
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
        return "memset(" + field.useName() + ", 0, sizeof(" + field.useName() + "));";
      case Field.BLOB:
        return "memset(&" + field.useName() + ", 0, sizeof(" + field.useName() + "));";
      case Field.FLOAT:
      case Field.DOUBLE:
      case Field.MONEY:
        return field.useName() + " = 0.0;";
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
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return "int32  " + field.useName();
    case Field.LONG:
      return "int32  " + field.useName();
    case Field.CHAR:
    case Field.ANSICHAR:
      return "char   " + field.useName() + "[" + (field.length + 1) + "]";
    case Field.USERSTAMP:
      return "char   " + field.useName() + "[51]";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return "TJLob  " + field.useName();
    case Field.DATE:
      return "char   " + field.useName() + "[9]";
    case Field.TIME:
      return "char   " + field.useName() + "[7]";
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "char   " + field.useName() + "[15]";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "double " + field.useName();
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
      return "int32  a" + field.useName();
    case Field.LONG:
      return "int32  a" + field.useName();
    case Field.CHAR:
    case Field.ANSICHAR:
      return "char*  a" + field.useName();
    case Field.USERSTAMP:
      return "char*  a" + field.useName();
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return "TJLob  a" + field.useName();
    case Field.DATE:
      return "char*  a" + field.useName();
    case Field.TIME:
      return "char*  a" + field.useName();
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "char*  a" + field.useName();
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "double a" + field.useName();
    }
    return field.useName() + " <unsupported>";
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppCopy(Field field)
  {
    switch (field.type)
    {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
    case Field.SEQUENCE:
      return field.useName() + " = a" + field.useName() + ";";
    case Field.CHAR:
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
      return "strncpy(" + field.useName() + ", a" + field.useName()
          + ", sizeof(" + field.useName() + ")-1);";
    case Field.ANSICHAR:
      return "memcpy(" + field.useName() + ", a" + field.useName()
          + ", sizeof(" + field.useName() + "));";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return field.useName() + " = a" + field.useName() + ";";
    case Field.USERSTAMP:
    case Field.IDENTITY:
    case Field.TIMESTAMP:
      return "// " + field.useName() + " -- generated";
    }
    return field.useName() + " <unsupported>";
  }
  /**
   * generate Holding variables
   */
  static void generateCppBind(Field field)
  {
    switch (field.type)
    {
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      writeln("  TJOCIDate " + field.useName() + "_OCIDate;");
    }
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppBind(Field field, String tableName, boolean isInsert)
  {
    switch (field.type)
    {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName();
    case Field.SEQUENCE:
      if (isInsert)
        return "q_.Sequence(" + field.useName() + ", \"" + tableName + "Seq\")";
      else
        return field.useName();
    case Field.CHAR:
      return field.useName() + ", " + (field.length + 1);
    case Field.ANSICHAR:
      return field.useName() + ", " + (field.length + 1) + ", 1";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return "q_.LobLocator(q_.ociLobs[" + field.useName().toUpperCase()
          + "_LOB], " + field.useName() + "), " + field.useName().toUpperCase()
          + "_LOB_TYPE";
    case Field.USERSTAMP:
      return "q_.UserStamp(" + field.useName() + "), 51";
    case Field.DATE:
      return "q_.Date(" + field.useName() + "_OCIDate, " + field.useName()
          + ")";
    case Field.TIME:
      return "q_.Time(" + field.useName() + "_OCIDate, " + field.useName()
          + ")";
    case Field.DATETIME:
      return "q_.DateTime(" + field.useName() + "_OCIDate, " + field.useName()
          + ")";
    case Field.TIMESTAMP:
      return "q_.TimeStamp(" + field.useName() + "_OCIDate, " + field.useName()
          + ")";
    }
    return field.useName() + ", <unsupported>";
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppBindArray(Field field, String tableName)
  {
    switch (field.type)
    {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
      return "(int16*)  (q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return "(int32*)    (q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.LONG:
      return "(int32*)   (q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.CHAR:
      return "(char*)   (q_.data+" + field.useName().toUpperCase() + "_POS), "
          + (field.length + 1);
    case Field.ANSICHAR:
      return "(char*)   (q_.data+" + field.useName().toUpperCase() + "_POS), "
          + (field.length + 1) + ", 1";
    case Field.USERSTAMP:
      return "(char*)   (q_.data+" + field.useName().toUpperCase() + "_POS), 51";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return "q_.LobLocator(q_.ociLobs[" + field.useName().toUpperCase()
          + "_LOB*NOROWS], " + field.useName() + "), "
          + field.useName().toUpperCase() + "_LOB_TYPE";
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "(TJOCIDate*)(q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "(double*) (q_.data+" + field.useName().toUpperCase() + "_POS)";
    }
    return field.useName() + " <unsupported>";
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
      return "(int16*)  (q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return "(int32*)    (q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.LONG:
      return "(int32*)   (q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.CHAR:
      return "(char*)   (q_.data+" + field.useName().toUpperCase() + "_POS), "
          + (field.length + 1);
    case Field.ANSICHAR:
      return "(char*)   (q_.data+" + field.useName().toUpperCase() + "_POS), "
          + (field.length + 1) + ", 1";
    case Field.USERSTAMP:
      return "(char*)   (q_.data+" + field.useName().toUpperCase() + "_POS), 51";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return "(OCILobLocator*) (q_.data+" + field.useName().toUpperCase()
          + "_POS), " + field.useName().toUpperCase() + "_LOB_TYPE";
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "(TJOCIDate*)(q_.data+" + field.useName().toUpperCase() + "_POS)";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "(double*) (q_.data+" + field.useName().toUpperCase() + "_POS)";
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
    case Field.LONG:
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return padder(field.useName() + ",", 32) + " q_.data+"
          + field.useName().toUpperCase() + "_POS";
    case Field.CHAR:
    case Field.ANSICHAR:
      return padder(field.useName() + ",", 32) + " q_.data+"
          + field.useName().toUpperCase() + "_POS, " + (field.length + 1);
    case Field.USERSTAMP:
      return padder(field.useName() + ",", 32) + " q_.data+"
          + field.useName().toUpperCase() + "_POS, 51";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return padder(field.useName() + ",", 32) + " q_.data+"
          + field.useName().toUpperCase() + "_POS";
    case Field.DATE:
      return padder("TJDate(" + field.useName() + "),", 32) + " q_.data+"
          + field.useName().toUpperCase() + "_POS";
    case Field.TIME:
      return padder("TJTime(" + field.useName() + "),", 32) + " q_.data+"
          + field.useName().toUpperCase() + "_POS";
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return padder("TJDateTime(" + field.useName() + "),", 32) + " q_.data+"
          + field.useName().toUpperCase() + "_POS";
    }
    return field.useName() + " <unsupported>";
  }
  /**
   * Translates field type to cpp data member type
   */
  static String cppPut(Field field)
  {
    switch (field.type)
    {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
    case Field.LONG:
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + field.useName();
    case Field.CHAR:
    case Field.ANSICHAR:
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + field.useName() + ", " + (field.length + 1);
    case Field.USERSTAMP:
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + field.useName() + ", 51";
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:      
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + field.useName();
    case Field.DATE:
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + "TJDate(" + field.useName() + ")";
    case Field.TIME:
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + "TJTime(" + field.useName() + ")";
    case Field.DATETIME:
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + "TJDateTime(" + field.useName() + ")";
    case Field.TIMESTAMP:
      return padder("q_.data+" + field.useName().toUpperCase() + "_POS,", 32)
          + "TJDateTime(" + field.useName() + ")";
    }
    return field.useName() + " <unsupported>";
  }
  /**
   */
  static boolean isStruct(Field field)
  {
    return field.type == Field.TLOB || field.type == Field.BLOB || field.type == Field.IMAGE;
  }
  static boolean isLob(Field field)
  {
    return field.type == Field.TLOB || field.type == Field.BLOB || field.type == Field.IMAGE;
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
      case Field.BLOB:
      case Field.DATE:
      case Field.DATETIME:
      case Field.TIMESTAMP:
      case Field.TIME:
        return true;
    }
    return false;
  }
  static boolean notString(Field field)
  {
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
    case Field.TLOB:
    case Field.IMAGE:
      return true;
    }
    return false;
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
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return 4;
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
      return field.length + 1;
    case Field.TLOB:
    case Field.BLOB:
    case Field.IMAGE:
      return 8;
    case Field.DATE:
      return 9;
    case Field.TIME:
      return 7;
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return 15;
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return 8;
    }
    return 4;
  }
  static String fromXMLFormat(Field field)
  {
    switch (field.type)
    {
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "memcpy(" + field.useName() + ", work.data, sizeof("
          + field.useName() + ")-1);";
    case Field.BOOLEAN:
    case Field.BYTE:
      return field.useName() + " = (uchar)atoi(work.data);";
    case Field.SHORT:
      return field.useName() + " = (short)atoi(work.data);";
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() + " = atoi(work.data);";
    case Field.LONG:
      return field.useName() + " = atol(work.data);";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName() + " = atof(work.data);";
    }
    return "// " + field.useName() + " <unsupported>";
  }
  static String toXMLFormat(Field field)
  {
    String front = xmlValue ? "XRec.append(\"  <" + field.useName() + " value='\");"
                            : "XRec.append(\"  <" + field.useName() + ">\");";
    String back = xmlValue  ? " XRec.append(\"'/>\");"
                            : " XRec.append(\"</" + field.useName() + ">\");";
    switch (field.type)
    {
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.BLOB:
    case Field.TLOB:
    case Field.IMAGE:
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return front + " XRec.ampappend(" + field.useName() + ");" + back;
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
    case Field.LONG:
      return front + " XRec.ampappend(JP_XML_FORMAT((int32)" + field.useName()
          + ").result);" + back;
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return front + " XRec.ampappend(JP_XML_FORMAT((double)" + field.useName()
          + ").result);" + back;
    }
    return "// " + field.useName() + " <unsupported>";
  }
  public static String generatePadding(Field field, int fillerNo)
  {
    int n;
    switch (field.type)
    {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
      return "IDL2_INT16_PAD(" + fillerNo + ");";
    case Field.INT:
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return "IDL2_INT32_PAD(" + fillerNo + ");";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.TLOB:
    case Field.IMAGE:
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      n = 8 - ((field.length + 1) % 8);
      if (n != 8)
        return "IDL2_CHAR_PAD(" + fillerNo + "," + n + ");";
    }
    return "";
  }
  public static String generatePadding(int fillerNo)
  {
    return "IDL2_INT16_PAD(" + fillerNo + ");";
  }
}
