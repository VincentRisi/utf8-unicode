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
/// $Date: 2004/10/18 13:45:46 $
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

public class DelphiCode extends Generator
{
  /**
  * Reads input from stored repository
  */
  public static void main(String[] args)
  {
    try
    {
      PrintWriter outLog = new PrintWriter(System.out);
      for (int i = 0; i <args.length; i++)
      {
        outLog.println(args[i]+": Generate Delphi BDE Code");
        ObjectInputStream in = new ObjectInputStream(new FileInputStream(args[i]));
        Database database = (Database)in.readObject();
        generate(database, "", outLog);
      }
      outLog.flush();
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }
  /**
  * Generates the procedure classes for each table present.
  */
  public static String description()
  {
    return "Generate Delphi BDE Code";
  }
  public static String documentation()
  {
    return "Generate Delphi BDE Code";
  }
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
      outLog.println("Code: "+output+table.useName() + ".pas");
      OutputStream outFile = new FileOutputStream(output+table.useName() + ".pas");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        outData.println("unit "+table.useName()+";");
        outData.println("// This code was generated, do not modify it, modify it at source and regenerate it.");
        outData.println();
        outData.println("interface");
        outData.println();
        outData.println("uses  SysUtils, Db, DbTables, Connector;");
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
            outData.println("var");
            outData.println("  "+table.useName()+proc.upperFirst()+" : String =");
            generateSQLCode(proc, outData);
            outData.println();
          }
        }
        outData.println("implementation");
        outData.println();
        if (table.hasStdProcs)
          generateStdImplementation(table, outData);
        generateOtherImplementation(table, outData);
        outData.println("end.");
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
    outData.println("type T"+table.useName()+" = Class");
    outData.println("  Conn : TConnector;");
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
      outData.println("  "+delphiVar(field)+";");
      if (field.isNull) // && notString(field))
        outData.println("  "+field.useName()+"IsNull : boolean;");
    }
    outData.println("  constructor Create(const aConnector : TConnector);");
    for (int i=0; i<table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      if (proc.isStd || proc.hasNoData())
        generateInterface(proc, outData);
    }
    outData.println("end;");
    outData.println();
  }
  /**
  * Build of all required standard procedures
  */
  static void generateStdImplementation(Table table, PrintWriter outData)
  {
    outData.println("constructor T"+table.useName()+".Create(const aConnector : TConnector);");
    outData.println("begin");
    outData.println("  Conn := aConnector;");
    for (int i=0; i<table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      outData.println("  "+initDelphiVar(field));
      if (field.isNull) // && notString(field))
        outData.println("  "+field.useName()+"IsNull := false;");
    }
    outData.println("end;");
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
      outData.println("type T" + table.useName() + proc.upperFirst() + " = Class");
      outData.println("  Conn : TConnector;");
      for (int j=0; j<proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        for (int c=0; c < field.comments.size(); c++)
        {
          String s = field.comments.elementAt(c);
          outData.println("  //"+s);
        }
        outData.println("  "+delphiVar(field)+";");
        if (field.isNull) // && notString(field))
          outData.println("  "+field.useName()+"IsNull : boolean;");
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
          outData.println("  "+delphiVar(field)+";");
          if (field.isNull) // && notString(field))
            outData.println("  "+field.useName()+"IsNull : boolean;");
        }
      }
      for (int j=0; j<proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        outData.println("  "+s+" : String;");
      }
      outData.println("  constructor Create(const aConnector : TConnector);");
      generateInterface(proc, outData);
      outData.println("end;");
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
      outData.println("constructor T"+table.useName()+proc.name+".Create(const aConnector : TConnector);");
      outData.println("begin");
      outData.println("  Conn := aConnector;");
      for (int j=0; j<proc.inputs.size(); j++)
      {
        Field field = proc.inputs.elementAt(j);
        outData.println("  "+initDelphiVar(field));
        if (field.isNull) // && notString(field))
          outData.println("  "+field.useName()+"IsNull := false;");
      }
      for (int j=0; j<proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        if (!proc.hasInput(field.name))
          outData.println("  "+initDelphiVar(field));
        if (field.isNull) // && notString(field))
          outData.println("  "+field.useName()+"IsNull := false;");
      }
      for (int j=0; j<proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        outData.println("  "+s+" := '';");
      }
      outData.println("end;");
      outData.println();
      generateImplementation(proc, outData, table.useName(), table.useName()+proc.name);
    }
  }
  static void generateWithParms(Proc proc, PrintWriter outData)
  {
    String semicolon = "  ";
    for (int j=0; j<proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      outData.println("  "+semicolon+"const a"+delphiVar(field));
      semicolon = "; ";
      if (field.isNull) // && notString(field))
        outData.println("  "+semicolon+"const a"+field.useName()+"IsNull : boolean");
    }
    for (int j=0; j<proc.dynamics.size(); j++)
    {
      String s = proc.dynamics.elementAt(j);
      outData.println("  "+semicolon+"const a"+s+" : String");
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
    if (proc.hasNoData())
    {
      outData.println("  class procedure "+proc.upperFirst()+"(const Conn : TConnector);");
    }
    else if (proc.outputs.size() == 0)
    {
      outData.println("  procedure "+proc.upperFirst()+";");
      if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
      {
        outData.println("  procedure wp"+proc.upperFirst()+"(");
        generateWithParms(proc, outData);
        outData.println("  );");
      }
    }
    else if (proc.isSingle)
    {
      outData.println("  function "+proc.upperFirst()+" : Boolean;");
      if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
      {
        outData.println("  function wp"+proc.upperFirst()+"(");
        generateWithParms(proc, outData);
        outData.println("  ) : Boolean;");
      }
    }
    else
    {
      outData.println("  function "+proc.upperFirst()+" : TQuery;");
      if ((proc.inputs.size() > 0) || proc.dynamics.size() > 0)
      {
        outData.println("  function wp"+proc.upperFirst()+"(");
        generateWithParms(proc, outData);
        outData.println("  ) : tQuery;");
      }
      outData.println("  function next"+proc.upperFirst()+"(const Query : TQuery) : Boolean;");
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
    String with;
    if (proc.hasNoData())
      outData.println("class procedure T"+fullName+"."+proc.upperFirst()+";");
    else if (proc.outputs.size() == 0)
      outData.println("procedure T"+fullName+"."+proc.upperFirst()+";");
    else if (proc.isSingle)
      outData.println("function T"+fullName+"."+proc.upperFirst()+" : Boolean;");
    else
      outData.println("function T"+fullName+"."+proc.upperFirst()+" : TQuery;");
    if (proc.dynamics.size() > 0 || proc.outputs.size() == 0 || proc.isSingle)
      outData.println("var");
    if (proc.outputs.size() == 0 || proc.isSingle)
      outData.println("  Query : TQuery;");
    if (proc.dynamics.size() > 0)
      outData.println("  "+tableName+proc.upperFirst()+" : String;");
    outData.println("begin");
    if (proc.dynamics.size() > 0)
    {
      outData.println("  "+tableName+proc.upperFirst()+" :=");
      generateSQLCode(proc, outData);
    }
    if (proc.outputs.size() == 0 || proc.isSingle)
    {
      outData.println("  Query := TQuery.Create(nil);");
      outData.println("  try");
      //outData.println("    with Query do begin");
      with = "Query";
    }
    else
    {
      outData.println("  result := TQuery.Create(nil);");
      outData.println("  try");
      //outData.println("    with result do begin");
      with = "result";
    }
    outData.println("    "+with+".DatabaseName := Conn.DatabaseName;");
    outData.println("    "+with+".SQL.Add("+tableName+proc.upperFirst()+");");
    for (int j=0; j<proc.inputs.size(); j++)
    {
      Field field = proc.inputs.elementAt(j);
      if (proc.isInsert)
      {
        if (field.isSequence)
          outData.println("    "+field.useName()+" := Conn.getSequence('"+proc.table.name+"');");
      }
      if (field.type == Field.TIMESTAMP)
        outData.println("    "+field.useName()+" := Conn.getTimeStamp;");
      if (field.type == Field.USERSTAMP)
        outData.println("    "+field.useName()+" := Conn.getUserStamp;");
      if (field.isNull) // && notString(field))
      {
        outData.println("    if not "+field.useName()+"IsNull then begin");
        outData.println("      "+with+"."+delphiInputs(field));
        outData.println("    end else begin");
        outData.println("      Query.Params.ParamByName('"+field.name+"').Clear;");
        outData.println("      Query.Params.ParamByName('"+field.name+"').DataType := "+delphiDataType(field)+";");
        outData.println("      Query.Params.ParamByName('"+field.name+"').Bound := true;");
        outData.println("    end;");
      }
      else
        outData.println("    "+with+"."+delphiInputs(field));
    }
    if (proc.outputs.size() == 0)
      outData.println("    "+with+".ExecSQL;");
    else
    {
      outData.println("    "+with+".Open;");
      if (proc.isSingle)
      {
        outData.println("    if not "+with+".eof then begin");
        for (int j=0; j<proc.outputs.size(); j++)
        {
          Field field = proc.outputs.elementAt(j);
          generateDelphiOutput(field, outData, "  ", with);
        }
        outData.println("      result := true;");
        outData.println("    end");
        outData.println("    else");
        outData.println("      result := false;");
      }
    }
    //outData.println("    end;");
    if (proc.outputs.size() == 0 || proc.isSingle)
    {
      outData.println("  finally");
      outData.println("    Query.free;");
    }
    else
    {
      outData.println("  except");
      outData.println("    result.free;");
    }
    outData.println("  end;");
    outData.println("end;");
    outData.println();
    if (proc.inputs.size() > 0 || proc.dynamics.size() > 0)
    {
      if (proc.outputs.size() == 0)
        outData.println("procedure T"+fullName+".wp"+proc.upperFirst()+";");
      else if (proc.isSingle)
        outData.println("function T"+fullName+".wp"+proc.upperFirst()+";");
      else
        outData.println("function T"+fullName+".wp"+proc.upperFirst()+";");
      outData.println("begin");
      for (int j=0; j<proc.inputs.size(); j++)
      {
        String Indent;
        Field field = proc.inputs.elementAt(j);
        Indent = "";
        if (field.isNull) // && notString(field))
        {
          outData.println("  "+field.useName()+"IsNull := a"+field.useName()+"IsNull;");
          outData.println("  if not "+field.useName()+"IsNull then");
          Indent = "  ";
        }
        outData.println("  "+Indent+field.useName()+" := a"+field.useName()+";");
      }
      for (int j=0; j<proc.dynamics.size(); j++)
      {
        String s = proc.dynamics.elementAt(j);
        outData.println("  "+s+" := a"+s+";");
      }
      if (proc.outputs.size() == 0)
        outData.println("  "+proc.upperFirst()+";");
      else
        outData.println("  result := "+proc.upperFirst()+";");
      outData.println("end;");
      outData.println();
    }
    if (proc.outputs.size() != 0 && !proc.isSingle)
    {
      outData.println("function T"+fullName+".next"+proc.upperFirst()+"(const Query : TQuery) : Boolean;");
      outData.println("begin");
      outData.println("  if not Query.eof then begin");
      for (int j=0; j<proc.outputs.size(); j++)
      {
        Field field = proc.outputs.elementAt(j);
        generateDelphiOutput(field, outData, "", "Query");
      }
      outData.println("    result := true;");
      outData.println("    Query.next;");
      outData.println("  end");
      outData.println("  else");
      outData.println("    result := false;");
      outData.println("end;");
      outData.println();
    }
  }
  /**
  * Emits SQL Code
  */
  static void generateDelphiOutput(Field field, PrintWriter outData, String gap, String with)
  {
    if (field.isNull) // && notString(field))
    {
      outData.println(gap+"    "+field.useName()+"IsNull := "+with+".FieldByName('"+field.name+"').isNull;");
      outData.println(gap+"    if not "+field.useName()+"IsNull then");
      outData.println(gap+"      "+delphiOutputs(field, with));
    }
    else
      outData.println(gap+"    "+delphiOutputs(field, with));
  }
  /**
  * Emits SQL Code
  */
  static void generateSQLCode(Proc proc, PrintWriter outData)
  {
    questionsSeen = 0;
    for (int i=0; i < proc.lines.size(); i++)
    {
      String x;
      if (i+1 < proc.lines.size())
        x = " +";
      else
        x = ";";
      Line l = proc.lines.elementAt(i);
      if (l.isVar)
        outData.println("    "+l.line+x);
      else
      {
        String out = "    '"+question(proc, l.line)+"'"+x;
        outData.println(out);
      }
    }
  }
  /**
  * Translates field type to delphi data member type
  */
  static String delphiVar(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() + " : Boolean";
    case Field.BYTE:
      return field.useName() + " : Shortint";
    case Field.SHORT:
      return field.useName() + " : Smallint";
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() + " : Integer";
    case Field.LONG:
      return field.useName() + " : Longint";
    case Field.CHAR:
    case Field.ANSICHAR:
      return field.useName() + " : String";
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIME:
    case Field.TIMESTAMP:
      return field.useName() + " : TDateTime";
    case Field.FLOAT:
    case Field.DOUBLE:
      return field.useName() + " : Double";
    case Field.BLOB:
    case Field.TLOB:
      return field.useName() + " : String";
    case Field.MONEY:
      return field.useName() + " : Double";
    case Field.USERSTAMP:
      return field.useName() + " : String";
    }
    return field.useName() + " : <unsupported>";
  }
  /**
  * returns the data member initialisation code (not always neccessary in java but
  * still we do it)
  */
  static String initDelphiVar(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() +" := false;";
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      return field.useName() +" := Date;";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName() +" := 0.0;";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() +" := 0;";
    case Field.TIME:
      return field.useName() +" := Time;";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.BLOB:
    case Field.TLOB:
      return field.useName() +" := '';";
    }
    return field.useName() +"<unsupported>";
  }
  /**
  */
  static String delphiInputs(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return "Params.ParamByName('"+field.name+"').AsBoolean := "+field.useName()+";";
    case Field.DATE:
      return "Params.ParamByName('"+field.name+"').AsDateTime := "+field.useName()+";";
    case Field.DATETIME:
      return "Params.ParamByName('"+field.name+"').AsDateTime := "+field.useName()+";";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return "Params.ParamByName('"+field.name+"').AsFloat := "+field.useName()+";";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
      return "Params.ParamByName('"+field.name+"').AsInteger := "+field.useName()+";";
    case Field.IDENTITY:
    case Field.SEQUENCE:
      return "Params.ParamByName('"+field.name+"').AsInteger := "+field.useName()+";";
    case Field.TIME:
    case Field.TIMESTAMP:
      return "Params.ParamByName('"+field.name+"').AsDateTime := "+field.useName()+";";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
      return "Params.ParamByName('"+field.name+"').AsString := "+field.useName()+";";
    case Field.BLOB:
      return "Params.ParamByName('"+field.name+"').AsBlob := "+field.useName()+";";
    case Field.TLOB:
      return "Params.ParamByName('"+field.name+"').AsMemo := "+field.useName()+";";
    }
    return field.useName() +"<unsupported>";
  }
  /**
  */
//  static boolean notString(Field field)
//  {
//    switch(field.type)
//    {
//    case Field.BOOLEAN:
//    case Field.DATE:
//    case Field.DATETIME:
//    case Field.FLOAT:
//    case Field.DOUBLE:
//    case Field.MONEY:
//    case Field.BYTE:
//    case Field.SHORT:
//    case Field.INT:
//    case Field.LONG:
//    case Field.IDENTITY:
//    case Field.SEQUENCE:
//    case Field.TIME:
//    case Field.TIMESTAMP:
//    case Field.BLOB:
//    case Field.TLOB:
//      return true;
//    }
//    return false;
//  }
  /**
  */
  static String delphiOutputs(Field field, String with)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsBoolean;";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
    case Field.LONG:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsInteger;";
    case Field.DATE:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsDateTime;";
    case Field.DATETIME:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsDateTime;";
    case Field.FLOAT:
    case Field.DOUBLE:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsFloat;";
    case Field.MONEY:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsCurrency;";
    case Field.TIME:
    case Field.TIMESTAMP:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsDateTime;";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsString;";
    case Field.BLOB:
    case Field.TLOB:
      return field.useName() +" := "+with+".FieldByName('"+field.name+"').AsString;";
    }
    return field.useName() +"<unsupported>";
  }
  /**
  */
  static String delphiDataType(Field field)
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
}

