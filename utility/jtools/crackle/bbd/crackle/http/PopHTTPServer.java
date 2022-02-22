package bbd.crackle.http;

import bbd.crackle.*;
import bbd.crackle.Module;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Collections;
import java.util.Vector;

public class PopHTTPServer extends Generator
{
  public static String description()
  {
    return "Generates HTTP Server Code (AIX|LINUX|WINDOWS)";
  }

  public static String documentation()
  {
    return "Generates HTTP Server Code (AIX|LINUX|WINDOWS)";
  }

  public static void main(String[] args)
  {
    try
    {
      PrintWriter outLog = new PrintWriter(System.out);
      for (int i = 0; i < args.length; i++)
      {
        outLog.println(args[i] + ": Generate ... ");
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
  private static String database;
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    outLog.println(module.name + " version " + module.version);
    loadProperties(module, output);
    database = getProperty("database", "dbportal");
    generateServerHeader(module, output, outLog);
    generateCServer(module, output, outLog);
    generateCMain(module, output, outLog);
  }

  private static boolean hasShutDownCode(Module module)
  {
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.codeType == Structure.NORMAL)
        continue;
      for (int j = 0; j < structure.code.size(); j++)
      {
        String codeLine = (String) structure.code.elementAt(j);
        if ((codeLine.trim()).equalsIgnoreCase("onshutdown:"))
          return true;
      }
    }
    return false;
  }

  private static void generateCMain(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name.toLowerCase() + "_main.cpp");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "_main.cpp");
      writer = new PrintWriter(outFile);
      indent_size = 4;
      try
      {
        boolean hasSlackHandler = false;
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.name.equals("SlackTimeHandler") == true)
          {
              hasSlackHandler = true;
              break;
          }
        }
        String modName = module.name;
        String lowName = module.name.toLowerCase();
        writeln(
            "// This code was generated, you may wish to copy it to another source once off and modify the other source.");
        writeln(format("#include \"%sserver_http.h\"", lowName));
        writeln("#include \"httpserver.h\"");
        writeln("#include \"getargs.h\"");
        writeln("");
        writeln(format("char* ConfigFile  = \"%shttp.ini\";", lowName));
        writeln(format("static char* LogFileName = \"%shttp.log\";", lowName));
        writeln();
        writeln("char *" + module.name + "Version = " + module.version + ";");
        writeln("int32 " + module.name + "Signature = " + module.signature + ";");
        writeln("");
        writeln("ARG argtab[] =");
        writeln("{");
        writeln(1, "{'c', STRING, (int *)&ConfigFile,  \"ConfigFile\"},");
        writeln(1, "{'l', STRING, (int *)&LogFileName, \"LogFile\"}");
        writeln("};");
        writeln("");
        writeln("#define TABSIZE (sizeof(argtab) / sizeof(ARG))");
        writeln("");
        writeln("// we add code for the implementation of HTTPServer ServiceClient");
        writeln("// and ServiceStart using function pointers");
        writeln("");
        writeln("static HTTPServer *httpServer;");
        writeln(format("static t%sServer_http *%sServer;", modName, lowName));
        writeln("");
        writeln("void ServiceClient(const char* function, string input, string &output)");
        writeln("{");
        writeln(1, format("%sServer->ServiceClient(function, input, output);", lowName));
        writeln("}");
        writeln("");
        if (hasSlackHandler == true)
        {
          writeln(format("void SlackHandler(void* handler)"));
          writeln("{");
          writeln(1, format("t%1$s* serv = (t%1$s*)handler;", modName));
          writeln(1, "serv->SlackTimeHandler();");
          writeln("}");
          writeln("");
        }
        writeln("int ServiceStart(tLogFile &logFile)");
        writeln("{");
        writeln(1, "HTTPServer &server = *httpServer;");
        if (database.equals("dbportal"))
        {
          writeln(1, "tDBConnect connect;");
          writeln(1, "connect.Logon(server.binFileName, server.connectString);");
          writeln(1, "if (server.dbTimeout > 0)");
          writeln(2, "connect.SetTimeout(server.dbTimeout);");
        }
        else if (database.equals("cliapi"))
        {
          writeln(1, "tJConnector connect;");
          writeln(1, "connect.Logon(server.user, server.password, server.server);");
        }
        else if (database.equals("ociapi"))
        {
          writeln(1, "tJConnector connect;");
          writeln(1, "connect.Logon(server.user, server.password, server.server);");
        }
        writeln(1, format("t%s %s(connect, logFile);", modName, lowName));
        writeln(1, format("%s.StartUpCode(&server);", lowName));
        if (hasSlackHandler == true)
        {
          writeln(1, "if (server.SlackTimeHandler != 0)");
          writeln(2, format("server.handler = &%s;", lowName));
        }
        writeln(1, format("%1$sServer = new t%2$sServer_http(%1$s);", lowName, modName));
        writeln(1, "server.WaitForCall(logFile);");
        if (hasShutDownCode(module) == true)
          writeln(1, format("%s.ShutDownCode();", lowName));
        writeln(1, "return 0;");
        writeln("}");
        writeln("");
        writeln("int main(int argc, char*argv[])");
        writeln("{");
        writeln(1, "try");
        writeln(1, "{");
        writeln(2, "argc = GetArgs(argc, argv, argtab, TABSIZE);");
        writeln(2, "httpServer = new HTTPServer(ConfigFile, LogFileName);");
        writeln(2, "HTTPServer &server = *httpServer;");
        writeln(2, "server.ServiceClient = ServiceClient;");
        writeln(2, "server.ServiceStart = ServiceStart;");
        writeln(2, "server.LoadConfig();");
        writeln(2, "tLogFile logFile(server.LogFileName);");
        writeln(2, "logFile.SetLogLevel(server.logLevel);");
        writeln(2, format(
            "logFile.lprintf(eLogInfo, \"Version: %%s Signature %%d %1$s(%%s)\", %1$sVersion, %1$sSignature, __DATE__);",
            modName));
        writeln(2, "server.OpenSocket();");
        writeln(2, "server.CreateHandlers(logFile);");
        writeln(2, "return 0;");
        writeln(1, "}");
        writeln(1, "catch (xCept &x)");
        writeln(1, "{");
        writeln(2, "cerr << \"Uncaught xCept error\" << endl << flush;");
        writeln(2, "DisplayVerbose(x, cout);");
        writeln(2, "return 1;");
        writeln(1, "}");
        writeln(1, "catch (...)");
        writeln(1, "{");
        writeln(2, "cerr << \"General error trapped here ...\" << endl << flush;");
        writeln(2, "return 1;");
        writeln(1, "}");
        writeln("}");
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
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

  private static void generateServerHeader(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name.toLowerCase() + "server_http.h");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "server_http.h");
      writer = new PrintWriter(outFile);
      indent_size = 4;
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#ifndef _" + module.name.toUpperCase() + "SERVER_HTTP_H");
        writeln("#define _" + module.name.toUpperCase() + "SERVER_HTTP_H");
        writeln("#include \"machine.h\"");
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln("#include <string.h>");
        writeln("#if defined(M_AIX) || defined(M_GNU)");
        writeln("#include <unistd.h>");
        writeln("#endif");
        writeln("#include \"" + module.name.toLowerCase() + ".h\"");
        //writeln("#include \"popgen.h\"");
        //writeln("#include \"dbportal.h\"");
        writeln("#include \"httpxcept.h\"");
        writeln();
        writeln("class t" + module.name + "Server_http");
        writeln("{");
        writeln(1, "t" + module.name + " &" + module.name + ";");
        writeln(1, "DataBuilderJson builder;");
        writeln("public:");
        writeln(1, "t" + module.name + "Server_http(t" + module.name + " &a" + module.name + ");");
        writeln(1, "~t" + module.name + "Server_http();");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          writeln(1, "void " + prototype.name + "(string input, string &output);");
        }
        writeln(1, "void ServiceClient(const char* name, string input, string &output);");
        writeln("};");
        writeln();
        writeln("#endif");
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
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

  private static boolean generateCServer(Module module, String output, PrintWriter outLog)
  {
    boolean hasCode = false;
    try
    {
      outLog.println("Code: " + output + module.name.toLowerCase() + "server_http.cpp");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "server_http.cpp");
      writer = new PrintWriter(outFile);
      indent_size = 4;
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#include \"" + module.name.toLowerCase() + "server_http.h\"");
        writeln();
        writeln("t" + module.name + "Server_http::t" + module.name + "Server_http(t" + module.name + " &a" + module.name
            + ")");
        writeln(": " + module.name + "(a" + module.name + ")");
        writeln("{");
        writeln("}");
        writeln();
        writeln("t" + module.name + "Server_http::~t" + module.name + "Server_http()");
        writeln("{");
        writeln("}");
        Vector<String> vector = new Vector<String>();
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          vector.addElement(prototype.name);
        }
        Collections.sort(vector);
        writeln();
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          if (prototype.isPythonImpl())
          {
            writeln("void t" + module.name + "Server_http::" + prototype.name + "(string input, string &output)");
            writeln("{");
            genValidateInputString(prototype);
            writeln(1, format("string scriptName = \"%s\";", prototype.name));
            writeln(1, format("if (%s.runPython)", module.name));
            writeln(2, format("%s.runPython(scriptName, input, output);", module.name));
            writeln("}");
            writeln();
          } else
            generateCServer(module, prototype, i);
        }
        writeln("enum e" + module.name + "Func");
        writeln("{");
        for (int i = 0; i < vector.size(); i++)
        {
          String name = vector.get(i);
          if (i == 0)
            writeln("  FUNC_" + name.toUpperCase());
          else
            writeln(", FUNC_" + name.toUpperCase());
        }
        writeln("};");
        writeln();
        writeln("struct t" + module.name + "Entry");
        writeln("{");
        writeln(1, "const char* name;");
        writeln(1, "const e" + module.name + "Func function;");
        writeln("};");
        writeln();
        writeln("static t" + module.name + "Entry " + module.name + "Entries[] =");
        writeln("{");
        for (int i = 0; i < vector.size(); i++)
        {
          String name = vector.get(i);
          writeln(format("%s{\"%s\", FUNC_%s}", i == 0 ? "  " : ", ", name, name.toUpperCase()));
        }
        writeln("};");
        writeln(format("const int no_entries = sizeof(%1$sEntries) / sizeof(t%1$sEntry);", module.name));
        writeln();
        writeln(format("static int _compare(const void* a, const void* b)"));
        writeln("{");
        writeln(1, format("t%sEntry *A = (t%sEntry*)a;", module.name, module.name));
        writeln(1, format("t%sEntry *B = (t%sEntry*)b;", module.name, module.name));
        writeln(1, "return strcmp(A->name, B->name);");
        writeln("}");
        writeln();
        writeln(0,
            "void t" + module.name + "Server_http::ServiceClient(const char* name, string input, string &output)");
        writeln(0, "{");
        writeln(1, "try");
        writeln(1, "{");
        writeln(2, format("t%1$sEntry item = { name, (e%1$sFunc) 0 };", module.name));
        writeln(2, format(
            "t%1$sEntry* found = (t%1$sEntry*)bsearch(&item, %1$sEntries, no_entries, sizeof(t%1$sEntry), _compare);",
            module.name));
        writeln(2, "if (found != 0) switch (found->function)");
        writeln(2, "{");
        for (int i = 0; i < vector.size(); i++)
        {
          String name = vector.get(i);
          writeln(2, format("case FUNC_%s: return %s(input, output);", name.toUpperCase(), name));
        }
        writeln(2, "}");
        writeln(1, "}");
        writeln(1, "catch (xCept &x)");
        writeln(1, "{");
        writeln(2,
            format("%s.LogFile.lprintf(eLogError, \"\\n%%s\\n%%s\", input.c_str(), x.ErrorStr());", module.name));
        writeln(2, "throw;");
        writeln(1, "}");
        writeln(0, "}");
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
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

  private static void generateCServer(Module module, Prototype prototype, int no)
  {
    boolean hasInput = false;
    boolean hasOutput = false;
    boolean hasResult = false;
    Vector<Argument> input_args = new Vector<Argument>();
    Vector<Argument> output_args = new Vector<Argument>();
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
    writeln("void t" + module.name + "Server_http::" + prototype.name + "(string input, string &output)");
    writeln("{");
    if (hasResult)
      writeln(1, "int32 returnCode;");
    if (hasInput)
      genInput(module, prototype, input_args);
    if (hasOutput)
      genOutput(module, prototype, output_args);
    String w1 = "";
    write("    ");
    if (hasResult)
      writer.print("returnCode = ");
    writer.print(module.name + "." + prototype.name + "(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field field = (Field) prototype.parameters.elementAt(i);
      String useAs = getUseAs(input_args, output_args, field);
      write(w1 + useAs);
      w1 = ", ";
    }
    writeln(");");
    if (hasResult == true || hasOutput == true)
      writeln(1, "Value output_value;");
    if (hasResult == true)
      writeln(1, "output_value[\"returnCode\"] = returnCode;");
    if (hasOutput)
    {
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field = output.getParameter(prototype);
        if (field == null)
          continue;
        if (field.type.reference == Type.BYREFPTR)
        {
          Operation op = output.sizeOperation();
          if (field.type.typeof == Type.USERTYPE)
          {
            writeln(1, format("Value %s_array;", field.name));
            writeln(1, format("for (int o%2$d=0; o%2$d < %1$s; o%2$d++)", op.name, i));
            writeln(1, "{");
            writeln(2, format("%s &rec = %s.arr[o%d];", name_of(module, field.type.name), field.name, i));
            makeValueRec("rec", 2);
            writeln(2, format("%s_array[o%d] = rec_out;", field.name, i));
            writeln(1, "}");
            writeln(1, format("output_value[\"%1$s\"] = %1$s_array;", field.name));
          } else if (field.type.typeof == Type.CHAR)
          {
            writeln(1, "output_value[\"" + field.name + "\"] = " + field.name + ".arr;");
          } else
          {
            writeln(1, "// " + field.name + " as simple array;");
          }
          continue;
        }
        if (field.type.reference == Type.BYPTR)
        {
          Operation op = output.sizeOperation();
          if (op == null)
          {
            if (field.type.typeof == Type.USERTYPE)
            {
              makeValueRec(field.name, 1);
              writeln(1, "output_value[\"" + field.name + "\"] = " + field.name + "_out;");
            } else
            {
              writeln(1, "output_value[\"" + field.name + "\"] = " + field.name + ";");
            }
            continue;
          }
          if (field.type.typeof == Type.USERTYPE)
          {
            writeln(1, format("Value %s_outarray;", field.name));
            writeln(1, format("for (int o%2$d=0; o%2$d < %1$s; o%2$d++)", op.name, i));
            writeln(1, "{");
            writeln(2, format("%s &rec = %s.arr[o%d];", name_of(module, field.type.name), field.name, i));
            makeValueRec("rec", 2);
            writeln(2, format("%s_outarray[o%d] = rec_out;", field.name, i));
            writeln(1, "}");            writeln(1, format("output_value[\"%1$s\"] = %1$s_outarray;", field.name));
          } else if (field.type.typeof == Type.CHAR)
          {
            writeln(1, "output_value[\"" + field.name + "\"] = " + field.name + ".arr;");
          } else
          {
            writeln(1, "// " + field.name + " as simple array;");
          }
          continue;
        }
        writeln(1, "output_value[\"" + field.name + "\"] = " + field.name + ";");
      }
    }
    if (hasOutput == true || hasResult == true)
    {
      writeln(1, "StreamWriterBuilder swb;");
      writeln(1, "output = writeString(swb, output_value);");
    }
    writeln("}");
    writeln();
  }

  private static void genInput(Module module, Prototype prototype, Vector<Argument> input_args)
  {
    genValidateInputString(prototype);
    writeln(1, "Value input_value;");
    writeln(1, "stringstream sstr(input);");
    writeln(1, "sstr >> input_value;");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field = input.getParameter(prototype);
      if (field == null)
        continue;
      Operation op = input.sizeOperation();
      String fieldTypeName = name_of(module, field.type.name);
      if (field.type.reference == Type.BYREFPTR)
        genInputByRefPtr(input_args, input, field, fieldTypeName);
      else if (field.type.reference == Type.BYPTR)
        genInputByPtr(input_args, i, input, field, op, fieldTypeName);
      else // no special reference
        genInputStd(input_args, field, fieldTypeName);
    }
  }

  private static void genValidateInputString(Prototype prototype)
  {
    String ifor = indent(1)+"if (";
    if (prototype.inputs.size() > 0)
    {
      writeln(1, "size_t _input_size_ = input.length();");
      for (int i = 0; i < prototype.inputs.size(); i++)
      {
        Action input = (Action) prototype.inputs.elementAt(i);
        Field field = input.getParameter(prototype);
        if (field == null)
          continue;
        write(format("%sinput.find(\"\\\"%s\\\":\") > _input_size_", ifor, field.name));
        ifor = "\n"+indent(1)+"||  ";
      }
      writeln(")");
      writeln(2, "throw XHTTP_ERROR(ERR_HTTP_POST_INPUT_INCORRECT, input.c_str());");
    }
  }

  private static void genInputStd(Vector<Argument> input_args, Field field, String fieldTypeName)
  {
    if (field.type.typeof == Type.CHAR)
    {
      writeln(1, format("string %s;", field.name));
      writeln(1, format("%1$s = input_value[\"%1$s\"]%2$s;", field.name, asType(field)));
      input_args.addElement(new Argument(field, field.name));
    } else if (field.type.typeof == Type.USERTYPE)
    {
      writeln(1, format("%s %s;", fieldTypeName, field.name));
      writeln(1, format("Value %1$s_value = input_value[\"%1$s\"];", field.name));
      writeln(1, format("builder.setValue(%s_value);", field.name));
      writeln(1, format("%s.SetData(builder);", field.name));
      input_args.addElement(new Argument(field, field.name));
    } else
    {
      writeln(1, format("%s %s;", fieldTypeName, field.name));
      writeln(1, format("%1$s = input_value[\"%1$s\"]%2$s;", field.name, asType(field)));
      input_args.addElement(new Argument(field, field.name));
    }
  }

  private static void genInputByPtr(Vector<Argument> input_args, int i, Action input, Field field, Operation op,
      String fieldTypeName)
  {
    if (input.hasSize() == false)
    {
      if (field.type.typeof == Type.CHAR)
      {
        writeln(1, format("string %s;", field.name));
        writeln(1, format("%1$s = input_value[\"%1$s\"]%2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "(char *)" + field.name + ".c_str()"));
      } else if (field.type.typeof == Type.USERTYPE)
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("Value %1$s_value = input_value[\"%1$s\"];", field.name));
        writeln(1, format("builder.setValue(%s_value);", field.name));
        writeln(1, format("%s.SetData(builder);", field.name));
        input_args.addElement(new Argument(field, "&" + field.name));
      } else // other std type
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("%1$s = input_value[\"%1$s\"]%2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "&" + field.name));
      }
    } else // has size
    {
      if (field.type.typeof == Type.CHAR)
      {
        writeln(1, format("string %s;", field.name));
        writeln(1, format("%1$s = input_value[\"%1$s\"]%2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "(char *)" + field.name + ".c_str()"));
      } 
      else if (field.type.typeof == Type.USERTYPE)
      {
        if (op != null && op.name != null)
        {
          String arrayName = format("InpArray%d", i);
          makeAutoArray(field, fieldTypeName, arrayName, op.name);
          writeln(1, format("Value %1$s_array = input_value[\"%1$s\"];", field.name));
          writeln(1, format("for (int i%2$d = 0; i%2$d < %1$s; i%2$d++)", op.name, i));
          writeln(1, "{");
          writeln(2, format("Value %1$s_value = %1$s_array[i%2$d];", field.name, i));
          writeln(2, "builder.clear();");
          writeln(2, format("builder.setValue(%1$s_value);", field.name));
          writeln(2, format("%1$s.arr[i%2$d].SetData(builder);", field.name, i));
          writeln(1, "}");
          input_args.addElement(new Argument(field, field.name + ".arr"));
        } else
        {
          writeln(1, format("//not handled byptr has size %s %d", field.name, field.type.typeof));
          writeln(1, format("//not handled byptr %s as list %s no op.name", fieldTypeName, input.name));
        }
      } 
      else // other std type
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("%1$s = input_value[\"%1$s\"]%2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "&" + field.name));
      }
    }
  }

  private static void genInputByRefPtr(Vector<Argument> input_args, Action input, Field field, String fieldTypeName)
  {
    if (input.hasSize() == false)
    {
      if (field.type.typeof == Type.CHAR)
      {
        writeln(1, format("string %s;", field.name));
        writeln(1, format("%1$s = input_value[\"%1$s\"] %2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, format("(char *)%s.c_str()", field.name)));
      } else if (field.type.typeof == Type.USERTYPE)
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("Value %1$s_array = input_value[\"%1$s\"];", field.name));
        writeln(1, "builder.setValue(" + field.name + "_value);");
        writeln(1, "" + field.name + ".SetData(builder);");
        input_args.addElement(new Argument(field, "&" + field.name));
      } else // other std type
      {
        genInputStd(input_args, field, fieldTypeName);
      }
    } else // has size
    {
      if (field.type.typeof == Type.CHAR)
      {
        writeln(1, format("string %s;", field.name));
        writeln(1, format("%1$s = input_value[\"%1$s\"] %2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, format("(char *)%s.c_str()", field.name)));
      } else if (field.type.typeof == Type.USERTYPE)
      {
        writeln(1, format("//not handled byrefptr input unexpected %s %s", fieldTypeName, field.name));
      } else // other std type
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("%1$s = input_value[\"%1$s\"] %2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "&" + field.name));
      }
    }
  }

  private static void genOutput(Module module, Prototype prototype, Vector<Argument> output_args)
  {
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      Action output = (Action) prototype.outputs.elementAt(i);
      Field field = output.getParameter(prototype);
      if (field == null)
        continue;
      String fieldTypeName = name_of(module, field.type.name);
      Action input = prototype.getInputAction(field.name);
      if (field.type.reference == Type.BYPTR)
      {
        if (output.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
            writeln("//#error unsized char* on output");
          else if (input == null)
          {
            if (field.type.typeof == Type.USERTYPE)
              writeln(1, "" + fieldTypeName + " " + field.name + ";");
            else
              writeln(1, "" + fieldTypeName + " " + field.name + "= 0;");
            output_args.addElement(new Argument(field, "&" + field.name));
          }
        } else if (input == null)
        {
          if (field.type.typeof == Type.USERTYPE)
            writeln(1, "" + fieldTypeName + " " + field.name + ";");
          else
            writeln(1, "" + fieldTypeName + " " + field.name + "= 0;");
          output_args.addElement(new Argument(field, "&" + field.name));
        }
      } else if (field.type.reference == Type.BYREFPTR)
      {
        if (output.hasSize() == false)
        {
          writeln("//#error unsized BYREFPTR on output");
        } else if (input == null)
        {
          String arrayName = format("OutArray%d", i);
          makeAutoArray(field, fieldTypeName, arrayName, null);
          output_args.addElement(new Argument(field, field.name + ".arr"));
        }
      } else
      {
        writeln(1, "" + fieldTypeName + " " + field.name + ";");
        output_args.addElement(new Argument(field, field.name));
      }
    }
  }

  private static void makeAutoArray(Field field, String fieldTypeName, String arrayName, String sizeName)
  {
    writeln(1, format("struct %s", arrayName));
    writeln(1, "{");
    writeln(2, format("%s* arr;", fieldTypeName));
    if (sizeName == null)
      writeln(2, format("%s() {arr = 0;}", arrayName));
    else
      writeln(2, format("%s(int size) {arr = new %s[size];}", arrayName, fieldTypeName));
    writeln(2, format("~%s() {if (arr != 0) free(arr);}", arrayName));
    if (sizeName == null)
      writeln(1, format("} %s;", field.name));
    else
      writeln(1, format("} %s(%s);", field.name, sizeName));
  }

  private static String getUseAs(Vector<Argument> input_args, Vector<Argument> output_args, Field field)
  {
    if (field.isInput)
    {
      for (Argument arg : input_args)
      {
        if (arg.field.name.compareTo(field.name) == 0)
          return arg.useAs;
      }
    } else if (field.isOutput)
    {
      for (Argument arg : output_args)
      {
        if (arg.field.name.compareTo(field.name) == 0)
          return arg.useAs;
      }
    }
    return field.name;
  }

  private static String name_of(Module module, String name)
  {
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.fields.size() > 0)
        if (structure.name.compareTo(name) == 0)
          return name;
    }
    return name;
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

  private static void makeValueRec(String name, int no)
  {
    writeln(no, "Value " + name + "_out;");
    writeln(no, "builder.clear();");
    writeln(no, "" + name + ".BuildData(builder);");
    writeln(no, "string " + name + "_result;");
    writeln(no, "builder.getRecord(" + name + "_result);");
    writeln(no, "stringstream " + name + "_str(" + name + "_result);");
    writeln(no, "" + name + "_str >> " + name + "_out;");
  }
}

