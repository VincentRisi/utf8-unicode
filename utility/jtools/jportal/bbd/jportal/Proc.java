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
/// $Date: 2004/10/18 13:45:50 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.util.Vector;

/**
* The hearts and souls. Holds the procedures for accessing the table.
*/
public class Proc implements Serializable
{
  /**
   *
   */
  private static final long serialVersionUID = 1L;
  /** parent table */
  public Table table;
  /** name of procedure */
  public String name;
  /** name of rows on multiples */
  public int noRows;
  /** List of input fields */
  public Vector<Field> inputs;
  /** List of output fields */
  public Vector<Field> outputs;
  /** List of dynamic SQL code fields */
  public Vector<String> dynamics;
  /** List of dynamic SQL code field sizes */
  public Vector<Integer> dynamicSizes;
  /** List of dynamic SQL code field sizes */
  public Vector<Boolean> dynamicStrung;
  /** SQL code for accessing the table*/
  public Vector<String> placeHolders;
  /** SQL code for accessing the table*/
  public Vector<Line> lines;
  /** The reasons and debates for the procedure */
  public Vector<String> comments;
  /** Generate options for procedure */
  public Vector<String> options;
  /** SelectBy DeleteBy std proc fields   */
  public Vector<String> fields;
  /** SelectFor update fields   */
  public Vector<String> updateFields;
  /** Select in order fields */
  public Vector<String> orderFields;
  /** Select in order fields */
  public Vector<String> withs;
  /** Indicates the procedure uses stored procedure logic Code */
  public boolean isProc;
  /** Indicates the procedure uses stored procedure logic Code */
  public boolean isSProc;
  /** If the procedure is only to produce passthru SQL Code */
  public boolean isData;
  /** If the procedure is only to produce passthru SQL Code */
  public boolean isIdlCode;
  /** Indicates the procedure is internal SQL code */
  public boolean isSql;
  /** Indicates a single result is expected */
  public boolean isSingle;
  /** Indicates a update Proc */
  public boolean isUpdate;
  /** Indicates an action no result is expected */
  public boolean isAction;
  /** Indicates procedure is a Standard procedure */
  public boolean isStd;
  /** Indicates the procedure uses the Standard Table definition */
  public boolean useStd;
  /** Indicates the procedures extends the Standard Table definition */
  public boolean extendsStd;
  /** Indicates the procedures uses the Primary key */
  public boolean useKey;
  /** Indicates the procedure has an Image field */
  public boolean hasImage;
  /** Indicates a single result is expected */
  public boolean isMultipleInput;
  /** Indicates the procedure is the Insert procedure */
  public boolean isInsert;
  /**  */
  public boolean isMerge;
  /**  */
  public boolean hasReturning;
  /**  */
  public boolean hasUpdates;
  /** Code starts at line */
  public int start;
  /** Constructs with default values */
  public Proc()
  {
    name = "";
    noRows = 0;
    inputs = new Vector<Field>();
    outputs = new Vector<Field>();
    dynamics = new Vector<String>();
    dynamicSizes = new Vector<Integer>();
    dynamicStrung = new Vector<Boolean>();
    placeHolders = new Vector<String>();
    lines = new Vector<Line>();
    comments = new Vector<String>();
    options = new Vector<String>();
    fields = new Vector<String>();
    updateFields = new Vector<String>();
    orderFields = new Vector<String>();
    withs = new Vector<String>();
    isProc = false;
    isSProc = false;
    isData = false;
    isIdlCode = false;
    isSql = false;
    isAction = false;
    isSingle = false;
    isUpdate = false;
    isStd = false;
    useStd = false;
    extendsStd = false;
    useKey = false;
    hasImage = false;
    isInsert = false;
    isMerge = false;
    isMultipleInput = false;
    hasReturning = false;
    hasUpdates = false;
    start = 0;
  }
  public void addLine(Line line)
  {
    lines.addElement(line);
  }
  public void addLine(String data, boolean isVar)
  {
      Line value = new Line(data, isVar);
      addLine(value);
  }
  public void addLine(String line)
  {
    addLine(line, false);
  }
  public void reader(DataInputStream ids) throws IOException
  {
    name = ids.readUTF();
    noRows = ids.readInt();
    int noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      Field value = new Field();
      value.reader(ids);
      inputs.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      Field value = new Field();
      value.reader(ids);
      outputs.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      dynamics.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      Integer value = new Integer(ids.readInt());
      dynamicSizes.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      Boolean value = new Boolean(ids.readBoolean());
      dynamicStrung.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      placeHolders.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String data = ids.readUTF();
      boolean isVar = ids.readBoolean();
      Line value = new Line(data, isVar);
      addLine(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      comments.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      options.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      fields.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      updateFields.addElement(value);
    }
    isProc = ids.readBoolean();
    isSProc = ids.readBoolean();
    isData = ids.readBoolean();
    isIdlCode = ids.readBoolean();
    isSql = ids.readBoolean();
    isAction = ids.readBoolean();
    isSingle = ids.readBoolean();
    isUpdate = ids.readBoolean();
    isStd = ids.readBoolean();
    useStd = ids.readBoolean();
    extendsStd = ids.readBoolean();
    useKey = ids.readBoolean();
    hasImage = ids.readBoolean();
    isInsert = ids.readBoolean();
    isMultipleInput = ids.readBoolean();
    hasReturning = ids.readBoolean();
    hasUpdates = ids.readBoolean();
    start = ids.readInt();
  }
  public void writer(DataOutputStream ods) throws IOException
  {
    ods.writeUTF(name);
    ods.writeInt(noRows);
    ods.writeInt(inputs.size());
    for (int i = 0; i < inputs.size(); i++)
    {
      Field value = inputs.elementAt(i);
      value.writer(ods);
    }
    ods.writeInt(outputs.size());
    for (int i = 0; i < outputs.size(); i++)
    {
      Field value = outputs.elementAt(i);
      value.writer(ods);
    }
    ods.writeInt(dynamics.size());
    for (int i = 0; i < dynamics.size(); i++)
    {
      String value = dynamics.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeInt(dynamicSizes.size());
    for (int i = 0; i < dynamicSizes.size(); i++)
    {
      Integer value = dynamicSizes.elementAt(i);
      ods.writeInt(value.intValue());
    }
    ods.writeInt(dynamicStrung.size());
    for (int i = 0; i < dynamicStrung.size(); i++)
    {
      Boolean value = dynamicStrung.elementAt(i);
      ods.writeBoolean(value.booleanValue());
    }
    ods.writeInt(placeHolders.size());
    for (int i = 0; i < placeHolders.size(); i++)
    {
      String value = placeHolders.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeInt(lines.size());
    for (int i = 0; i < lines.size(); i++)
    {
      Line value = lines.elementAt(i);
      value.writer(ods);
    }
    ods.writeInt(comments.size());
    for (int i = 0; i < comments.size(); i++)
    {
      String value = comments.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeInt(options.size());
    for (int i = 0; i < options.size(); i++)
    {
      String value = options.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeInt(fields.size());
    for (int i = 0; i < fields.size(); i++)
    {
      String value = fields.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeInt(updateFields.size());
    for (int i = 0; i < updateFields.size(); i++)
    {
      String value = updateFields.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeBoolean(isProc);
    ods.writeBoolean(isSProc);
    ods.writeBoolean(isData);
    ods.writeBoolean(isIdlCode);
    ods.writeBoolean(isSql);
    ods.writeBoolean(isAction);
    ods.writeBoolean(isSingle);
    ods.writeBoolean(isUpdate);
    ods.writeBoolean(isStd);
    ods.writeBoolean(useStd);
    ods.writeBoolean(extendsStd);
    ods.writeBoolean(useKey);
    ods.writeBoolean(hasImage);
    ods.writeBoolean(isInsert);
    ods.writeBoolean(isMultipleInput);
    ods.writeBoolean(hasReturning);
    ods.writeBoolean(hasUpdates);
    ods.writeInt(start);
  }
  /** Folds the first character of name to an upper case character */
  public String upperFirst()
  {
    if (name.length() < 1)
    {
      System.out.println("Table:" + table + " name length appears to be less than 1");
      return name;
    }
    String f = name.substring(0, 1);
    return f.toUpperCase() + name.substring(1);
  }
  /** Folds the first character of name to an upper case character */
  public String upperFirstOnly()
  {
    if (name.length() < 1)
    {
      System.out.println("Table:" + table + " name length appears to be less than 1");
      return name;
    }
    String f = name.substring(0, 1);
    return f.toUpperCase();
  }
  /** Folds the first character of name to an lower case character */
  public String lowerFirst()
  {
    if (name.length() < 1)
    {
      System.out.println("Table:" + table + " name length appears to be less than 1");
      return name;
    }
    String f = name.substring(0, 1);
    return f.toLowerCase() + name.substring(1);
  }
  /** Checks for for name in input list */
  public boolean hasInput(String s)
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      Field field = inputs.elementAt(i);
      if (field.name.equalsIgnoreCase(s))
        return true;
    }
    return false;
  }
  public Field getInput(String s)
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      Field field = inputs.elementAt(i);
      if (field.name.equalsIgnoreCase(s))
        return field;
    }
    return null;
  }
  public boolean hasModifieds()
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      Field field = inputs.elementAt(i);
      if ((field.type == Field.SEQUENCE && isInsert == true)
      || (field.type == Field.BIGSEQUENCE && isInsert == true)
      || field.type == Field.USERSTAMP
      || field.type == Field.TIMESTAMP)
        return true;
    }
    return false;
  }
  /** Checks for for name in input list */
  public int indexOf(String s)
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      Field field = inputs.elementAt(i);
      if (field.name.equalsIgnoreCase(s))
        return i;
    }
    return -1;
  }
  /** Checks for for name in output list */
  public boolean hasOutput(String s)
  {
    for (int i = 0; i < outputs.size(); i++)
    {
      Field field = outputs.elementAt(i);
      if (field.name.equalsIgnoreCase(s))
        return true;
    }
    return false;
  }
  /** Checks for for name in output list */
  public Field getOutput(String s)
  {
    for (int i = 0; i < outputs.size(); i++)
    {
      Field field = outputs.elementAt(i);
      if (field.name.equalsIgnoreCase(s))
        return field;
    }
    return null;
  }
  /** Checks for for name in dynamics list */
  public boolean hasDynamic(String s)
  {
    for (int i = 0; i < dynamics.size(); i++)
    {
      String name = dynamics.elementAt(i);
      if (name.equals(s))
        return true;
    }
    return false;
  }
  /** Checks for for name in dynamics list */
  public int getDynamicSize(String s)
  {
    for (int i = 0; i < dynamics.size(); i++)
    {
      String name = dynamics.elementAt(i);
      if (name.equals(s))
      {
        Integer n = dynamicSizes.elementAt(i);
        return n.intValue();
      }
    }
    return 256;
  }
  /**
   * Checks if a strung dynamic
   */
  public boolean isStrung(String s)
  {
    for (int i = 0; i < dynamics.size(); i++)
    {
      String name = dynamics.elementAt(i);
      if (name.equals(s))
      {
        Boolean b = dynamicStrung.elementAt(i);
        return b.booleanValue();
      }
    }
    return false;
  }
  /** Checks if proc uses data */
  public boolean hasNoData()
  {
    return inputs.size() == 0
            && outputs.size() == 0
            && dynamics.size() == 0;
  }
  /** Checks if proc has unique input ie. not already in output*/
  public boolean hasDiscreteInput()
  {
    if (dynamics.size() > 0)
      return true;
    for (int i = 0; i < inputs.size(); i++)
    {
      Field field = inputs.elementAt(i);
      if (hasOutput(field.name))
        continue;
      return true;
    }
    return false;
  }
  /** */
  public void checkPlaceHolders()
  {
    for (int i = 0; i < lines.size(); i++)
    {
      Line code = lines.elementAt(i);
      if (code.isVar == true)
        continue;
      String work = code.line.toUpperCase();
      String work2 = code.line;
      String alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_#$";
      String alphanum = alpha + "0123456789";
      int n = work.indexOf(':');
      while (n != -1)
      {
        work = work.substring(n + 1);
        work2 = work2.substring(n + 1);
        int p = 0;
        if (alpha.indexOf(work.charAt(0)) != -1)
          for (p = 1; p < work.length(); p++)
            if (alphanum.indexOf(work.charAt(p)) == -1)
              break;
        if (p > 1)
        {
          String placeHolder = work2.substring(0, p);
          if (hasInput(placeHolder))
            placeHolders.addElement(placeHolder);
          else
            System.out.println("placeHolder(" + placeHolder + ") is not defined as an Input for proc(" + name + ")");
        }
        n = work.indexOf(':');
      }
    }
  }
  public String toString()
  {
    return name;
  }
  public boolean hasOption(String value)
  {
    for (int i = 0; i < options.size(); i++)
    {
      String option = options.elementAt(i);
      if (option.toLowerCase().compareTo(value.toLowerCase()) == 0)
        return true;
    }
    return false;
  }
  public boolean hasFields(String value)
  {
    for (int i = 0; i < fields.size(); i++)
    {
      String option = fields.elementAt(i);
      if (option.toLowerCase().compareTo(value.toLowerCase()) == 0)
        return true;
    }
    return false;
  }
  public boolean hasOrders(String value)
  {
    for (int i = 0; i < orderFields.size(); i++)
    {
      String option = orderFields.elementAt(i);
      if (option.toLowerCase().compareTo(value.toLowerCase()) == 0)
        return true;
    }
    return false;
  }
  public boolean hasUpdateFields(String value)
  {
    for (int i = 0; i < updateFields.size(); i++)
    {
      String option = updateFields.elementAt(i);
      if (option.toLowerCase().compareTo(value.toLowerCase()) == 0)
        return true;
    }
    return false;
  }
  public boolean isStdExtended()
  {
    if (isStd == true) return true;
    if (extendsStd == true)
    {
      if (useStd == true) return true;
      if (dynamics.size() > 0) return false;
      for (int i = 0; i < inputs.size(); i++)
      {
        Field field = inputs.elementAt(i);
        if (table.hasField(field.name) == false)
          return false;
      }
      for (int i = 0; i < outputs.size(); i++)
      {
        Field field = outputs.elementAt(i);
        if (table.hasField(field.name) == false)
          return false;
      }
      return true;
    }
    return false;
  }
}


