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
/// $Date: 2004/10/18 13:45:45 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal.old;

import bbd.jportal.*;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

public class CBuilderAdoCode extends Generator
{
  public static String description()
  {
    return "Generate C++ Builder ADO Code";
  }
  public static String documentation()
  {
    return "Generate C++ Builder ADO Code";
  }
  /**
  * Generates the procedure classes for each table present.
  */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    for (int i=0; i<database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      generate(table, output, outLog);
    }
  }
  /**
  * Build of standard and user defined procedures
  */
  static void generate(Table table, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: "+output+table.useName() + ".h");
      OutputStream outFile = new FileOutputStream(output+table.useName() + ".h");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        outData.println("// This code was generated, do not modify it, modify it at source and regenerate it.");
        outData.println("#ifndef "+table.useName()+"H");
        outData.println("#define "+table.useName()+"H");
        outData.println();
        outData.println("#include <vcl\\Classes.hpp>");
        outData.println("#include <vcl\\Controls.hpp>");
        outData.println("#include <vcl\\StdCtrls.hpp>");
        outData.println();
        outData.println("#include \"CBuilderAdoConnector.h\"");
        outData.println();
        if (table.hasStdProcs)
          generateStdInterface(table, outData);
        generateOtherInterface(table, outData);
        for (int i=0; i<table.procs.size(); i++)
        {
          Proc proc = table.procs.elementAt(i);
          if (proc.isData)
            continue;
          if (proc.dynamics.size() < 1)
          {
            outData.println("const AnsiString "+table.useName()+proc.upperFirst()+" =");
            generateSQLCode(proc, outData, "");
            outData.println();
          }
        }
        outData.println("#endif");
        outData.flush();
        outFile.close();
        outLog.println("Code: "+output+table.useName() + ".cpp");
        outFile = new FileOutputStream(output+table.useName() + ".cpp");
        outData = new PrintWriter(outFile);
        outData.println("// This code was generated, do not modify it, modify it at source and regenerate it.");
        outData.println("#include <vcl\\vcl.h>");
        outData.println("#pragma hdrstop");
        outData.println();
        outData.println("#include \""+table.useName()+".h\"");
        outData.println();
        if (table.hasStdProcs)
          generateStdImplementation(table, outData);
        generateOtherImplementation(table, outData);
        outData.flush();
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
  /**
  * Build of all required standard procedures
  */
  static void generateStdInterface(Table table, PrintWriter outData)
  {
    for (int i=0; i < table.comments.size(); i++)
    {
      String s = table.comments.elementAt(i);
      outData.println("//"+s);
    }
    outData.println("class T"+table.useName());
    outData.println("{");
    outData.println("public:");
    outData.println("  TConnector &Connector;");
    for (int i=0; i<table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      if (field.comments.size() > 0)
      {
        for (int c=0; c < field.comments.size(); c++)
        {
          String s = field.comments.elementAt(c);
          outData.println("  //"+s);
        }
      }
      outData.println("  "+cbuilderVar(field,"")+";");
      if (field.isNull)
        outData.println("  bool "+field.useName()+"IsNull;");
    }
    outData.println("  T"+table.useName()+"(TConnector &aConnector);");
    for (int i=0; i<table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      if (proc.isStd || proc.hasNoData())
        generateInterface(proc, outData);
    }
    outData.println("  #if defined(_TBUFFER_H_) // if tbuffer.h include prior");
    outData.println("  void XML(TBAmp &XRec, char* Outer=\"D"+table.useName().toUpperCase()+"\", char* Attr=0)");
    outData.println("  {");
    outData.println("    char Work[32];");
    outData.println("    XRec.clear();");
    outData.println("    XRec.append(\"<\");XRec.append(Outer);if (Attr) XRec.append(Attr);XRec.append(\">\");");
    for (int i=0; i<table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      outData.println("    "+xmlOf(field));
    }
    outData.println("    XRec.append(\"</\");XRec.append(Outer);XRec.append(\">\");");
    outData.println("  }");
    outData.println("  #endif");
    outData.println("};");
    outData.println();
  }
  /**
  * Build of all required standard procedures
  */
  static void generateStdImplementation(Table table, PrintWriter outData)
  {
    outData.println("T"+table.useName()+"::T"+table.useName()+"(TConnector &aConnector)");
    outData.println(": Connector(aConnector)");
    outData.println("{");
    for (int i=0; i<table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      outData.println("  "+initCBuilderVar(field));
      if (field.isNull)
        outData.println("  "+field.useName()+"IsNull = false;");
    }
    outData.println("}");
    outData.println();
    for (int i=0; i<table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      if (proc.isStd || proc.hasNoData())
        generateImplementation(proc, outData, table.useName(), table.useName());
    }
  }
  /**
  * Build of user defined procedures
  */
  static void generateOtherInterface(Table table, PrintWriter outData)
  {
    for (int i=0; i<table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData || proc.isStd || proc.hasNoData())
        continue;
      for (int j=0; j<proc.comments.size(); j++)
      {
        String comment = proc.comments.elementAt(j);
        outData.println("//"+comment);
      }
      outData.println("class T"+table.useName()+proc.upperFirst());
      outData.println("{");
      outData.println("public:");
      outData.println("  TConnector &Connector;");
      for (int j=0; j<proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        for (int c=0; c < field.comments.size(); c++)
        {
          String s = field.comments.elementAt(c);
          outData.println("  //"+s);
        }
        outData.println("  "+cbuilderVar(field,"")+";");
        if (field.isNull)
          outData.println("  bool "+field.useName()+"IsNull;");
      }
      for (int j=0; j<proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (!proc.hasInput(field.name))
        {
          for (int c=0; c < field.comments.size(); c++)
          {
            String s = field.comments.elementAt(c);
            outData.println("  //"+s);
          }
          outData.println("  "+cbuilderVar(field,"")+";");
          if (field.isNull)
            outData.println("  bool "+field.useName()+"IsNull;");
        }
      }
      for (int j=0; j<proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        outData.println("  AnsiString "+s+";");
      }
      outData.println("  T"+table.useName()+proc.upperFirst()+"(TConnector &aConnector);");
      generateInterface(proc, outData);
      outData.println("  #if defined(_TBUFFER_H_) // if tbuffer.h include prior");
      outData.println("  void XML(TBAmp &XRec, char* Outer=\"D"+table.useName().toUpperCase()+proc.name.toUpperCase()+"\", char *Attr = 0)");
      outData.println("  {");
      outData.println("    char Work[32];");
      outData.println("    XRec.clear();");
      outData.println("    XRec.append(\"<\");XRec.append(Outer);if (Attr) XRec.append(Attr);XRec.append(\">\");");
      for (int j=0; j<proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        if (proc.hasOutput(field.name))
          continue;
        outData.println("    "+xmlOf(field));
      }
      for (int j=0; j<proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        outData.println("    "+xmlOf(field));
      }
      outData.println("    XRec.append(\"</\");XRec.append(Outer);XRec.append(\">\");");
      outData.println("  }");
      outData.println("  #endif");
      outData.println("};");
      outData.println();
    }
  }
  static void generateOtherImplementation(Table table, PrintWriter outData)
  {
    for (int i=0; i<table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData || proc.isStd || proc.hasNoData())
        continue;
      outData.println("T"+table.useName()+proc.upperFirst()+
                 "::T"+table.useName()+proc.upperFirst()+"(TConnector &aConnector)");
      outData.println(": Connector(aConnector)");
      outData.println("{");
      for (int j=0; j<proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        outData.println("  "+initCBuilderVar(field));
        if (field.isNull)
          outData.println("  "+field.useName()+"IsNull = false;");
      }
      for (int j=0; j<proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (!proc.hasInput(field.name))
          outData.println("  "+initCBuilderVar(field));
        if (field.isNull)
          outData.println("  "+field.useName()+"IsNull = false;");
      }
      for (int j=0; j<proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        outData.println("  "+s+" = \"\";");
      }
      outData.println("}");
      outData.println();
      generateImplementation(proc, outData, table.useName(), table.useName()+proc.name);
    }
  }
  static void generateWithParms(Proc proc, PrintWriter outData, String pad)
  {
    String comma = "  ";
    for (int j=0; j<proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      outData.println(pad+comma+"const "+cbuilderVar(field, "a"));
      comma = ", ";
    }
    for (int j=0; j<proc.dynamics.size(); j++)
    {
      String s = proc.dynamics.elementAt(j);
      outData.println(pad+comma+"const AnsiString a"+s);
      comma = ", ";
    }
  }
  /** Emits class method for processing the database activity */
  static void generateInterface(Proc proc, PrintWriter outData)
  {
    if (proc.comments.size() > 0)
    {
      for (int i=0; i<proc.comments.size(); i++)
      {
        String comment = proc.comments.elementAt(i);
        outData.println("  //"+comment);
      }
    }
    if (proc.outputs.size() == 0)
    {
      outData.println("  void "+proc.upperFirst()+"();");
      if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
      {
        outData.println("  void "+proc.upperFirst()+"(");
        generateWithParms(proc, outData, "  ");
        outData.println("  );");
      }
    }
    else if (proc.isSingle)
    {
      outData.println("  bool "+proc.upperFirst()+"();");
      if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
      {
        outData.println("  bool "+proc.upperFirst()+"(");
        generateWithParms(proc, outData, "  ");
        outData.println("  );");
      }
    }
    else
    {
      outData.println("  TADOQuery *"+proc.upperFirst()+"();");
      if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
      {
        outData.println("  TADOQuery *"+proc.upperFirst()+"(");
        generateWithParms(proc, outData, "  ");
        outData.println("  );");
      }
      outData.println("  bool "+proc.upperFirst()+"(TADOQuery *Query);");
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
  * Emits class method for processing the database activity
  */
  static void generateImplementation(Proc proc, PrintWriter outData, String tableName, String fullName)
  {
    String pad = "";
    if (proc.outputs.size() == 0)
    {
      outData.println("void T"+fullName+"::"+proc.upperFirst()+"()");
      outData.println("{");
    }
    else if (proc.isSingle)
    {
      outData.println("bool T"+fullName+"::"+proc.upperFirst()+"()");
      outData.println("{");
      outData.println("  bool result;");
    }
    else
    {
      outData.println("TADOQuery *T"+fullName+"::"+proc.upperFirst()+"()");
      outData.println("{");
    }
    if (proc.dynamics.size() > 0)
      outData.println("  AnsiString "+tableName+proc.upperFirst()+";");
    if (proc.dynamics.size() > 0)
    {
      outData.println("  "+tableName+proc.upperFirst()+" =");
      generateSQLCode(proc, outData, "  ");
    }
    outData.println("  TADOQuery *Query = new TADOQuery(0);");
    if (proc.outputs.size() == 0 || proc.isSingle)
    {
      outData.println("  try");
      outData.println("  {");
      pad = "  ";
    }
    outData.println(pad+"  Query->Connection = Connector.Conn;");
    outData.println(pad+"  Query->SQL->Add("+tableName+proc.upperFirst()+");");
    for (int j=0; j<proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if (proc.isInsert)
      {
        if (field.isSequence)
          outData.println(pad+"  "+field.useName()+" = Connector.getSequence(\""+proc.table.name+"\");");
      }
      if (field.type == Field.TIMESTAMP)
        outData.println(pad+"  "+field.useName()+" = Connector.getTimeStamp();");
      if (field.type == Field.USERSTAMP)
        outData.println(pad+"  "+field.useName()+" = Connector.getUserStamp();");
      if (field.isNull)
      {
        outData.println(pad+"  if (!"+field.useName()+"IsNull)");
        outData.println(pad+"    Query->Parameters->ParamByName("+cbuilderInputs(field));
        outData.println(pad+"  else");
        outData.println(pad+"    Query->Parameters->ParamByName(\""+field.name+"\")->DataType = "+cbuilderDataType(field)+";");
      }
      else
        outData.println(pad+"  Query->Parameters->ParamByName("+cbuilderInputs(field));
    }
    if (proc.outputs.size() == 0)
      outData.println(pad+"  Query->ExecSQL();");
    else
    {
      outData.println(pad+"  Query->Open();");
      if (proc.isSingle)
      {
        outData.println(pad+"  if (!Query->Eof)");
        outData.println(pad+"  {");
        for (int j=0; j<proc.outputs.size(); j++)
        {
          Field field = proc.outputs.elementAt(j);
          generateCBuilderOutput(field, outData, pad);
        }
        outData.println(pad+"    result = true;");
        outData.println(pad+"  }");
        outData.println(pad+"  else");
        outData.println(pad+"    result = false;");
      }
      else
        outData.println("  return Query;");
    }
    if (proc.outputs.size() == 0 || proc.isSingle)
    {
      outData.println("  }");
      outData.println("  __finally");
      outData.println("  {");
      outData.println("    delete Query;");
      outData.println("  }");
    }
    if (proc.isSingle)
      outData.println("  return result;");
    outData.println("}");
    outData.println();
    if (proc.inputs.size() > 0 || proc.dynamics.size() > 0)
    {
      if (proc.outputs.size() == 0)
        outData.println("void T"+fullName+"::"+proc.upperFirst()+"(");
      else if (proc.isSingle)
        outData.println("bool T"+fullName+"::"+proc.upperFirst()+"(");
      else
        outData.println("TADOQuery *T"+fullName+"::"+proc.upperFirst()+"(");
      generateWithParms(proc, outData, "");
      outData.println(")");
      outData.println("{");
      for (int j=0; j<proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        outData.println("  "+field.useName()+" = a"+field.useName()+";");
      }
      for (int j=0; j<proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        outData.println("  "+s+" = a"+s+";");
      }
      if (proc.outputs.size() == 0)
        outData.println("  "+proc.upperFirst()+"();");
      else
        outData.println("  return "+proc.upperFirst()+"();");
      outData.println("}");
      outData.println();
    }
    if (proc.outputs.size() != 0 && !proc.isSingle)
    {
      outData.println("bool T"+fullName+"::"+proc.upperFirst()+"(TADOQuery *Query)");
      outData.println("{");
      outData.println("  if (!Query->Eof)");
      outData.println("  {");
      for (int j=0; j<proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        generateCBuilderOutput(field, outData, pad);
      }
      outData.println("    Query->Next();");
      outData.println("    return true;");
      outData.println("  }");
      outData.println("  return false;");
      outData.println("}");
      outData.println();
    }
  }
  /**
  * Emits SQL Code
  */
  static void generateCBuilderOutput(Field field, PrintWriter outData, String pad)
  {
    if (field.isNull)
    {
      outData.println(pad+"    "+field.useName()+"IsNull = Query->FieldByName(\""+field.name+"\")->IsNull;");
      outData.println(pad+"    if (!"+field.useName()+"IsNull)");
      outData.println(pad+"      "+cbuilderOutputs(field));
    }
    else
      outData.println(pad+"    "+cbuilderOutputs(field));
  }
  /**
  * Emits SQL Code
  */
  static void generateSQLCode(Proc proc, PrintWriter outData, String pad)
  {
    questionsSeen = 0;
    String p = "";
    for (int i=0; i < proc.lines.size(); i++)
    {
      String x;
      if (i+1 < proc.lines.size())
        x = "";
      else
        x = ";";
      Line l = proc.lines.elementAt(i);
      if (l.isVar)
      {
        outData.println(pad+p+"  + "+l.line+x);
        p = "+ ";
      }
      else
      {
        String out = "  \""+question(proc, l.line)+"\""+x;
        outData.println(pad+p+out);
        p = "";
      }
    }
  }
  /**
  * Translates field type to delphi data member type
  */
  static String cbuilderVar(Field field, String a)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return "bool "+a+field.useName();
    case Field.BYTE:
      return "signed char "+a+field.useName();
    case Field.SHORT:
      return "short int "+a+field.useName();
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return "int "+a+field.useName();
    case Field.LONG:
      return "long int "+a+field.useName();
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.BLOB:
    case Field.TLOB:
      return "AnsiString "+a+field.useName();
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIME:
    case Field.TIMESTAMP:
      return "TDateTime "+a+field.useName();
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "double "+a+field.useName();
    }
    return field.useName() + " : <unsupported>";
  }
  /**
  * returns the data member initialisation code (not always neccessary in java but
  * still we do it)
  */
  static String initCBuilderVar(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() +" = false;";
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return field.useName() +" = Date();";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName() +" = 0.0;";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() +" = 0;";
    case Field.TIME:
      return field.useName() +" = Time();";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.BLOB:
    case Field.TLOB:
      return field.useName() +" = \"\";";
    }
    return field.useName() +"<unsupported>";
  }
  /**
  */
  static String cbuilderInputs(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return "\""+field.name+"\")->Value/*AsBoolean*/ = "+field.useName()+";";
    case Field.DATE:
      return "\""+field.name+"\")->Value/*AsDateTime*/ = "+field.useName()+";";
    case Field.DATETIME:
      return "\""+field.name+"\")->Value/*AsDateTime*/ = "+field.useName()+";";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "\""+field.name+"\")->Value/*AsFloat*/ = "+field.useName()+";";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
      return "\""+field.name+"\")->Value/*AsInteger*/ = "+field.useName()+";";
    case Field.IDENTITY:
    case Field.SEQUENCE:
      return "\""+field.name+"\")->Value/*AsInteger*/ = "+field.useName()+";";
    case Field.TIME:
    case Field.TIMESTAMP:
      return "\""+field.name+"\")->Value/*AsDateTime*/ = "+field.useName()+";";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
      return "\""+field.name+"\")->Value/*AsString*/ = "+field.useName()+";";
    case Field.BLOB:
      return "\""+field.name+"\")->Value/*AsBlob*/ = "+field.useName()+";";
    case Field.TLOB:
      return "\""+field.name+"\")->Value/*AsMemo*/ = "+field.useName()+";";
    }
    return field.useName() +"<unsupported>";
  }
  /**
  */
  static String cbuilderDataType(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
    case Field.DATE:
      return "ftDate";
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return "ftDateTime";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "ftFloat";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
      return "ftInteger";
    case Field.TIME:
      return "ftTime";
    }
    return "ftString";
  }
  /**
  */
  static String cbuilderOutputs(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsBoolean*/;";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
    case Field.LONG:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsInteger*/;";
    case Field.DATE:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsDateTime*/;";
    case Field.DATETIME:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsDateTime*/;";
    case Field.FLOAT:
    case Field.DOUBLE:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsFloat*/;";
    case Field.MONEY:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsCurrency*/;";
    case Field.TIME:
    case Field.TIMESTAMP:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsDateTime*/;";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsString*/;";
    case Field.BLOB:
    case Field.TLOB:
      return field.useName() +" = Query->FieldByName(\""+field.name+"\")->Value/*AsString*/;";
    }
    return field.useName() +"<unsupported>";
  }
  /**
  * Translates field type to cpp data member type
  */
  static String xmlOf(Field field)        ////
  {
    String result="";
    if (field.isNull)
      result="if ("+field.useName()+"IsNull == 0)\r\n      ";
    result=result+"XRec.append(\"<"+field.useName().toUpperCase()+">\");";
    switch(field.type)
    {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
    case Field.LONG:
      result=result+"sprintf(Work, \"%d\", "+field.useName()+");";
      result=result+"XRec.append(Work);";
      break;
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.BLOB:
    case Field.TLOB:
      result=result+"XRec.ampappend("+field.useName()+");";
      break;
    case Field.USERSTAMP:
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      result=result+"XRec.append("+field.useName()+");";
      break;
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      result=result+"sprintf(Work, \"%f\", "+field.useName()+");";
      result=result+"XRec.append(Work);";
      break;
    }
    result=result+"XRec.append(\"</"+field.useName().toUpperCase()+">\");";
    if (field.isNull)
      result=result+"\r\n    else\r\n      XRec.append(\"<"+field.useName().toUpperCase()+"/>\");";
    return result;
  }
}

