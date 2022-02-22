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
/// $Date: 2004/10/18 13:45:44 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;

import static bbd.jportal.Writer.format;

public class MySqlDDL extends Generator
{
  public static String description()
  {
    return "Generate MySQL DDL";
  }
  public static String documentation()
  {
    return "Generate MySQL DDL.";
  }
  /**
   * Generates the SQL for MySQL Table creation.
   */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    try
    {
      String fileName;
      if (database.output.length() > 0)
        fileName = database.output;
      else
        fileName = database.name;
      outLog.println("DDL: " + output + fileName + ".sql");
      OutputStream outFile = new FileOutputStream(output + fileName + ".sql");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        for (int i = 0; i < database.tables.size(); i++)
          generateTable(database, database.tables.elementAt(i), outData);
        outData.flush();
      }
      finally
      {
        outFile.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate My SQL IO Error");
    }
  }
  /**
   * @param database
   * @param table
   * @param outData
   */
  private static void generateTable(Database database, Table table, PrintWriter outData)
  {
    String tableOwner = "";
    String tablename = table.fixEscape();
    if (table.database.schema.length() > 0)
      tableOwner = table.database.schema + ".";
    boolean hasNotNull = false;
    if (table.fields.size() > 0)
    {
      outData.println("DROP TABLE IF EXISTS " + tableOwner + tablename + " CASCADE;");
      outData.println();
    }
    String comma = "( ";
    if (table.fields.size() > 0)
    {
      outData.println("CREATE TABLE " + tableOwner + tablename);
      for (int i = 0; i < table.fields.size(); i++, comma = ", ")
      {
        Field field = table.fields.elementAt(i);
        outData.print(comma + field.fixEscape() + " " + varType(field));
        if (field.defaultValue.length() > 0)
          hasNotNull = true;
        if (field.checkValue.length() > 0)
          hasNotNull = true;
        else if (!field.isNull)
          outData.print(" NOT NULL");
        else
          outData.print(" NULL");
        outData.println();
      }
      outData.print(")");
      for (int i = 0; i < table.options.size(); i++)
      {
        String option = table.options.elementAt(i);
        if (option.toLowerCase().indexOf("tablespace") == 0)
        {
          outData.println();
          outData.print(option);
        }
      }
      outData.println(";");
      outData.println();
      for (int i = 0; i < table.grants.size(); i++)
      {
        Grant grant = table.grants.elementAt(i);
        generateGrant(grant, outData, tableOwner + tablename);
      }
      for (int i = 0; i < table.keys.size(); i++)
      {
        Key key = table.keys.elementAt(i);
        if (!key.isPrimary && !key.isUnique)
          generateIndex(table, key, outData);
      }
    }
    for (int i = 0; i < table.views.size(); i++)
    {
      View view = table.views.elementAt(i);
      generateView(view, outData, tablename, tableOwner);
    }
    if (hasNotNull == true)
    {
      String alterTable = "ALTER TABLE " + tableOwner + tablename;
      for (int i = 0; i < table.fields.size(); i++)
      {
        Field field = table.fields.elementAt(i);
        if (field.isNull && field.defaultValue.length() == 0 && field.checkValue.length() == 0)
          continue;
        outData.print(alterTable + " ALTER " + field.name + " SET");
        if (field.defaultValue.length() > 0)
          outData.print(" DEFAULT " + field.defaultValue);
        if (field.checkValue.length() > 0)
          outData.print(" CHECK (" + field.checkValue + ")");
        outData.println(";");
      }
      outData.println();
    }
    if (table.keys.size() > 0)
    {
      for (int i = 0; i < table.keys.size(); i++)
      {
        Key key = table.keys.elementAt(i);
        if (key.isPrimary)
        {
          outData.println("ALTER TABLE " + tableOwner + tablename);
          generatePrimary(table, key, outData);
          outData.println(";");
        }
        else if (key.isUnique)
        {
          outData.println("ALTER TABLE " + tableOwner + tablename);
          generateUnique(table, key, outData);
          outData.println(";");
        }
      }
      outData.println();
    }
    if (table.links.size() > 0)
    {
      for (int i = 0; i < table.links.size(); i++)
      {
        Link link = table.links.elementAt(i);
        outData.println("ALTER TABLE " + tableOwner + tablename);
        if (link.linkName.length() == 0)
          link.linkName = table.name.toUpperCase() + "_FK" + bSO(i);
        generateLink(link, tableOwner, outData);
        outData.println(";");
      }
      outData.println();
    }
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        generateProc(proc, outData);
    }
  }
  /**
   * @param proc
   * @param outData
   */
  private static void generateProc(Proc proc, PrintWriter outData)
  {
    for (int i = 0; i < proc.lines.size(); i++)
    {
      Line l = proc.lines.elementAt(i);
      outData.println(l.line);
    }
    outData.println();
  }
  /**
   * @param link
   * @param outData
   */
  private static void generateLink(Link link, String tableOwner, PrintWriter outData)
  {
    String comma = "  ( ";
    outData.println(" ADD CONSTRAINT " + link.linkName + " FOREIGN KEY");
    for (int i = 0; i < link.fields.size(); i++, comma = "  , ")
    {
      String name = link.fields.elementAt(i);
      outData.println(comma + name);
    }
    comma = "  ( ";
    outData.println("  ) REFERENCES " + tableOwner + link.name);
    boolean useLF = false;
    int no = link.linkFields.size();
    if (no == 0)
    {
      no = link.fields.size();
      useLF = true;
    }
    for (int i = 0; i < no; i++, comma = "  , ")
    {
      String name;
      if (useLF == true)
        name = link.fields.elementAt(i);
      else
        name = link.linkFields.elementAt(i);
      outData.println(comma + name);
    }
    outData.println("  )");
    if (link.isDeleteCascade)
    {
      outData.println("  ON DELETE CASCADE");
    }
  }
  /**
   * @param table
   * @param key
   * @param outData
   */
  private static void generateUnique(Table table, Key key, PrintWriter outData)
  {
    String comma = "  ( ";
    String keyname = key.name.toUpperCase();
    if (keyname.indexOf(table.name.toUpperCase()) == -1)
      keyname = table.name.toUpperCase() + "_" + keyname;
    outData.println(" ADD CONSTRAINT " + keyname + " UNIQUE");
    for (int i = 0; i < key.fields.size(); i++, comma = "  , ")
    {
      String name = key.fields.elementAt(i);
      outData.println(comma + name);
    }
    outData.println("  )");
  }
  /**
   * @param table
   * @param key
   * @param outData
   */
  private static void generatePrimary(Table table, Key key, PrintWriter outData)
  {
    String comma = "  ( ";
    String keyname = key.name.toUpperCase();
    if (keyname.indexOf(table.name.toUpperCase()) == -1)
      keyname = table.name.toUpperCase() + "_" + keyname;
    outData.println(" ADD CONSTRAINT " + keyname + " PRIMARY KEY");
    for (int i = 0; i < key.fields.size(); i++, comma = "  , ")
    {
      String name = key.fields.elementAt(i);
      outData.println(comma + name);
    }
    outData.print("  )");
    for (int i = 0; i < key.options.size(); i++)
    {
      String option = key.options.elementAt(i);
      if (option.toLowerCase().indexOf("tablespace") == 0)
      {
        outData.print(" USING INDEX " + option);
      }
    }
    outData.println();
  }
  /**
   * @param i
   * @return
   */
  private static String bSO(int i)
  {
    String x = "" + (101 + i);
    return x.substring(1);
  }
  /**
   * @param view
   * @param outData
   * @param tableOwner
   */
  private static void generateView(View view, PrintWriter outData, String tableName, String tableOwner)
  {
    outData.println("CREATE OR REPLACE VIEW " + tableOwner + tableName + view.name);
    if (view.aliases.size() > 0)
    {
      String comma = "( ";
      for (int i = 0; i < view.aliases.size(); i++)
      {
        String alias = (String)view.aliases.elementAt(i);
        outData.println(comma + alias);
        comma = ", ";
      }
      outData.println(")");
    }
    outData.println("AS (");
    for (int i = 0; i < view.lines.size(); i++)
    {
      String line = (String)view.lines.elementAt(i);
      outData.println(line);
    }
    outData.println(");");
    outData.println();
    for (int i = 0; i < view.users.size(); i++)
    {
      String user = (String)view.users.elementAt(i);
      outData.println("GRANT SELECT ON " + tableOwner + tableName + view.name + " TO " + user + ";");
    }
    outData.println();
  }
  /**
   * @param table
   * @param key
   * @param outData
   */
  private static void generateIndex(Table table, Key key, PrintWriter outData)
  {
    String tableOwner = "";
    String tablename = table.fixEscape();
    if (table.database.schema.length() > 0)
      tableOwner = table.database.schema + ".";
    String comma = "( ";
    String keyname = key.name.toUpperCase();
    if (keyname.indexOf(table.name.toUpperCase()) == -1)
      keyname = table.name.toUpperCase() + "_" + keyname;
    outData.println("CREATE INDEX " + keyname + " ON " + tableOwner + tablename);
    for (int i = 0; i < key.fields.size(); i++, comma = ", ")
    {
      String name = key.fields.elementAt(i);
      outData.println(comma + name);
    }
    outData.print(")");
    for (int i = 0; i < key.options.size(); i++)
    {
      String option = key.options.elementAt(i);
      if (option.toLowerCase().indexOf("tablespace") == 0)
      {
        outData.println();
        outData.print(option);
      }
    }
    outData.println(";");
    outData.println();
  }
  /**
   * @param grant
   * @param outData
   */
  private static void generateGrant(Grant grant, PrintWriter outData, String on)
  {
    for (int i = 0; i < grant.perms.size(); i++)
    {
      String perm = grant.perms.elementAt(i);
      for (int j = 0; j < grant.users.size(); j++)
      {
        String user = grant.users.elementAt(j);
        if (user.toLowerCase().compareTo("public") == 0)
          continue;
        outData.println("GRANT " + perm + " ON " + on + " TO " + user + ";");
        outData.println();
      }
    }
  }
  /**
   * @param field
   * @return
   */
  private static String varType(Field field)
  {
    switch (field.type)
    {
      case Field.BYTE:
        return "tinyint";
      case Field.SHORT:
        return "smallint";
      case Field.INT:
        return "integer";
      case Field.SEQUENCE:
        return "int AUTO_INCREMENT UNIQUE";
      case Field.BIGSEQUENCE:
        return "bigint AUTO_INCREMENT UNIQUE";
      case Field.LONG:
        return "bigint";
      case Field.CHAR:
        return "varchar(" + field.length + ")";
      case Field.ANSICHAR:
        return "char(" + field.length + ")";
      case Field.UTF8:
        return format("VARBINARY(%d)", (field.length) * 6);
      case Field.UNICODE:
        return format("VARBINARY(%d)", (field.length + 1) * 4);
      case Field.DATE:
        return "date";
      case Field.DATETIME:
        return "datetime";
      case Field.TIME:
        return "time";
      case Field.TIMESTAMP:
        return "timestamp";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision == 0 && field.scale == 0) return "double";
        return "double(" + field.precision + ", " + field.scale + ")";
      case Field.BLOB:
        return "blob";
      case Field.TLOB:
        return "text";
      case Field.MONEY:
        return "decimal";
      case Field.USERSTAMP:
        return "VARCHAR(50)";
      case Field.IDENTITY:
        return "<not supported>";
    }
    return "unknown";
  }
}
