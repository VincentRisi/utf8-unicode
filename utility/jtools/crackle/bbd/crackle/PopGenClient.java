package bbd.crackle;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class PopGenClient extends Generator
{
  public static String description()
  {
    return "Generates Client C Dll and C++ class Code";
  }
  public static String documentation()
  {
    return "Generates Client Code"
      + "\r\nHandles following pragmas"
      + "\r\n  AlignForSUN - ensure that all fields are on 8 byte boundaries."
      ;
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
      for (int i = 0; i <args.length; i++)
      {
        outLog.println(args[i]+": Generate ... ");
        ObjectInputStream in = new ObjectInputStream(new FileInputStream(args[i]));
        Module module = (Module)in.readObject();
        in.close();
        generate(module, "", outLog);
      }
      outLog.flush();
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }
  /**
  * Generates
  * - VB Front end interface file
  * - C Header for Server and Client
  * - C Client DLL Marshalling code
  * - C Server Marshalling code
  */
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    errLog = outLog;
    outLog.println(module.name+" version "+module.version);
    for (int i = 0; i < module.pragmas.size(); i++)
    {
      String pragma = (String) module.pragmas.elementAt(i);
      if (pragma.trim().equalsIgnoreCase("AlignForSUN") == true)
        alignForSun = true;
    }
    generateCClient(module, output, outLog);
    generateCClientHeader(module, output, outLog);
    generateCClientMake(module, output, outLog);
  }
  /**
  * Sets up the writer and generates the general stuff
  */
  public static void generateCClientMake(Module module, String output, PrintWriter outLog)
  {
    try
    {
      File makefile = new File(output+"makefile");
      if (makefile.exists())
        return;
      outLog.println("Code: "+output+"makefile");
      OutputStream outFile = new FileOutputStream(output+"makefile");
      writer = new PrintWriter(outFile);
      try
      {
        writeln(".AUTODEPEND");
        writeln();
        writeln("# "+module.name+" makefile");
        writeln("# Preset BBDLIBS to latest TI Library eg. c:\\dev\\407\\.");
        writeln("# Preset ORANO to 7 8 or 9 etc.");
        writeln("# $(BCB) should be Preset.");
        writeln("# If you want to use Microsoft C++ to compile build");
        writeln("# a similar makefile. Or Build using an IDE.");
        writeln("CC = bcc32");
        writeln();
        writeln("INCS = -I..;$(BCB)\\include");
        writeln();
        writeln("LIBS = \\");
        writeln("\t$(BBDLIBS)\\libidl2_$(ORANO)b \\");
        writeln("\t$(BBDLIBS)\\libsec_$(ORANO)b \\");
        writeln("\t$(BBDLIBS)\\librpc_$(ORANO)b \\");
        writeln("\t$(BBDLIBS)\\libtib \\");
        writeln();
        writeln("CFLAGS = -c -Od -H- -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -tWM -Tkh30000");
        writeln();
        writeln(".cpp.obj:");
        writeln("\t$(CC) -c $(CFLAGS) $(INCS) $<");
        writeln();
        writeln(module.name.toUpperCase()+" =\\");
        writeln("\t"+module.name.toLowerCase()+"Client.obj");
        writeln();
        writeln(module.name.toLowerCase()+".dll: $("+module.name.toUpperCase()+")");
        writeln("\t$(CC) $(CFLAGS) -e"+module.name.toLowerCase()+".dll -Ls:$(BCB)\\lib @&&|");
        writeln("$("+module.name.toUpperCase()+")");
        writeln("$(LIBS)");
        writeln("|");
        writeln();
      }
      finally
      {
        writer.flush();
        outFile.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
    catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }
  /**
  * Sets up the writer and generates the general stuff
  */
  public static void generateCClientHeader(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: "+output+module.name.toLowerCase()+"ntclient.h");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"ntclient.h");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln("#ifndef _"+module.name+"NTCLIENT_H_");
        writeln("#define _"+module.name+"NTCLIENT_H_");
        writeln("#include \"machine.h\"");
        writeln();
        writeln("#ifndef DLLEXPORT_STDCALL");
        writeln("#  ifdef M_W32");
        writeln("#    define DLLEXPORT_STDCALL(type) __declspec(dllexport) type __stdcall");
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
        writeln("#if defined M_W32");
        writeln("#include \"snaprpcntclient.h\"");
        writeln("#elif defined M_AIX");
        writeln("#include \"snaprpcaixclient.h\"");
        writeln("#elif defined M_GNU");
        writeln("#include \"snaprpcgnuclient.h\"");
        writeln("#endif");
        writeln("#include \"handles.h\"");
        writeln();
        writeln("extern const char *" + module.name + "Version;");
        writeln("extern const int32 "+module.name+"Signature;");
        PopGen.generateCExterns(module, writer);
        writeln();
        writeln("#if defined M_W32");
        writeln("#pragma pack (push,1)");
        writeln("#elif defined M_AIX");
        writeln("#pragma pack (1)");
        writeln("#elif defined M_GNU");
        writeln("#pragma pack (push,1)");
        writeln("#endif");
        PopGen.generateCStructs(module, writer, true);
        writeln("#if defined M_W32");
        writeln("#pragma pack (pop,1)");
        writeln("#elif defined M_AIX");
        writeln("#pragma pack (pop)");
        writeln("#elif defined M_GNU");
        writeln("#pragma pack (pop)");
        writeln("#endif");
        writeln();
        writeln("class t"+module.name);
        writeln("{");
        writeln(1, "int32 Handle;");
        writeln(1, "e"+module.name+" errorCode;");
        writeln(1, "bool loggedOn;");
        writeln(1, "char fErrBuffer[4096];");
        writeln(1, "char fErrorDesc[256];");
        writeln("public:");
        writeln(1, "t"+module.name+"() {loggedOn = false;}");
        writeln(1, "~t"+module.name+"() {if (loggedOn) Logoff();}");
        writeln(1, "int32 getHandle();");
        writeln(1, "tRPCClient* getRPC();");
        writeln(1, "void Logon(char* Service, char* Host, int32 Timeout=150000);");
        writeln(1, "void Logoff();");
        writeln(1, "char* ErrBuffer() {return ErrBuffer(fErrBuffer, sizeof(fErrBuffer));}");
        writeln(1, "char* ErrorDesc() {return ErrorDesc(fErrorDesc, sizeof(fErrorDesc));}");
        writeln(1, "char* ErrorDescForNo(e"+module.name+" ErrorNo) { return ErrorDescForNo(ErrorNo, fErrorDesc, sizeof(fErrorDesc));}");
        writeln(1, "char* ErrorDescForNo(e"+module.name+" ErrorNo, char *Buffer, int32 BufferLen);");
        writeln(1, "char* ErrBuffer(char *Buffer, int32 BufferLen);");
        writeln(1, "char* ErrorDesc(char *Buffer, int32 BufferLen);");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          PopGen.generateCHeader(module, prototype, writer);
        }
        writeln("};");
        writeln();
        writeln("#endif");
      }
      finally
      {
        writer.flush();
        outFile.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
    catch (Throwable e)
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
      outLog.println("Code: "+output+module.name.toLowerCase()+"client0.cpp");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"client0.cpp");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#include \"" + module.name.toLowerCase() + "ntclient.h\"");
        writeln();
        //writeln("#include \"ti.h\"");
        //writeln();
        writeln("const char *" + module.name + "Version = " + module.version + ";");
        writeln("const int32 " + module.name + "Signature = " + module.signature + ";");
        writeln();
        writeln("const char *" + module.name + "Errors[] = ");
        writeln("{ \"No Error\"");
        for (int i = 0; i < module.messages.size(); i++)
        {
          Message message = (Message) module.messages.elementAt(i);
          writeln(", "+message.value+"   // "+message.name);
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
          writeln("const char *"+module.name+table.name+"[] = ");
          writeln("{");
          for (int j = 0; j < table.messages.size(); j++)
          {
            Message message = (Message) table.messages.elementAt(j);
            writeln(comma+message.value+"   // "+message.name);
            comma = ", ";
          }
          writeln("};");
          writeln();
        }
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
        writeln("struct t"+module.name+"SnapCB");
        writeln("{");
        writeln(1, "tRPCClient* RPC;");
        writeln(1, "char *sendBuff;");
        writeln(1, "int32 sendBuffLen;");
        writeln(1, "int32 recvSize;");
        writeln(1, "e"+module.name+" result;");
        writeln(1, "int32 RC;");
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
            && (parameter.type.reference == Type.BYPTR
                || parameter.type.reference == Type.BYREFPTR))
            {
              if (prototype.hasOutputSize(parameter.name)
              ||  prototype.hasInputSize(parameter.name))
              {
                if (parameter.type.reference == Type.BYREFPTR)
                  writeln(1, ""+parameter.type.cDefRefPAsP(prototype.name+parameter.name, false)+";");
                else
                  writeln(1, ""+parameter.type.cDef(prototype.name+parameter.name, false)+";");
              }
              if (prototype.hasOutputSize(parameter.name))
                retrievals.addElement(prototype.name+parameter.name);
              if (prototype.hasInputSize(parameter.name))
                submittals.addElement(prototype.name+parameter.name);
            }
          }
        }
        writeln(1, "t"+module.name+"SnapCB(tString Service, tString Host, int32 Timeout)");
        w1 = ":";
        // There generally wont be the same retrieval and submittal,
        // even if there is it should not really matter much. If it
        // does then we will change this code.
        for (int i = 0; i < retrievals.size(); i++)
        {
          String s = (String) retrievals.elementAt(i);
          writeln(1, ""+w1+" "+s+"(0)");
          w1 = ",";
        }
        writeln(1, "{");
        writeln("#if defined M_W32");
        writeln(2, "RPC = new tRPCClient(Host, Service, Timeout);");
        writeln("#elif defined M_AIX ");
        writeln(2, format("RPC = new tRPCClient(\"%s\", Host, Service, Timeout);", module.name));
        writeln("#elif defined M_GNU ");
        writeln(2, format("RPC = new tRPCClient(\"%s\", Host, Service, Timeout);", module.name));
        writeln("#endif");
        writeln(1, "}");
        writeln(1, "~t"+module.name+"SnapCB()");
        writeln(1, "{");
        writeln(2, "delete RPC;");
        writeln(1, "}");
        writeln("};");
        writeln();
        writeln("const int CookieStart = 1719;");
        writeln("const int NoCookies = 32;");
        writeln("static tHandle<t"+module.name+"SnapCB*, CookieStart, NoCookies> "+module.name+"CBHandle;");
        writeln();
        writeln("extern \"C\" DLLEXPORT_STDCALL(e"+module.name+") "+module.name+"SnapLogon(int32* Handle, char* Service, char* Host, int32 Timeout)");
        writeln("{");
        writeln(1, "try");
        writeln(1, "{");
        writeln(2, "*Handle = "+module.name+"CBHandle.Create(new t"+module.name+"SnapCB(Service, Host, Timeout));");
        writeln(2, "return "+module.name.toUpperCase()+"_OK;");
        writeln(1, "}");
        writeln(1, "catch (xCept &x)");
        writeln(1, "{");
        writeln(2, "*Handle = -1;");
        writeln(2, "return "+module.name.toUpperCase()+"_SNAP_ERROR;");
        writeln(1, "}");
        writeln("}");
        writeln();
        writeln("extern \"C\" DLLEXPORT_STDCALL(e"+module.name+") "+module.name+"SnapLogoff(int32* Handle)");
        writeln("{");
        writeln(1, "if (*Handle < CookieStart || *Handle >= CookieStart+NoCookies)");
        writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln(1, ""+module.name+"CBHandle.Release(*Handle);");
        writeln(1, "*Handle = -1;");
        writeln(1, "return "+module.name.toUpperCase()+"_OK;");
        writeln("}");
        writeln();
        writeln("extern \"C\" DLLEXPORT_STDCALL(e"+module.name+") "+module.name+"SnapErrBuffer(int32 Handle, char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "if (snapCB->RPC->ErrSize())");
        writeln(2, "strncpy(Buffer, snapCB->RPC->ErrBuffer(), BufferLen);");
        writeln(1, "else");
        writeln(2, "strcpy(Buffer, \"\");");
        writeln(1, "for(int i = strlen(Buffer); i < BufferLen; i++)");
        writeln(2, "Buffer[i]  = ' ';");
        writeln(1, "return "+module.name.toUpperCase()+"_OK;");
        writeln("}");
        writeln();
        writeln("extern \"C\" DLLEXPORT_STDCALL(void) "+module.name+"SnapErrorDesc(e"+module.name+" RC, char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, "char W1[20]=\"\";");
        writeln(1, "if (RC < "+module.name.toUpperCase()+"_OK");
        w1 = "RC";
        if (module.messageBase > 0)
        {
          writeln(1, "|| (RC > "+module.name.toUpperCase()+"_OK && RC < "+(module.messageBase)+")");
          w1 = "(RC?RC-("+module.messageBase+"-1):0)";
        }
        writeln(1, "||  RC > "+module.name.toUpperCase()+"_LAST_LAST)");
        writeln(1, "{");
        writeln(2, "sprintf(W1, \" (RC=%d)\", RC);");
        writeln(2, "RC = "+module.name.toUpperCase()+"_LAST_LAST;");
        writeln(1, "}");
        writeln(1, "int n = (int)"+w1+";");
        writeln(1, "strncpy(Buffer, "+module.name+"Errors[n], BufferLen-(strlen(W1)+1));");
        writeln(1, "if (RC == "+module.name.toUpperCase()+"_LAST_LAST)");
        writeln(2, "strcat(Buffer, W1);");
        writeln(1, "for(int i = strlen(Buffer); i < BufferLen; i++)");
        writeln(2, "Buffer[i]  = ' ';");
        writeln("}");
        writeln();
        writeln("extern \"C\" DLLEXPORT_STDCALL(void) "+module.name+"SnapVersion(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, "strncpy(Buffer, "+module.name+"Version, BufferLen);");
        writeln(1, "for(int i = strlen(Buffer); i < BufferLen; i++)");
        writeln(2, "Buffer[i]  = ' ';");
        writeln("}");
        writeln();
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          generateCClient(module, prototype, i);
        }
        writeln("int32 t"+module.name+"::getHandle()");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "throw "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln(1, "return Handle;");
        writeln("}");
        writeln();
        writeln("tRPCClient* t"+module.name+"::getRPC()");
        writeln("{");
        writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "return snapCB->RPC;");
        writeln("}");
        writeln();
        writeln("void t"+module.name+"::Logon(char* Service, char* Host, int32 Timeout)");
        writeln("{");
        writeln(1, "errorCode = "+module.name+"SnapLogon(&Handle, Service, Host, Timeout);");
        writeln(1, "if (errorCode != 0)");
        writeln(1, "{");
        writeln(2, "Handle = -1;");
        writeln(2, "throw errorCode;");
        writeln(1, "}");
        writeln(1, "loggedOn = true;");
        writeln("}");
        writeln();
        writeln("void t"+module.name+"::Logoff()");
        writeln("{");
        writeln(1, "if (loggedOn == false)");
        writeln(2, "return;");
        writeln(1, "loggedOn = false;");
        writeln(1, "errorCode = "+module.name+"SnapLogoff(&Handle);");
        writeln(1, "if (errorCode != 0)");
        writeln(1, "{");
        writeln(2, "Handle = -1;");
        writeln(2, "throw errorCode;");
        writeln(1, "}");
        writeln("}");
        writeln();
        writeln("char* t"+module.name+"::ErrBuffer(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, ""+module.name+"SnapErrBuffer(getHandle(), Buffer, BufferLen-1);");
        writeln(1, "Buffer[BufferLen-1] = 0;");
        writeln(1, "return strtrim(Buffer);");
        writeln("}");
        writeln();
        writeln("char* t"+module.name+"::ErrorDesc(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, ""+module.name+"SnapErrorDesc(errorCode, Buffer, BufferLen-1);");
        writeln(1, "Buffer[BufferLen-1] = 0;");
        writeln(1, "return strtrim(Buffer);");
        writeln("}");
        writeln();
        writeln("char* t"+module.name+"::ErrorDescForNo(e"+module.name+" ErrorNo, char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, ""+module.name+"SnapErrorDesc(ErrorNo, Buffer, BufferLen-1);");
        writeln(1, "Buffer[BufferLen-1] = 0;");
        writeln(1, "return strtrim(Buffer);");
        writeln("}");
        writeln();
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          generateCClientImp(module, prototype);
        }
      }
      finally
      {
        writer.flush();
        outFile.close();
      }
    }
    catch (IOException e1)
    {
      outLog.println("Generate Procs IO Error");
    }
    catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }
  /**
  * Generates the prototypes defined
  */
  static int n=0;
  public static void generateCSwaps(Module module, Prototype prototype, Field field, Operation op)
  {
    String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "", w6 = "";
    if (field.needsSwap()
    ||  field.isStruct(module))
    {
      if (field.type.reference == Type.BYPTR
      ||  field.type.reference == Type.BYREFPTR)
      {
        w3 = "->";
        w4 = "*";
      }
      if (op != null)
      {
        Field opField = prototype.getParameter(op.name);
        if (opField != null
        && (opField.type.reference == Type.BYPTR
        ||  opField.type.reference == Type.BYREFPTR))
          w5 = "*";
        n++;
        writeln(1, ""+w2+"{for (int i_"+n+" = 0; i_"+n+" < "+w5+op.name+"; i_"+n+"++)");
        w1 = w1 + "[i_"+n+"]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
        w6 = w6+"}";
      }
      for (int j=0; j < field.type.arraySizes.size(); j++)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(j);
        n++;
        writeln(2, ""+w2+"{for (int i_"+n+"_"+j+" = 0; i_"+n+"_"+j
                        + " < " + integer.intValue()
                        + "; i_"+n+"_"+j+"++)");
        w1 = w1 + "[i_"+n+"_"+j+"]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
        w6 = w6+"}";
      }
    }
    if (field.isStruct(module))
      writeln(2, ""+w2+field.name+w1+w3+"Swaps();"+w6);
    else if (field.needsSwap())
      writeln(2, ""+w2+"SwapBytes("+w4+field.name+w1+");"+w6);
    else if (field.type.typeof == Type.USERTYPE)
    {
      //Toolkit.getDefaultToolkit().beep();
      errLog.println("Warning: "+prototype.name+" "+field.name+" is of UserType and may require swapping.");
    }
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateCClientImp(Module module, Prototype prototype)
  {
    String w1 = "";
    write(prototype.type.cDef("t"+module.name+"::"+prototype.name, false)+"(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(w1 + parameter.type.cDef(parameter.name, false));
      w1 = ", ";
    }
    writeln(")");
    writeln("{");
    if (prototype.type.typeof != Type.VOID
    ||  prototype.type.reference == Type.BYPTR)
      writeln(1, ""+prototype.type.cName(false)+" Result;");
    write("  errorCode = "+module.name+"Snap" + prototype.name+"(getHandle()");
    write(", "+prototype.signature(true));
    if (prototype.type.typeof != Type.VOID
    ||  prototype.type.reference == Type.BYPTR)
      write(", Result");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(", "+parameter.name);
    }
    writeln(");");
    writeln(1, "if (errorCode != "+module.name.toUpperCase()+"_OK)");
    writeln(2, "throw errorCode;");
    if (prototype.type.typeof != Type.VOID
    ||  prototype.type.reference == Type.BYPTR)
      writeln(1, "return Result;");
    writeln("}");
    writeln();
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateCClient(Module module, Prototype prototype, int no)
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
    if (prototype.type.reference == Type.BYVAL
    &&  prototype.type.typeof != Type.VOID)
      hasReturn = true;
    write("extern \"C\" DLLEXPORT_STDCALL(e"+module.name+") "+module.name+"Snap" + prototype.name+"(int32 Handle, int32 Signature");
    if (hasReturn)
        write(", "+prototype.type.cName(false)+"& Result");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(", " + parameter.type.cDef(parameter.name, false));
    }
    writeln(")");
    writeln("{");
    writeln(1, "if (Signature != "+prototype.signature(true)+")");
    writeln(2, "return "+module.name.toUpperCase()+"_INV_SIGNATURE;");
    writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
    writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
    writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
    writeln(1, "try");
    writeln(1, "{");
    if (alignForSun == true)
      writeln(2, "snapCB->sendBuffLen = alignData(4);");
    else
      writeln(2, "snapCB->sendBuffLen = 4;");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action input = (Action) prototype.inputs.elementAt(i);
      String opPtr="";
      Operation op = input.sizeOperation();
      if (op !=null)
      {
         Field opField = prototype.getParameter(op.name);
         if (opField != null && (opField.type.reference == Type.BYPTR || opField.type.reference == Type.BYREFPTR))
           opPtr = "*";
      }
      Field field  = input.getParameter(prototype);
      if (field == null)
      {
        writeln("#error "+input.name+" is an undefined input parameter");
        errLog.println("#error "+input.name+" is an undefined input parameter");
        continue;
      }
      if (field.type.reference == Type.BYPTR
      ||  field.type.reference == Type.BYREFPTR)
      {
        if (input.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            if (alignForSun == true)
              writeln(2, "snapCB->sendBuffLen += alignData(alignData(sizeof(int32))+strlen("+field.name+")+1);");
            else
              writeln(2, "snapCB->sendBuffLen += (sizeof(int32)+strlen("+field.name+")+1);");
          }
          else
          {
            if (alignForSun == true)
              writeln(2, "snapCB->sendBuffLen += alignData(sizeof(*"+field.name+"));");
            else
              writeln(2, "snapCB->sendBuffLen += sizeof(*"+field.name+");");
          }
        }
        else
        {
          if (alignForSun == true)
            writeln(2, "snapCB->sendBuffLen += alignData(alignData(sizeof(int32))+"+opPtr+op.name+"*sizeof(*"+field.name+"));");
          else
            writeln(2, "snapCB->sendBuffLen += (sizeof(int32)+"+opPtr+op.name+"*sizeof(*"+field.name+"));");
        }
      }
      else
      {
        if (input.hasSize() == false)
        {
          if (alignForSun == true)
            writeln(2, "snapCB->sendBuffLen += alignData(sizeof("+field.name+"));");
          else
            writeln(2, "snapCB->sendBuffLen += sizeof("+field.name+");");
        }
        else
        {
          writeln("#error "+field.name+" is not a pointer parameter but has a size");
          errLog.println("#error "+field.name+" is not a pointer parameter but has a size");
        }
      }
    }
    writeln(2, "snapCB->sendBuff = new char[snapCB->sendBuffLen];");
    writeln(2, "char* ip = snapCB->sendBuff;");
    String w1 = "";
    if (prototype.inputs.size() == 0)
      w1 = "// ";
    writeln(2, "*(int32*)ip = Signature;");
    writeln(2, "SwapBytes(*(int32*)ip);");
    if (alignForSun == true)
      writeln(2, ""+w1+"ip += alignData(sizeof(int32));");
    else
      writeln(2, ""+w1+"ip += sizeof(int32);");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      if (i+1 == prototype.inputs.size())
        w1 = "// ";
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field  = input.getParameter(prototype);
      if (field == null)
        continue;
      String opPtr="";
      Operation op = input.sizeOperation();
      if (op !=null)
      {
         Field opField = prototype.getParameter(op.name);
         if (opField != null && (opField.type.reference == Type.BYPTR || opField.type.reference == Type.BYREFPTR))
           opPtr = "*";
      }
      generateCSwaps(module, prototype, field, op);
      if (field.type.reference == Type.BYPTR
      ||  field.type.reference == Type.BYREFPTR)
      {
        if (input.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln(2, "*(int32*)ip = (strlen("+field.name+")+1);");
            writeln(2, "SwapBytes(*(int32*)ip);");
            if (alignForSun == true)
              writeln(2, "ip += alignData(sizeof(int32));");
            else
              writeln(2, "ip += sizeof(int32);");
            writeln(2, "memcpy(ip, "+field.name+", (int32)strlen("+field.name+")+1);");
            if (alignForSun == true)
              writeln(2, ""+w1+"ip += alignData(strlen("+field.name+")+1);");
            else
              writeln(2, ""+w1+"ip += (strlen("+field.name+")+1);");
          }
          else
          {
            writeln(2, "memcpy(ip, (void*)"+field.name+", (int32)sizeof(*"+field.name+"));");
            if (alignForSun == true)
              writeln(2, ""+w1+"ip += alignData(sizeof(*"+field.name+"));");
            else
              writeln(2, ""+w1+"ip += sizeof(*"+field.name+");");
          }
        }
        else
        {
          if (field.type.typeof != Type.CHAR)
            submittals.addElement(field);
          writeln(2, "*(int32*)ip = ("+opPtr+op.name+"*sizeof(*"+field.name+"));");
          writeln(2, "SwapBytes(*(int32*)ip);");
          if (alignForSun == true)
            writeln(2, "ip += alignData(sizeof(int32));");
          else
            writeln(2, "ip += sizeof(int32);");
          writeln(2, "memcpy(ip, (void*)"+field.name+", (int32)("+opPtr+op.name+"*sizeof(*"+field.name+")));");
          if (alignForSun == true)
            writeln(2, ""+w1+"ip += alignData(int32("+opPtr+op.name+"*sizeof(*"+field.name+")));");
          else
            writeln(2, ""+w1+"ip += (int32)("+opPtr+op.name+"*sizeof(*"+field.name+"));");
        }
      }
      else
      {
        writeln(2, "memcpy(ip, (char*)&"+field.name+", (int32)sizeof("+field.name+"));");
        if (alignForSun == true)
          writeln(2, ""+w1+"ip += alignData(sizeof("+field.name+"));");
        else
          writeln(2, ""+w1+"ip += sizeof("+field.name+");");
      }
      generateCSwaps(module, prototype, field, op);
    }
    if (prototype.message.length() > 0)
      w1 = prototype.message;
    else
      w1 = ""+no;
    writeln(2, "snapCB->RPC->Call("+w1+", snapCB->sendBuff, snapCB->sendBuffLen);");
    writeln(2, "delete [] snapCB->sendBuff;");
    writeln(2, "snapCB->sendBuff = 0;");
    boolean hasRX     = false;
    w1 = "// ";
    if (prototype.outputs.size() > 0)
      w1 = "";
    if (prototype.outputs.size() > 0 || hasReturn)
    {
      hasRX = true;
      writeln(2, "if (snapCB->RPC->RxSize())");
      writeln(2, "{");
      writeln(3, "ip = (char*)snapCB->RPC->RxBuffer();");
      if (hasReturn)
      {
        writeln(3, "memcpy(&Result, ip, (int32)sizeof(Result));");
        writeln(3, "SwapBytes(Result);");
        if (alignForSun == true)
          writeln(3, ""+w1+"ip += alignData(sizeof(Result));");
        else
          writeln(3, ""+w1+"ip += sizeof(Result);");
      }
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        if (i+1 == prototype.outputs.size())
          w1 = "// ";
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field  = output.getParameter(prototype);
        if (field == null)
        {
          writeln("#error "+output.name+" is an undefined input parameter");
          errLog.println("#error "+output.name+" is an undefined input parameter");
          continue;
        }
        Operation op = output.sizeOperation();
        if (field.type.reference == Type.BYPTR
        ||  field.type.reference == Type.BYREFPTR)
        {
          if (output.hasSize() == false)
          {
            if (field.type.typeof == Type.CHAR)
            {
              writeln("#error "+output.name+" unsized chars cannot be used as output");
              errLog.println("#error "+output.name+" unsized chars cannot be used as output");
              continue;
            }
            writeln(3, "memcpy("+field.name+", ip, sizeof(*"+field.name+"));");
            if (alignForSun == true)
              writeln(3, ""+w1+"ip += alignData(sizeof(*"+field.name+"));");
            else
              writeln(3, ""+w1+"ip += sizeof(*"+field.name+");");
          }
          else
          {
            if (field.type.typeof != Type.CHAR)
              retrievals.addElement(field);
            writeln(3, "snapCB->recvSize = *(int32*)ip;");
            writeln(3, "SwapBytes(snapCB->recvSize);");
            if (alignForSun == true)
              writeln(3, "ip += alignData(sizeof(int32));");
            else
              writeln(3, "ip += sizeof(int32);");
            if (field.type.reference == Type.BYREFPTR)
            {
              String s = prototype.getOutputSizeName(field.name);
              Field sf = prototype.getParameter(s);
              String w = "";
              if (sf.type.reference == Type.BYPTR)
                w = "*";
              writeln(3, ""+field.name+" = new "+field.type.cName(false)+"["+w+s+"];");
            }
            writeln(3, "memcpy("+field.name+", ip, snapCB->recvSize);");
            if (alignForSun == true)
              writeln(3, ""+w1+"ip += alignData(snapCB->recvSize);");
            else
              writeln(3, ""+w1+"ip += snapCB->recvSize;");
          }
        }
        else
        {
          writeln(3, "memcpy(&"+field.name+", ip, sizeof("+field.name+"));");
          if (alignForSun == true)
            writeln(3, ""+w1+"ip += alignData(sizeof("+field.name+"));");
          else
            writeln(3, ""+w1+"ip += sizeof("+field.name+");");
        }
        generateCSwaps(module, prototype, field, op);
      }
    }
    if (hasRX)
    {
      writeln(3, "snapCB->RPC->RxFree();");
      writeln(2, "}");
    }
    writeln(2, "return (e"+module.name+")snapCB->RPC->ReturnCode();");
    writeln(1, "}");
    writeln(1, "catch(xCept &x)");
    writeln(1, "{");
    writeln(2, "snapCB->RPC->ErrBuffer(x.ErrorStr());");
    writeln(2, "return "+module.name.toUpperCase()+"_SNAP_ERROR;");
    writeln(1, "}");
    writeln(1, "catch(...)");
    writeln(1, "{");
    writeln(2, "return "+module.name.toUpperCase()+"_SNAP_ERROR;");
    writeln(1, "}");
    writeln("}");
    writeln();
    if (submittals.size() > 0)
    {
      for (int i = 0; i < submittals.size(); i++)
      {
        Field parameter = (Field) submittals.elementAt(i);
        writeln("extern \"C\" DLLEXPORT_STDCALL(void) "+module.name+"Snap"+prototype.name+parameter.name+"Prepare(int32 Handle, int32 Size)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return;");
        writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
        //writeln(1, "if(snapCB->"+prototype.name+parameter.name+")");
        //writeln(2, "delete [] snapCB->"+prototype.name+parameter.name+";");
        writeln(1, "snapCB->"+prototype.name+parameter.name+" = new "
                            +parameter.type.cName(false)+" [Size];");
        allocated.addElement(parameter.name);
        writeln("}");
        writeln();
        write("extern \"C\" DLLEXPORT_STDCALL(int32) "+module.name+"Snap"+prototype.name+parameter.name+"Fill(int32 Handle");
        write(", " + parameter.type.cDef("Rec", false));
        writeln(", int32 Index)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return 0;");
        writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "snapCB->"+prototype.name+parameter.name+"[Index] = *Rec;");
        writeln(1, "return 1;");
        writeln("}");
        writeln();
      }
    }
    if (retrievals.size() > 0 || submittals.size() > 0)
    {
      write("extern \"C\" DLLEXPORT_STDCALL(e"+module.name+") "+module.name+"Snap" + prototype.name + "Start(int32 Handle, int32 Signature");
      if (hasReturn)
        write(", "+prototype.type.cName(false)+"& Result");
      for (int i = 0; i < prototype.parameters.size(); i++)
      {
        Field parameter = (Field) prototype.parameters.elementAt(i);
        if (parameter.type.typeof != Type.CHAR
        && (parameter.type.reference == Type.BYPTR
        ||  parameter.type.reference == Type.BYREFPTR))
          if (prototype.hasOutputSize(parameter.name)
          ||  prototype.hasInputSize(parameter.name))
            continue;
        write(", " + parameter.type.cDef(parameter.name, false));
      }
      writeln(")");
      writeln("{");
      writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
      writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
      writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
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
          writeln("#error "+parameter.name+" size " + s + " not found");
          continue;
        }
        boolean alreadyDone = false;
        for (int j=0; j<allocated.size(); j++)
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
        writeln(1, "snapCB->"+prototype.name+parameter.name+" = new "
                        +parameter.type.cName(false)+"["+w+s+"];");
      }
      write("  e"+module.name+" result = "+module.name+"Snap"+prototype.name+"(Handle, Signature");
      if (hasReturn)
        write(", Result");
      for (int i = 0; i < prototype.parameters.size(); i++)
      {
        Field parameter = (Field) prototype.parameters.elementAt(i);
        if (parameter.type.typeof != Type.CHAR
        && (parameter.type.reference == Type.BYPTR
        ||  parameter.type.reference == Type.BYREFPTR)
        && (prototype.hasOutputSize(parameter.name)
        ||  prototype.hasInputSize(parameter.name)))
          write(", snapCB->"+prototype.name+parameter.name);
        else
          write(", "+parameter.name);
      }
      writeln(");");
      for (int i = 0; i < submittals.size(); i++)
      {
        Field parameter = (Field) submittals.elementAt(i);
        writeln(1, "delete [] snapCB->"+prototype.name+parameter.name+";");
        writeln(1, "snapCB->"+prototype.name+parameter.name+" = 0;");
      }
//ccc
      writeln(1, "return result;");
      writeln("}");
      writeln();
    }
    if (retrievals.size() > 0)
    {
      for (int i = 0; i < retrievals.size(); i++)
      {
        Field parameter = (Field) retrievals.elementAt(i);
        write("extern \"C\" DLLEXPORT_STDCALL(int32) "+module.name+"Snap"+prototype.name+parameter.name+"Next(int32 Handle");
        write(", " + parameter.type.cDefRefPAsP("Rec", false));
        writeln(", int32 Index)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return 0;");
        writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "*Rec = snapCB->"+prototype.name+parameter.name+"[Index];");
        writeln(1, "return 1;");
        writeln("}");
        writeln();
        writeln("extern \"C\" DLLEXPORT_STDCALL(void) "+module.name+"Snap"+prototype.name+parameter.name+"Done(int32 Handle)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return;");
        writeln(1, "t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "if(snapCB->"+prototype.name+parameter.name+")");
        writeln(2, "delete [] snapCB->"+prototype.name+parameter.name+";");
        writeln(1, "snapCB->"+prototype.name+parameter.name+" = 0;");
        writeln("}");
        writeln();
      }
    }
  }
}
