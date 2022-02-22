package bbd.crackle;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class PopGenServer extends Generator
{
  public static String description()
  {
    return "Generates Server Code (AIX|SUN|NT)";
  }

  public static String documentation()
  {
    return "Generates Server Code (AIX|SUN|NT)" + "\r\nHandles following pragmas"
        + "\r\n  GetConnectFromFile - reads ini file to get database connect."
        + "\r\n  AlignForSUN - ensure that all fields are on 8 byte boundaries."
        + "\r\n  NoDBConnect - do not require database activity." + "\r\n  DBFill - DBFill template must be generated."
        + "\r\n  Metrics - metric switch handling code to be generated."
        + "\r\n  JSON - JSON handling code to be generated.";
  }

  /*
   * Static constructor
   */
  {
    setupPragmaVector();
  }

  private static void setupPragmaVector()
  {
    if (pragmaVector == null || pragmaVector.size() == 0)
    {
      pragmaVector = new Vector<Pragma>();
      pragmaVector.addElement(new Pragma("AlignForSun", false, "Ensure that all fields are on 8 byte boundaries."));
      pragmaVector.addElement(new Pragma("DBConnect", true, "Requires automatic database activity."));
      pragmaVector.addElement(new Pragma("DBFill", true, "DBFill template must be generated."));
      pragmaVector.addElement(new Pragma("GetConnectFromFile", false, "Reads ini file to get database connect."));
      pragmaVector.addElement(new Pragma("Metrics", false, "Metric switch handling code to be generated."));
      pragmaVector.addElement(new Pragma("JSON", false, "JSON handling code to be generated."));
    }
  }

  private static boolean alignForSun;
  private static boolean doConnect;
  private static boolean doDBFill;
  private static boolean doNedTelShuffle;
  private static boolean doMetrics;
  private static boolean doJSON;
  
  private static boolean hasPython = false;

  private static void setPragmas(Module module)
  {
    // Ensure these are in the same order as aobove
    setupPragmaVector();
    alignForSun = pragmaVector.elementAt(0).value;
    doConnect = pragmaVector.elementAt(1).value;
    doDBFill = pragmaVector.elementAt(2).value;
    doNedTelShuffle = pragmaVector.elementAt(3).value;
    doMetrics = pragmaVector.elementAt(4).value;
    doJSON = pragmaVector.elementAt(5).value;
    for (int i = 0; i < module.pragmas.size(); i++)
    {
      String pragma = (String) module.pragmas.elementAt(i);
      if (pragma.trim().equalsIgnoreCase("GetConnectFromFile") == true)
        doNedTelShuffle = true;
      else if (pragma.trim().equalsIgnoreCase("AlignForSUN") == true)
        alignForSun = true;
      else if (pragma.trim().equalsIgnoreCase("NoDBConnect") == true)
        doConnect = false;
      else if (pragma.trim().equalsIgnoreCase("DBFill") == true)
        doDBFill = true;
      else if (pragma.trim().equalsIgnoreCase("Metrics") == true)
        doMetrics = true;
      else if (pragma.trim().equalsIgnoreCase("JSON") == true)
        doJSON = true;
    }
  }

  private static boolean hasShutDownCode = false;

  /**
   * Reads input from stored repository
   */
  public static void main(String[] args)
  {
    try
    {
      PrintWriter outLog = new PrintWriter(System.out);
      logger = outLog;
      for (int i = 0; i < args.length; i++)
      {
        logln(args[i] + ": Generate ... ");
        ObjectInputStream in = new ObjectInputStream(new FileInputStream(args[i]));
        Module module = (Module) in.readObject();
        in.close();
        generate(module, "", outLog);
      }
      outLog.flush();
    } catch (Exception e)
    {
      e.printStackTrace();
    }
  }

  /**
   * Generates - C Header for Server - C Server Marshalling code
   */
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    logger = outLog;
    logln(module.name + " version " + module.version);
    setPragmas(module);
    for (int i = 0; i < module.prototypes.size(); i++)
    {
      Prototype prototype = (Prototype) module.prototypes.elementAt(i);
      if (prototype.isPythonImpl() == true)
      {
        hasPython = true;
        break;
      }
    }
    generateCHeader(module, output);
    if (module.tables.size() > 0)
    {
      generateCTablesHeader(module, output);
      generateCTablesCode(module, output);
    }
    generateReqIDTable(module, output);
    if (generateCServer(module, output))
      generateCCode(module, output);
  }

  private static void generateCTablesHeader(Module module, String output)
  {
    try
    {
      logln("Code: " + output + module.name.toLowerCase() + "tables.h");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "tables.h");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#ifndef _" + module.name + "Tables_H_");
        writeln("#define _" + module.name + "Tables_H_");
        writeln();
        for (int i = 0; i < module.tables.size(); i++)
        {
          Table table = (Table) module.tables.elementAt(i);
          writeln("enum e" + module.name + table.name);
          writeln("{");
          String comma = "  ";
          for (int j = 0; j < table.messages.size(); j++)
          {
            Message message = (Message) table.messages.elementAt(j);
            writeln(1, comma + table.name.toLowerCase() + message.name + "   // " + message.value);
            comma = ", ";
          }
          writeln(comma + table.name.toLowerCase() + "NoOf");
          writeln("};");
          writeln();
          writeln("extern char *" + module.name + table.name + "[];");
          writeln();
        }
        writeln("#endif");
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      logln("Generate Procs IO Error");
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  private static void generateCTablesCode(Module module, String output)
  {
    try
    {
      logln("Code: " + output + module.name.toLowerCase() + "tables.cpp");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "tables.cpp");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#include \"machine.h\"");
        writeln();
        writeln("#include \"" + module.name.toLowerCase() + "tables.h\"");
        writeln();
        for (int i = 0; i < module.tables.size(); i++)
        {
          Table table = (Table) module.tables.elementAt(i);
          String comma = "  ";
          writeln("char *" + module.name + table.name + "[] = ");
          writeln("{");
          for (int j = 0; j < table.messages.size(); j++)
          {
            Message message = (Message) table.messages.elementAt(j);
            writeln(1, comma + message.value + "   // " + message.name);
            comma = ", ";
          }
          writeln("};");
          writeln();
        }
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      logln("Generate Procs IO Error");
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  private static String cleanUp(String data)
  {
    if (data.indexOf("HEADER:") == 0)
      return data.substring(7);
    if (data.indexOf("CODE:") == 0)
      return data.substring(5);
    if (data.indexOf("BOTH:") == 0)
      return data.substring(5);
    return data;
  }

  private static void generateCHeader(Module module, String output) //, PrintWriter outLog)
  {
    try
    {
      logln("Code: " + output + module.name.toLowerCase() + ".h");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + ".h");
      writer = new PrintWriter(outFile);
      try
      {
        String upperModule = module.name.toUpperCase();
        boolean hasProtected = false;
        boolean hasSlackHandler = false;
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#ifndef _" + module.name + "H_");
        writeln("#define _" + module.name + "H_");
        writeln("#include \"machine.h\"");
        writeln("#include \"popgen.h\"");
        writeln("#include \"logfile.h\"");
        writeln("#include \"ini.h\"");
        if (doConnect == true)
          writeln("#include \"dbportal.h\"");
        if (hasPython == true || doJSON == true)
        {
          writeln("#include \"json/json.h\"");
          writeln("using namespace Json;");
          writeln("#include \"json_databuild.h\"");
        }
        writeln("#ifndef " + upperModule + "_STANDALONE");
        writeln("#  if defined(M_AIX)");
        writeln("#    include \"snaprpcaixserver.h\"");
        writeln("#  elif defined(M_GNU)");
        writeln("#    include \"snaprpcgnuserver.h\"");
        writeln("#  else");
        writeln("#    include \"snaprpcntserver.h\"");
        writeln("#  endif");
        writeln("#  include \"snapprocess.h\"");
        writeln("#endif");
        writeln();
        PopGen.generateCTableAndEnums(module, writer, true);    
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.name.equals("SlackTimeHandler") == true)
          {
              hasSlackHandler = true;
              break;
          }
        }
        if (module.code.size() > 0)
        {
          boolean firstTime = true;
          for (int j = 0; j < module.code.size(); j++)
          {
            String codeLine = (String) module.code.elementAt(j);
            if (codeLine.indexOf("PYTHON:") >= 0)
              break;
            boolean outputIt = false;
            // #include go to header - unless flagged as CODE
            if (codeLine.indexOf("#include") >= 0 && codeLine.indexOf("CODE:") < 0)
              outputIt = true;
            // #other code goes to header if flagged as BOTH or HEADER
            else if (codeLine.indexOf("HEADER:") >= 0 || codeLine.indexOf("BOTH:") >= 0)
              outputIt = true;
            if (outputIt == true)
            {
              if (firstTime)
                firstTime = false;
              write(cleanUp(codeLine));
            }
          }
          if (firstTime == false)
            writeln();
        }
        PopGen.generateCExterns(module, writer);
        writeln("extern char *" + module.name + "Version;");
        writeln("extern int32 " + module.name + "Signature;");
        writeln("extern char *ConfigFile;");
        writeln();
        PopGen.generateCStructs(module, writer, true);
        writeln();
        writeln("#  if !defined " + upperModule + "_STANDALONE");
        writeln("class t" + module.name + "Server;");
        writeln("class t" + module.name + " : public tSnapIDL");
        writeln("{");
        writeln("#  elif defined HTTP_SERVER");
        writeln("#include \"httpserver.h\"");
        writeln("class HTTPServer;");
        if (hasSlackHandler == true)
          writeln("void SlackHandler(void *);");
        writeln("class t" + module.name);
        writeln("{");
        writeln("#  else");
        writeln("class t" + module.name);
        writeln("{");
        writeln("#  endif");
        writeln();
        writeln("private:");
        writeln("#  if !defined " + upperModule + "_STANDALONE");
        writeln(1, "tRPCServer *RPC;");
        writeln(1, "t" + module.name + "Server &" + module.name + "Server;");
        writeln("#  elif defined HTTP_SERVER");
        writeln(1, "HTTPServer *RPC;");
        writeln("#  endif");
        writeln();
        for (int i = 0; i < module.structures.size(); i++)
        {
          Structure structure = (Structure) module.structures.elementAt(i);
          if (structure.codeType == Structure.PROTECTED)
            hasProtected = true;
          if (structure.codeType != Structure.PRIVATE)
            continue;
          for (int j = 0; j < structure.fields.size(); j++)
          {
            Field field = (Field) structure.fields.elementAt(j);
            writeln(1, field.type.cDef(field.name, false) + ";");
          }
        }
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType == Prototype.PROTECTED)
            hasProtected = true;
          if (prototype.codeType != Prototype.PRIVATE)
            continue;
          PopGen.generateCHeader(module, prototype, writer);
        }
        writeln("public:");
        if (doConnect == true)
          writeln(1, "tDBConnect &Connect;");
        writeln(1, "tLogFile   &LogFile;");
        for (int i = 0; i < module.structures.size(); i++)
        {
          Structure structure = (Structure) module.structures.elementAt(i);
          if (structure.codeType != Structure.PUBLIC)
            continue;
          for (int j = 0; j < structure.fields.size(); j++)
          {
            Field field = (Field) structure.fields.elementAt(j);
            writeln(1, field.type.cDef(field.name, false) + ";");
          }
        }
        writeln("#  if !defined " + upperModule + "_STANDALONE");
        if (doConnect == true)
        {
          writeln(1, "t" + module.name + "(tDBConnect &aConnect");
          writeln(1, "               , tLogFile &aLogFile");
          writeln(1, "               , t" + module.name + "Server &a" + module.name + "Server)");
          writeln(1, ": Connect(aConnect)");
          writeln(1, ", LogFile(aLogFile)");
        } else
        {
          writeln(1, ", t" + module.name + "(tLogFile &aLogFile");
          writeln(1, "                 , t" + module.name + "Server &a" + module.name + "Server)");
          writeln(1, ": LogFile(aLogFile)");
        }
        writeln(1, ", " + module.name + "Server(a" + module.name + "Server)");
        writeln(1, "{");
        writeln(1, "}");
        writeln(1, "void setRPC(tRPCServer *aRPC) {RPC = aRPC;}");
        writeln("#  elif defined HTTP_SERVER");
        writeln(1, format("t%s(tDBConnect& aConnect", module.name));
        writeln(2, ", tLogFile& aLogFile)");
        writeln(2, ": Connect(aConnect)");
        writeln(2, ", LogFile(aLogFile)");
        writeln(1, "{");
        writeln(1, "}");
        //writeln(1, "void setRPC(HTTPServer *aRPC) {RPC = aRPC;}");
        writeln("#  else");
        writeln(1, format("t%s(tDBConnect& aConnect", module.name));
        writeln(2, ", tLogFile& aLogFile)");
        writeln(2, ": Connect(aConnect)");
        writeln(2, ", LogFile(aLogFile)");
        writeln(1, "{");
        writeln(1, "}");
        writeln("#  endif");
        writeln();
        writeln("#  if defined HTTP_SERVER");
        writeln(1, "void StartUpCode(HTTPServer* aRPC)");
        writeln(1, "{");
        writeln(2, "RPC = aRPC;");
        writeln("#  else");
        writeln(1, "void StartUpCode()");
        writeln(1, "{");
        writeln("#  endif");
        //final byte in_ALL=0, in_TCPIP=1, in_HTTP=2;
        //byte state = in_ALL;
        boolean closeEndif = false;
        for (int i = 0; i < module.structures.size(); i++)
        {
          Structure structure = (Structure) module.structures.elementAt(i);
          if (structure.codeType == Structure.NORMAL)
            continue;
          for (int j = 0; j < structure.code.size(); j++)
          {
            String codeLine = (String) structure.code.elementAt(j);
            if ((codeLine.trim()).equalsIgnoreCase("inall:"))
            {
              if (closeEndif == true)
              {
                closeEndif = false;
                writeln("#  endif");
                writeln();
              }
              //state=in_ALL;
              continue;
            }
            if ((codeLine.trim()).equalsIgnoreCase("intcp:"))
            {
              if (closeEndif == true)
              {
                closeEndif = false;
                writeln("#  endif");
                writeln();
              }
              //state=in_TCPIP;
              writeln(format("#  if !defined %s_STANDALONE", upperModule));
              closeEndif = true;
              continue;
            }
            if ((codeLine.trim()).equalsIgnoreCase("inhttp:"))
            {
              if (closeEndif == true)
              {
                closeEndif = false;
                writeln("#  endif");
                writeln();
              }
              //state=in_HTTP;
              writeln("#  if defined HTTP_SERVER");
              closeEndif = true;
              continue;
            }
            if ((codeLine.trim()).equalsIgnoreCase("onshutdown:"))
            {
              if (closeEndif == true)
              {
                closeEndif = false;
                writeln("#  endif");
                writeln();
              }
              //state = in_ALL;
              writeln(1, "}");
              writeln(1, "void ShutDownCode()");
              writeln(1, "{");
              hasShutDownCode = true;
            } 
            else
              write(1, codeLine);
          }
          if (closeEndif == true)
          {
            closeEndif = false;
            writeln("#  endif");
            writeln();
          }
          //state = in_ALL;
          writeln(1, "}");
        }
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType == Prototype.PRIVATE || prototype.codeType == Prototype.PROTECTED)
            continue;
          PopGen.generateCHeader(module, prototype, writer);
        }
        if (hasPython == true)
          writeln(1, "void (*runPython)(string scriptName, string input, string &output);");
        if (hasProtected == true)
        {
          writeln("protected:");
          for (int i = 0; i < module.structures.size(); i++)
          {
            Structure structure = (Structure) module.structures.elementAt(i);
            if (structure.codeType != Structure.PROTECTED)
              continue;
            for (int j = 0; j < structure.fields.size(); j++)
            {
              Field field = (Field) structure.fields.elementAt(j);
              writeln(1, field.type.cDef(field.name, false) + ";");
            }
          }
          for (int i = 0; i < module.prototypes.size(); i++)
          {
            Prototype prototype = (Prototype) module.prototypes.elementAt(i);
            if (prototype.codeType != Prototype.PROTECTED)
              continue;
            PopGen.generateCHeader(module, prototype, writer);
          }
        }
        writeln("};");
        writeln();
        writeln("#  if !defined " + upperModule + "_STANDALONE");
        writeln("class t" + module.name + "Server : public tSnapProcess");
        writeln("{");
        writeln("public:");
        writeln(1, "t" + module.name + "Server(tString IniFile);");
        writeln(1, "~t" + module.name + "Server();");
        writeln(1, "void ServiceClient();");
        writeln(1, "t" + module.name + " *" + module.name + ";");
        writeln(1, "tRPCServer *RPC;");
        if (doConnect == true)
        {
          writeln(1, "tString BinFile;");
          writeln(1, "tString Connection;");
          writeln(1, "int DBTimeout;");
        }
        writeln(1, "#ifdef UPM_AWARE");
        writeln(1, "tString UserID;");
        writeln(1, "tString HostName;");
        writeln(1, "#endif");
        if (doConnect == true)
          writeln(1, "tDBConnect *Connect;");
        writeln(1, "char *ReplyBody;");
        writeln(1, "int32 Result;");
        writeln(1, "int32 ReplySize;");
        writeln(1, "int32 RecvSize;");
        writeln(1, "void ReadIniFile(tString IniFile);");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          writeln(1, "e" + module.name + " " + prototype.name + "(char *ip);");
        }
        writeln("};");
        writeln("#  endif");
        writeln();
        writeln("#endif");
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      logln("Generate Procs IO Error");
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  private static void generateCCode(Module module, String output)
  {
    try
    {
      logln("Code: " + output + module.name.toLowerCase() + "impl.cpp");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "impl.cpp");
      writer = new PrintWriter(outFile);
      try
      {
        String upperModule = module.name.toUpperCase();
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln();
        writeln("#include \"ti.h\"");
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln("#include <string.h>");
        writeln("#include <assert.h>");
        writeln();
        writeln("#include \"" + module.name.toLowerCase() + ".h\"");
        writeln();
        writeln("#ifdef DIM");
        writeln("#undef DIM");
        writeln("#endif");
        writeln();
        writeln("#define DIM(a)   (sizeof(a) / sizeof(a[0]))");
        writeln();
        writeln("const char *" + module.name + "Errors[] = ");
        writeln("{ \"No Error\"");
        for (int i = 0; i < module.messages.size(); i++)
        {
          Message message = (Message) module.messages.elementAt(i);
          writeln(", " + message.value + "   // " + message.name);
        }
        writeln(", \"Invalid Signature\"");
        writeln(", \"Invalid Logon Cookie\"");
        writeln(", \"Invalid INI File\"");
        writeln(", \"Uncaught DB Connect Error\"");
        writeln(", \"Unknown function call\"");
        writeln(", \"Snap Creation Error\"");
        writeln(", \"AddList (Index == 0) != (List == 0) Error\"");
        writeln(", \"AddList (realloc == 0) Error\"");
        writeln(", \"Last error not in message table\"");
        writeln("};");
        writeln();
        writeln("const char *" + module.name + "Error(int no)");
        writeln("{");
        writeln(1, "int size = DIM(" + module.name + "Errors);");
        writeln(1, "if (no == 0) return " + module.name + "Errors[0];");
        writeln(1, "int index = (no+1) - " + module.messageBase + ";");
        writeln(1, "if (index >= 0 && index < size)");
        writeln(2, "return " + module.name + "Errors[index];");
        writeln(1, "return \"<<error index out of range>>\";");
        writeln("}");
        writeln();
        writeln("#  ifndef " + upperModule + "_STANDALONE");
        writeln("#define THROW(err) throw " + upperModule + "_##err");
        writeln("#define THROW2(err, str) RPC->ErrBuffer(str);throw " + module.name.toUpperCase() + "_##err");
        writeln("#define GETERRSTR RPC->ErrBuffer()");
        writeln("#  else");
        writeln("TBChar " + upperModule + "_ErrBuffer;");
        writeln("#define THROW(err) throw " + upperModule + "_##err");
        writeln("#define THROW2(err, str) " + upperModule + "_ErrBuffer.set(str);throw " + module.name.toUpperCase() + "_##err");
        writeln("#define GETERRSTR " + upperModule + "_ErrBuffer.data");
        writeln("#  endif");
        writeln();
        writeln("template <class TYPE, class INDEX>");
        writeln("void AddList(TYPE*& List, INDEX& Index, const TYPE& Rec, const INDEX Delta)");
        writeln("{");
        writeln(1, "if ((List == 0) != (Index == 0))");
        writeln(2, "THROW(ADDLIST_ERROR);");
        writeln(1, "if (Index % Delta == 0)");
        writeln(1, "{");
        writeln(2, "TYPE* Resized = (TYPE*) realloc(List, sizeof(Rec)*(Index+Delta));");
        writeln(2, "if (Resized == 0) // Cater for alloc failure");
        writeln(3, "THROW(ADDLIST_REALLOC_FAILED);");
        writeln(2, "List = Resized;");
        writeln(1, "}");
        writeln(1, "List[Index++] = Rec;");
        writeln("}");
        writeln();
        if (doConnect == true && doDBFill == true)
        {
          writeln("template <class REC, class QUERY> ");
          writeln("struct tDBFill : public QUERY");
          writeln("{");
          writeln(1, "REC& Rec;");
          writeln(1, "tDBFill(tDBConnect &Connect, REC& aRec) ");
          writeln(1, ": QUERY(Connect, &aRec)");
          writeln(1, ", Rec(aRec) {}");
          writeln(1, "int32 Limited(REC* List, int32 Limit)");
          writeln(1, "{");
          writeln(2, "int Count = 0;");
          writeln(2, "while (Count < Limit && Fetch())");
          writeln(3, "List[Count++] = Rec;");
          writeln(2, "return Count;");
          writeln(1, "}");
          writeln(1, "int32 Dynamic(REC*& List)");
          writeln(1, "{");
          writeln(2, "int32 Count = 0;");
          writeln(2, "while (Fetch())");
          writeln(3, "AddList(List, Count, Rec, 16);");
          writeln(2, "return Count;");
          writeln(1, "}");
          writeln(1, "int32 DynamicLimited(REC*& List, int32 Limit)");
          writeln(1, "{");
          writeln(2, "int32 Count = 0;");
          writeln(2, "while (Count < Limit && Fetch())");
          writeln(3, "AddList(List, Count, Rec, 16);");
          writeln(2, "return Count;");
          writeln(1, "}");
          writeln("};");
          writeln();
        }
        if (module.code.size() > 0)
        {
          boolean firstTime = true;
          for (int j = 0; j < module.code.size(); j++)
          {
            String codeLine = (String) module.code.elementAt(j);
            boolean outputIt = true;
            if (codeLine.indexOf("PYTHON:") >= 0)
              break;
            if (codeLine.indexOf("HEADER:") >= 0)
              outputIt = false;
            else if (codeLine.indexOf("BOTH:") >= 0 || codeLine.indexOf("CODE:") >= 0)
              ;// outputIt = true;
            else if (codeLine.indexOf("#include") >= 0)
              outputIt = false;
            if (firstTime == true)
              firstTime = false;
            if (outputIt == true)
              write(cleanUp(codeLine));
          }
          if (firstTime == false)
            writeln();
        }
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.isPythonImpl() == true)
          {
            generateForPython(module, prototype);
            continue;
          }
          if (prototype.code.size() == 0)
            continue;
          PopGen.generateCImplCode(module, prototype, writer);
          writeln("{");
          for (int j = 0; j < prototype.code.size(); j++)
          {
            String codeLine = (String) prototype.code.elementAt(j);
            write(codeLine);
          }
          writeln("}");
          writeln();
        }
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      logln("Generate Procs IO Error");
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  private static void generateForPython(Module module, Prototype prototype)
  {
    boolean hasInput = false;
    boolean hasOutput = false;
    boolean hasResult = false;
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field = input.getParameter(prototype);
      if (field == null)
        continue;
      hasInput = true;
      break;
    }
    if (prototype.type.typeof != Type.VOID)
    {
      if (prototype.type.reference != Type.BYPTR)
        hasResult = true;
      else
        hasOutput = true;
    }
    if (prototype.outputs.size() > 0)
      hasOutput = true;
    PopGen.generateCImplCode(module, prototype, writer);
    writeln("{");
    writeln(1, "if (runPython == 0)");
    if (hasResult)
      writeln(2, "return 0;");
    else
      writeln(2, "return;");
    writeln(1, "try");
    writeln(1, "{");
    if (hasResult)
      writeln(2, "int32 returnCode;");
    writeln(2, "string input, output;");
    writeln(2, "DataBuilderJson builder = DataBuilderJson();");
    if (hasInput)
      generateForPythonInputs(prototype);
    writeln(2, format("string scriptName = \"%s\";", prototype.name));
    writeln(2, "runPython(scriptName, input, output);");
    if (hasOutput || hasResult)
      generateForPythonOutputs(prototype, hasResult);
    writeln(1, "}");
    writeln(1, "catch (...)");
    writeln(1, "{");
    writeln(2, "TBChar errorBuf;");
    writeln(2, "PyErrorHandler(errorBuf);");
    writeln(2, "THROW2(PYTHON_ERROR, errorBuf.data);");
    writeln(1, "}");
    writeln("}");
    writeln();
  }

  private static void generateForPythonOutputs(Prototype prototype, boolean hasResult)
  {
    writeln(2, "Value output_value;");
    writeln(2, "stringstream sstr(output);");
    writeln(2, "sstr >> output_value;");
    if (hasResult)
      writeln(2, "returnCode = output_value[\"returnCode\"].asInt();");
    for (int i=0; i < prototype.outputs.size(); i++) 
    {
      Action action = (Action) prototype.outputs.elementAt(i);
      Field field = action.getParameter(prototype);
      if (field == null)
        continue;
      boolean hasSize = action.hasSize();
      if (field.type.reference == Type.BYREFPTR)
      {
        if (field.type.typeof == Type.CHAR)
        {
          if (hasSize)
          {
            writeln(2, "//o BYREFPTR CHAR "+field.name);
            Operation op = action.sizeOperation();
            writeln(2, format("string %1$s_string = output_value[\"%1$s\"].asString();", field.name));
            writeln(2, format("%s = (char*) calloc(*%s + 1, 1);", field.name, op.name));
            writeln(2, format("memcpy(%1$s, %1$s_string.c_str(), *%2$s);", field.name, op.name));
          }
          else
            writeln(2, "//o BYREFPTR CHAR no size "+field.name);

          continue;
        }
        if (field.type.typeof == Type.USERTYPE)
        {
          if (hasSize)
          {
            writeln(2, "//o BYREFPTR USERTYPE "+field.name);
            Operation op = action.sizeOperation();
            writeln(2, format("%1$s = (%2$s*)calloc(*%3$s, sizeof(%2$s));", field.name, field.type.name, op.name));
            writeln(2, format("Value Recs_array = output_value[\"Recs\"];")); 
            writeln(2, format("for (int i = 0; i < *%s; i++)", op.name));
            writeln(2, "{");
            writeln(3, format("Value %1$s_value = %1$s_array[i];", field.name));
            writeln(3, format("builder.setValue(%s_value);", field.name));
            writeln(3, format("%s[i].SetData(builder);", field.name));
            writeln(2, "}");
          }
          else
            writeln(2, "//o BYREFPTR USERTYPE no size "+field.name);
          continue;
        }
        writeln(2, "//o BYREFPTR STDTYPE "+field.name);
      } 
      else if (field.type.reference == Type.BYPTR)
      {
        if (field.type.typeof == Type.CHAR)
        {
          writeln(1, "//o BYPTR CHAR "+field.name);
          continue;
        }
        if (field.type.typeof == Type.USERTYPE)
        {
          writeln(2, "//o BYPTR USERTYPE "+field.name);
          writeln(2, format("Value %1$s_value = output_value[\"%1$s\"];", field.name));
          writeln(2, format("builder.setValue(%s_value);", field.name));
          writeln(2, format("%s->SetData(builder);", field.name));
          continue;
        }
        writeln(2, "//o BYPTR STDTYPE "+field.name);
        writeln(2, format("*%s = output_value[\"%s\"]%s;", field.name, field.name, asType(field)));
      } 
      else
      {
        writeln(2, "//o no ref "+field.name);
      }
    }
    if (hasResult)
      writeln(2, "return returnCode;");
}

  private static void generateForPythonInputs(Prototype prototype)
  {
    writeln(2, "Value input_value;");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action action = (Action) prototype.inputs.elementAt(i);
      Field field = action.getParameter(prototype);
      if (field == null)
        continue;
      if (field.type.reference == Type.BYPTR)
      {
        if (action.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln(2, "//i BYPTR CHAR no size "+field.name);
            writeln(2, format("input_value[\"%s\"] = %s;", field.name, field.name));
          }
          else if (field.type.typeof == Type.USERTYPE)
          {
            writeln(2, "//i BYPTR USERTYPE no size "+field.name);
            writeln(2, format("%s->BuildData(builder);", field.name));
            writeln(2, format("string %s_result;", field.name));
            writeln(2, format("input_value[\"%s\"] = builder.getRecord(%s_result);", field.name, field.name));
          }
          else
          {
            writeln(2, "//i BYPTR STDTYPE no size "+field.name);
            writeln(2, format("input_value[\"%s\"] = *%s;", field.name, field.name));
          }
        }
        else // has size
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln(2, format("//i BYPTR CHAR has size %s;", field.name));
          }
          else if (field.type.typeof == Type.USERTYPE)
          {
            writeln(2, format("//i BYPTR USERTYPE has size %s;", field.name));
            Operation op = action.sizeOperation();
            writeln(2, "Value " + field.name + "_array;");
            writeln(2, format("for (int i=0; i < %s; i++)", op.name));
            writeln(2, "{");
            makeValueRec(field.name, 3);
            writeln(3, "" + field.name + "_array[i] = " + field.name + "_out;");
            writeln(2, "}");
            writeln(2, format("input_value[\"%s\"] = %s_array;", field.name, field.name));
          }
          else
          {
            writeln(2, format("//i BYPTR STD has size %s;", field.name));
          }
        }
      } 
      else if (field.type.reference == Type.BYREFPTR)
      {
        writeln(2, format("//i BYREFPTR %s;", field.name));
      }
      else
      {
        writeln(2, format("//i STDTYPE %s;", field.name));
        writeln(2, format("input_value[\"%s\"] = %s;", field.name, field.name));
      }
    }
    writeln(2, "StreamWriterBuilder swb;");
    writeln(2, "input = writeString(swb, input_value);");
  }

  private static void makeValueRec(String name, int no)
  {
    writeln(no, "Value " + name + "_out;");
    writeln(no, "" + name + "[i].BuildData(builder);");
    writeln(no, "string " + name + "_result;");
    writeln(no, "" + name + "_out = builder.getRecord(" + name + "_result);");
  }

  private static String asType(Field field)
  {
    switch (field.type.typeof)
    {
    case Type.CHAR:
    case Type.STRING:
      return ".asString()";
    case Type.BYTE:
    case Type.BOOLEAN:
    case Type.INT:
    case Type.SHORT:
      return ".asInt()";
    case Type.LONG:
      return ".asInt64()";
    case Type.FLOAT:
    case Type.DOUBLE:
      return ".asDouble()";
    default:
      break;
    }
    return "";
  }
  
  private static void generateReqIDTable(Module module, String output)
  {
    try
    {
      logln("Code: " + output + module.name.toLowerCase() + "ReqID.txt");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "ReqID.txt");
      writer = new PrintWriter(outFile);
      try
      {
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          if (prototype.message.length() > 0)
            writeln("'" + module.name + "', " + prototype.message + ", '" + prototype.name + "'");
          else
            writeln("'" + module.name + "', " + i + ", '" + prototype.name + "'");
        }
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      logln("Generate Procs IO Error");
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  /**
   * Sets up the writer and generates the general stuff
   */
  private static boolean generateCServer(Module module, String output)
  {
    boolean hasCode = true;
    try
    {
      logln("Code: " + output + module.name.toLowerCase() + "server.cpp");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "server.cpp");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#include \"ti.h\"");
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln("#include <string.h>");
        writeln("#if defined(M_AIX) || defined(M_GNU)");
        writeln("#include <unistd.h>");
        writeln("#endif");
        writeln("#include \"popgen.h\"");
        if (doConnect == true)
          writeln("#include \"dbportal.h\"");
        writeln();
        writeln("char *" + module.name + "Version = " + module.version + ";");
        writeln("int32 " + module.name + "Signature = " + module.signature + ";");
        writeln();
        writeln("#define INI_FILE \"" + module.name.toLowerCase() + ".ini\"");
        writeln();
        writeln("#include \"" + module.name.toLowerCase() + ".h\"");
        writeln();
        writeln("char *ConfigFile = INI_FILE;");
        writeln();
        if (alignForSun == true)
        {
          writeln("inline int32 alignData(int32 size)");
          writeln("{");
          writeln(1, "int n = size % 8;");
          writeln(1, "if (n > 0) size += (8-n);");
          writeln(1, "return size;");
          writeln("}");
          writeln();
        }
        for (int i = 0; i < module.tables.size(); i++)
        {
          Table table = (Table) module.tables.elementAt(i);
          String comma = "  ";
          writeln("const char *" + module.name + table.name + "[] = ");
          writeln("{");
          for (int j = 0; j < table.messages.size(); j++)
          {
            Message message = (Message) table.messages.elementAt(j);
            writeln(1, comma + message.value + "   // " + message.name);
            comma = ", ";
          }
          writeln("};");
          writeln();
        }
        writeln("t" + module.name + "Server::t" + module.name + "Server(tString IniFile)");
        writeln(": tSnapProcess(IniFile)");
        writeln("{");
        if (doConnect == true)
        {
          writeln(1, "try");
          writeln(1, "{");
          writeln(2, "Connect=0;");
          writeln(2, "RPC=0;");
          writeln(2, "ReadIniFile(IniFile);");
          writeln(2, format("LogFile->lprintf(eLogInfo, \"Version: %%s Signature %%d %1$s(%%s)\", %1$sVersion, %1$sSignature, __DATE__);", module.name));
          writeln(2, "Connect=new tDBConnect();");
          writeln(2, "" + module.name + " = new t" + module.name + "(*Connect, *LogFile, *this);");
          writeln(1, "}");
          writeln(1, "catch (xDBError &x)");
          writeln(1, "{");
          writeln(2, "LogFile->Log(x);");
          writeln(1, "}");
        } else
        {
          writeln(1, "RPC=0;");
          writeln(1, "ReadIniFile(IniFile);");
          writeln(1, "" + module.name + " = new t" + module.name + "(*LogFile, *this);");
        }
        writeln("}");
        writeln();
        writeln("t" + module.name + "Server::~t" + module.name + "Server()");
        writeln("{");
        if (doConnect == true)
          writeln(1, "if (Connect) delete Connect;");
        writeln("}");
        writeln();
        writeln("void t" + module.name + "Server::ReadIniFile(tString IniFile)");
        writeln("{");
        writeln(1, "tINI Ini(IniFile);");
        if (doConnect == true)
        {
          writeln(1, "Ini.QueryValue(\"{Connection}\", Connection, \"DataBase\");");
          writeln(1, "Ini.QueryValue(\"{BinFile}\", BinFile, \"DataBase\");");
          writeln(1, "Ini.QueryValue(\"{Timeout}\", DBTimeout, \"DataBase\");");
          if (doNedTelShuffle == true)
          {
            writeln(1, "tINI DBIni(Connection);");
            writeln(1, "DBIni.QueryValue(\"{Sql Logon String}\", Connection, \"DBLOGON\");");
          }
        }
        writeln("#ifdef UPM_AWARE");
        writeln(1, "Ini.QueryValue(\"{User ID}\", UserID, \"UPM\");");
        writeln(1, "Ini.QueryValue(\"{HostName}\", HostName, \"UPM\");");
        writeln("#endif");
        writeln("}");
        writeln();
        writeln("void t" + module.name + "Server::ServiceClient()");
        writeln("{");
        writeln("#if defined(M_AIX) || defined(M_GNU)");
        writeln(1, "sprintf(LogPID, \"[%d] \", getpid());");
        writeln(1, "LogFile->SetCustomString(LogPID);");
        writeln("#else");
        writeln(1, "LogFile->SetCustomString(\"\");");
        writeln("#endif");
        if (doConnect == true)
        {
          writeln(1, "if (!Connect) return;");
          writeln(1, "try");
          writeln(1, "{");
          writeln(1, "Connect->Logon(BinFile, Connection);");
          if (doNedTelShuffle == true)
            writeln(2, "Connection = \"<removed>\";");
          writeln(2, "Connect->CB()->isVB = -1; // Kludge for Server C++ but Client VB");
          writeln(2, "Connect->SetTimeout(DBTimeout); // Timeout to kill transaction in seconds (0=no timeout)");
        } else
        {
          writeln(1, "try");
          writeln(1, "{");
        }
        writeln(2, "RPC = new tRPCServer(*Sock);");
        writeln("#ifdef UPM_AWARE");
        writeln(2, "RPC->UserID(UserID);");
        writeln(2, "RPC->HostName(HostName);");
        writeln("#endif");
        writeln(2, "RPC->mLogReceived = (logreceive == 0) ? false : true;");
        writeln(2, "RPC->mLogTransmitted = (logtransmit == 0) ? false : true;");
        if (doMetrics == true)
          writeln(2, "RPC->mMetrics = (metrics == 0) ? false : true;");
        writeln("#  if defined HTTP_SERVER");
        writeln(2, "" + module.name + "->StartUpCode(RPC);");
        writeln("#  else");
        writeln(2, "" + module.name + "->setRPC(RPC);");
        writeln(2, "" + module.name + "->StartUpCode();");
        writeln("#  endif");
        writeln("#ifdef _SNAPPROCESS_HAS_STATIC_BOOL_SHUTDOWN");
        writeln(2, "while (tSnapProcess::shutdown == false) // Recoverable exceptions Loop");
        writeln("#else");
        writeln(2, "while (RPC->Shutdown() == false) // Recoverable exceptions Loop");
        writeln("#endif");
        writeln(2, "{");
        writeln(3, "int CountDown = 10;");
        writeln(3, "try");
        writeln(3, "{");
        writeln(4, "ReplyBody=0;");
        writeln(4, "ReplySize=0;");
        writeln(4, "RPC->WaitForCall();");
        writeln("#ifdef _SNAPPROCESS_HAS_STATIC_BOOL_SHUTDOWN");
        writeln(4, "if (tSnapProcess::shutdown == true)");
        writeln("#else");
        writeln(4, "if (RPC->Shutdown() == true)");
        writeln("#endif");
        writeln(5, "break;");
        writeln(4, "char* ip = (char*)RPC->RxBuffer();");
        writeln(4, "int32 Signature;");
        writeln(4, "Signature = *(int32*)ip;");
        writeln(4, "SwapBytes(Signature);");
        if (alignForSun == true)
          writeln(4, "ip += alignData(sizeof(int32));");
        else
          writeln(4, "ip += sizeof(int32);");
        writeln(4, "switch (RPC->ReqID())");
        writeln(4, "{");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          if (prototype.code.size() > 0 )
            hasCode = true;
          if (prototype.message.length() > 0)
            writeln(4, "case " + prototype.message + ":");
          else
            writeln(4, "case " + i + ":");
          writeln(5, "LogFile->lprintf(eLogDebug, \"ReqID=%d " + prototype.name + "[" + prototype.signature(true)
              + "]\", RPC->ReqID());");
          writeln(5, "if (Signature != " + prototype.signature(true) + ")");
          writeln(6, "Result = (int32)" + module.name.toUpperCase() + "_INV_SIGNATURE;");
          writeln(5, "else");
          writeln(6, "Result = (int32)" + prototype.name + "(ip);");
          writeln(5, "LogFile->lprintf(eLogDebug, \"Result=%d " + prototype.name + "[" + prototype.signature(true)
              + "]\", Result);");
          writeln(5, "break;");
        }
        writeln(4, "default:");
        writeln(5, "Result = (int32)" + module.name.toUpperCase() + "_UNKNOWN_FUNCTION;");
        writeln(4, "}");
        writeln(4, "RPC->RespondToCall(Result, ReplyBody, ReplySize);");
        writeln(4, "if (ReplyBody)");
        writeln(5, "delete[] ReplyBody;");
        writeln(3, "}");
        writeln(3, "catch(xSockErr &x)");
        writeln(3, "{");
        writeln(4, "LogFile->Log(x);");
        writeln(4, "if (ReplyBody)");
        writeln(5, "delete[] ReplyBody;");
        writeln(4, "switch (x.Error())");
        writeln(4, "{");
        writeln(5, "case xSockErr::errSockTimeout:");
        writeln(6, "LogFile->Log(\"Timeout -- continuing\");");
        writeln(6, "continue;");
        writeln(5, "default:");
        writeln(6, "if (--CountDown)");
        writeln(6, "{");
        writeln(7, "LogFile->Log(\"Counting down --  continuing\");");
        writeln("#if defined(M_AIX) || defined(M_GNU)");
        writeln(7, "sleep(5 / CountDown);");
        writeln("#endif");
        writeln(7, "continue;");
        writeln(6, "}");
        writeln(6, "LogFile->Log(\"Counted out blowing popstand\");");
        writeln(6, "throw;");
        writeln(4, "}");
        writeln(3, "}");
        if (doConnect == true)
        {
          writeln(3, "catch (xDBError &x)");
          writeln(3, "{");
          writeln(4, "if (ReplyBody)");
          writeln(5, "delete[] ReplyBody;");
          writeln(4, "LogFile->Log(x);");
          writeln(4, "Connect->Rollback();");
          writeln(4, "Result=(int32)" + module.name.toUpperCase() + "_UNCAUGHT_DB_ERROR;");
          writeln(3, "}");
        }
        writeln(2, "}");
        if (hasShutDownCode)
          writeln(2, "" + module.name + "->ShutDownCode();");
        if (doConnect == true)
          writeln(2, "Connect->Logoff();");
        writeln(2, "delete RPC;");
        writeln(1, "}");
        writeln(1, "// Critical exception handlers");
        writeln(1, "catch (xCept &x)");
        writeln(1, "{");
        writeln(2, "LogFile->Log(x);");
        writeln(1, "}");
        writeln("}");
        writeln();
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          generateCServerMarshall(module, prototype, i);
        }
        writeln("#if defined(M_AIX) || defined(M_GNU) || defined(__MSVC__) || _MSC_VER >= 1200");
        writeln();
        writeln("#include \"getargs.h\"");
        writeln();
        writeln("static ARG argtab[] =");
        writeln("{{'c', STRING, (int*)&ConfigFile,    \"Which config file\"}");
        writeln("};");
        writeln("#define TABSIZE (sizeof(argtab) / sizeof(ARG))");
        writeln();
        writeln("t" + module.name + "Server *" + module.name + "Server;");
        writeln("");
        writeln("#ifndef _SNAPPROCESS_HAS_STATIC_BOOL_SHUTDOWN");
        writeln("#  if defined(M_AIX)");
        writeln("void ShutdownHandler(int arg)");
        writeln("{");
        writeln(1, " " + module.name + "Server->RPC->SetShutdown();");
        writeln("}");
        writeln("#  elif defined(M_GNU)");
        writeln("#include <sys/signal.h>");
        writeln("extern \"C\" void ShutdownHandler(int arg)");
        writeln("{");
        writeln(1, " " + module.name + "Server->RPC->SetShutdown();");
        writeln("}");
        writeln("#  endif");
        writeln("#endif");
        writeln();
        writeln("int main(int argc, char *argv[])");
        writeln("{");
        writeln(1, "int rc=0;");
        writeln(1, "try");
        writeln(1, "{");
        writeln("#if defined(M_AIX) || defined(M_GNU)");
        writeln(2, "Init();");
        writeln("#endif");
        writeln(2, "argc = GetArgs(argc, argv, argtab, TABSIZE);");
        writeln(2, "" + module.name + "Server = new t" + module.name + "Server(ConfigFile);");
        writeln("#ifndef _SNAPPROCESS_HAS_STATIC_BOOL_SHUTDOWN");
        writeln("#  if defined(M_AIX)");
        writeln(2, "sleep(1);");
        writeln(2, "struct sigaction Action, OAction;");
        writeln(2, "Action.sa_mask.losigs = 0;");
        writeln(2, "Action.sa_mask.hisigs = 0;");
        writeln(2, "Action.sa_flags = 0;");
        writeln(2, "Action.sa_handler = ShutdownHandler;");
        writeln(2, "sigaction(SIGUSR1, &Action, &OAction);");
        writeln(2, "sigaction(SIGTERM, &Action, &OAction);");
        writeln(2, "sigaction(SIGQUIT, &Action, &OAction);");
        writeln("#  elif defined(M_GNU)");
        writeln(2, "sleep(1);");
        writeln(2, "signal(SIGUSR1, ShutdownHandler);");
        writeln(2, "signal(SIGTERM, ShutdownHandler);");
        writeln(2, "signal(SIGQUIT, ShutdownHandler);");
        writeln("#  endif");
        writeln("#endif");
        writeln(2, "" + module.name + "Server->CreateChildren();");
        writeln("#if defined(_SNAPPROCESS_HAS_EXITCODE)");
        writeln(2, "// see snapprocess.h for a discussion on exitCode");
        writeln(2, "rc=" + module.name + "Server->exitCode;");
        writeln("#endif");
        writeln(2, "delete " + module.name + "Server;");
        writeln(1, "}");
        writeln(1, "catch (xCept &x)");
        writeln(1, "{");
        writeln(2, "cerr << \"Uncaught xCept error\" << endl << flush;");
        writeln(2, "DisplayVerbose(x, cout);");
        writeln(2, "rc=-1;");
        writeln(1, "}");
        writeln(1, "catch (...)");
        writeln(1, "{");
        writeln(2, "cout << \"General error trapped here ...\" << endl << flush;");
        writeln(2, "rc=-2;");
        writeln(1, "}");
        writeln(1, "return rc;");
        writeln("}");
        writeln("#endif");
        writeln();
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      logln("Generate Procs IO Error");
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
    return hasCode;
  }

  /**
   * Generates the prototypes defined
   */
  private static void generateCServerMarshall(Module module, Prototype prototype, int no)
  {
    writeln("e" + module.name + " t" + module.name + "Server::" + prototype.name + "(char *ip)");
    writeln("{");
    writeln(1, "e" + module.name + " resultCode = " + module.name.toUpperCase() + "_OK;");
    writeln(1, "try");
    writeln(1, "{");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      boolean addUp = i + 1 != prototype.inputs.size();
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field = input.getParameter(prototype);
      if (field == null)
        continue;
      byte savedReference = field.type.reference;
      // Action output = (Action) prototype.getOutputAction(field.name);
      // if (output != null)
      // op = output.sizeOperation();
      if (field.type.reference == Type.BYPTR || field.type.reference == Type.BYREFPTR)
      {
        if (input.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln(2, "RecvSize = *(int32*)ip;");
            writeln(2, "SwapBytes(RecvSize);");
            if (alignForSun == true)
              writeln(2, "ip += alignData(sizeof(int32));");
            else
              writeln(2, "ip += sizeof(int32);");
            writeln(2, "char* " + field.name + " = ip;");
            if (addUp)
            {
              if (alignForSun == true)
                writeln(2, "ip += alignData(RecvSize);");
              else
                writeln(2, "ip += RecvSize;");
            }
          } else
          {
            writeln(2, "" + field.type.cName(false) + "* " + field.name + " = (" + field.type.cName(false) + "*)ip;");
            if (addUp)
            {
              if (alignForSun == true)
                writeln(2, "ip += alignData(sizeof(*" + field.name + "));");
              else
                writeln(2, "ip += sizeof(*" + field.name + ");");
            }
          }
        } else
        {
          writeln(2, "RecvSize = *(int32*)ip;");
          writeln(2, "SwapBytes(RecvSize);");
          if (alignForSun == true)
            writeln(2, "ip += alignData(sizeof(int32));");
          else
            writeln(2, "ip += sizeof(int32);");
          writeln(2, "" + field.type.cName(false) + "* " + field.name + " = (" + field.type.cName(false) + "*)ip;");
          if (addUp)
          {
            if (alignForSun == true)
              writeln(2, "ip += alignData(RecvSize);");
            else
              writeln(2, "ip += RecvSize;");
          }
        }
      } else
      {
        writeln(2, "" + field.type.cName(false) + "* " + field.name + " = (" + field.type.cName(false) + "*)ip;");
        if (addUp)
        {
          if (alignForSun == true)
            writeln(2, "ip += alignData(sizeof(*" + field.name + "));");
          else
            writeln(2, "ip += sizeof(*" + field.name + ");");
        }
        field.type.reference = Type.BYPTR;
      }
      Field opField0 = field;
      Operation inOp = input.sizeOperation();
      byte saveRef0 = 0;
      if (inOp != null)
      {
        opField0 = prototype.getParameter(inOp.name);
        if (opField0 != null)
        {
          saveRef0 = opField0.type.reference;
          opField0.type.reference = Type.BYPTR;
        }
      }
      generateCSwaps(module, prototype, field, inOp);
      if (inOp != null && opField0 != null)
        opField0.type.reference = saveRef0;
      field.type.reference = savedReference;
    }
    boolean hasOutput = false;
    boolean hasResult = false;
    if (prototype.type.typeof != Type.VOID || prototype.type.reference == Type.BYPTR)
    {
      if (prototype.type.reference != Type.BYPTR)
      {
        if (alignForSun == true)
          writeln(2, "ReplySize += alignData((int32)sizeof(" + prototype.type.cName(false) + ")); // Result Size");
        else
          writeln(2, "ReplySize += (int32)sizeof(" + prototype.type.cName(false) + "); // Result Size");
        hasOutput = true;
        hasResult = true;
      }
    }
    int Variants = 0;
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      Action output = (Action) prototype.outputs.elementAt(i);
      Field field = output.getParameter(prototype);
      if (field == null)
        continue;
      hasOutput = true;
      Operation op = output.sizeOperation();
      // Action input = (Action) prototype.getInputAction(field.name);
      if (field.type.reference == Type.BYPTR)
      {
        if (output.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln("#error unsized char* on output");
            logger.println("#error unsized char* on output");
          } else
          {
            if (alignForSun == true)
              writeln(2, "ReplySize += alignData(sizeof(" + field.type.cName(false) + ")); // size " + field.name);
            else
              writeln(2, "ReplySize += sizeof(" + field.type.cName(false) + "); // size " + field.name);
          }
        } else
        {
          String w1 = "";
          if (op.isConstant == false)
            w1 = "*";
          if (alignForSun == true)
            writeln(2, "ReplySize += alignData(sizeof(int32));  // size of Block");
          else
            writeln(2, "ReplySize += sizeof(int32);  // size of Block");
          if (alignForSun == true)
            writeln(2, "ReplySize += alignData(" + w1 + "" + op.name + " * sizeof(" + field.type.cName(false)
                + ")); // size " + field.name);
          else
            writeln(2, "ReplySize += (" + w1 + "" + op.name + " * sizeof(" + field.type.cName(false) + ")); // size "
                + field.name);
        }
      } else if (field.type.reference == Type.BYREFPTR)
      {
        if (output.hasSize() == false)
        {
          writeln("#error unsized BYREFPTR on output");
          logger.println("#error unsized BYREFPTR on output");
        } else
        {
          Variants++;
          if (alignForSun == true)
            writeln(2, "ReplySize += alignData(sizeof(" + field.type.cName(false) + "*)); // Variant " + field.name);
          else
            writeln(2, "ReplySize += sizeof(" + field.type.cName(false) + "*); // Variant " + field.name);
        }
      } else
      {
        if (alignForSun == true)
          writeln(2, "ReplySize += alignData(sizeof(" + field.type.cName(false) + ")); // size " + field.name);
        else
          writeln(2, "ReplySize += sizeof(" + field.type.cName(false) + "); // size " + field.name);
      }
    }
    if (hasOutput)
    {
      if (Variants > 0)
      {
        writeln(2, "int32 Variants[" + Variants + "];");
        writeln(2, "int32 VariantSize[" + Variants + "];");
        Variants = 0;
      }
      writeln(2, "ReplyBody = new char [ReplySize];");
      writeln(2, "memset(ReplyBody, 0, ReplySize);");
      writeln(2, "ip = ReplyBody;");
    }
    if (prototype.type.typeof != Type.VOID || prototype.type.reference == Type.BYPTR)
    {
      if (prototype.type.reference != Type.BYPTR)
      {
        writeln(2, "" + prototype.type.cName(false) + "* Result = (" + prototype.type.cName(false) + "*) ip;");
        if (prototype.outputs.size() > 0)
        {
          if (alignForSun == true)
            writeln(2, "ip += alignData(sizeof(" + prototype.type.cName(false) + "));");
          else
            writeln(2, "ip += sizeof(" + prototype.type.cName(false) + ");");
        }
      }
    }
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      boolean addUp = i + 1 != prototype.outputs.size();
      Action output = (Action) prototype.outputs.elementAt(i);
      Field field = output.getParameter(prototype);
      if (field == null)
        continue;
      Operation op = output.sizeOperation();
      Action input = prototype.getInputAction(field.name);
      // if (op != null)
      // {
      // opInput = (Action) prototype.getInputAction(op.name);
      // opOutput = (Action) prototype.getOutputAction(op.name);
      // }
      if (field.type.reference == Type.BYPTR)
      {
        if (output.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
            continue;
          else
          {
            if (input != null)
            {
              writeln(2, "memcpy(ip, " + field.name + ", sizeof(" + field.type.cName(false) + "));");
              writeln(2, "" + field.name + " = (" + field.type.cName(false) + "*)ip;");
            } else
              writeln(2, "" + field.type.cName(false) + "* " + field.name + " = (" + field.type.cName(false) + "*)ip;");
            if (addUp)
            {
              if (alignForSun == true)
                writeln(2, "ip += alignData(sizeof(" + field.type.cName(false) + "));");
              else
                writeln(2, "ip += sizeof(" + field.type.cName(false) + ");");
            }
          }
        } else
        {
          String w1 = "";
          if (op.isConstant == false)
            w1 = "*";
          writeln(2, "*(int32*)ip = (" + w1 + "" + op.name + " * sizeof(" + field.type.cName(false) + "));");
          writeln(2, "SwapBytes(*(int32*)ip);");
          if (alignForSun == true)
            writeln(2, "ip += alignData(sizeof(int32));");
          else
            writeln(2, "ip += sizeof(int32);");
          if (input != null)
          {
            writeln(2,
                "memcpy(ip, " + field.name + ", (" + w1 + op.name + " * sizeof(" + field.type.cName(false) + ")));");
            writeln(2, "" + field.name + " = (" + field.type.cName(false) + "*)ip;");
          } else
            writeln(2, "" + field.type.cName(false) + "* " + field.name + " = (" + field.type.cName(false) + "*)ip;");
          if (addUp)
          {
            if (alignForSun == true)
              writeln(2, "ip += alignData(" + w1 + op.name + " * sizeof(" + field.type.cName(false) + "));");
            else
              writeln(2, "ip += (" + w1 + op.name + " * sizeof(" + field.type.cName(false) + "));");
          }
        }
      } else if (field.type.reference == Type.BYREFPTR)
      {
        if (output.hasSize() == false)
          continue;
        else
        {
          if (op.isConstant == false)
          {
          } else
            writeln("#error Constant Size with BYREFPTR output");
          writeln(2, "Variants[" + (Variants++) + "] = ip-ReplyBody;");
          if (input != null)
            writeln(2, "(" + field.type.cName(false) + "*)*ip = " + field.name + ";");
          else
            writeln(2, "" + field.type.cName(false) + "** " + field.name + " = (" + field.type.cName(false) + "**)ip;");
          if (addUp)
          {
            if (alignForSun == true)
              writeln(2, "ip += alignData(sizeof(" + field.type.cName(false) + "*));");
            else
              writeln(2, "ip += sizeof(" + field.type.cName(false) + "*);");
          }
        }
      } else
      {
        if (input != null)
        {
          writeln(2, "memcpy(ip, " + field.name + ", sizeof(" + field.type.cName(false) + "));");
          writeln(2, "" + field.name + " = (" + field.type.cName(false) + "*)ip;");
        } else
          writeln(2, "" + field.type.cName(false) + "* " + field.name + " = (" + field.type.cName(false) + "*)ip;");
        if (addUp)
        {
          if (alignForSun == true)
            writeln(2, "ip += alignData(sizeof(" + field.type.cName(false) + "));");
          else
            writeln(2, "ip += sizeof(" + field.type.cName(false) + ");");
        }
      }
    }
    writeln(2, "try");
    writeln(2, "{");
    String w1 = "", w2 = "";
    write("      ");
    if (hasResult)
      write("*Result = ");
    write(module.name + "->" + prototype.name + "(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field field = (Field) prototype.parameters.elementAt(i);
      if (field.type.reference == Type.BYPTR)
        // || field.type.reference == Type.BYREFPTR)
        w2 = "";
      else
        w2 = "*";
      write(w1 + w2 + field.name);
      w1 = ", ";
    }
    writeln(");");
    writeln(2, "}");
    writeln(2, "catch (e" + module.name + " rc)");
    writeln(2, "{");
    writeln(3, "resultCode = rc;");
    writeln(2, "}");
    if (hasResult && prototype.needsSwap())
      writeln(2, "SwapBytes(*Result);");
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      Action output = (Action) prototype.outputs.elementAt(i);
      Field field = output.getParameter(prototype);
      if (field == null)
        continue;
      Operation op = output.sizeOperation();
      Field opField = field;
      byte saveRef = 0;
      if (op != null)
      {
        opField = prototype.getParameter(op.name);
        if (opField != null)
        {
          saveRef = opField.type.reference;
          opField.type.reference = Type.BYPTR;
        }
      }
      generateCStructSwaps(module, prototype, field, op);
      if (op != null && opField != null)
        opField.type.reference = saveRef;
    }
    if (Variants > 0)
    {
      writeln(2, "// We are going to setup a new ReplyBody because of Variants");
      writeln(2, "char *oldReplyBody = ReplyBody;");
      writeln(2, "ip = ReplyBody;");
      writeln(2, "int32 tail = ReplySize;");
      writeln(2, "// Calc new ReplySize for Contiguous memory from Disjoint memory");
      writeln(2, "ReplySize = 0;");
      if (prototype.type.typeof != Type.VOID || prototype.type.reference == Type.BYPTR)
      {
        if (prototype.type.reference != Type.BYPTR)
        {
          if (alignForSun == true)
            writeln(2, "ReplySize += alignData((int32)sizeof(" + prototype.type.cName(false) + ")); // Result Size");
          else
            writeln(2, "ReplySize += (int32)sizeof(" + prototype.type.cName(false) + "); // Result Size");
          hasOutput = true;
          hasResult = true;
        }
      }
      int VariantSize = 0;
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field = output.getParameter(prototype);
        if (field == null)
          continue;
        hasOutput = true;
        Operation op = output.sizeOperation();
        // Action input = (Action) prototype.getInputAction(field.name);
        if (field.type.reference == Type.BYPTR || field.type.reference == Type.BYREFPTR)
        {
          if (output.hasSize() == false)
          {
            if (field.type.typeof != Type.CHAR)
            {
              if (alignForSun == true)
                writeln(2, "ReplySize += alignData(sizeof(" + field.type.cName(false) + ")); // size " + field.name);
              else
                writeln(2, "ReplySize += sizeof(" + field.type.cName(false) + "); // size " + field.name);
            }
          } else
          {
            w1 = "";
            if (op.isConstant == false)
              w1 = "*";
            if (alignForSun == true)
            {
              writeln(2, "ReplySize += alignData(sizeof(int32));  // size of Block");
              writeln(2, "ReplySize += alignData(" + w1 + "" + op.name + " * sizeof(" + field.type.cName(false)
                  + ")); // size " + field.name);
            } else
            {
              writeln(2, "ReplySize += sizeof(int32);  // size of Block");
              writeln(2, "ReplySize += (" + w1 + "" + op.name + " * sizeof(" + field.type.cName(false) + ")); // size "
                  + field.name);
            }
            if (field.type.reference == Type.BYREFPTR)
              writeln(2, "VariantSize[" + (VariantSize++) + "] = (" + w1 + "" + op.name + " * sizeof("
                  + field.type.cName(false) + "));");
          }
        } else
        {
          if (alignForSun == true)
            writeln(2, "ReplySize += alignData(sizeof(" + field.type.cName(false) + ")); // size " + field.name);
          else
            writeln(2, "ReplySize += sizeof(" + field.type.cName(false) + "); // size " + field.name);
        }
      }
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field = output.getParameter(prototype);
        if (field == null)
          continue;
        Operation op = output.sizeOperation();
        generateCNonStructSwaps(module, prototype, field, op);
      }
      writeln(2, "ReplyBody = new char [ReplySize];");
      writeln(2, "memset(ReplyBody, 0, ReplySize);");
      writeln(2, "char *op = ReplyBody;");
      writeln(2, "int32 pos = 0;");
      writeln(2, "{");
      writeln(3, "for (int i = 0; i < " + Variants + "; i++)");
      writeln(3, "{");
      writeln(4, "int32 size = Variants[i]-pos;");
      writeln(4, "if (size > 0)");
      writeln(4, "{");
      writeln(5, "memcpy(op, ip, size);");
      if (alignForSun == true)
      {
        writeln(5, "ip += alignData(size);");
        writeln(5, "op += alignData(size);");
        writeln(5, "tail -= alignData(size);");
      } else
      {
        writeln(5, "ip += size;");
        writeln(5, "op += size;");
        writeln(5, "tail -= size;");
      }
      writeln(4, "}");
      writeln(4, "memcpy(op, &VariantSize[i], sizeof(VariantSize[i])); ");
      writeln(4, "SwapBytes(*(int32*)op);");
      if (alignForSun == true)
        writeln(4, "op += alignData(sizeof(VariantSize[i]));");
      else
        writeln(4, "op += sizeof(VariantSize[i]);");
      writeln(4, "if (VariantSize[i] > 0)");
      writeln(4, "{");
      writeln(5, "char **block = (char **)ip;");
      writeln(5, "memcpy(op, *block, VariantSize[i]);");
      writeln(5, "free(*block);");
      writeln(4, "}");
      if (alignForSun == true)
      {
        writeln(4, "op += alignData(VariantSize[i]);");
        writeln(4, "ip += alignData(sizeof(char *));");
        writeln(4, "tail -= alignData(sizeof(char *));");
      } else
      {
        writeln(4, "op += VariantSize[i];");
        writeln(4, "ip += sizeof(char *);");
        writeln(4, "tail -= sizeof(char *);");
      }
      if (alignForSun == true)
        writeln(4, "pos = alignData(Variants[i])+alignData(sizeof(char *));");
      else
        writeln(4, "pos = Variants[i]+sizeof(char *);");
      writeln(3, "}");
      writeln(2, "}");
      writeln(2, "if (tail > 0)");
      writeln(3, "memcpy(op, ip, tail);");
      writeln(2, "delete [] oldReplyBody;");
    } else
    {
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field = output.getParameter(prototype);
        if (field == null)
          continue;
        Operation op = output.sizeOperation();
        generateCNonStructSwaps(module, prototype, field, op);
      }
    }
    writeln(1, "}");
    writeln(1, "catch (e" + module.name + " rc)");
    writeln(1, "{");
    writeln(2, "return rc;");
    writeln(1, "}");
    writeln(1, "return resultCode;"); // +module.name.toUpperCase()+"_OK;");
    writeln("}");
    writeln();
  }

  /**
   * Generates the prototypes defined
   */
  public static void generateCSwaps(Module module, Prototype prototype, Field field, Operation op)
  {
    String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "", w6 = "", w7 = "", w8 = "", w9 = "";
    if (field.needsSwap() || field.isStruct(module))
    {
      if (field.type.reference == Type.BYPTR || field.type.reference == Type.BYREFPTR)
      {
        w3 = "->";
        w4 = "*";
      }
      if (op != null)
      {
        w8 = "{";
        w9 = "}";
        Field opField = prototype.getParameter(op.name);
        if (opField != null && (opField.type.reference == Type.BYPTR || opField.type.reference == Type.BYREFPTR))
          w5 = "*";
        writeln(2, "" + w2 + w8 + "for (int i = 0; i < " + w5 + op.name + "; i++)");
        w1 = w1 + "[i]";
        w2 = "  " + w2;
        if (field.type.reference == Type.BYREFPTR)
        {
          w6 = "(*";
          w7 = ")";
        }
        w3 = ".";
        w4 = "";
      }
      for (int j = 0; j < field.type.arraySizes.size(); j++)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(j);
        writeln(2, "" + w2 + "for (int i" + j + " = 0; i" + j + " < " + integer.intValue() + "; i" + j + "++)");
        w1 = w1 + "[i" + j + "]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
      }
    }
    if (field.isStruct(module))
      writeln(2, "" + w2 + w6 + field.name + w7 + w1 + w3 + "Swaps();" + w9);
    else if (field.needsSwap())
      writeln(2, "" + w2 + "SwapBytes(" + w4 + field.name + w1 + ");" + w9);
    else if (field.type.typeof == Type.USERTYPE)
    {
      writeln("#ifndef ALLOW_" + field.type.name.toUpperCase());
      writeln("#error " + prototype.name + " " + field.name + " " + field.type.name
          + " is of UserType and may require swapping.");
      writeln("#endif");
      logger.println("Warning: " + prototype.name + " " + field.name + " is of UserType and may require swapping.");
    }
  }

  public static void generateCStructSwaps(Module module, Prototype prototype, Field field, Operation op)
  {
    if (field.isStruct(module) == true)
      generateCSwaps(module, prototype, field, op);
  }

  public static void generateCNonStructSwaps(Module module, Prototype prototype, Field field, Operation op)
  {
    if (field.isStruct(module) == false)
      generateCSwaps(module, prototype, field, op);
  }
}
