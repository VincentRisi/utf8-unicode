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
import java.util.Vector;

import static bbd.jportal.Writer.format;

public class MSSqlDDL extends Generator
{
  public static String description()
  {
    return "Generate MSSql DDL";
  }
  public static String documentation()
  {
    return "Generate MSSql DDL";
  }
  protected static Vector flagsVector;
  static boolean addTimestamp;
  static boolean useInsertTrigger;
  static boolean useUpdateTrigger;
  static boolean internalStamps;
  static boolean generate42;
  static boolean auditTrigger;
  private static void flagDefaults()
  {
    addTimestamp = false;
    useInsertTrigger = false;
    useUpdateTrigger = false;
    internalStamps = false;
    auditTrigger = false;
    generate42 = false;
  }
  public static Vector flags()
  {
    if (flagsVector == null)
    {
      flagDefaults();
      flagsVector = new Vector();
      flagsVector.addElement(new Flag("add timestamp", addTimestamp, "Add Timestamp - legacy flags"));
      flagsVector.addElement(new Flag("use insert trigger", useInsertTrigger, "Use Insert Trigger - legacy flags"));
      flagsVector.addElement(new Flag("use update trigger", useUpdateTrigger, "Use Update Trigger - legacy flags"));
      flagsVector.addElement(new Flag("internal stamps", internalStamps, "Use Internal Stamps - legacy flags"));
      flagsVector.addElement(new Flag("generate 4.2", generate42, "Generate for SqlServer 4.2 - legacy flags"));
      flagsVector.addElement(new Flag("auditTrigger", auditTrigger, "Generate Auditing Table and Triggers"));
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
      addTimestamp = toBoolean(((Flag)flagsVector.elementAt(0)).value);
      useInsertTrigger = toBoolean(((Flag)flagsVector.elementAt(1)).value);
      useUpdateTrigger = toBoolean(((Flag)flagsVector.elementAt(2)).value);
      internalStamps = toBoolean(((Flag)flagsVector.elementAt(3)).value);
      generate42 = toBoolean(((Flag)flagsVector.elementAt(4)).value);
      auditTrigger = toBoolean(((Flag)flagsVector.elementAt(5)).value);
    }
    else
      flagDefaults();
    for (int i = 0; i < database.flags.size(); i++)
    {
      String flag = database.flags.elementAt(i);
      if (flag.equalsIgnoreCase("add timestamp"))
        addTimestamp = true;
      else if (flag.equalsIgnoreCase("use triggers"))
      {
        useInsertTrigger = true;
        useUpdateTrigger = true;
      }
      else if (flag.equalsIgnoreCase("use insert trigger"))
        useInsertTrigger = true;
      else if (flag.equalsIgnoreCase("use update trigger"))
        useUpdateTrigger = true;
      else if (flag.equalsIgnoreCase("internal stamps"))
        internalStamps = true;
      else if (flag.equalsIgnoreCase("generate 4.2"))
        generate42 = true;
      else if (flag.equalsIgnoreCase("audit triggers"))
        auditTrigger = true;
    }
    if (addTimestamp)
      outLog.println(" (add timestamp)");
    if (useInsertTrigger)
      outLog.println(" (use insert trigger)");
    if (useUpdateTrigger)
      outLog.println(" (use update trigger)");
    if (internalStamps)
      outLog.println(" (internal stamps)");
    if (generate42)
      outLog.println(" (generate 4.2)");
    if (auditTrigger)
      outLog.println(" (audit triggers)");
  }
  private static String tableOwner;
  private static String tableSchema;
  /**
  * Generates the SQL for SQLServer Table creation.
  */
  public static void generate(Database database, String output, PrintWriter outLog)
  {
    try
    {
      setFlags(database, outLog);
      String fileName;
      if (database.output.length() > 0)
        fileName = database.output;
      else
        fileName = database.name;
      if (database.schema.length() > 0)
      {
        tableOwner = database.schema + ".";
        tableSchema = database.schema;
      }
      else
      {
        tableOwner = "";
        tableSchema = "";
      }
      outLog.println("DDL: " + output + fileName + ".sql");
      OutputStream outFile = new FileOutputStream(output + fileName + ".sql");
      try
      {
        PrintWriter outData = new PrintWriter(outFile);
        outData.println("USE " + database.name);
        outData.println();
        for (int i = 0; i < database.tables.size(); i++)
          generateTable(database.tables.elementAt(i), outData);
        for (int i = 0; i < database.views.size(); i++)
          generateView(database.views.elementAt(i), outData);
        outData.flush();
      }
      finally
      {
        outFile.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate SQLServer SQL IO Error");
    }
  }
  static String useExtra(String name, String extra)
  {
	String work = name + extra;
	int last = name.length()-1;
	if ((name.charAt(0) == '[' && name.charAt(last) == ']')
   	||  (name.charAt(0) == '\"' && name.charAt(last) == '\"'))
	  work = name.substring(0, last-1)+extra+name.substring(last);
	return work;
  }
  static void generateAuditTable(Table table, PrintWriter outData)
  {
    String auditName = tableOwner + useExtra(table.useLiteral(), "Audit");
    outData.println("IF OBJECT_ID('" + auditName + "', 'U') IS NOT NULL");
    outData.println("    DROP TABLE " + auditName);
    outData.println("GO");
    outData.println();
    outData.println("CREATE TABLE " + auditName);
    outData.println("(");
    outData.println("  AuditId INTEGER IDENTITY(1,1) NOT NULL PRIMARY KEY");
    outData.println(", AuditAction INTEGER NOT NULL -- 1 = INSERT, 2 = DELETE, 3 = UPDATE");
    outData.println(", AuditWhen DATETIME NOT NULL");
    for (int i = 0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      outData.println(", " + varType(field, true, false) + " NULL");
    }
    outData.println(")");
    outData.println("GO");
    outData.println();
  }
  static void generateAuditTrigger(Table table, PrintWriter outData)
  {
    String triggerName = tableOwner + useExtra(table.useLiteral(), "AuditTrigger");
    String auditName = tableOwner + useExtra(table.useLiteral(), "Audit");
    outData.println("IF OBJECT_ID('" + triggerName + "','TR') IS NOT NULL");
    outData.println("    DROP TRIGGER " + triggerName);
    outData.println("GO");
    outData.println();
    outData.println("CREATE TRIGGER " + triggerName + " ON " + triggerName);
    outData.println("FOR INSERT, DELETE, UPDATE AS");
    outData.println("BEGIN");
    outData.println("  DECLARE @INSERT INT, @DELETE INT, @ACTION INT;");
    outData.println("  SELECT @INSERT = COUNT(*) FROM INSERTED;");
    outData.println("  SELECT @DELETE = COUNT(*) FROM DELETED;");
    outData.println("  IF @INSERT > 0 SELECT @ACTION = 1 ELSE SELECT @ACTION = 0;");
    outData.println("  IF @DELETE > 0 SELECT @ACTION = @ACTION + 2;");
    outData.println("  -- 1 = INSERT, 2 = DELETE, 3 = UPDATE");
    outData.println("  IF @ACTION = 2 BEGIN");
    outData.println("    INSERT INTO " + auditName);
    outData.println("    SELECT @ACTION");
    outData.println("         , GETDATE()");
    for (int i = 0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      outData.println("          , " + field.useLiteral());
    }
    outData.println("    FROM DELETED;");
    outData.println("  END ELSE");
    outData.println("  BEGIN");
    outData.println("    INSERT INTO " + auditName);
    outData.println("    SELECT @ACTION");
    outData.println("         , GETDATE()");
    for (int i = 0; i < table.fields.size(); i++)
    {
      Field field = table.fields.elementAt(i);
      outData.println("         , " + field.useLiteral());
    }
    outData.println("    FROM INSERTED;");
    outData.println("  END");
    outData.println("END");
    outData.println("GO");
    outData.println();
  }
  static void generateTable(Table table, PrintWriter outData)
  {
    String tableName = tableOwner + table.useLiteral();
    String comma = "  ";
    outData.println("IF OBJECT_ID('" + tableName + "','U') IS NOT NULL");
    outData.println("    DROP TABLE " + tableName);
    outData.println("GO");
    outData.println();
    outData.println("CREATE TABLE " + tableName);
    outData.println("(");
    for (int i = 0; i < table.fields.size(); i++, comma = ", ")
    {
      Field field = table.fields.elementAt(i);
      outData.print(comma + varType(field, false, table.hasSequenceReturning));
      if (field.defaultValue.length() > 0)
        outData.print(" CONSTRAINT  DF_" + tableName + "_" + field.useLiteral() + " DEFAULT " + field.defaultValue);
      if (!field.isNull)
        outData.println(" NOT NULL");
      else
        outData.println(" NULL");
    }
    if (internalStamps)
    {
      outData.println(comma + "UpdateWhen  DATETIME DEFAULT CURRENT_TIMESTAMP NULL");
      outData.println(comma + "UpdateByWho CHAR(8)  DEFAULT USER NULL ");
    }
    if (addTimestamp)
      outData.println(comma + "TIMESTAMP");
    if (!generate42)
    {
      for (int i = 0; i < table.keys.size(); i++)
      {
        Key key = table.keys.elementAt(i);
        if (key.isPrimary)
          generatePrimary(key, tableSchema + "_" + table.name, outData);
        else if (key.isUnique)
          generateUnique(key, tableSchema + "_" + table.name, outData);
      }
      for (int i = 0; i < table.links.size(); i++)
      {
        Link link = table.links.elementAt(i);
        generateLink(link, tableSchema + "_" + table.name, outData);
      }
      //      if (hasEnums)
      //        generateEnumLinks(table, outData);
    }
    outData.println(")");
    outData.println("GO");
    outData.println();
    for (int i = 0; i < table.keys.size(); i++)
    {
      Key key = table.keys.elementAt(i);
      if (generate42 || (!key.isPrimary && !key.isUnique))
        generateKey(key, outData, tableName);
    }
    if (generate42)
    {
      for (int i = 0; i < table.links.size(); i++)
      {
        Link link = table.links.elementAt(i);
        generateSpLink(link, outData, tableName);
      }
    }
    for (int i = 0; i < table.grants.size(); i++)
    {
      Grant grant = table.grants.elementAt(i);
      generateGrant(grant, outData, table.database.userid + "." + tableName);
    }
    if (useInsertTrigger)
    {
      String insertTrigger = tableOwner + useExtra(table.useLiteral(), "InsertTrigger");
      if (table.hasSequence || table.hasUserStamp || table.hasTimeStamp)
      {
        outData.println("IF OBJECT_ID('" + insertTrigger + "','TR') IS NOT NULL");
        outData.println("    DROP TRIGGER " + insertTrigger);
        outData.println("GO");
        outData.println();
        outData.println("CREATE TRIGGER " + insertTrigger + " ON " + tableName + " FOR INSERT AS");
        for (int i = 0; i < table.fields.size(); i++)
        {
          Field field = table.fields.elementAt(i);
          if (field.type == Field.SEQUENCE)
          {
            outData.println("UPDATE " + tableName + " SET " + field.useLiteral() + "=" + field.useLiteral() + "+0");
            outData.println("WHERE " + field.useLiteral() + "=(SELECT MAX(" + field.useLiteral() + ") FROM " + tableName + ")");
          }
        }
        outData.println("UPDATE " + tableName);
        outData.println("SET");
        comma = "  ";
        for (int i = 0; i < table.fields.size(); i++)
        {
          Field field = table.fields.elementAt(i);
          if (field.type == Field.SEQUENCE)
          {
            outData.println(comma + field.useLiteral() + " = (SELECT MAX(" + field.useLiteral() + ") FROM " + tableName + ")+1");
            comma = ", ";
          }
          else if (field.type == Field.USERSTAMP)
          {
            outData.println(comma + field.useLiteral() + " = USER_NAME()");
            comma = ", ";
          }
          else if (field.type == Field.TIMESTAMP)
          {
            outData.println(comma + field.useLiteral() + " = GETDATE()");
            comma = ", ";
          }
        }
        String cond = "WHERE ";
        for (int i = 0; i < table.fields.size(); i++)
        {
          Field field = table.fields.elementAt(i);
          if (field.isPrimaryKey)
          {
            outData.println(cond + field.useLiteral() + " = (SELECT " + field.useLiteral() + " FROM INSERTED)");
            cond = "  AND ";
          }
        }
        outData.println("GO");
        outData.println();
      }
    }
    if ((table.hasUserStamp || table.hasTimeStamp) && auditTrigger)
    {
      String updateTrigger = tableOwner + useExtra(table.useLiteral(), "UpdateTrigger");
      outData.println("IF OBJECT_ID('" + updateTrigger + "','TR') IS NOT NULL");
      outData.println("    DROP TRIGGER " + updateTrigger);
      outData.println("GO");
      outData.println();
      outData.println("CREATE TRIGGER " + updateTrigger + " ON " + tableName + " FOR UPDATE AS");
      outData.println("UPDATE " + tableName);
      outData.println("SET");
      comma = "  ";
      for (int i = 0; i < table.fields.size(); i++)
      {
        Field field = table.fields.elementAt(i);
        if (field.type == Field.USERSTAMP)
        {
          outData.println(comma + field.useLiteral() + " = USER_NAME()");
          comma = ", ";
        }
        else if (field.type == Field.TIMESTAMP)
        {
          outData.println(comma + field.useLiteral() + " = GETDATE()");
          comma = ", ";
        }
      }
      outData.println("FROM INSERTED I");
      String cond = "WHERE ";
      for (int i = 0; i < table.fields.size(); i++)
      {
        Field field = table.fields.elementAt(i);
        if (field.isPrimaryKey)
        {
          outData.println(cond + tableName + "." + field.useLiteral() + " = I." + field.useLiteral() + " ");
          cond = "  AND ";
        }
      }
      outData.println("GO");
      outData.println();
    }
    if (auditTrigger)
    {
      generateAuditTable(table, outData);
      generateAuditTrigger(table, outData);
    }
    for (int i = 0; i < table.views.size(); i++)
    {
      View view = table.views.elementAt(i);
      generateView(view, outData, table);
    }
    for (int i = 0; i < table.procs.size(); i++)
    {
      Proc proc = table.procs.elementAt(i);
      if (proc.isData)
        generateData(proc, outData);
    }
  }
  /**
  * Generates SQL code for SQL Server Index
  */
  static void generateKey(Key key, PrintWriter outData, String table)
  {
    String comma = "  ";
    if (key.isPrimary)
      outData.println("CREATE UNIQUE CLUSTERED INDEX " + key.name + " ON " + table);
    else if (key.isUnique)
      outData.println("CREATE UNIQUE INDEX " + key.name + " ON " + table);
    else if (key.isClustered)
      outData.println("CREATE CLUSTERED INDEX " + key.name + " ON " + table);
    else
      outData.println("CREATE INDEX " + key.name + " ON " + table);
    outData.println("(");
    for (int i = 0; i < key.fields.size(); i++, comma = ", ")
    {
      String name = key.fields.elementAt(i);
      outData.println(comma + name);
    }
    outData.println(")");
    for (int i = 0; i < key.options.size(); i++)
    {
      String option = key.options.elementAt(i);
      outData.println(option);
    }
    outData.println("GO");
    outData.println();
    if (key.isPrimary)
    {
      outData.println("sp_primarykey " + table);
      for (int i = 0; i < key.fields.size(); i++)
      {
        String name = key.fields.elementAt(i);
        outData.println(", " + name);
      }
      outData.println("GO");
      outData.println();
    }
  }
  /**
  * Generates SQL code for ORACLE Primary Key create
  */
  static void generatePrimary(Key key, String tableName, PrintWriter outData)
  {
    String comma = "    ";
    outData.println(", CONSTRAINT PK_" + tableName + "_" + key.name + " PRIMARY KEY (");
    for (int i = 0; i < key.fields.size(); i++, comma = "  , ")
    {
      String name = key.fields.elementAt(i);
      outData.println(comma + name);
    }
    outData.println("  )");
  }
  /**
  * Generates SQL code for ORACLE Unique Key create
  */
  static void generateUnique(Key key, String tableName, PrintWriter outData)
  {
    String comma = "    ";
    outData.println(", CONSTRAINT UK_"  + tableName + "_" + key.name + (key.isClustered ? " CLUSTERED UNIQUE (" : " UNIQUE (") );
    for (int i = 0; i < key.fields.size(); i++, comma = "  , ")
    {
      String name = key.fields.elementAt(i);
      outData.println(comma + name);
    }
    outData.println("  )");
  }
  /**
  * Generates foreign key SQL Code appended to table
  */
  static void generateLink(Link link, String table, PrintWriter outData)
  {
    String comma = "    ";
    String temp = "";
    for (int i = 0; i < link.fields.size(); i++)
    {
      String name = link.fields.elementAt(i);
      temp += "_" + name;
    }
    outData.println(", CONSTRAINT FK_" + table + "_" + link.useName() + temp + " FOREIGN KEY (");
    for (int i = 0; i < link.fields.size(); i++, comma = "   ,")
    {
      String name = link.fields.elementAt(i);
      outData.println(comma + name);
    }
    outData.println("  )");
    if (link.linkFields.size() > 0)
    {
      outData.println("  REFERENCES " + link.name + "(");
      comma = "    ";
      for (int i = 0; i < link.linkFields.size(); i++, comma = "   ,")
      {
        String name = link.linkFields.elementAt(i);
        outData.println(comma + name);
      }
      outData.println("  )");
    }
    else
    {
      outData.println("  REFERENCES " + link.name);
    }
    if (link.isDeleteCascade)
    {
      outData.println("    ON DELETE CASCADE");
    }
    if (link.isUpdateCascade)
    {
      outData.println("    ON UPDATE CASCADE");
    }
    for (int i = 0; i < link.options.size(); i++)
    {
      String option = (String)link.options.elementAt(i);
      outData.println("    " + option);
    }
  }
  /**
  * Generates foreign key SQL Code for SQL Server
  */
  static void generateSpLink(Link link, PrintWriter outData, String table)
  {
    outData.println("sp_foreignkey " + table + ", " + link.name);
    for (int i = 0; i < link.fields.size(); i++)
    {
      String name = link.fields.elementAt(i);
      outData.println(", " + name);
    }
    outData.println("GO");
    outData.println();
  }
  /**
  * Generates grant SQL Code for SQL Server
  */
  static void generateGrant(Grant grant, PrintWriter outData, String object)
  {
    for (int i = 0; i < grant.perms.size(); i++)
    {
      String perm = grant.perms.elementAt(i);
      for (int j = 0; j < grant.users.size(); j++)
      {
        String user = grant.users.elementAt(j);
        outData.println("GRANT " + perm + " ON " + object + " TO " + user);
        outData.println("GO");
        outData.println();
      }
    }
  }
  /**
  * Generates view SQL Code for SQL Server
  */
  private static void generateView(View view, PrintWriter outData, String viewName) 
  {
	outData.println("IF OBJECT_ID('" + viewName + "','V') IS NOT NULL");
    outData.println("    DROP VIEW " + viewName);
    outData.println("GO");
    outData.println();
    outData.println("CREATE VIEW " + viewName);
    outData.println("(");
    String comma = "  ";
    for (int i = 0; i < view.aliases.size(); i++)
    {
      String alias = (String)view.aliases.elementAt(i);
      outData.println(comma + alias);
      comma = ", ";
    }
    outData.println(") AS");
    outData.println("(");
    for (int i = 0; i < view.lines.size(); i++)
    {
      String line = (String)view.lines.elementAt(i);
      outData.println(line);
    }
    outData.println(")");
    outData.println("GO");
    outData.println();
    for (int i = 0; i < view.users.size(); i++)
    {
      String user = (String)view.users.elementAt(i);
      outData.println("GRANT SELECT ON " + viewName + " TO " + user);
    }
    if (view.users.size() > 0)
    {
      outData.println("GO");
      outData.println();
    }
  }
  static void generateView(View view, PrintWriter outData, Table table)
  {
	String viewName = tableOwner + useExtra(table.useLiteral(), view.name);
    generateView(view, outData, viewName);
  }
  static void generateView(View view, PrintWriter outData)
  {
    generateView(view, outData, view.name);
  }
  static void generateData(Proc proc, PrintWriter outData)
  {
    for (int i = 0; i < proc.lines.size(); i++)
    {
      Line l = proc.lines.elementAt(i);
      if (l.line.startsWith("--start"))
        outData.println("BEGIN TRANSACTION;");
      else
        outData.println(l);
    }
    outData.println();
  }
  /**
  * Translates field type to SQLServer SQL column types
  */
  static String varType(Field field, boolean typeOnly, boolean hasSequenceReturning)
  {
    switch (field.type)
    {
      case Field.BOOLEAN:
        return field.useLiteral() + " BIT";
      case Field.BYTE:
        return field.useLiteral() + " TINYINT";
      case Field.SHORT:
        return field.useLiteral() + " SMALLINT";
      case Field.INT:
        return field.useLiteral() + " INT";
      case Field.LONG:
        return field.useLiteral() + " BIGINT";
      case Field.SEQUENCE:
        if (hasSequenceReturning)
          return field.useLiteral() + " INTEGER IDENTITY(1,1)";
        return field.useLiteral() + " INTEGER";
      case Field.BIGSEQUENCE:
        if (hasSequenceReturning)
          return field.useLiteral() + " BIGINT IDENTITY(1,1)";
        return field.useLiteral() + " BIGINT";
      case Field.IDENTITY:
        if (typeOnly == true)
          return field.useLiteral() + " INTEGER";
        else
          return field.useLiteral() + " INTEGER IDENTITY(1,1)";
      case Field.BIGIDENTITY:
        if (typeOnly == true)
          return field.useLiteral() + " BIGINT";
        else
          return field.useLiteral() + " BIGINT IDENTITY(1,1)";
      case Field.CHAR:
        if (field.length > 8000)
        {
          return field.useLiteral() + " VARCHAR(MAX)";
        }
        return field.useLiteral() + " VARCHAR(" + field.length + ")";
      case Field.ANSICHAR:
        return field.useLiteral() + " CHAR(" + field.length + ")";
      case Field.UTF8:
        int utf8len = (field.length) * 6;
        if (utf8len < 4000)
           return format("%s NVARCHAR(%d)", field.useLiteral(), utf8len);
        else
           return format("%s NVARCHAR(MAX)", field.useLiteral());
      case Field.UNICODE:
        int unicodelen = (field.length + 1) * 4;
        if (unicodelen < 4000)
           return format("%s NVARCHAR2(%d)", field.useLiteral(), unicodelen);
        else
          return format("%s NVARCHAR(MAX)", field.useLiteral());
      case Field.WCHAR:
        if (field.length > 4000)
        {
          return field.useLiteral() + " NVARCHAR(MAX)";
        }
        return field.useLiteral() + " NVARCHAR(" + field.length + ")";
      case Field.WANSICHAR:
        return field.useLiteral() + " NCHAR(" + field.length + ")";
      case Field.DATE:
        return field.useLiteral() + " DATETIME";
      case Field.DATETIME:
        return field.useLiteral() + " DATETIME";
      case Field.TIME:
        return field.useLiteral() + " DATETIME";
      case Field.TIMESTAMP:
        return field.useLiteral() + " DATETIME";
      case Field.FLOAT:
      case Field.DOUBLE:
        if (field.precision > 15)
          return field.useLiteral() + " DECIMAL(" + field.precision + "," + field.scale + ")";
        return field.useLiteral() + " FLOAT";
      case Field.BLOB:
        return field.useLiteral() + " IMAGE";
      case Field.TLOB:
        return field.useLiteral() + " TEXT";
      case Field.BIGXML:
      case Field.XML:
        return field.useLiteral() + " XML";
      case Field.MONEY:
        return field.useLiteral() + " MONEY";
      case Field.USERSTAMP:
        return field.useLiteral() + " VARCHAR(50)";
      case Field.UID:
        return field.useLiteral() + " UNIQUEIDENTIFIER";
    }
    return "unknown";
  }
}
