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
/// $Date: 2004/10/18 13:45:51 $
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
import java.util.Vector;

public class VBCode2 extends Generator
{
  public static String description()
  {
    return "Generate VB aDO Code";
  }
  public static String documentation()
  {
	return "Generates ADO (OleDB) VB Code\r\n"
		  +"If has flag 'io routines' then will generate get and put files "
		  +"for the records.";
  }
  protected static Vector flagsVector;
  static boolean ioRoutines;
  private static void flagDefaults()
  {
    ioRoutines = false;
  }
  public static Vector flags()
  {
    if (flagsVector == null)
    {
      flagsVector = new Vector();
      flagDefaults();
      flagsVector.addElement(new Flag("io routines", new Boolean (ioRoutines), "Generate IO Routines"));
    }
    return flagsVector;
  }
  static void setFlags(Table table, PrintWriter outLog)
  {
    if (flagsVector != null)
      ioRoutines = toBoolean (((Flag)flagsVector.elementAt(0)).value);
    else
      flagDefaults();
    for (int i=0; i < table.options.size(); i++)
    {
      String option = table.options.elementAt(i);
      if (option.equalsIgnoreCase("io routines"))
        ioRoutines = true;
    }
    if (ioRoutines)
      outLog.println(" (io routines)");
  }
  /**
  * Generates the procedure classes for each table present.
  */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    for (int i=0; i < database.tables.size(); i++)
    {
      Table table = database.tables.elementAt(i);
      setFlags(table, outLog);
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
      outLog.println("Code: "+output+table.useName() + ".cls");
      OutputStream outFile = new FileOutputStream(output+table.useName() + ".cls");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        generateStd(table, outData);
        generateOther(table, output, outLog);
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
  public static void generateStd(Table table, PrintWriter outData)
  {
    outData.println("VERSION 1.0 CLASS");
    outData.println("BEGIN");
    outData.println("  MultiUse = -1");
    outData.println("END");
    outData.println("ATTRIBUTE VB_NAME=\"T"+table.useName()+"\"");
    outData.println("Option explicit");
    outData.println("' This code was generated, do not modify it, modify it at source and regenerate it.");
    for (int i=0; i < table.comments.size(); i++)
    {
      String s = table.comments.elementAt(i);
      outData.println("'"+s);
    }
    boolean hasMulti = false;
    boolean hasInput = false;
    for (int i=0; i< table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData
      ||  proc.hasNoData()
      ||  proc.isStd == false)
        continue;
      if (proc.inputs.size() > 0)
        hasInput = true;
      if (proc.outputs.size() == 0
      ||  proc.isSingle == true)
        continue;
      hasMulti = true;
      outData.println("Private "+proc.name+"Cursor As TCursor");
    }
    outData.println();
    outData.println("Private Type TRec");
    if (hasMulti)
      outData.println("  TagFlag as Variant");
    generateRec(table.fields, outData);
    outData.println("End Type");
    outData.println();
    if (hasInput)
      outData.println("Private SaveRec as TRec");
    if (hasMulti)
    {
      outData.println("Private Rec() as TRec");
      outData.println("Public RecCount as Long");
    }
    else
      outData.println("Private Rec as TRec");
    outData.println();
    generateCopy(table.useName(), outData, hasMulti);
    String optZero = hasMulti ? "(0)" : "";
    String optIndex = hasMulti ? "Optional aIndex as Long" : "";
    String optIndex1 = hasMulti ? ", Optional aIndex as Long" : "";
    String optIndex2 = hasMulti ? "Optional aIndex as Long, " : "";
    String optUseIndex = hasMulti ? "(aIndex)" : "";
    outData.println("Private Sub Class_Initialize()");
    outData.println("  RecClear");
    for (int i=0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      outData.println("  Rec"+optZero+"."+initVBVar(field));
      if (field.isNull) // && notString(field))
         outData.println("  Rec"+optZero+"."+field.useName()+"IsNull = false");
    }
    outData.println("End Sub");
    outData.println();
    if (hasInput)
    {
      outData.println("Public Sub SaveCopy("+optIndex+")");
      outData.println("  SaveRec = Rec"+optUseIndex);
      outData.println("End Sub");
      outData.println();
      outData.println("Public Sub RestoreCopy("+optIndex+")");
      outData.println("  Rec"+optUseIndex+" = SaveRec");
      outData.println("End Sub");
      outData.println();
    }
    if (ioRoutines)
    {
      outData.println("Public Sub Class_PutFile(OutFile as Long"+optIndex1+")");
      outData.println("  Put OutFile,, Rec"+optUseIndex);
      outData.println("End Sub");
      outData.println();
      outData.println("Public Sub Class_GetFile(InFile as Long"+optIndex1+")");
      outData.println("  Get InFile,, Rec"+optUseIndex);
      outData.println("End Sub");
      outData.println();
    }
    if (hasMulti)
    {
      outData.println("Public Property Get TagFlag(Optional aIndex as Long = 0) as Variant");
      outData.println("  TagFlag = Rec(aIndex).TagFlag");
      outData.println("End Property");
      outData.println();
      outData.println("Public Property Let TagFlag(Optional aIndex as Long = 0, aValue as Variant)");
      outData.println("  Rec(aIndex).TagFlag = aValue");
      outData.println("End Property");
      outData.println();
    }
    for (int i=0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      generateProperties(field, outData, optIndex, optIndex2, optUseIndex);
    }
    for (int i=0; i< table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        continue;
      if (proc.hasNoData())
        emitCode(proc, outData, table.useName(), "", "");
      else if (proc.isStd)
        emitCode(proc, outData, table.useName(), optIndex1, optUseIndex);
    }
  }

  static void generateCopy(String name, PrintWriter outData, boolean hasMulti)
  {
    String optIndex = hasMulti ? "Optional aIndex as Long" : "";
    String optIndex1 = hasMulti ? "Optional aDest as Long, Optional aSource as Long, " : "";
    String optUseIndex = hasMulti ? "(aIndex)" : "";
    String optDestIndex = hasMulti ? "(aDest)" : "";
    String optSourceIndex = hasMulti ? "(aSource)" : "";
    outData.println("Friend Property Get ZZQI("+optIndex+") As TRec");
    outData.println("  ZZQI = Rec"+optUseIndex);
    outData.println("End Property");
    outData.println();
    outData.println("Friend Property Let Copy("+optIndex1+"aClass as T"+name+")");
    outData.println("  Rec"+optDestIndex+" = aClass.ZZQI"+optSourceIndex);
    outData.println("End Property");
    outData.println();
    if (hasMulti)
    {
      outData.println("Public Sub RecClear()");
      outData.println("  ReDim Preserve Rec(0 to 0)");
      outData.println("  RecCount = 0");
      outData.println("End Sub");
      outData.println();
      outData.println("Public Sub Resize(aSize As Long)");
      outData.println("  ReDim Preserve Rec(0 to aSize)");
      outData.println("  RecCount = aSize");
      outData.println("End Sub");
      outData.println();
      outData.println("Public Sub Swap(A As Long, B As Long)");
      outData.println("  Dim Temp As TRec");
      outData.println("  Temp = Rec(A)");
      outData.println("  Rec(A) = Rec(B)");
      outData.println("  Rec(B) = Temp");
      outData.println("End Sub");
      outData.println();
    }
  }

  static void generateProperties(Field field, PrintWriter outData, String optIndex, String optIndex2, String optUseIndex)
  {
    outData.println("Public Property Get "+getPropertyType(field, optIndex));
    outData.println("  "+field.useName()+" = Rec"+optUseIndex+"."+field.useName());
    outData.println("End Property");
    outData.println();
    outData.println("Public Property Let "+setPropertyType(field, optIndex2));
    outData.println("  Rec"+optUseIndex+"."+field.useName()+" = aValue");
    outData.println("End Property");
    outData.println();
    if (field.isNull) // && notString(field))
    {
      outData.println("Public Property Get "+field.useName()+"IsNull("+optIndex+") as Boolean");
      outData.println("  "+field.useName()+"IsNull = Rec"+optUseIndex+"."+field.useName()+"IsNull");
      outData.println("End Property");
      outData.println();
      outData.println("Public Property Let "+field.useName()+"IsNull("+optIndex2+"aValue as Boolean)");
      outData.println("  Rec"+optUseIndex+"."+field.useName()+"IsNull = aValue");
      outData.println("End Property");
      outData.println();
    }
  }
  /**
  * Build of all required standard procedures
  */
  static void generateOther(Table table, String output, PrintWriter outLog)
  {
    for (int i=0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData || proc.isStd || proc.hasNoData())
        continue;
      try
      {
        outLog.println("Code: "+output+table.useName()+proc.upperFirst()+".cls");
        OutputStream outFile = new FileOutputStream(output+table.useName()+proc.upperFirst()+".cls");
        try
        {
          PrintWriter outData = new PrintWriter(outFile);
          outData.println("VERSION 1.0 CLASS");
          outData.println("BEGIN");
          outData.println("  MultiUse = -1");
          outData.println("END");
          outData.println("ATTRIBUTE VB_NAME=\"t"+table.useName()+proc.upperFirst()+"\"");
          outData.println("Option explicit");
          outData.println("' This code was generated, do not modify it, modify it at source and regenerate it.");
          for (int j=0; j<proc.comments.size(); j++)
          {
            String comment = proc.comments.elementAt(j);
            outData.println("'"+comment);
          }
          boolean hasMulti = true;
          boolean hasInput = proc.inputs.size() > 0;
          if (proc.outputs.size() == 0
          ||  proc.isSingle == true)
            hasMulti = false;
          outData.println();
          outData.println("Private Type TRec");
          if (hasMulti)
            outData.println("  TagFlag as Variant");
          for (int j=0; j<proc.inputs.size(); j++)
          {
            Field field = proc.inputs.elementAt(j);
            String io;
            if (!proc.hasOutput(field.name))
              io = "input";
            else
              io = "input-output";
            outData.println("  "+varType(field)+" '"+io);
            for (int c=0; c < field.comments.size(); c++)
            {
              String s = field.comments.elementAt(c);
              outData.println("'"+s);
            }
            if (field.isNull) // && notString(field))
              outData.println("  "+field.useName()+"IsNull as Boolean");
          }
          for (int j=0; j<proc.outputs.size(); j++)
          {
            Field field = proc.outputs.elementAt(j);
            if (!proc.hasInput(field.name))
            {
              outData.println("  "+varType(field)+" 'output");
              for (int c=0; c < field.comments.size(); c++)
              {
                String s = field.comments.elementAt(c);
                outData.println("'"+s);
              }
            if (field.isNull) // && notString(field))
              outData.println("  "+field.useName()+"IsNull as Boolean");
            }
          }
          for (int j=0; j<proc.dynamics.size(); j++)
          {
            String s = proc.dynamics.elementAt(j);
            outData.println("  "+s+" As String 'dynamic");
          }
          outData.println("End Type");
          outData.println();
          if (hasInput)
            outData.println("Private SaveRec as TRec");
          if (hasMulti)
          {
            outData.println("Private Rec() as TRec");
            outData.println("Public RecCount as Long");
            outData.println("Private "+proc.name+"Cursor As TCursor");
          }
          else
            outData.println("Private Rec as TRec");
          outData.println();
          generateCopy(table.useName()+proc.upperFirst(), outData, hasMulti);
          String optZero = hasMulti ? "(0)" : "";
          String optIndex = hasMulti ? "Optional aIndex as Long" : "";
          String optIndex1 = hasMulti ? ", Optional aIndex as Long" : "";
          String optIndex2 = hasMulti ? "Optional aIndex as Long, " : "";
          String optUseIndex = hasMulti ? "(aIndex)" : "";
          outData.println("Private Sub Class_Initialize()");
          if (hasMulti)
            outData.println("  RecClear");
          for (int j=0; j<proc.inputs.size(); j++)
          {
            Field field = proc.inputs.elementAt(j);
            outData.println("  Rec"+optZero+"."+initVBVar(field));
            if (field.isNull) // && notString(field))
               outData.println("  Rec"+optZero+"."+field.useName()+"IsNull = false");
          }
          for (int j=0; j<proc.outputs.size(); j++)
          {
            Field field = proc.outputs.elementAt(j);
            if (proc.hasInput(field.name))
              continue;
            outData.println("  Rec"+optZero+"."+initVBVar(field));
            if (field.isNull) // && notString(field))
               outData.println("  Rec"+optZero+"."+field.useName()+"IsNull = false");
          }
          for (int j=0; j<proc.dynamics.size(); j++)
          {
            String s = proc.dynamics.elementAt(j);
            outData.println("  Rec"+optZero+"."+s+" = \"\"");
          }
          outData.println("End Sub");
          outData.println();
          if (hasInput)
          {
            outData.println("Public Sub SaveCopy("+optIndex+")");
            outData.println("  SaveRec = Rec"+optUseIndex);
            outData.println("End Sub");
            outData.println();
            outData.println("Public Sub RestoreCopy("+optIndex+")");
            outData.println("  Rec"+optUseIndex+" = SaveRec");
            outData.println("End Sub");
            outData.println();
          }
          if (ioRoutines)
          {
            outData.println("Public Sub Class_PutFile(OutFile as Long"+optIndex1+")");
            outData.println("  Put OutFile,, Rec"+optUseIndex);
            outData.println("End Sub");
            outData.println();
            outData.println("Public Sub Class_GetFile(InFile as Long"+optIndex1+")");
            outData.println("  Get InFile,, Rec"+optUseIndex);
            outData.println("End Sub");
            outData.println();
          }
          if (hasMulti)
          {
            outData.println("Public Property Get TagFlag(Optional aIndex as Long = 0) as Variant");
            outData.println("  TagFlag = Rec(aIndex).TagFlag");
            outData.println("End Property");
            outData.println();
            outData.println("Public Property Let TagFlag(Optional aIndex as Long = 0, aValue as Variant)");
            outData.println("  Rec(aIndex).TagFlag = aValue");
            outData.println("End Property");
            outData.println();
          }
          for (int j=0; j<proc.inputs.size(); j++)
          {
            Field field = proc.inputs.elementAt(j);
            generateProperties(field, outData, optIndex, optIndex2, optUseIndex);
          }
          for (int j=0; j<proc.outputs.size(); j++)
          {
            Field field = proc.outputs.elementAt(j);
            if (proc.hasInput(field.name))
              continue;
            generateProperties(field, outData, optIndex, optIndex2, optUseIndex);
          }
          for (int j=0; j<proc.dynamics.size(); j++)
          {
            String s = proc.dynamics.elementAt(j);
            outData.println("Public Property Get "+s+"("+optIndex+") as String");
            outData.println("  "+s+" = Rec"+optUseIndex+"."+s);
            outData.println("End Property");
            outData.println();
            outData.println("Public Property Let "+s+"("+optIndex2+"aValue as String)");
            outData.println("  Rec"+optUseIndex+"."+s+" = aValue");
            outData.println("End Property");
            outData.println();
          }
          emitCode(proc, outData, table.useName(), optIndex1, optUseIndex);
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
  }
  /**
  * Build's a VB Record
  */
  static void generateRec(Vector fields, PrintWriter outData)
  {
    for (int i=0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      outData.println("  "+varType(field));
      if (field.comments.size() > 0)
      {
        for (int c=0; c < field.comments.size(); c++)
        {
          String s = field.comments.elementAt(c);
          outData.println("'"+s);
        }
      }
      if (field.isNull) // && notString(field))
        outData.println("  "+field.useName()+"IsNull as Boolean");
    }
  }
//  static void emitSpecialInputCode(Proc proc, int index, PrintWriter outData, String tableName, String optUseIndex)
//  {
//    Field field = (Field) proc.inputs.elementAt(index);
//    if (proc.isInsert)
//    {
//      if (field.isSequence)
//        outData.println("  Rec"+optUseIndex+"."+field.useName()+" = getSequence(\""+tableName+"\") ' User supplied Function for Sequences");
//    }
//    if (field.type == field.TIMESTAMP)
//      outData.println("  Rec"+optUseIndex+"."+field.useName()+" = getTimeStamp ' User supplied Function for Time Stamp");
//    if (field.type == field.USERSTAMP)
//      outData.println("  Rec"+optUseIndex+"."+field.useName()+" = getUserStamp ' User supplied Function for User Stamp");
//  }
  /**
  * Emit function for processing the inputs
  */
  static int emitInputCode(Proc proc, int index, int pos, PrintWriter outData, String optUseIndex)
  {
    Field field = proc.inputs.elementAt(index);
    if (field.type == Field.IDENTITY && proc.isInsert)
      return pos;
    if (field.isNull) // && notString(field))
      outData.println("  QD!Parameter"+ (++pos) + " = IIf(Rec"+optUseIndex+"."+field.useName()+"IsNull, Null, Rec"+optUseIndex+"."+field.useName()+")");
    else
      outData.println("  QD!Parameter"+ (++pos) + " = Rec"+optUseIndex+"."+field.useName());
    return pos;
  }
  /**
  * Emits functions for processing the database activity
  */
  static void emitCode(Proc proc, PrintWriter outData, String tableName, String optIndex, String optUseIndex)
  {
    if (proc.outputs.size() == 0)
    {
      outData.println("Public Sub "+proc.upperFirst()+"(Connect as TConnect"+optIndex+")");
    }
    else if (proc.isSingle)
      outData.println("Public Function "+proc.upperFirst()+"(Connect as TConnect"+optIndex+") as Boolean");
    else
    {
      outData.println("Public Sub "+proc.upperFirst()+"(Connect as TConnect)");
      if (optUseIndex != "") optUseIndex = "(0)";
    }
    if (proc.comments.size() > 0)
    {
      for (int i=0; i < proc.comments.size(); i++)
      {
        String comment = proc.comments.elementAt(i);
        outData.println("'"+comment);
      }
    }
    if (proc.outputs.size() == 0)
    {
      outData.println("' Returns no output.");
    }
    else if (proc.isSingle)
    {
      outData.println("' Returns at most one record.");
      outData.println("' Returns true if a record is found");
      outData.println("  Dim RS as RecordSet");
      outData.println("  "+proc.upperFirst()+" = False");
    }
    else
    {
      outData.println("' Returns any number of records.");
      outData.println("  Set "+proc.upperFirst()+"Cursor = new TCursor");
    }
    outData.println("  Connect.RoutineName = \""+proc.upperFirst()+"\"");
    outData.println("  Dim QD as QueryDef");
    for (int index=0; index < proc.inputs.size(); index++)
//      emitSpecialInputCode(proc, index, outData, tableName, optUseIndex);
    {
      Field field = proc.inputs.elementAt(index);
      if (proc.isInsert)
      {
        if (field.isSequence)
          outData.println("  Rec"+optUseIndex+"."+field.useName()+" = getSequence(\""+tableName+"\") ' User supplied Function for Sequences");
      }
      if (field.type == Field.TIMESTAMP)
        outData.println("  Rec"+optUseIndex+"."+field.useName()+" = getTimeStamp ' User supplied Function for Time Stamp");
      if (field.type == Field.USERSTAMP)
        outData.println("  Rec"+optUseIndex+"."+field.useName()+" = getUserStamp ' User supplied Function for User Stamp");
    }
    outData.print("  Set QD = Connect.DB.CreateQueryDef(\"\", ");
    String vbline = "";
    for (int i=0; i < proc.lines.size(); i++)
    {
      String x = "";
      if (i+1 < proc.lines.size())
        x = " & ";
      Line l = proc.lines.elementAt(i);
      if (l.isVar)
        vbline = vbline +l.line+x;
      else
        vbline = vbline + "\""+l.line+"\""+x;
    }
    int p;
    while ((p = vbline.indexOf("\" & \"")) > -1)
      vbline = vbline.substring(0, p) + vbline.substring(p+5);
    if (proc.placeHolders.size() > 0)
    {
      for (int i = 0; i < proc.placeHolders.size(); i++)
      {
        String placeHolder = ":" + proc.placeHolders.elementAt(i);
        String work = "";
        int n = vbline.indexOf(placeHolder);
        if (n == -1)
        {
          outData.println("Error with placeholders "+placeHolder);
          break;
        }
        if (n > 0)
          work = vbline.substring(0, n);
        work = work + "?";
        n += placeHolder.length();
        if (n < vbline.length());
          work = work + vbline.substring(n);
        vbline = work;
      }
      outData.println(vbline + ")");
      for (int i=0, pos=0; i < proc.placeHolders.size(); i++)
      {
        String placeHolder = proc.placeHolders.elementAt(i);
        int index =  proc.indexOf(placeHolder);
        pos = emitInputCode(proc, index, pos, outData, optUseIndex);
      }
    }
    else
    {
      outData.println(vbline + ")");
      for (int index=0, pos=0; index < proc.inputs.size(); index++)
        pos = emitInputCode(proc, index, pos, outData, optUseIndex);
//      {
//        Field field = (Field) proc.inputs.elementAt(i);
//        if (field.type == field.IDENTITY && proc.isInsert)
//          continue;
//        if (field.isNull && notString(field))
//          outData.println("  QD!Parameter"+ (++n) + " = IIf(Rec"+optUseIndex+"."+field.useName()+"IsNull, Null, Rec"+optUseIndex+"."+field.useName()+")");
//        else
//          outData.println("  QD!Parameter"+ (++n) + " = Rec"+optUseIndex+"."+field.useName());
//      }
    }
    if (proc.outputs.size() == 0)
    {
      outData.println("  QD.execute");
      outData.println("  QD.Close");
    }
    else if (proc.isSingle)
    {
      outData.println("  Set RS = QD.OpenRecordSet(dbOpenSnapShot, dbForwardOnly and dbReadOnly)");
      outData.println("  If Not RS.eof Then");
      for (int i=0; i < proc.outputs.size(); i++)
      {
        Field field = proc.outputs.elementAt(i);
        if (field.isNull) // && notString(field))
        {
					outData.println("    Rec"+optUseIndex+"."+initVBVar(field));
          outData.println("    Rec"+optUseIndex+"."+field.useName()+"IsNull = isNull(RS!"+field.name+")");
          outData.println("    If Not Rec"+optUseIndex+"."+field.useName()+"IsNull Then Rec"+optUseIndex+"."+field.useName()+" = RS!"+field.name);
        }
        else
          outData.println("    Rec"+optUseIndex+"."+field.useName()+" =  RS!"+field.name);
      }
      outData.println("    "+proc.upperFirst()+" = True");
      outData.println("  End If");
      outData.println("  RS.Close");
      outData.println("  QD.Close");
    }
    else
    {
      outData.println("  Set "+proc.upperFirst()+"Cursor.Connect = Connect");
      outData.println("  Set "+proc.upperFirst()+"Cursor.QD = QD");
      outData.println("  Set "+proc.upperFirst()+"Cursor.RS = "+proc.upperFirst()+"Cursor.QD.openRecordSet(dbOpenSnapShot, dbForwardOnly and dbReadOnly)");
    }
    if (proc.isSingle)
      outData.println("End Function");
    else
      outData.println("End Sub");
    outData.println();
    if (proc.outputs.size() > 0 && !proc.isSingle)
    {
      outData.println("Public Function next"+proc.upperFirst()+"() as Boolean");
      outData.println("' Returns true if a record is found");
      outData.println("  If "+proc.upperFirst()+"Cursor Is Nothing Then Err.Raise 1998, \""+proc.upperFirst()+"\"");
      outData.println("  "+proc.upperFirst()+"Cursor.Connect.RoutineName = \"next"+proc.upperFirst()+"\"");
      outData.println("  next"+proc.upperFirst()+" = False");
      outData.println("  If Not "+proc.upperFirst()+"Cursor.RS.eof Then");
      for (int i=0; i<proc.outputs.size(); i++)
      {
        Field field = proc.outputs.elementAt(i);
        if (field.isNull) // && notString(field))
        {
					outData.println("    Rec"+optUseIndex+"."+initVBVar(field));
          outData.println("    Rec"+optUseIndex+"."+field.useName()+"IsNull = isNull("+proc.upperFirst()+"Cursor.RS!"+field.name+")");
          outData.println("    If Not Rec"+optUseIndex+"."+field.useName()+"IsNull Then Rec"+optUseIndex+"."+field.useName()+" = "+proc.upperFirst()+"Cursor.RS!"+field.name);
        }
        else
          outData.println("    Rec"+optUseIndex+"."+field.useName()+" = "+proc.upperFirst()+"Cursor.RS!"+field.name);
      }
      outData.println("    next"+proc.upperFirst()+" = True");
      outData.println("    "+proc.upperFirst()+"Cursor.RS.MoveNext");
      outData.println("  Else");
      outData.println("    "+proc.upperFirst()+"Cursor.RS.Close");
      outData.println("  End If");
      outData.println("End Function");
      outData.println();
      outData.println("Public Sub cancel"+proc.upperFirst()+"()");
      outData.println("' You must call call this when you terminate the fetch loop prematurly");
      outData.println("' but you must call this only if you do");
      outData.println("  if "+proc.upperFirst()+"Cursor.isOpen then "+proc.upperFirst()+"Cursor.Done");
      outData.println("End Sub");
      outData.println();
      outData.println("Public Sub load"+proc.upperFirst()+"(Connect as TConnect, Optional Max As Long)");
      outData.println("  RecClear");
      outData.println("  "+proc.upperFirst()+" Connect");
      outData.println("  Do While next"+proc.upperFirst()+"()");
      outData.println("    RecCount = RecCount + 1");
      outData.println("    ReDim Preserve Rec(0 to RecCount)");
      outData.println("    Rec(RecCount) = Rec(0)");
      outData.println("    If RecCount = Max Then");
      outData.println("      cancel"+proc.upperFirst()+"");
      outData.println("      Exit Do");
      outData.println("    End If");
      outData.println("  Loop");
      outData.println("End Sub");
      outData.println();
    }
  }
//  static boolean notString(Field field)
//  {
//    switch(field.type)
//    {
//    case Field.BOOLEAN:
//    case Field.BYTE:
//    case Field.SHORT:
//    case Field.INT:
//    case Field.LONG:
//    case Field.SEQUENCE:
//    case Field.IDENTITY:
//    case Field.BLOB:
//  //case Field.DATE:
//  //case Field.DATETIME:
//  //case Field.TIME:
//  //case Field.TIMESTAMP:
//    case Field.FLOAT:
//    case Field.DOUBLE:
//    case Field.MONEY:
//      return true;
//    }
//    return false;
//  }
  static String varType(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() + " as Boolean";
    case Field.BYTE:
      return field.useName() + " as Byte";
    case Field.SHORT:
      return field.useName() + " as Integer";
    case Field.INT:
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() + " as Long";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.TLOB:
    case Field.BLOB:
      return field.useName() + " as String";
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIME:
    case Field.TIMESTAMP:
      return field.useName() + " as Date";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName() + " as Double";
    }
    return "as unsupported";
  }
  static String getPropertyType(Field field, String optIndex)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() + "("+optIndex+") as Boolean";
    case Field.BYTE:
      return field.useName() + "("+optIndex+") as Byte";
    case Field.SHORT:
      return field.useName() + "("+optIndex+") as Integer";
    case Field.INT:
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() + "("+optIndex+") as Long";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.TLOB:
    case Field.BLOB:
      return field.useName() + "("+optIndex+") as String";
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIME:
    case Field.TIMESTAMP:
      return field.useName() + "("+optIndex+") as Date";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName() + "("+optIndex+") as Double";
    }
    return "as unsupported";
  }
  static String setPropertyType(Field field, String optIndex)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() + "("+optIndex+"aValue as Boolean)";
    case Field.BYTE:
      return field.useName() + "("+optIndex+"aValue as Byte)";
    case Field.SHORT:
      return field.useName() + "("+optIndex+"aValue as Integer)";
    case Field.INT:
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() + "("+optIndex+"aValue as Long)";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.TLOB:
    case Field.BLOB:
      return field.useName() + "("+optIndex+"aValue as String)";
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIME:
    case Field.TIMESTAMP:
      return field.useName() + "("+optIndex+"aValue as Date)";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName() + "("+optIndex+"aValue as Double)";
    }
    return "as unsupported";
  }
  static String initVBVar(Field field)
  {
    switch(field.type)
    {
    case Field.BOOLEAN:
      return field.useName() + " = False";
    case Field.BYTE:
    case Field.SHORT:
    case Field.INT:
    case Field.LONG:
    case Field.SEQUENCE:
    case Field.IDENTITY:
      return field.useName() + " = 0";
    case Field.CHAR:
    case Field.ANSICHAR:
    case Field.USERSTAMP:
    case Field.TLOB:
    case Field.BLOB:
      return field.useName() + " = \"\"";
    case Field.DATE:
    case Field.DATETIME:
    case Field.TIME:
    case Field.TIMESTAMP:
      return field.useName() + " = Now";
    case Field.FLOAT:
    case Field.DOUBLE:
    case Field.MONEY:
      return field.useName() + " = 0.0";
    }
    return "as unsupported";
  }
}
