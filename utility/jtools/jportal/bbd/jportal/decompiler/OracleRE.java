package bbd.jportal.decompiler;

import java.io.PrintWriter;
import java.sql.SQLException;
import java.util.Vector;

import bbd.jportal.util.Connector;
import bbd.jportal.util.ConnectorOracle;
import bbd.jportal.util.Query;
import bbd.jportal.Database;
import bbd.jportal.Field;
import bbd.jportal.Grant;
import bbd.jportal.Key;
import bbd.jportal.Link;
import bbd.jportal.Proc;
import bbd.jportal.Sequence;
import bbd.jportal.Table;
import bbd.jportal.View;

class LinksTo
{
  String tableName;
  String linkName;
}

public class OracleRE
{
  protected static Database database;
  protected static Table table;
  protected static Field field;
  protected static Grant grant;
  protected static Key key;
  protected static Link link;
  protected static View view;
  protected static Proc proc;
  protected static Sequence sequence;
  protected static Connector connector;
  protected static Vector tables;
  protected static Vector links;
  protected static Vector sequencesDone;
  protected static Oracle oracle;
  /**
  * parameters expected to be in the form user/password@sid.
  * If the sid contains : eg "hulk:1521:qa01" then "thin" will
  * be used for the connection else "oci7" will be used.
  */
  public static Database devolve(String parameters, PrintWriter outLog)
  {
    try
    {
      String userid, password, sid, driver;
      int slash = parameters.indexOf('/');
      int at = parameters.indexOf('@');
      if (slash == -1 || at == -1)
        return null;
      userid = parameters.substring(0, slash);
      password = parameters.substring(slash + 1, at);
      sid = parameters.substring(at + 1);
      if (sid.indexOf(':') != -1)
        driver = "thin";
      else
        driver = "oci7";
      connector = new ConnectorOracle(driver, sid, userid, password);
      oracle = new Oracle(connector);
      database = new Database();
      database.name = fixDatabaseName(sid);
      database.server = sid;
      database.userid = userid.toUpperCase();
      database.password = password.toUpperCase();
      sequencesDone = new Vector();
      loadTables(outLog);
      loadViews(outLog);
      loadSequences(outLog);
    } catch (SQLException e)
    {
      System.out.println(sqlExceptionMessage(e));
      System.out.flush();
      e.printStackTrace();
      return null;
    } catch (ClassNotFoundException e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
      return null;
    } catch (Throwable e)
    {
      e.printStackTrace();
      return null;
    }
    return database;
  }
  static String fixDatabaseName(String sid)
  {
    int i = sid.indexOf(':');
    if (i == -1)
      return sid.toUpperCase();
    String name = sid.substring(0, i);
    sid = sid.substring(i + 1);
    i = sid.indexOf(':');
    if (i != -1)
    {
      name = name + sid.substring(0, i);
      sid = sid.substring(i + 1);
    }
    name = name + sid;
    return name.toUpperCase();
  }
  static String sqlExceptionMessage(SQLException e)
  {
    return "\n\nSQLException thrown"
      + "\n  SQL State  "
      + e.getSQLState()
      + "\n  Error Code "
      + e.getErrorCode()
      + "\n  Message    "
      + e.getMessage();
  }
  static String setKorM(int value)
  {
    if ((value % 1024) != 0 || (value / 1024) == 0)
      return String.valueOf(value);
    int kvalue = value / 1024;
    if ((kvalue % 1024) != 0 || (kvalue / 1024) == 0)
      return kvalue + "K";
    int mvalue = kvalue / 1024;
    return mvalue + "M";
  }
  static String caseChange(String input, int ch)
  {
    String result = input.substring(0, 1).toUpperCase();
    input = input.substring(1);
    int n = input.indexOf(ch);
    while (n != -1)
    {
      result = result + input.substring(0, n);
      input = input.substring(n + 1);
      if (input.length() == 0)
        break;
      result = result + input.substring(0, 1).toUpperCase();
      input = input.substring(1);
      n = input.indexOf(ch);
    }
    return result + input;
  }
  static String caseChange(String input)
  {
    return caseChange(
      caseChange(caseChange(input.toLowerCase(), '_'), '#'),
      '$');
  }
  static void loadTables(PrintWriter outLog) throws SQLException
  {
    Oracle.Tables tablesRec = oracle.getTables();
    Query query = tablesRec.tables(database.userid);
    tables = new Vector();
    while (tablesRec.tables(query))
    {
      table = new Table();
      table.database = database;
      table.name = tablesRec.tableName;
      table.alias = caseChange(table.name);
      table.options.addElement(
        "TABLESPACE "
          + tablesRec.tableSpaceName
          + " STORAGE (INITIAL "
          + setKorM(tablesRec.initialExtent)
          + " NEXT "
          + setKorM(tablesRec.nextExtent)
          + ")");
      outLog.println(table.name);
      outLog.flush();
      loadTableFields(outLog);
      loadTablePrimaryKey(outLog);
      loadTableUniqueKeys(outLog);
      loadTableIndexes(outLog);
      loadTableForeignKeys(outLog);
      loadTableGrants(outLog);
      buildStdProcs(outLog);
      tables.addElement(table);
    }
    Oracle.ForeignLinks foreignLinksRec = oracle.getForeignLinks();
    query = foreignLinksRec.foreignLinks(database.userid);
    links = new Vector();
    while (foreignLinksRec.foreignLinks(query))
    {
      LinksTo linkTo = new LinksTo();
      linkTo.tableName = foreignLinksRec.tableName;
      linkTo.linkName = foreignLinksRec.linkName;
      links.addElement(linkTo);
    }
    for (int i = 0; i < links.size(); i++)
    {
      LinksTo linkTo = (LinksTo) links.elementAt(i);
      descend(linkTo.tableName, 0, outLog);
    }
    for (int i = 0; i < tables.size(); i++)
    {
      table = (Table) tables.elementAt(i);
      database.tables.addElement(table);
    }
  }
  static void descend(String tableName, int depth, PrintWriter outLog)
  {
    if (depth > 20)
    {
      outLog.println("Possible Cycle on TABLE  " + tableName);
      return;
    }
    for (int i = 0; i < links.size(); i++)
    {
      LinksTo linkTo = (LinksTo) links.elementAt(i);
      if (linkTo.tableName.compareTo(tableName) < 0)
        continue;
      if (linkTo.tableName.compareTo(tableName) > 0)
        break;
      descend(linkTo.linkName, depth + 1, outLog);
    }
    for (int i = 0; i < tables.size(); i++)
    {
      table = (Table) tables.elementAt(i);
      if (table.name.compareTo(tableName) < 0)
        continue;
      if (table.name.compareTo(tableName) > 0)
        break;
      database.tables.addElement(table);
      tables.removeElementAt(i);
      break;
    }
  }
  static void buildStdProcs(PrintWriter outLog)
  {
    table.hasStdProcs = true;
    buildInsert(outLog);
    if (table.hasPrimaryKey)
    {
      buildUpdate(outLog);
      buildDeleteOne(outLog);
      buildSelectOne(outLog);
      buildExists(outLog);
    }
    buildDeleteAll(outLog);
    buildSelectAll(outLog);
    buildCount(outLog);
  }
  static void buildInsert(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "Insert";
    table.buildInsert(proc);
    table.procs.addElement(proc);
  }
  static void buildUpdate(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "Update";
    table.buildUpdate(proc);
    table.procs.addElement(proc);
  }
  static void buildDeleteOne(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "DeleteOne";
    table.buildDeleteOne(proc);
    table.procs.addElement(proc);
  }
  static void buildExists(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "Exists";
    table.buildExists(proc);
    table.procs.addElement(proc);
  }
  static void buildSelectOne(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "SelectOne";
    table.buildSelectOne(proc, false, false);
    table.procs.addElement(proc);
  }
  static void buildDeleteAll(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "DeleteAll";
    table.buildDeleteAll(proc);
    table.procs.addElement(proc);
  }
  static void buildSelectAll(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "SelectAll";
    table.buildSelectAll(proc, false, false, false, false);
    table.procs.addElement(proc);
  }
  static void buildCount(PrintWriter outLog)
  {
    proc = new Proc();
    proc.table = table;
    proc.name = "Count";
    table.buildCount(proc);
    table.procs.addElement(proc);
  }
  static void loadTableFields(PrintWriter outLog) throws SQLException
  {
    Oracle.TableSequence tableSequenceRec = oracle.getTableSequence();
    Vector sequenceFields = new Vector();
    Vector typeCFields = new Vector();
    String sequenceName = table.name + "SEQ";
    Query query1 =
      tableSequenceRec.tableSequence(database.userid, table.name, sequenceName);
    while (tableSequenceRec.tableSequence(query1))
      sequenceFields.addElement(tableSequenceRec.columnName);
    Oracle.TableTypeC tableTypeCRec = oracle.getTableTypeC();
    Query query3 = tableTypeCRec.tableTypeC(database.userid, table.name);
    while (tableTypeCRec.tableTypeC(query3))
    {
      String condition = tableTypeCRec.condition;
      if (condition.indexOf("IS NOT NULL") == -1)
        typeCFields.addElement(condition);
    }
    Oracle.TableColumns tableColumnsRec = oracle.getTableColumns();
    Query query2 = tableColumnsRec.tableColumns(database.userid, table.name);
    while (tableColumnsRec.tableColumns(query2))
    {
      field = new Field();
      field.name = tableColumnsRec.columnName;
      field.alias = caseChange(field.name);
      boolean usedSeq =
        fieldType(
                sequenceFields.indexOf(field.name) != -1,
          tableColumnsRec.dataType,
          tableColumnsRec.dataLength,
          tableColumnsRec.dataPrecision,
          tableColumnsRec.dataScale);
      if (usedSeq)
      {
        table.hasSequence = true;
        sequencesDone.addElement(sequenceName);
      }
      field.isNull = (tableColumnsRec.nullable.compareTo("Y") == 0);
      if (tableColumnsRec.defaultLength != -1)
        field.defaultValue = tableColumnsRec.defaultValue;
      for (int i = 0; i < typeCFields.size(); i++)
      {
        String condition = (String) typeCFields.elementAt(i);
        if (condition.indexOf(field.name + " ") == 0)
          field.checkValue = condition;
      }
      table.fields.addElement(field);
    }
  }
  static boolean fieldType(
    boolean isSeqField,
    String dataType,
    int dataLength,
    int dataPrecision,
    int dataScale)
  {
    boolean result = false;
    if (dataType.compareTo("NUMBER") == 0)
    {
      if (dataScale < 0 || dataPrecision < 0)
      {
        field.type = Field.FLOAT;
        field.length = 8;
        field.precision = 0;
        field.scale = 0;
      } else if (dataScale > 0)
      {
        field.type = Field.FLOAT;
        field.length = 8;
        field.precision = dataPrecision;
        field.scale = dataScale;
      } else if (dataPrecision > 10)
      {
        field.type = Field.LONG;
        field.length = 8;
        field.precision = dataPrecision;
        field.scale = 0;
      } else if (dataPrecision > 5)
      {
        if (isSeqField)
        {
          field.type = Field.SEQUENCE;
          result = true;
        } else
          field.type = Field.INT;
        field.length = 4;
        field.precision = dataPrecision;
        field.scale = 0;
      } else if (dataPrecision > 3)
      {
        field.type = Field.SHORT;
        field.length = 2;
        field.precision = dataPrecision;
        field.scale = 0;
      } else
      {
        field.type = Field.BYTE;
        field.length = 1;
        field.precision = dataPrecision;
        field.scale = 0;
      }
    } else if (dataType.compareTo("DATE") == 0)
    {
      field.type = Field.DATETIME;
      field.length = 14;
    } else // treat all else as char
      {
      field.type = Field.CHAR;
      field.length = dataLength;
    }
    return result;
  }
  static void loadTablePrimaryKey(PrintWriter outLog) throws SQLException
  {
    Oracle.TablePrimaryKey tablePrimaryKeyRec = oracle.getTablePrimaryKey();
    if (tablePrimaryKeyRec.tablePrimaryKey(database.userid, table.name))
    {
      key = new Key();
      key.name = tablePrimaryKeyRec.constraintName;
      key.isPrimary = true;
      table.hasPrimaryKey = true;
      Oracle.ConstraintColumns constraintColumnsRec = oracle.getConstraintColumns();
      Query query = constraintColumnsRec.constraintColumns(database.userid, key.name);
      while (constraintColumnsRec.constraintColumns(query))
      {
        table.setPrimary(constraintColumnsRec.column);
        key.fields.addElement(constraintColumnsRec.column);
      }
      Oracle.GetIndexTableSpace getIndexTableSpaceRec = oracle.getGetIndexTableSpace(); 
      if (getIndexTableSpaceRec.getIndexTableSpace(database.userid, key.name))
        key.options.addElement(
          "USING INDEX TABLESPACE "
            + getIndexTableSpaceRec.tableSpaceName
            + " STORAGE (INITIAL "
            + setKorM(getIndexTableSpaceRec.initialExtent)
            + " NEXT "
            + setKorM(getIndexTableSpaceRec.nextExtent)
            + ")");
      table.keys.addElement(key);
    }
  }
  static void loadTableUniqueKeys(PrintWriter outLog) throws SQLException
  {
    Oracle.TableUniqueKeys tableUniqueKeysRec = oracle.getTableUniqueKeys();
    Query query1 = tableUniqueKeysRec.tableUniqueKeys(database.userid, table.name);
    while (tableUniqueKeysRec.tableUniqueKeys(query1))
    {
      key = new Key();
      key.name = tableUniqueKeysRec.constraintName;
      key.isUnique = true;
      Oracle.ConstraintColumns constraintColumnsRec = oracle.getConstraintColumns();
      Query query2 = constraintColumnsRec.constraintColumns(database.userid, key.name);
      while (constraintColumnsRec.constraintColumns(query2))
        key.fields.addElement(constraintColumnsRec.column);
      Oracle.GetIndexTableSpace getIndexTableSpaceRec = oracle.getGetIndexTableSpace();
      if (getIndexTableSpaceRec.getIndexTableSpace(database.userid, key.name))
        key.options.addElement(
          "USING INDEX TABLESPACE "
            + getIndexTableSpaceRec.tableSpaceName
            + " STORAGE (INITIAL "
            + setKorM(getIndexTableSpaceRec.initialExtent)
            + " NEXT "
            + setKorM(getIndexTableSpaceRec.nextExtent)
            + ")");
      table.keys.addElement(key);
    }
  }
  static void loadTableIndexes(PrintWriter outLog) throws SQLException
  {
    Oracle.TableIndexes tableIndexesRec = oracle.getTableIndexes();
    Query query1 = tableIndexesRec.tableIndexes(database.userid, table.name);
    while (tableIndexesRec.tableIndexes(query1))
    {
      key = new Key();
      key.name = tableIndexesRec.indexName;
      Oracle.IndexColumns indexColumnsRec = oracle.getIndexColumns();
      Query query2 = indexColumnsRec.indexColumns(database.userid, key.name);
      while (indexColumnsRec.indexColumns(query2))
        key.fields.addElement(indexColumnsRec.column);
      key.options.addElement(
        "TABLESPACE "
          + tableIndexesRec.tableSpaceName
          + " STORAGE (INITIAL "
          + setKorM(tableIndexesRec.initialExtent)
          + " NEXT "
          + setKorM(tableIndexesRec.nextExtent)
          + ")");
      table.keys.addElement(key);
    }
  }
  static void loadTableForeignKeys(PrintWriter outLog) throws SQLException
  {
    Oracle.TableForeignKeys tableForeignKeysRec = oracle.getTableForeignKeys();
    Query query1 = tableForeignKeysRec.tableForeignKeys(database.userid, table.name);
    while (tableForeignKeysRec.tableForeignKeys(query1))
    {
      link = new Link();
      link.name = tableForeignKeysRec.rTableName;
      link.linkName = tableForeignKeysRec.constraintName;
      Oracle.ConstraintColumns constraintColumnsRec = oracle.getConstraintColumns();
      Query query2 = constraintColumnsRec.constraintColumns(database.userid, link.linkName);
      while (constraintColumnsRec.constraintColumns(query2))
        link.fields.addElement(constraintColumnsRec.column);
      table.links.addElement(link);
    }
  }
  static void loadTableGrants(PrintWriter outLog) throws SQLException
  {
    Oracle.TableGrants tableGrantsRec = oracle.getTableGrants();
    Query query1 = tableGrantsRec.tableGrants(database.userid, table.name);
    while (tableGrantsRec.tableGrants(query1))
    {
      grant = new Grant();
      grant.perms.addElement(tableGrantsRec.privilege);
      grant.users.addElement(tableGrantsRec.grantee);
      table.grants.addElement(grant);
    }
  }
  static void loadViews(PrintWriter outLog) throws SQLException
  {
    Oracle.Views viewsRec = oracle.getViews();
    Query query1 = viewsRec.views(database.userid);
    while (viewsRec.views(query1))
    {
      view = new View();
      view.name = viewsRec.name;
      String text = viewsRec.text;
      while (text.indexOf("\n") != -1)
      {
        int n = text.indexOf("\n");
        String line = text.substring(0, n);
        text = text.substring(n + 1);
        view.lines.addElement(line);
      }
      Oracle.ViewColumns viewColumnsRec = oracle.getViewColumns();
      Query query2 = viewColumnsRec.viewColumns(database.userid, view.name);
      while (viewColumnsRec.viewColumns(query2))
        view.aliases.addElement(viewColumnsRec.column);
      database.views.addElement(view);
    }
  }
  static void loadSequences(PrintWriter outLog) throws SQLException
  {
    try
    {
      Oracle.Sequences sequencesRec = oracle.getSequences();
      Query query1 = sequencesRec.sequences(database.userid);
      while (sequencesRec.sequences(query1))
      {
        outLog.println(sequencesRec.name);
        outLog.flush();
        if (sequencesDone.indexOf(sequencesRec.name) != -1)
          continue;
        sequence = new Sequence();
        sequence.name = sequencesRec.name;
        sequence.minValue = (int) sequencesRec.minValue;
        if (sequencesRec.maxValue < 1000000000)
          sequence.maxValue = (int) sequencesRec.maxValue;
        else
          sequence.maxValue = 999999999;
        sequence.increment = (int) sequencesRec.increment;
        sequence.cycleFlag = (sequencesRec.cycleFlag.compareTo("Y") == 0);
        sequence.orderFlag = (sequencesRec.orderFlag.compareTo("Y") == 0);
        if (sequencesRec.startWith < 1000000000)
          sequence.startWith = (int) sequencesRec.startWith;
        else
          sequence.startWith = 0;
        database.sequences.addElement(sequence);
      }
    } catch (SQLException e)
    {
      outLog.println(sqlExceptionMessage(e));
      outLog.flush();
    }
  }
}
