package bbd.crackle;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class PopGenC32Client extends Generator
{
  public static String description()
  {
    return "Generates Client C Dll and C++ class Code";
  }
  public static String documentation()
  {
    return "Generates VB6 based Client Code" + "\r\nHandles following pragmas"
        + "\r\n  AlignForSUN - ensure that all fields are on 8 byte boundaries.";
  }
  private static boolean alignForSun = false;
  private static PrintWriter errLog;
  /**
   * Reads input from stored repository
   */
  public static void main(String[] args)
  {
    try
    {
      PrintWriter outLog = new PrintWriter(System.out);
      errLog = outLog;
      for (int i = 0; i < args.length; i++)
      {
        outLog.println(args[i] + ": Generate ... ");
        ObjectInputStream in = new ObjectInputStream(new FileInputStream(args[i]));
        Module module = (Module) in.readObject();
        generate(module, "", outLog);
      }
      outLog.flush();
    } catch (Exception e)
    {
      e.printStackTrace();
    }
  }
  /**
   * Generates - VB6 Front end interface file - C Header for Server and Client -
   * C Client DLL Marshalling code - C Server Marshalling code
   */
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    indent_size = 4;
    errLog = outLog;
    outLog.println(module.name + " version " + module.version);
    for (int i = 0; i < module.pragmas.size(); i++)
    {
      String pragma = (String) module.pragmas.elementAt(i);
      if (pragma.trim().equalsIgnoreCase("AlignForSUN") == true)
        alignForSun = true;
    }
    generateCClient(module, output, outLog);
    generateCClientHeader(module, output, outLog);
  }
  /**
   * Sets up the writer and generates the general stuff
   */
  private static void generateCClientHeader(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name.toLowerCase() + "client.h");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "client.h");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln("#ifndef _" + module.name + "CLIENT_H_");
        writeln("#define _" + module.name + "CLIENT_H_");
        writeln("#include \"machine.h\"");
        writeln();
        writeln("#ifndef DLLEXPORT_STDCALL");
        writeln("#  ifdef M_W32");
        writeln("#    define DLLEXPORT_STDCALL(type) __declspec(DLLEXPORT_STDCALL) type __stdcall");
        writeln("#  else");
        writeln("#    define DLLEXPORT_STDCALL(type) type");
        writeln("#  endif");
        writeln("#endif");
        writeln();
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln();
        writeln("#include \"popgen.h\"");
        writeln("#include \"xstring.h\"");
        writeln("#include \"snaprpcntclient.h\"");
        writeln("#include \"handles.h\"");
        writeln();
        writeln("extern const char *" + module.name + "Version;");
        writeln("extern const int " + module.name + "Signature;");
        PopGenC32.generateC32Externs(module, writer);
        writeln();
        writeln("#pragma pack (push,1)");
        PopGenC32.generateC32Structs(module, writer, true);
        writeln("#pragma pack (pop)");
        writeln();
        writeln("class t" + module.name);
        writeln("{");
        writeln("  int Handle;");
        writeln("  e" + module.name + " errorCode;");
        writeln("  bool loggedOn;");
        writeln("  char fErrBuffer[4096];");
        writeln("  char fErrorDesc[256];");
        writeln("public:");
        writeln("  t" + module.name + "() {loggedOn = false;}");
        writeln("  ~t" + module.name + "() {if (loggedOn) Logoff();}");
        writeln("  int getHandle();");
        writeln("  void Logon(char* Service, char* Host, int Timeout=150000);");
        writeln("  void Logoff();");
        writeln("  char* ErrBuffer() {return ErrBuffer(fErrBuffer, sizeof(fErrBuffer));}");
        writeln("  char* ErrorDesc() {return ErrorDesc(fErrorDesc, sizeof(fErrorDesc));}");
        writeln("  char* ErrorDescForNo(e" + module.name
            + " ErrorNo) { return ErrorDescForNo(ErrorNo, fErrorDesc, sizeof(fErrorDesc));}");
        writeln("  char* ErrorDescForNo(e" + module.name + " ErrorNo, char *Buffer, int BufferLen);");
        writeln("  char* ErrBuffer(char *Buffer, int BufferLen);");
        writeln("  char* ErrorDesc(char *Buffer, int BufferLen);");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          PopGenC32.generateC32Header(module, prototype, writer);
        }
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
  public static void generateCClient(Module module, String output, PrintWriter outLog)
  {
    try
    {
      String w1 = "";
      outLog.println("Code: " + output + module.name.toLowerCase() + "client.cpp");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "client.cpp");
      PrintWriter outData = new PrintWriter(outFile);
      try
      {
        outLog.println("Code: " + output + module.name.toLowerCase() + "externs.h");
        OutputStream extFile = new FileOutputStream(output + module.name.toLowerCase() + "externs.h");
        PrintWriter extData = new PrintWriter(extFile);
        try
        {
          extData.println("// This code was generated, do not modify it, modify it at source and regenerate it.");
          extData.println("#ifndef _" + module.name.toUpperCase() + "EXTERNS_H");
          extData.println("#define _" + module.name.toUpperCase() + "EXTERNS_H");
          extData.println();
          writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
          writeln("#include \"" + module.name.toLowerCase() + "client.h\"");
          writeln();
          // writeln("#include \"ti.h\"");
          // writeln();
          writeln("const char *" + module.name + "Version = " + module.version + ";");
          writeln("const int " + module.name + "Signature = " + module.signature + ";");
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
          for (int i = 0; i < module.tables.size(); i++)
          {
            Table table = (Table) module.tables.elementAt(i);
            String comma = "  ";
            writeln("const char *" + module.name + table.name + "[] = ");
            writeln("{");
            for (int j = 0; j < table.messages.size(); j++)
            {
              Message message = (Message) table.messages.elementAt(j);
              writeln(comma + message.value + "   // " + message.name);
              comma = ", ";
            }
            writeln("};");
            writeln();
          }
          if (alignForSun == true)
          {
            writeln("inline int alignData(int size)");
            writeln("{");
            writeln("  int n = size % 8;");
            writeln("  if (n > 0) size += (8-n);");
            writeln("  return size;");
            writeln("}");
            writeln();
          }
          writeln("struct t" + module.name + "SnapCB");
          writeln("{");
          writeln("  tRPCClient* RPC;");
          writeln("  char *sendBuff;");
          writeln("  int sendBuffLen;");
          writeln("  int recvSize;");
          writeln("  e" + module.name + " result;");
          writeln("  int RC;");
          Vector retrievals = new Vector();
          Vector submittals = new Vector();
          for (int i = 0; i < module.prototypes.size(); i++)
          {
            Prototype prototype = (Prototype) module.prototypes.elementAt(i);
            if (prototype.codeType != Prototype.RPCCALL)
              continue;
            for (int j = 0; j < prototype.parameters.size(); j++)
            {
              Field parameter = (Field) prototype.parameters.elementAt(j);
              if (parameter.type.typeof != Type.CHAR
                  && (parameter.type.reference == Type.BYPTR || parameter.type.reference == Type.BYREFPTR))
              {
                if (prototype.hasOutputSize(parameter.name) || prototype.hasInputSize(parameter.name))
                {
                  if (parameter.type.reference == Type.BYREFPTR)
                    writeln("  " + parameter.type.c32DefRefPAsP(prototype.name + parameter.name, false) + ";");
                  else
                    writeln("  " + parameter.type.c32Def(prototype.name + parameter.name, false) + ";");
                }
                if (prototype.hasOutputSize(parameter.name))
                  retrievals.addElement(prototype.name + parameter.name);
                if (prototype.hasInputSize(parameter.name))
                  submittals.addElement(prototype.name + parameter.name);
              }
            }
          }
          writeln("  t" + module.name + "SnapCB(tString Service, tString Host, int Timeout)");
          w1 = ":";
          // There generally wont be the same retrieval and submittal,
          // even if there is it should not really matter much. If it
          // does then we will change this code.
          for (int i = 0; i < retrievals.size(); i++)
          {
            String s = (String) retrievals.elementAt(i);
            writeln("  " + w1 + " " + s + "(0)");
            w1 = ",";
          }
          writeln("  {");
          writeln("    RPC = new tRPCClient(Host, Service, Timeout);");
          writeln("  }");
          writeln("  ~t" + module.name + "SnapCB()");
          writeln("  {");
          writeln("    delete RPC;");
          writeln("  }");
          writeln("};");
          writeln();
          writeln("const int CookieStart = 1719;");
          writeln("const int NoCookies = 32;");
          writeln("static tHandle<t" + module.name + "SnapCB*, CookieStart, NoCookies> " + module.name + "CBHandle;");
          writeln();
          extData.println("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "SnapLogon(int* Handle, char* Service, char* Host, int Timeout);");
          writeln("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "SnapLogon(int* Handle, char* Service, char* Host, int Timeout)");
          writeln("{");
          writeln("  try");
          writeln("  {");
          writeln("    *Handle = " + module.name + "CBHandle.Create(new t" + module.name
              + "SnapCB(Service, Host, Timeout));");
          writeln("    return " + module.name.toUpperCase() + "_OK;");
          writeln("  }");
          writeln("  catch (xCept &x)");
          writeln("  {");
          writeln("    *Handle = -1;");
          writeln("    return " + module.name.toUpperCase() + "_SNAP_ERROR;");
          writeln("  }");
          writeln("}");
          writeln();
          extData.println("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "SnapLogoff(int* Handle);");
          writeln("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "SnapLogoff(int* Handle)");
          writeln("{");
          writeln("  if (*Handle < CookieStart || *Handle >= CookieStart+NoCookies)");
          writeln("    return " + module.name.toUpperCase() + "_INV_COOKIE;");
          writeln("  " + module.name + "CBHandle.Release(*Handle);");
          writeln("  *Handle = -1;");
          writeln("  return " + module.name.toUpperCase() + "_OK;");
          writeln("}");
          writeln();
          extData.println("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "SnapErrBuffer(int Handle, char *Buffer, int BufferLen);");
          writeln("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "SnapErrBuffer(int Handle, char *Buffer, int BufferLen)");
          writeln("{");
          writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
          writeln("    return " + module.name.toUpperCase() + "_INV_COOKIE;");
          writeln("  t" + module.name + "SnapCB* snapCB = " + module.name + "CBHandle.Use(Handle);");
          writeln("  if (snapCB->RPC->ErrSize())");
          writeln("    strncpy(Buffer, snapCB->RPC->ErrBuffer(), BufferLen);");
          writeln("  else");
          writeln("    strcpy(Buffer, \"\");");
          writeln("  for(int i = strlen(Buffer); i < BufferLen; i++)");
          writeln("    Buffer[i]  = ' ';");
          writeln("  return " + module.name.toUpperCase() + "_OK;");
          writeln("}");
          writeln();
          extData.println("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "SnapErrorDesc(e" + module.name + " RC, char *Buffer, int BufferLen);");
          writeln("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "SnapErrorDesc(e" + module.name + " RC, char *Buffer, int BufferLen)");
          writeln("{");
          writeln("  char W1[20]=\"\";");
          writeln("  if (RC < " + module.name.toUpperCase() + "_OK");
          w1 = "RC";
          if (module.messageBase > 0)
          {
            writeln("  || (RC > " + module.name.toUpperCase() + "_OK && RC < " + (module.messageBase) + ")");
            w1 = "(RC?RC-(" + module.messageBase + "-1):0)";
          }
          writeln("  ||  RC > " + module.name.toUpperCase() + "_LAST_LAST)");
          writeln("  {");
          writeln("    sprintf(W1, \" (RC=%d)\", RC);");
          writeln("    RC = " + module.name.toUpperCase() + "_LAST_LAST;");
          writeln("  }");
          writeln("  int n = (int)" + w1 + ";");
          writeln("  strncpy(Buffer, " + module.name + "Errors[n], BufferLen-(strlen(W1)+1));");
          writeln("  if (RC == " + module.name.toUpperCase() + "_LAST_LAST)");
          writeln("    strcat(Buffer, W1);");
          writeln("  for(int i = strlen(Buffer); i < BufferLen; i++)");
          writeln("    Buffer[i]  = ' ';");
          writeln("}");
          writeln();
          extData.println("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "SnapVersion(char *Buffer, int BufferLen);");
          writeln("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "SnapVersion(char *Buffer, int BufferLen)");
          writeln("{");
          writeln("  strncpy(Buffer, " + module.name + "Version, BufferLen);");
          writeln("  for(int i = strlen(Buffer); i < BufferLen; i++)");
          writeln("    Buffer[i]  = ' ';");
          writeln("}");
          writeln();
          for (int i = 0; i < module.prototypes.size(); i++)
          {
            Prototype prototype = (Prototype) module.prototypes.elementAt(i);
            if (prototype.codeType != Prototype.RPCCALL)
              continue;
            generateCClient(module, prototype, i, outData, extData);
          }
          writeln("int t" + module.name + "::getHandle()");
          writeln("{");
          writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
          writeln("    throw " + module.name.toUpperCase() + "_INV_COOKIE;");
          writeln("  return Handle;");
          writeln("}");
          writeln();
          writeln("void t" + module.name + "::Logon(char* Service, char* Host, int Timeout)");
          writeln("{");
          writeln("  errorCode = " + module.name + "SnapLogon(&Handle, Service, Host, Timeout);");
          writeln("  if (errorCode != 0)");
          writeln("  {");
          writeln("    Handle = -1;");
          writeln("    throw errorCode;");
          writeln("  }");
          writeln("  loggedOn = true;");
          writeln("}");
          writeln();
          writeln("void t" + module.name + "::Logoff()");
          writeln("{");
          writeln("  if (loggedOn == false)");
          writeln("    return;");
          writeln("  loggedOn = false;");
          writeln("  errorCode = " + module.name + "SnapLogoff(&Handle);");
          writeln("  if (errorCode != 0)");
          writeln("  {");
          writeln("    Handle = -1;");
          writeln("    throw errorCode;");
          writeln("  }");
          writeln("}");
          writeln();
          writeln("char* t" + module.name + "::ErrBuffer(char *Buffer, int BufferLen)");
          writeln("{");
          writeln("  " + module.name + "SnapErrBuffer(getHandle(), Buffer, BufferLen-1);");
          writeln("  Buffer[BufferLen-1] = 0;");
          writeln("  return strtrim(Buffer);");
          writeln("}");
          writeln();
          writeln("char* t" + module.name + "::ErrorDesc(char *Buffer, int BufferLen)");
          writeln("{");
          writeln("  " + module.name + "SnapErrorDesc(errorCode, Buffer, BufferLen-1);");
          writeln("  Buffer[BufferLen-1] = 0;");
          writeln("  return strtrim(Buffer);");
          writeln("}");
          writeln();
          writeln("char* t" + module.name + "::ErrorDescForNo(e" + module.name + " ErrorNo, char *Buffer, int BufferLen)");
          writeln("{");
          writeln("  " + module.name + "SnapErrorDesc(ErrorNo, Buffer, BufferLen-1);");
          writeln("  Buffer[BufferLen-1] = 0;");
          writeln("  return strtrim(Buffer);");
          writeln("}");
          writeln();
          for (int i = 0; i < module.prototypes.size(); i++)
          {
            Prototype prototype = (Prototype) module.prototypes.elementAt(i);
            if (prototype.codeType != Prototype.RPCCALL)
              continue;
            generateCClientImp(module, prototype, outData);
          }
          extData.println();
          extData.println("#endif");
        } finally
        {
          extData.flush();
          extFile.close();
          outData.flush();
          outFile.close();
        }
      } catch (IOException e1)
      {
        outLog.println("Generate Procs IO Error");
      } catch (Exception e)
      {
        System.out.println(e);
        System.out.flush();
        e.printStackTrace();
      }
    } catch (IOException e2)
    {
      outLog.println("Generate Procs IO Error");
    } catch (Exception e3)
    {
      System.out.println(e3);
      System.out.flush();
      e3.printStackTrace();
    }
  }
  /**
   * Generates the prototypes defined
   */
  static int n = 0;
  public static void generateCSwaps(Module module, Prototype prototype, Field field, Operation op, PrintWriter outData)
  {
    String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "", w6 = "";
    if (field.needsSwap() || field.isStruct(module))
    {
      if (field.type.reference == Type.BYPTR || field.type.reference == Type.BYREFPTR)
      {
        w3 = "->";
        w4 = "*";
      }
      if (op != null)
      {
        Field opField = prototype.getParameter(op.name);
        if (opField != null && (opField.type.reference == Type.BYPTR || opField.type.reference == Type.BYREFPTR))
          w5 = "*";
        n++;
        writeln("  " + w2 + "{for (int i_" + n + " = 0; i_" + n + " < " + w5 + op.name + "; i_" + n + "++)");
        w1 = w1 + "[i_" + n + "]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
        w6 = w6 + "}";
      }
      for (int j = 0; j < field.type.arraySizes.size(); j++)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(j);
        n++;
        writeln("    " + w2 + "{for (int i_" + n + "_" + j + " = 0; i_" + n + "_" + j + " < " + integer.intValue() + "; i_"
            + n + "_" + j + "++)");
        w1 = w1 + "[i_" + n + "_" + j + "]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
        w6 = w6 + "}";
      }
    }
    if (field.isStruct(module))
      writeln("    " + w2 + field.name + w1 + w3 + "Swaps();" + w6);
    else if (field.needsSwap())
      writeln("    " + w2 + "SwapBytes(" + w4 + field.name + w1 + ");" + w6);
    else if (field.type.typeof == Type.USERTYPE)
    {
      // Toolkit.getDefaultToolkit().beep();
      errLog.println("Warning: " + prototype.name + " " + field.name + " is of UserType and may require swapping.");
    }
  }
  /**
   * Generates the prototypes defined
   */
  public static void generateCClientImp(Module module, Prototype prototype, PrintWriter outData)
  {
    String w1 = "";
    write(prototype.type.c32Def("t" + module.name + "::" + prototype.name, false) + "(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(w1 + parameter.type.c32Def(parameter.name, false));
      w1 = ", ";
    }
    writeln(")");
    writeln("{");
    if (prototype.type.typeof != Type.VOID || prototype.type.reference == Type.BYPTR)
      writeln("  " + prototype.type.c32Name(false) + " Result;");
    write("  errorCode = " + module.name + "Snap" + prototype.name + "(getHandle()");
    write(", " + prototype.signature(true));
    if (prototype.type.typeof != Type.VOID || prototype.type.reference == Type.BYPTR)
      write(", Result");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(", " + parameter.name);
    }
    writeln(");");
    writeln("  if (errorCode != " + module.name.toUpperCase() + "_OK)");
    writeln("    throw errorCode;");
    if (prototype.type.typeof != Type.VOID || prototype.type.reference == Type.BYPTR)
      writeln("  return Result;");
    writeln("}");
    writeln();
  }
  /**
   * Generates the prototypes defined
   */
  public static void generateCClient(Module module, Prototype prototype, int no, PrintWriter outData, PrintWriter extData)
  {
    boolean hasReturn = false;
    Vector retrievals = new Vector();
    Vector submittals = new Vector();
    Vector allocated = new Vector();
    if (prototype.type.reference != Type.BYVAL)
    {
      writeln("#error Only non pointers are allowed as return values");
      errLog.println("#error Only non pointers are allowed as return values");
    }
    if (prototype.type.reference == Type.BYVAL && prototype.type.typeof != Type.VOID)
      hasReturn = true;
    extData.print("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "Snap" + prototype.name + "(int Handle, int Signature");
    if (hasReturn)
      extData.print(", " + prototype.type.c32Name(false) + "& Result");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      extData.print(", " + parameter.type.c32Def(parameter.name, false));
    }
    extData.println(");");
    write("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "Snap" + prototype.name + "(int Handle, int Signature");
    if (hasReturn)
      write(", " + prototype.type.c32Name(false) + "& Result");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(", " + parameter.type.c32Def(parameter.name, false));
    }
    writeln(")");
    writeln("{");
    writeln("  if (Signature != " + prototype.signature(true) + ")");
    writeln("    return " + module.name.toUpperCase() + "_INV_SIGNATURE;");
    writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
    writeln("    return " + module.name.toUpperCase() + "_INV_COOKIE;");
    writeln("  t" + module.name + "SnapCB* snapCB = " + module.name + "CBHandle.Use(Handle);");
    writeln("  try");
    writeln("  {");
    if (alignForSun == true)
      writeln("    snapCB->sendBuffLen = alignData(4);");
    else
      writeln("    snapCB->sendBuffLen = 4;");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field = input.getParameter(prototype);
      if (field == null)
      {
        writeln("#error " + input.name + " is an undefined input parameter");
        errLog.println("#error " + input.name + " is an undefined input parameter");
        continue;
      }
      if (field.type.reference == Type.BYPTR || field.type.reference == Type.BYREFPTR)
      {
        if (input.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            if (alignForSun == true)
              writeln("    snapCB->sendBuffLen += alignData(alignData(sizeof(int))+strlen(" + field.name + ")+1);");
            else
              writeln("    snapCB->sendBuffLen += (sizeof(int)+strlen(" + field.name + ")+1);");
          } else
          {
            if (alignForSun == true)
              writeln("    snapCB->sendBuffLen += alignData(sizeof(*" + field.name + "));");
            else
              writeln("    snapCB->sendBuffLen += sizeof(*" + field.name + ");");
          }
        } else
        {
          Operation op = input.sizeOperation();
          if (alignForSun == true)
            writeln("    snapCB->sendBuffLen += alignData(alignData(sizeof(int))+" + op.name + "*sizeof(*" + field.name
                + "));");
          else
            writeln("    snapCB->sendBuffLen += (sizeof(int)+" + op.name + "*sizeof(*" + field.name + "));");
        }
      } else
      {
        if (input.hasSize() == false)
        {
          if (alignForSun == true)
            writeln("    snapCB->sendBuffLen += alignData(sizeof(" + field.name + "));");
          else
            writeln("    snapCB->sendBuffLen += sizeof(" + field.name + ");");
        } else
        {
          writeln("#error " + field.name + " is not a pointer parameter but has a size");
          errLog.println("#error " + field.name + " is not a pointer parameter but has a size");
        }
      }
    }
    writeln("    snapCB->sendBuff = new char[snapCB->sendBuffLen];");
    writeln("    char* ip = snapCB->sendBuff;");
    String w1 = "";
    if (prototype.inputs.size() == 0)
      w1 = "// ";
    writeln("    *(int*)ip = Signature;");
    writeln("    SwapBytes(*(int*)ip);");
    if (alignForSun == true)
      writeln("    " + w1 + "ip += alignData(sizeof(int));");
    else
      writeln("    " + w1 + "ip += sizeof(int);");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      if (i + 1 == prototype.inputs.size())
        w1 = "// ";
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field = input.getParameter(prototype);
      if (field == null)
        continue;
      Operation op = input.sizeOperation();
      generateCSwaps(module, prototype, field, op, outData);
      if (field.type.reference == Type.BYPTR || field.type.reference == Type.BYREFPTR)
      {
        if (input.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln("    *(int*)ip = (strlen(" + field.name + ")+1);");
            writeln("    SwapBytes(*(int*)ip);");
            if (alignForSun == true)
              writeln("    ip += alignData(sizeof(int));");
            else
              writeln("    ip += sizeof(int);");
            writeln("    memcpy(ip, " + field.name + ", (int)strlen(" + field.name + ")+1);");
            if (alignForSun == true)
              writeln("    " + w1 + "ip += alignData(strlen(" + field.name + ")+1);");
            else
              writeln("    " + w1 + "ip += (strlen(" + field.name + ")+1);");
          } else
          {
            writeln("    memcpy(ip, (void*)" + field.name + ", (int)sizeof(*" + field.name + "));");
            if (alignForSun == true)
              writeln("    " + w1 + "ip += alignData(sizeof(*" + field.name + "));");
            else
              writeln("    " + w1 + "ip += sizeof(*" + field.name + ");");
          }
        } else
        {
          if (field.type.typeof != Type.CHAR)
            submittals.addElement(field);
          writeln("    *(int*)ip = (" + op.name + "*sizeof(*" + field.name + "));");
          writeln("    SwapBytes(*(int*)ip);");
          if (alignForSun == true)
            writeln("    ip += alignData(sizeof(int));");
          else
            writeln("    ip += sizeof(int);");
          writeln("    memcpy(ip, (void*)" + field.name + ", (int)(" + op.name + "*sizeof(*" + field.name + ")));");
          if (alignForSun == true)
            writeln("    " + w1 + "ip += alignData(int(" + op.name + "*sizeof(*" + field.name + ")));");
          else
            writeln("    " + w1 + "ip += (int)(" + op.name + "*sizeof(*" + field.name + "));");
        }
      } else
      {
        writeln("    memcpy(ip, (char*)&" + field.name + ", (int)sizeof(" + field.name + "));");
        if (alignForSun == true)
          writeln("    " + w1 + "ip += alignData(sizeof(" + field.name + "));");
        else
          writeln("    " + w1 + "ip += sizeof(" + field.name + ");");
      }
      generateCSwaps(module, prototype, field, op, outData);
    }
    if (prototype.message.length() > 0)
      w1 = prototype.message;
    else
      w1 = "" + no;
    writeln("    snapCB->RPC->Call(" + w1 + ", snapCB->sendBuff, snapCB->sendBuffLen);");
    writeln("    delete [] snapCB->sendBuff;");
    writeln("    snapCB->sendBuff = 0;");
    boolean hasRX = false;
    w1 = "// ";
    if (prototype.outputs.size() > 0)
      w1 = "";
    if (prototype.outputs.size() > 0 || hasReturn)
    {
      hasRX = true;
      writeln("    if (snapCB->RPC->RxSize())");
      writeln("    {");
      writeln("      ip = (char*)snapCB->RPC->RxBuffer();");
      if (hasReturn)
      {
        writeln("      memcpy(&Result, ip, (int)sizeof(Result));");
        writeln("      SwapBytes(Result);");
        if (alignForSun == true)
          writeln("      " + w1 + "ip += alignData(sizeof(Result));");
        else
          writeln("      " + w1 + "ip += sizeof(Result);");
      }
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        if (i + 1 == prototype.outputs.size())
          w1 = "// ";
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field = output.getParameter(prototype);
        if (field == null)
        {
          writeln("#error " + output.name + " is an undefined input parameter");
          errLog.println("#error " + output.name + " is an undefined input parameter");
          continue;
        }
        Operation op = output.sizeOperation();
        if (field.type.reference == Type.BYPTR || field.type.reference == Type.BYREFPTR)
        {
          if (output.hasSize() == false)
          {
            if (field.type.typeof == Type.CHAR)
            {
              writeln("#error " + output.name + " unsized chars cannot be used as output");
              errLog.println("#error " + output.name + " unsized chars cannot be used as output");
              continue;
            }
            writeln("      memcpy(" + field.name + ", ip, sizeof(*" + field.name + "));");
            if (alignForSun == true)
              writeln("      " + w1 + "ip += alignData(sizeof(*" + field.name + "));");
            else
              writeln("      " + w1 + "ip += sizeof(*" + field.name + ");");
          } else
          {
            if (field.type.typeof != Type.CHAR)
              retrievals.addElement(field);
            writeln("      snapCB->recvSize = *(int*)ip;");
            writeln("      SwapBytes(snapCB->recvSize);");
            if (alignForSun == true)
              writeln("      ip += alignData(sizeof(int));");
            else
              writeln("      ip += sizeof(int);");
            if (field.type.reference == Type.BYREFPTR)
            {
              String s = prototype.getOutputSizeName(field.name);
              Field sf = prototype.getParameter(s);
              String w = "";
              if (sf.type.reference == Type.BYPTR)
                w = "*";
              writeln("      " + field.name + " = new " + field.type.c32Name(false) + "[" + w + s + "];");
            }
            writeln("      memcpy(" + field.name + ", ip, snapCB->recvSize);");
            if (alignForSun == true)
              writeln("      " + w1 + "ip += alignData(snapCB->recvSize);");
            else
              writeln("      " + w1 + "ip += snapCB->recvSize;");
          }
        } else
        {
          writeln("      memcpy(&" + field.name + ", ip, sizeof(" + field.name + "));");
          if (alignForSun == true)
            writeln("      " + w1 + "ip += alignData(sizeof(" + field.name + "));");
          else
            writeln("      " + w1 + "ip += sizeof(" + field.name + ");");
        }
        generateCSwaps(module, prototype, field, op, outData);
      }
    }
    if (hasRX)
    {
      writeln("      snapCB->RPC->RxFree();");
      writeln("    }");
    }
    writeln("    return (e" + module.name + ")snapCB->RPC->ReturnCode();");
    writeln("  }");
    writeln("  catch(xCept &x)");
    writeln("  {");
    writeln("    snapCB->RPC->ErrBuffer(x.ErrorStr());");
    writeln("    return " + module.name.toUpperCase() + "_SNAP_ERROR;");
    writeln("  }");
    writeln("  catch(...)");
    writeln("  {");
    writeln("    return " + module.name.toUpperCase() + "_SNAP_ERROR;");
    writeln("  }");
    writeln("}");
    writeln();
    if (submittals.size() > 0)
    {
      for (int i = 0; i < submittals.size(); i++)
      {
        Field parameter = (Field) submittals.elementAt(i);
        extData.println("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "Snap" + prototype.name + parameter.name + "Prepare(int Handle, int Size);");
        writeln("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "Snap" + prototype.name + parameter.name + "Prepare(int Handle, int Size)");
        writeln("{");
        writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln("    return;");
        writeln("  t" + module.name + "SnapCB* snapCB = " + module.name + "CBHandle.Use(Handle);");
        // writeln("  if(snapCB->"+prototype.name+parameter.name+")");
        // writeln("    delete [] snapCB->"+prototype.name+parameter.name+";");
        writeln("  snapCB->" + prototype.name + parameter.name + " = new " + parameter.type.c32Name(false) + " [Size];");
        allocated.addElement(parameter.name);
        writeln("}");
        writeln();
        extData.print("extern \"C\" DLLEXPORT_STDCALL(int) " + module.name + "Snap" + prototype.name + parameter.name + "Fill(int Handle");
        extData.print(", " + parameter.type.c32Def("Rec", false));
        extData.println(", int Index);");
        write("extern \"C\" DLLEXPORT_STDCALL(int) " + module.name + "Snap" + prototype.name + parameter.name + "Fill(int Handle");
        write(", " + parameter.type.c32Def("Rec", false));
        writeln(", int Index)");
        writeln("{");
        writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln("    return 0;");
        writeln("  t" + module.name + "SnapCB* snapCB = " + module.name + "CBHandle.Use(Handle);");
        writeln("  snapCB->" + prototype.name + parameter.name + "[Index] = *Rec;");
        writeln("  return 1;");
        writeln("}");
        writeln();
      }
    }
    if (retrievals.size() > 0 || submittals.size() > 0)
    {
      extData.print("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "Snap" + prototype.name
          + "Start(int Handle, int Signature");
      if (hasReturn)
        extData.print(", " + prototype.type.c32Name(false) + "& Result");
      for (int i = 0; i < prototype.parameters.size(); i++)
      {
        Field parameter = (Field) prototype.parameters.elementAt(i);
        if (parameter.type.typeof != Type.CHAR
            && (parameter.type.reference == Type.BYPTR || parameter.type.reference == Type.BYREFPTR))
          if (prototype.hasOutputSize(parameter.name) || prototype.hasInputSize(parameter.name))
            continue;
        extData.print(", " + parameter.type.c32Def(parameter.name, false));
      }
      extData.println(");");
      write("extern \"C\" DLLEXPORT_STDCALL(e" + module.name + ") " + module.name + "Snap" + prototype.name
          + "Start(int Handle, int Signature");
      if (hasReturn)
        write(", " + prototype.type.c32Name(false) + "& Result");
      for (int i = 0; i < prototype.parameters.size(); i++)
      {
        Field parameter = (Field) prototype.parameters.elementAt(i);
        if (parameter.type.typeof != Type.CHAR
            && (parameter.type.reference == Type.BYPTR || parameter.type.reference == Type.BYREFPTR))
          if (prototype.hasOutputSize(parameter.name) || prototype.hasInputSize(parameter.name))
            continue;
        write(", " + parameter.type.c32Def(parameter.name, false));
      }
      writeln(")");
      writeln("{");
      writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
      writeln("    return " + module.name.toUpperCase() + "_INV_COOKIE;");
      writeln("  t" + module.name + "SnapCB* snapCB = " + module.name + "CBHandle.Use(Handle);");
      for (int i = 0; i < retrievals.size(); i++)
      {
        Field parameter = (Field) retrievals.elementAt(i);
        if (parameter.type.reference == Type.BYREFPTR)
          continue;
        String s = prototype.getOutputSizeName(parameter.name);
        Field sf = prototype.getParameter(s);
        String w = "";
        if (sf == null)
        {
          writeln("#error " + parameter.name + " size " + s + " not found");
          continue;
        }
        boolean alreadyDone = false;
        for (int j = 0; j < allocated.size(); j++)
        {
          String name = (String) allocated.elementAt(j);
          if (name.compareTo(parameter.name) == 0)
          {
            alreadyDone = true;
            break;
          }
        }
        if (alreadyDone)
          continue;
        if (sf.type.reference == Type.BYPTR)
          w = "*";
        writeln("  snapCB->" + prototype.name + parameter.name + " = new " + parameter.type.c32Name(false) + "[" + w + s
            + "];");
      }
      write("  e" + module.name + " result = " + module.name + "Snap" + prototype.name + "(Handle, Signature");
      if (hasReturn)
        write(", Result");
      for (int i = 0; i < prototype.parameters.size(); i++)
      {
        Field parameter = (Field) prototype.parameters.elementAt(i);
        if (parameter.type.typeof != Type.CHAR
            && (parameter.type.reference == Type.BYPTR || parameter.type.reference == Type.BYREFPTR)
            && (prototype.hasOutputSize(parameter.name) || prototype.hasInputSize(parameter.name)))
          write(", snapCB->" + prototype.name + parameter.name);
        else
          write(", " + parameter.name);
      }
      writeln(");");
      for (int i = 0; i < submittals.size(); i++)
      {
        Field parameter = (Field) submittals.elementAt(i);
        writeln("  delete [] snapCB->" + prototype.name + parameter.name + ";");
        writeln("  snapCB->" + prototype.name + parameter.name + " = 0;");
      }
      // ccc
      writeln("  return result;");
      writeln("}");
      writeln();
    }
    if (retrievals.size() > 0)
    {
      for (int i = 0; i < retrievals.size(); i++)
      {
        Field parameter = (Field) retrievals.elementAt(i);
        extData.print("extern \"C\" DLLEXPORT_STDCALL(int) " + module.name + "Snap" + prototype.name + parameter.name + "Next(int Handle");
        extData.print(", " + parameter.type.c32DefRefPAsP("Rec", false));
        extData.println(", int Index);");
        write("extern \"C\" DLLEXPORT_STDCALL(int) " + module.name + "Snap" + prototype.name + parameter.name + "Next(int Handle");
        write(", " + parameter.type.c32DefRefPAsP("Rec", false));
        writeln(", int Index)");
        writeln("{");
        writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln("    return 0;");
        writeln("  t" + module.name + "SnapCB* snapCB = " + module.name + "CBHandle.Use(Handle);");
        writeln("  *Rec = snapCB->" + prototype.name + parameter.name + "[Index];");
        writeln("  return 1;");
        writeln("}");
        writeln();
        extData.println("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "Snap" + prototype.name + parameter.name + "Done(int Handle);");
        writeln("extern \"C\" DLLEXPORT_STDCALL(void) " + module.name + "Snap" + prototype.name + parameter.name + "Done(int Handle)");
        writeln("{");
        writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln("    return;");
        writeln("  t" + module.name + "SnapCB* snapCB = " + module.name + "CBHandle.Use(Handle);");
        writeln("  if(snapCB->" + prototype.name + parameter.name + ")");
        writeln("    delete [] snapCB->" + prototype.name + parameter.name + ";");
        writeln("  snapCB->" + prototype.name + parameter.name + " = 0;");
        writeln("}");
        writeln();
      }
    }
  }
}
