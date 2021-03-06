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
///    Caspar Lessing
/// ------------------------------------------------------------------

package bbd.jportal.old;

import bbd.jportal.*;
import bbd.jportal.Enum;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class AdoPythonCode extends Generator
{
  public static String description()
  {
    return "Generates ADO (OleDB) Python Code";
  }
  public static String documentation()
  {
    return "Generates ADO (OleDB) Python Code";
  }
  static String padder(String s, int length)
  {
    for (int i = s.length(); i < length-1; i++)
      s = s + " ";
    return s + " ";
  }
  static boolean aix;
  static boolean lowercase;
  static void setFlags(Database database, PrintWriter outLog)
  {
    lowercase = false;
    for (int i=0; i < database.flags.size(); i++) {
      String flag = database.flags.elementAt(i);
      if (flag.equalsIgnoreCase("lowercase"))
        lowercase = true;
    }
    if (lowercase)
      outLog.println(" (lowercase)");
  }
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    setFlags(database, outLog);
    for (int i=0; i<database.tables.size(); i++) {
      Table table = database.tables.elementAt(i);
      generate(table, output, outLog);
    }
  }
  static String fileName(String output, String node, String ext)
  {
    if (lowercase == true)
      node = node.toLowerCase();
    return output+node+ext;
  }
  static void generate(Table table, String output, PrintWriter outLog)
  {
    try {
      outLog.println("Code: "+fileName(output, table.useName(), ".py"));
      OutputStream outFile = new FileOutputStream(fileName(output, table.useName(), ".py"));
      try {
        PrintWriter outData = new PrintWriter(outFile);
        try {
          outData.println("# This code was generated, do not modify it, modify it at source and regenerate it.");
          outData.println("# "+table.useName()+".py");
          outData.println("import adoconnector");
          outData.println();
          generateEnums(table,outData);
          if (table.hasStdProcs)
            generateStdOutputRec(table, outData);
          generateUserOutputRecs(table, outData);
          generateInterface(table, outData);
          outData.flush();
        } finally {
          outData.flush();
        }
      } finally {
        outFile.close();
      }
    } catch (IOException e1) {
      outLog.println("Generate Procs IO Error");
    }
  }
  static void generateDataFields(Vector fields,String superName, String tableName, PrintWriter outData)
  {
    outData.println(" def __init__(self):");
    if (superName.length() > 0) {
      outData.println("  "+superName+".__init__(self)");
    }
    for (int i=0; i<fields.size(); i++) {
      Field field = (Field) fields.elementAt(i);

      for (int c=0; c < field.comments.size(); c++)
        outData.println("  #"+ field.comments.elementAt(c));
      outData.println("  self."+field.useName()+" = None");
      if (field.type == Field.BLOB || field.type == Field.TLOB)
        outData.println("  self."+field.useName()+"LOBLen = None");
    }
    outData.println(" def XML(self,Outer=\"D"+tableName.toUpperCase()+"\",Attr=''):");
    outData.println("  if len(Outer): s='<'+Outer+' '+Attr+'>'");
    outData.println("  else:          s=''");
    if (superName.length() > 0) {
      outData.println("  s += "+superName+".XML(self,'',Attr)");
    }
    for (int j=0; j<fields.size(); j++)
      outData.println("  "+xmlOf((Field) fields.elementAt(j)));
    outData.println("  if len(Outer): s+='</'+Outer+'>'");
    outData.println("  return s");
  }
  static void generateStdOutputRec(Table table, PrintWriter outData)
  {
    for (int i=0; i < table.comments.size(); i++) {
      String s = table.comments.elementAt(i);
      outData.println("//"+s);
    }
    outData.println("class D"+table.useName()+":");
    generateDataFields(table.fields,"",table.useName(),outData);

    outData.println();
    outData.println("O"+table.useName()+" = D"+table.useName());
    outData.println();
  }
  static void generateUserOutputRecs(Table table, PrintWriter outData)
  {
    for (int i=0; i<table.procs.size(); i++) {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData || proc.isStd  || proc.hasNoData())
        continue;
      String work = "";
      String superName = "";
      if (proc.outputs.size() > 0) {
        for (int j=0; j<proc.comments.size(); j++)
          outData.println("#"+ proc.comments.elementAt(j));
        String typeChar = "D";
        if (proc.hasDiscreteInput())
          typeChar = "O";
        work = " ("+typeChar+table.useName()+proc.upperFirst()+")";
        superName = typeChar+table.useName()+proc.upperFirst();
        outData.println("class "+typeChar+table.useName()+proc.upperFirst()+":");
        generateDataFields(proc.outputs,"",table.useName(),outData);
        outData.println();
      }
      if (proc.hasDiscreteInput()) {
        outData.println("class D"+table.useName()+proc.upperFirst()+work+":");
        Vector discreteInputs = new Vector();
        for (int j=0; j<proc.inputs.size(); j++) {
          Field field = proc.inputs.elementAt(j);
          if (!proc.hasOutput(field.name)) discreteInputs.addElement(field);
        }

        for (int j=0; j<proc.dynamics.size(); j++) {
          Field f = new Field();
          f.name = proc.dynamics.elementAt(j);
          f.type = Field.CHAR;
          discreteInputs.addElement(f);
        }

        generateDataFields(discreteInputs,superName,table.useName(),outData);
        // Broken xml gen on this one: fixed

        outData.println();
      } else if (proc.outputs.size() > 0) {
        outData.println("O"+table.useName()+proc.upperFirst()+" = D"+table.useName()+proc.upperFirst());
        outData.println();
      }
    }
  }
  static void generateEnums(Table table, PrintWriter outData)
  {
    for (int i=0; i<table.fields.size(); i++) {
      Field field = table.fields.elementAt(i);
      generateEnums(table.useName()+field.useName(), field, outData);
    }
    for (int i=0; i<table.procs.size(); i++) {
      Proc proc = table.procs.elementAt(i);
      if (!proc.isStd) {
        for (int j=0; j<proc.inputs.size(); j++) {
          Field field = proc.inputs.elementAt(j);
          generateEnums(table.useName()+proc.name+field.useName(), field, outData);
        }
        for (int j=0; j<proc.outputs.size(); j++) {
          Field field = proc.outputs.elementAt(j);
          generateEnums(table.useName()+proc.name+field.useName(), field, outData);
        }
      }
    }
  }
  static void generateEnums(String baseName, Field field, PrintWriter outData)
  {
    if (field.enums.size() > 0) {
      for (int j=0; j<field.enums.size(); j++) {
        bbd.jportal.Enum element = field.enums.elementAt(j);
        outData.println(baseName+element.name+ " = "+element.value);
      }

      outData.println("def "+baseName+"Lookup(no):");
      String start = " if (no == ";
      for (int j=0; j<field.enums.size(); j++) {
        Enum element = field.enums.elementAt(j);
        outData.println(start+baseName+element.name+ "): return '"+element.name+"'");
        start = " elif (no == ";
      }
      outData.println(" else: return \"<n/a>\";");
      outData.println();
    }
  }
  static void generateInterface(Table table, PrintWriter outData)
  {
    for (int i=0; i<table.procs.size(); i++) {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      generateInterface(table, proc, outData);
    }
  }
  static void generateInterface(Table table, Proc proc, PrintWriter outData)
  {
    String dataStruct;
    for (int i=0; i<proc.comments.size(); i++)
      outData.println("  #"+ proc.comments.elementAt(i));
    if (proc.hasNoData()) {
      outData.println("class T"+table.useName()+proc.upperFirst()+":");
      outData.println(" def __init__(self,connector):");
      outData.println("  self.conn = connector");
      outData.println("  self.qry  = adoconnector.TQuery()");
      outData.println(" def Exec(self):");
      generateCommand(proc,outData);
      outData.println("  self.qry.Open(self.conn,command)");
      outData.println("  self.qry.Exec()");
      outData.println();
    } else {
      if (proc.isStd)
        dataStruct = "D"+table.useName();
      else
        dataStruct = "D"+table.useName()+proc.upperFirst();
      outData.println("class T"+table.useName()+proc.upperFirst()+"("+dataStruct+"):");
      generateInterface(table, proc, dataStruct, outData);
      outData.println();
    }
  }
  static void generateInterface(Table table, Proc proc, String dataStruct, PrintWriter outData)
  {

    outData.println(" def __init__(self,connector):");
    outData.println("  "+dataStruct+".__init__(self)");
    outData.println("  self.conn = connector");
    outData.println("  self.qry  = adoconnector.TQuery()");
    outData.println(" def Clear(self):");
    outData.println("  "+dataStruct+".__init__(self)");
    outData.println(" def Close(self):");
    outData.println("  self.qry.Close()");
    outData.println(" def Exec(self):");
    generateCommand(proc,outData);
    outData.println("  self.qry.Open(self.conn,command)");
    generatePuts(proc, table, outData);
    outData.println("  self.qry.Exec()");
    generateParmPut(proc, table, outData);
    if (proc.outputs.size() > 0) {
      outData.println(" def Fetch(self):");
      outData.println("  if self.qry.EndOfFile():");
      outData.println("   return 0");
      generateGets(proc,outData);
      outData.println("  self.qry.Next()");
      outData.println("  return 1");
      outData.println();
    }
  }
  static void generateGets(Proc proc,PrintWriter outData)
  {
    for (int j=0; j<proc.outputs.size(); j++) {
      Field field = proc.outputs.elementAt(j);
      if (field.isNull) {
        outData.println("  if self.qry.isNull("+j+"): self."+field.useName()+" = None");
        outData.print  ("  else: ");
      }
      // Field.XML support not present
      switch (field.type) {
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
      case Field.SEQUENCE:
        outData.println("  self."+field.useName()+" = self.qry.getInt("+j+")");
        break;
      case Field.IDENTITY:
      case Field.LONG:
        outData.println("  self."+field.useName()+" = self.qry.getLong("+j+")");
        break;
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.USERSTAMP:
      case Field.BLOB:
      case Field.TLOB:
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
        outData.println("  self."+field.useName()+" = self.qry.getString("+j+")");
        break;
      case Field.FLOAT:
      case Field.DOUBLE:
      case Field.MONEY:
        outData.println("  self."+field.useName()+" = self.qry.getDouble("+j+")");
        break;
      }
    }
  }
  static Vector generatePutFields(Proc proc) {
    Vector holders = new Vector();
    if (proc.placeHolders.size() > 0) {
      for (int j=0; j<proc.placeHolders.size(); j++) {
        String s = proc.placeHolders.elementAt(j);
        int n = proc.indexOf(s);
        if (n < 0)
          holders.addElement("("+s+" is not an input)");
        else
          holders.addElement(proc.inputs.elementAt(n));
      }
    } 
    else
    {
      for (int j=0; j<proc.inputs.size(); j++) {
        holders.addElement( proc.inputs.elementAt(j) );
      }
    }
    return holders;
  }
  static void generateParmPut(Proc proc,Table table,PrintWriter outData) {
    Vector holders = proc.inputs;
    if (holders.size() == 0) {
      return;
    }
    String t = "";
    for (int i = 0; i < holders.size(); i++) {
      Field f = (Field)holders.elementAt(i);
      if ((f.type != Field.SEQUENCE && f.type != Field.TIMESTAMP && f.type != Field.USERSTAMP) || !proc.isInsert) {
        t += ", "+f.useName();
      }
    }
    for (int j=0; j<proc.dynamics.size(); j++) {
      t += ", "+ proc.dynamics.elementAt(j);
    }
    outData.println(" def ParmExec(self"+t+"):");
    for (int i = 0; i < holders.size(); i++) {
      Field f = (Field)holders.elementAt(i);
      if ((f.type != Field.SEQUENCE && f.type != Field.TIMESTAMP && f.type != Field.USERSTAMP) || !proc.isInsert) {
        outData.println("  self."+f.useName()+" = "+f.useName());
      }
    }
    for (int j=0; j<proc.dynamics.size(); j++) {
      String s = proc.dynamics.elementAt(j);
      outData.println("  self."+s+" = "+s);
    }
    outData.println("  self.Exec()");
  }
  static void generatePuts(Proc proc,Table table,PrintWriter outData)
  {
    Vector holders = generatePutFields(proc);
    for (int j=0; j<holders.size(); j++) {
      Field field = (Field) holders.elementAt(j);
      if (field.type == Field.SEQUENCE && proc.isInsert)
        outData.println("  self."+field.useName()+" = self.qry.conn.GetSequence(\""+table.name+"\");");
      else if (field.type == Field.TIMESTAMP)
        outData.println("  self."+field.useName()+" = self.qry.conn.GetTimeStamp();");
      else if (field.type == Field.USERSTAMP)
        outData.println("  self."+field.useName()+" = self.qry.conn.GetUserStamp("+field.useName()+");");
      String NullCheck = ", 0";
      if (field.isNull) NullCheck = ", self."+field.useName()+"==None";
      // Field.XML support not present
      switch (field.type) {
      case Field.SEQUENCE:
      case Field.BOOLEAN:
      case Field.BYTE:
      case Field.SHORT:
      case Field.INT:
        if (field.isNull) {
          outData.println("  if self."+field.useName()+":self.qry.setInt('"+field.useName()+"', self."+field.useName() +", 0)");
          outData.println("  else: self.qry.setInt('"+field.useName()+"', 0,1)");
        } else
          outData.println("  self.qry.setInt('"+field.useName()+"', self."+field.useName() +",0)");
        break;
      case Field.IDENTITY:
      case Field.LONG:
        outData.println("  self.qry.setLong('"+field.useName()+"', self."+field.useName() +NullCheck+")");
        break;
      case Field.CHAR:
      case Field.ANSICHAR:
      case Field.USERSTAMP:
      case Field.BLOB:
      case Field.TLOB:
      case Field.DATE:
      case Field.TIME:
      case Field.DATETIME:
      case Field.TIMESTAMP:
        if (field.isNull) {
          outData.println("  if self."+field.useName()+":self.qry.setString('"+field.useName()+"', str(self."+field.useName() +"), 0)");
          outData.println("  else: self.qry.setString('"+field.useName()+"', '',1)");
        } else
          outData.println("  self.qry.setString('"+field.useName()+"', str(self."+field.useName() +"),0)");
        break;
      case Field.FLOAT:
      case Field.DOUBLE:
      case Field.MONEY:
        outData.println("  self.qry.setDouble('"+field.useName()+"', self."+field.useName() +NullCheck+")");
        break;
      }
    }
  }
  static int checkPlaceHolders(Proc proc, PrintWriter outData, String l, int phIndex)
  {
    if (phIndex >= proc.placeHolders.size()) {
      outData.print(l);
      return phIndex;
    }
    int n = 0;
    while (phIndex < proc.placeHolders.size()) {
      String placeHolder = ":" + proc.placeHolders.elementAt(phIndex);
      n = l.indexOf(placeHolder);
      if (n == -1)
        break;
      phIndex++;
      String work = "";
      if (n > 0)
        work = work + l.substring(0, n);
      work = work + "?";
      n += placeHolder.length();
      if (n < l.length());
      work = work + l.substring(n);
      l = work;
    }
    outData.print(l);
    return phIndex;
  }
  static void generateCommand(Proc proc, PrintWriter outData)
  {
    outData.println("  command = ''");
    if (proc.lines.size() > 0) {
      String strcat = "  command += ";
      String tail = "";
      int phIndex = 0;
      for (int i=0; i<proc.lines.size(); i++) {
        Line l = proc.lines.elementAt(i);
        if (l.isVar) {
          tail = "";
          if (i != 0)
            outData.println(tail);
          strcat = "  command += ";
          outData.print(strcat+"self."+l.line);
        } else {
          if (i != 0)
            outData.println(tail);
          tail = "";
          phIndex = checkPlaceHolders(proc, outData, strcat+"\""+l.line+"\"", phIndex);
          strcat = "  command += ";
        }
      }
    }
    outData.println();
  }
  static String xmlOf(Field field)
  {
    String result="s +='<"+field.useName().toUpperCase()+">'+";
    // Field.XML support not present
    switch (field.type) {
    case Field.BOOLEAN:
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.SEQUENCE:
    case Field.IDENTITY:
    case Field.LONG:
      result=result+"str(self."+field.useName()+")";
      break;
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.BLOB:
    case Field.TLOB:
    case Field.USERSTAMP:
    case Field.DATE:
    case Field.TIME:
    case Field.DATETIME:
    case Field.TIMESTAMP:
      result=result+"str(self."+field.useName()+").replace('&','&amp;').replace('<','&lt;').replace('>','&gt;')";
      break;
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      result=result+"str(self."+field.useName()+")";
      break;
    }
    result+="+'<"+field.useName().toUpperCase()+">'";
    return result;
  }
}
