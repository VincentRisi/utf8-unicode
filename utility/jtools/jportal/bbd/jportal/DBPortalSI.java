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

package bbd.jportal;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;

import static bbd.jportal.Writer.*;

public class DBPortalSI extends Generator
{
  public static String description()
  {
    return "Generate DBPortal SI";
  }
  public static String documentation()
  {
    return "generate DBPortal SI";
  }
  /**
  * Generates the procedure classes for each table present.
  */
  static String pad(String s, int length)
  {
    for (int i = s.length(); i < length-1; i++)
      s = s + " ";
    return s + " ";
  }
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    logger = outLog;
    for (int i=0; i<database.tables.size(); i++)
      generateDatabase(database, output, database.tables.elementAt(i));
  }
  static String loseStorage(String s)
  {
    int n = s.indexOf("STORAGE (");
    if (n != -1)
      s = s.substring(0, n) + s.substring(n+9, s.length()-1);
    return s;
  }
  static void generateDatabase(Database database, String output, Table table)
  {
    try
    {
      logln(output+table.useName().toLowerCase() + ".si");
      OutputStream outFile = new FileOutputStream(output+table.useName().toLowerCase() + ".si");
      try
      {
        writer = new PrintWriter(outFile);
        indent_size = 4;
        writeln(pad("SERVER", 8)+"@"+database.server);
        if (database.userid.length() > 0)
           writeln(pad("CONNECT", 8)+database.userid+"/"+database.password);
        else
          writeln(pad("CONNECT scott/tiger", 8));
        writeln();
        for (int i = 0; i < table.options.size(); i++)
          writeln(loseStorage(table.options.elementAt(i)));
        writeln(pad("TABLE", 8)+table.name);
        for (int i = 0; i < table.fields.size(); i++)
          generateField(table.fields.elementAt(i), null);
        writeln();
        for (int i = 0; i < table.grants.size(); i++)
          generateGrant(table.grants.elementAt(i));
        writeln();
        for (int i = 0; i < table.keys.size(); i++)
          generateKey(table.keys.elementAt(i));
        for (int i = 0; i < table.links.size(); i++)
          generateLink(table.links.elementAt(i));
        for (int i = 0; i < table.views.size(); i++)
          generateView(table.views.elementAt(i));
        for (int i = 0; i < table.procs.size(); i++)
          generateProc(table.procs.elementAt(i));
        writeln();
      }
      finally
      {
        writer.close();
      }
    }
    catch (IOException e1)
    {
      logln("Generate Procs IO Error");
    }
  }
  private static boolean checkInTable(Field field, Table table)
  {
    if (table.hasField(field.name) == true)
    {
      Field tField = table.getField(field.name);
      return tField.length == field.length && tField.precision == field.precision && tField.scale == field.scale;
    }
    return false;
  }
  static void generateField(Field field, Table table)
  {
    String line = "  " + checkReserved(field.name);
    String ft = "=";
    if (field.alias.length() > 0)
      line = pad(line, 20) + "(" + field.alias + ")";
    if (table == null || checkInTable(field, table) == false)
    {
      switch (field.type)
      {
      case Field.BLOB:
        ft = "blob";
        break;
      case Field.BOOLEAN:
        ft = "boolean";
        break;
      case Field.BYTE:
        ft = "tinyint";
        break;
      case Field.CHAR:
      case Field.ANSICHAR:
        ft = "char (" + field.length + ")";
        break;
      case Field.DATE:
        ft = "date";
        break;
      case Field.DATETIME:
        ft = "datetime";
        break;
      case Field.DOUBLE:
        ft = "double";
        if (field.precision > 0 || field.scale > 0)
          ft = ft + "(" + field.precision + "," + field.scale + ")";
        break;
      case Field.DYNAMIC:
        return;
      case Field.FLOAT:
        ft = "float";
        if (field.precision > 0 || field.scale > 0)
          ft = ft + "(" + field.precision + "," + field.scale + ")";
        break;
      case Field.IDENTITY:
        ft = "identity";
        break;
      case Field.INT:
        ft = "int";
        break;
      case Field.LONG:
        ft = "long";
        break;
      case Field.MONEY:
        ft = "double (15,2)";
        break;
      case Field.SEQUENCE:
        if (table == null)
          ft = "sequence";
        else
          ft = "int";
        break;
      case Field.BIGSEQUENCE:
        if (table == null)
          ft = "bigsequence";
        else
          ft = "long";
        break;
      case Field.SHORT:
        ft = "short";
        break;
      case Field.STATUS:
        return;
      case Field.TIME:
        ft = "time";
        break;
      case Field.TIMESTAMP:
        if (table == null)
          ft = "timestamp";
        else
          ft = "datetime";
        break;
      case Field.TLOB:
        ft = "tlob";
        break;
      case Field.USERSTAMP:
        if (table == null)
          ft = "userstamp";
        else
          ft = "char (" + field.length + ")";
        break;
      }
    }
    line = pad(line, 36) + ft;
    if (table == null)
    {
      if (field.enums.size() > 0)
      {
        line += " (";
        for (int i = 0; i < field.enums.size(); i++)
        {
          if (i > 0)
            line += " ";
          Enum e = field.enums.elementAt(i);
          line += (checkReserved(e.name) + "=" + e.value);
        }
        line += ")";
      } else if (field.valueList.size() > 0)
      {
        line += " {";
        for (int i = 0; i < field.valueList.size(); i++)
        {
          if (i > 0)
            line += " ";
          String e = field.valueList.elementAt(i);
          line += checkReserved(e);
        }
        line += "}";
      }
    }
    if (field.isNull)
      line = pad(line, 48) + "    NULL";
    if (field.comments.size() > 0)
      line = pad(line, 56) + field.comments.elementAt(0);
    writeln(line);
  }
  static void generateGrant(Grant grant)
  {
    String line = "GRANT";
    for (int i = 0; i < grant.perms.size(); i++)
      line = line + " " + grant.perms.elementAt(i);
    line = line + " TO";
    for (int i = 0; i < grant.users.size(); i++)
      line = line + " " + grant.users.elementAt(i);
    writeln(line);
  }
  static void generateKey(Key key)
  {
    writeln(pad("KEY", 8) + key.name);
    if (key.isPrimary)
      writeln("PRIMARY");
    else if (key.isUnique)
      writeln("UNIQUE");
    for (int i = 0; i < key.options.size(); i++)
    {
      String line = key.options.elementAt(i);
      if (line.indexOf("USING INDEX ") == 0)
        line = line.substring(12);
      writeln(loseStorage(line));
    }
    for (int i = 0; i < key.fields.size(); i++)
      writeln(1, key.fields.elementAt(i));
    writeln();
  }
  static void generateLink(Link link)
  {
    writeln(pad("LINK", 8) + link.name);
    for (int i = 0; i < link.fields.size(); i++)
      writeln("  " + link.fields.elementAt(i));
    writeln();
  }
  static void generateView(View view)
  {
    writeln(pad("VIEW", 8) + view.name);
    if (view.users.size() > 0)
    {
      String line = pad("TO", 7);
      for (int i = 0; i < view.users.size(); i++)
        line = line + view.users.elementAt(i);
    }
    writeln("CODE");
    for (int i = 0; i < view.lines.size(); i++)
      writeln("  " + view.lines.elementAt(i));
    writeln("ENDCODE");
    writeln();
  }
  static void generateProc(Proc proc)
  {
    boolean stdProc = true;
    if (proc.name.compareTo("Insert") == 0
    ||  proc.name.compareTo("Update") == 0
    ||  proc.name.compareTo("DeleteOne") == 0
    ||  proc.name.compareTo("DeleteAll") == 0
    ||  proc.name.compareTo("SelectOne") == 0
    ||  proc.name.compareTo("SelectAll") == 0
    ||  proc.name.compareTo("Count") == 0
    ||  proc.name.compareTo("Exists") == 0)
      writeln(pad("PROC", 8) + proc.name);
    else if (proc.name.compareTo("BulkInsert") == 0
    ||       proc.name.compareTo("BulkUpdate") == 0)
      writeln(pad("## PROC", 8) + proc.name);
    else if (proc.name.compareTo("SelectOneUpd") == 0)
      writeln(pad("PROC", 8) + "SelectOne FOR UPDATE");
    else if (proc.name.compareTo("SelectAllUpd") == 0)
      writeln(pad("PROC", 8) + "SelectAll FOR UPDATE");
    else if (proc.name.compareTo("SelectAllSorted") == 0)
      writeln(pad("PROC", 8) + "SelectAll IN ORDER");
    else if (proc.name.compareTo("SelectAllSortedUpd") == 0)
      writeln(pad("PROC", 8) + "SelectAll IN ORDER FOR UPDATE");
    else
    {
      writeln(pad("PROC", 8) + proc.name);
      stdProc = false;
    }
    for (int i=0; i < proc.options.size(); i++)
      writeln(pad("OPTIONS", 8) + "'" + proc.options.elementAt(i) + "'");
    if (stdProc)
      return;
    if (proc.inputs.size() > 0)
    {
      writeln("INPUT");
      for (int i=0; i < proc.inputs.size(); i++)
        generateField(proc.inputs.elementAt(i), proc.table);
    }
    if (proc.outputs.size() > 0)
    {
      writeln("OUTPUT");
      for (int i=0; i < proc.outputs.size(); i++)
        generateField(proc.outputs.elementAt(i), proc.table);
    }
    writeln("SQLCODE");
    for (int i=0; i < proc.lines.size(); i++)
    {
      Line line = proc.lines.elementAt(i);
      if (line.isVar)
        writeln("@"+line.line+"(2048)");
      else
        writeln(line.line);
    }
    writeln("ENDCODE");
    writeln();
  }
  static final String reservedWords=":"
      +"all:" 
      +"ansi:"
      +"ansichar:"
      +"autotimestamp:"
      +"bigidentity:"
      +"bigsequence:"
      +"bigxml:"
      +"bit:"
      +"blob:"
      +"boolean:"
      +"bulkinsert:"
      +"bulkupdate:"
      +"byte:"
      +"cascade:"
      +"char:"
      +"check:"
      +"clob:"
      +"const:"
      +"constant:"
      +"count:"
      +"cursor:"
      +"database:"
      +"date:"
      +"datetime:"
      +"declare:"
      +"default:"
      +"delete:"
      +"deleteall:"
      +"deleteone:"
      +"double:"
      +"dynamic:"
      +"execute:"
      +"exists:"
      +"flags:"
      +"float:"
      +"for:"
      +"grant:"
      +"identity:"
      +"import:"
      +"in:"
      +"inout:"
      +"input:"
      +"insert:"
      +"int:"
      +"integer:"
      +"key:"
      +"link:"
      +"long:"
      +"lookup:"
      +"merge:"
      +"money:"
      +"multiple:"
      +"names:"
      +"not:"
      +"null:"
      +"options:"
      +"order:"
      +"output:"
      +"package:"
      +"password:"
      +"primary:"
      +"proc:"
      +"readonly:"
      +"returning:"
      +"schema:"
      +"select:"
      +"selectall:"
      +"selectone:"
      +"sequence:"
      +"server:"
      +"short:"
      +"single:"
      +"sorted:"
      +"sproc:"
      +"sql:"
      +"storedproc:"
      +"table:"
      +"time:"
      +"timestamp:"
      +"tlob:"
      +"to:"
      +"uid:"
      +"unique:"
      +"update:"
      +"userid:"
      +"userstamp:"
      +"utf8:"
      +"value:"
      +"view:"
      +"viewonly:"
      +"wansi:"
      +"wansichar:"
      +"wchar:"
      +"xml:";
  private static String checkReserved(String name)
  {
    String work=String.format(":%s:", name.toLowerCase());
    if (reservedWords.indexOf(work) != -1)
      return String.format("L'%s'", name);
    return name;
  }
}

