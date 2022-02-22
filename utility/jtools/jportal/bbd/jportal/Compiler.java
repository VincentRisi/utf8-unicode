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

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Compiler
{
  private static PrintWriter outLog;
  private static String inputs;
  private static String nubDir;
  private static String rootDir="";
  public static int compile(String source, String[] args, PrintWriter outLog) throws FileNotFoundException, ClassNotFoundException, SecurityException, NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException
  {
    String[] pieces = source.split("\\+");
    Database database = null;
    boolean hasErrors = false;
    for (int p = 0; p < pieces.length; p++)
    {
      Database db = JPortal.run(pieces[p], nubDir, outLog);
      if (db == null)
      {
        outLog.println("::>" + pieces[p] + "<:: compile has errors");
        outLog.flush();
        hasErrors = true;
        continue;
      }
      if (database == null)
        database = db;
      else
      {
        for (int t=0; t<db.tables.size(); t++)
          database.tables.addElement(db.tables.elementAt(t));
        for (int s=0; s<db.sequences.size(); s++)
          database.sequences.addElement(db.sequences.elementAt(s));
        for (int v=0; v<db.views.size(); v++)
          database.views.addElement(db.views.elementAt(v));
      }
    }
    if (hasErrors == true)
      return 1;
    outLog.flush();
    String output = "";
    boolean reported = false;
    for (int i=0; i<args.length; i++)
    {
      if (args[i].equals("-o"))
      {
        if (i+1 < args.length)
        {
          output = checkRoot(args[++i]);
          char term = '\\';
          if (output.indexOf('/') != -1)
            term = '/';
          char ch = output.charAt(output.length()-1);
          if (ch != term)
            output = output + term;
        }
        continue;
      }
      else if (args[i].equals("-R"))
      {
        if (i+1 < args.length)
          rootDir = args[++i];
        else
          rootDir = "";
        continue;
      }
      else if (args[i].equals("-l"))
      {
        if (i+1 < args.length)
        {
          String log = args[++i];
          OutputStream outFile = new FileOutputStream(log);
          outLog.flush();
          outLog = new PrintWriter(outFile);
        }
        continue;
      }
      else if (args[i].equals("-f") || args[i].equals("-F"))
      {
        if (i+1 < args.length)
        {
          String flag = args[++i];
          database.flags.addElement(flag);
        }
        continue;
      }
      else if (args[i].charAt(0) == '-')
      {
        if (reported == false)
        {
          reported = true;
          if (args[i].length() > 1)
          {
            char sw = args[i].charAt(1);
            outLog.println(String.format("Valid compile switch arguments here are (%c)", sw));
            outLog.println(" -o <name>     Directory to place output");
            outLog.println(" -(F|f) <flag> Flag to add to Database flags");
            outLog.println(" -l <name>     Change logging name");
          }
        }
        continue;
      }
      outLog.println(args[i]);
      Class c = Class.forName("bbd.jportal."+args[i]);
      Class[] d = {database.getClass(), output.getClass(), outLog.getClass()};
      Method m = c.getMethod("generate", d);
      Object[] o = {database, output, outLog};
      m.invoke(database, o);
    }
    outLog.flush();
    return 0;
  }
  private static String checkRoot(String name)
  {
    if (rootDir.length() == 0)
      return name;
    if (name.startsWith("{root}"))
      return rootDir + name.substring(6);
    if (name.startsWith("${root}"))
      return rootDir + name.substring(7);
    return name;
  }
  private static String[] frontSwitches(String[] args) throws IOException
  {
    String log = "";
    int i = 0;
    boolean reported = false;
    while (true)
    {
      if (args.length > i && args[i].equals("-l"))
      {
        if (i + 1 < args.length)
        {
          log = args[++i];
          OutputStream outFile = new FileOutputStream(log);
          outLog.flush();
          outLog = new PrintWriter(outFile);
        }
        i++;
        continue;
      }
      if (args[i].equals("-R"))
      {
        if (i+1 < args.length)
          rootDir = args[++i];
        else
          rootDir = "";
        i++;
        continue;
      }
      if (args.length > i && args[i].equals("-n"))
      {
        if (i + 1 < args.length)
          nubDir = args[++i];
        i++;
        continue;
      }
      if (args.length > i && (args[i].equals("-f") || args[i].equals("-i")))
      {
        if (i+1 < args.length)
        {
          String fileName = checkRoot(args[++i]);
          FileReader fileReader = new FileReader(fileName);
          BufferedReader bufferedReader = new BufferedReader(fileReader);
          try
          {
            String semicolon = inputs.length() > 0 ? ";" : "";
            while (bufferedReader.ready())
            {
              String line = checkRoot(bufferedReader.readLine());
              inputs = inputs + semicolon + line;
              semicolon = ";";
            }
          }
          catch (NullPointerException e2)
          {
          }
        }
        i++;
        break;
      }
      if (args.length > i && args[i].charAt(0) == '-')
      {
        if (reported == false)
        {
          reported = true;
          if (args[i].length() > 1)
          {
            char sw = args[i].charAt(1);
            outLog.println(String.format("Valid front switch arguments here are (%c)", sw));
            outLog.println(" -l <name>     Change logging name");
            outLog.println(" -(i|f) <name> File for list of .si files to compile");
            outLog.println(" -n <name>     Not really used nubdir");
          }
        }
        i++;
        continue;
      }
      break;
    }
    if (args.length > i && inputs.length() == 0)
    {
      inputs = args[i]; 
      i++;
    }
    String[] newargs = new String[args.length-i];
    System.arraycopy(args, i, newargs, 0, newargs.length);
    return newargs;
  }
  private static String abbreviate(String inputs)
  {
    String[] sources = inputs.split(";");
    if (sources.length > 5)
      return sources[0] + " ... " + sources[sources.length-1];
    return inputs;
  }
  /**
  * Reads input from stored repository
  */
  public static void main(String[] args)
  {
    try
    {
      outLog = new PrintWriter(System.out);
      inputs = "";
      nubDir = "";
      args = frontSwitches(args);
      System.out.println(abbreviate(inputs));
      if (args.length < 2)
      {
        outLog.println("usage java jportal.Compiler -l log (-(i|f) inputs | infile) (generators)+");
        outLog.println("for example to create DDL for Sql Server and Java, VB and Delphi code");
        outLog.println();
        outLog.println("java jportal.Compiler airline.si (-(F|f) flag)* -o ./dir1 MSSqlDDL -o ./dir2 JavaCode -o ./dir3 VBCode -o ./dir4 DelphiCode");
        outLog.flush();
        System.exit(1);
      }
      outLog.println(inputs);
      outLog.flush();
      String[] sources = inputs.split(";");
      for (int f=0; f<sources.length; f++)
      {
        int rc = compile(sources[f], args, outLog);
        outLog.flush();
        if (rc != 0)
          System.exit(1);
      }
      System.exit(0);
    }
    catch (Throwable e)
    {
      e.printStackTrace();
      outLog.println("Error: "+e);
      outLog.flush();
      System.exit(1);
    }
  }
}
