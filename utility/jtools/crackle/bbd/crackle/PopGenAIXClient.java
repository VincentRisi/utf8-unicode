package bbd.crackle;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

public class PopGenAIXClient extends Generator
{
  public static String description()
  {
    return "Generate AIX(Unix) Client Code";
  }
  public static String documentation()
  {
    return "Generate AIX(Unix) Client Code";
  }
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
        generate(module, "", outLog);
      }
      outLog.flush();
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    indent_size = 2;
    errLog = outLog;
    outLog.println(module.name+" version "+module.version);
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
        writeln("CC = bcc32");
        writeln();
        writeln("INCS = -I..;s:\\bc5\\include");
        writeln();
        writeln("LIBS = \\");
        writeln("\tf:\\jenga\\lib\\libsnap.lib \\");
        writeln();
        writeln("CFLAGS = -H- -d -v -y -WD");
        writeln();
        writeln(".cpp.obj:");
        writeln("\t$(CC) -c $(CFLAGS) $(INCS) $<");
        writeln();
        writeln(module.name.toUpperCase()+" =\\");
        writeln("\t"+module.name.toLowerCase()+"Client.obj");
        writeln();
        writeln(module.name.toLowerCase()+".dll: $("+module.name.toUpperCase()+")");
        writeln("\t$(CC) $(CFLAGS) -e"+module.name.toLowerCase()+".dll -Ls:\\bc5\\lib @&&|");
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
      outLog.println("Code: "+output+module.name.toLowerCase()+"client.h");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"client.h");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln("#ifndef _"+module.name+"CLIENT_H_");
        writeln("#define _"+module.name+"CLIENT_H_");
        //writeln("#pragma option -b");
        //writeln();
        writeln("#include \"machine.h\"");
        writeln("#include \"popgen.h\"");
        writeln();
        writeln("extern char *"+module.name+"Version;");
        writeln("extern int32 "+module.name+"Signature;");
        PopGen.generateCExterns(module, writer);
        writeln("#pragma pack (push,1)");
        PopGen.generateCStructs(module, writer, true);
        writeln("#pragma pack (pop)");
        writeln();
        writeln("class t"+module.name);
        writeln("{");
        writeln("  int32 Handle;");
        writeln("  e"+module.name+" errorCode;");
        writeln("  bool loggedOn;");
        writeln("  char fErrBuffer[4096];");
        writeln("  char fErrorDesc[256];");
        writeln("public:");
        writeln("  t"+module.name+"() {loggedOn = false;}");
        writeln("  ~t"+module.name+"() {if (loggedOn) Logoff();}");
        writeln("  int32 getHandle();");
        writeln("  void Logon(char* UserID, char* Service, char* Host, int32 Timeout=150000);");
        writeln("  void Logoff();");
        writeln("  char* ErrBuffer() {return ErrBuffer(fErrBuffer, sizeof(fErrBuffer));}");
        writeln("  char* ErrorDesc() {return ErrorDesc(fErrorDesc, sizeof(fErrorDesc));}");
        writeln("  char* ErrBuffer(char *Buffer, int32 BufferLen);");
        writeln("  char* ErrorDesc(char *Buffer, int32 BufferLen);");
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
      outLog.println("Code: "+output+module.name.toLowerCase()+"Client.cpp");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"Client.cpp");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln();
        writeln("#include \"ti.h\"");
        writeln("#define _AIXCLIENT_");
        writeln();
        writeln("char *"+module.name+"Version = "+module.version+";");
        writeln("int32 "+module.name+"Signature = "+module.signature+";");
        writeln();
        writeln("#include <xstring.h>");
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln();
        writeln("#include \"popgen.h\"");
        writeln("#if defined(M_AIX)");
        writeln("#include \"snaprpcaixclient.h\"");
        writeln("#elif defined(M_GNU)");
        writeln("#include \"snaprpcgnuclient.h\"");
        writeln("#else");
        writeln("#include \"snaprpcntclient.h\"");
        writeln("#endif");
        writeln("#include \"handles.h\"");
        writeln();
        writeln("#include \""+module.name.toLowerCase()+"client.h\"");
        writeln();
        writeln("const char *"+module.name+"Errors[] = ");
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
          writeln("char *"+module.name+table.name+"[] = ");
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
        writeln("struct t"+module.name+"SnapCB");
        writeln("{");
        writeln("  tRPCClient* RPC;");
        writeln("  char *sendBuff;");
        writeln("  int32 sendBuffLen;");
        writeln("  int32 recvSize;");
        writeln("  e"+module.name+" result;");
        writeln("  int32 RC;");
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
                  writeln("  "+parameter.type.cDefRefPAsP(prototype.name+parameter.name, false)+";");
                else
                  writeln("  "+parameter.type.cDef(prototype.name+parameter.name, false)+";");
              }
            }
          }
        }
        writeln("  t"+module.name+"SnapCB(tString UserID, tString Service, tString Host, int32 Timeout)");
        w1 = ":";
        // There generally wont be the same retrieval and submittal,
        // even if there is it should not really matter much. If it
        // does then we will change this code.
        writeln("  {");
        writeln("    RPC = new tRPCClient(UserID, Host, Service, Timeout);");
        writeln("  }");
        writeln("  ~t"+module.name+"SnapCB()");
        writeln("  {");
        writeln("    delete RPC;");
        writeln("  }");
        writeln("};");
        writeln();
        writeln("const int CookieStart = 1719;");
        writeln("const int NoCookies = 32;");
        writeln("static tHandle<t"+module.name+"SnapCB*, CookieStart, NoCookies> "+module.name+"CBHandle;");
        writeln();
        writeln("e"+module.name+" "+module.name+"SnapLogon(int32* Handle, char* UserID, char* Service, char* Host, int32 Timeout)");
        writeln("{");
        writeln("  try");
        writeln("  {");
        writeln("    *Handle = "+module.name+"CBHandle.Create(new t"+module.name+"SnapCB(UserID, Service, Host, Timeout));");
        writeln("    return "+module.name.toUpperCase()+"_OK;");
        writeln("  }");
        writeln("  catch (xCept &x)");
        writeln("  {");
        writeln("    *Handle = -1;");
        writeln("    return "+module.name.toUpperCase()+"_SNAP_ERROR;");
        writeln("  }");
        writeln("}");
        writeln();
        writeln("e"+module.name+" "+module.name+"SnapLogoff(int32* Handle)");
        writeln("{");
        writeln("  if (*Handle < CookieStart || *Handle >= CookieStart+NoCookies)");
        writeln("    return "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln("  "+module.name+"CBHandle.Release(*Handle);");
        writeln("  *Handle = -1;");
        writeln("  return "+module.name.toUpperCase()+"_OK;");
        writeln("}");
        writeln();
        writeln("e"+module.name+" "+module.name+"SnapErrBuffer(int32 Handle, char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln("    return "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln("  t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
        writeln("  if (snapCB->RPC->ErrSize())");
        writeln("    strncpy(Buffer, snapCB->RPC->ErrBuffer(), BufferLen);");
        writeln("  else");
        writeln("    strcpy(Buffer, \"\");");
        writeln("  for(int i = strlen(Buffer); i < BufferLen; i++)");
        writeln("    Buffer[i]  = ' ';");
        writeln("  return "+module.name.toUpperCase()+"_OK;");
        writeln("}");
        writeln();
        writeln("void "+module.name+"SnapErrorDesc(e"+module.name+" RC, char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln("  char W1[20]=\"\";");
        writeln("  if (RC < "+module.name.toUpperCase()+"_OK");
        w1 = "RC";
        if (module.messageBase > 0)
        {
          writeln("  || (RC > "+module.name.toUpperCase()+"_OK && RC < "+(module.messageBase)+")");
          w1 = "(RC?RC-("+module.messageBase+"-1):0)";
        }
        writeln("  ||  RC > "+module.name.toUpperCase()+"_LAST_LAST)");
        writeln("  {");
        writeln("    sprintf(W1, \" (RC=%d)\", RC);");
        writeln("    RC = "+module.name.toUpperCase()+"_LAST_LAST;");
        writeln("  }");
        writeln("  int n = (int)"+w1+";");
        writeln("  strncpy(Buffer, "+module.name+"Errors[n], BufferLen-(strlen(W1)+1));");
        writeln("  if (RC == "+module.name.toUpperCase()+"_LAST_LAST)");
        writeln("    strcat(Buffer, W1);");
        writeln("  for(int i = strlen(Buffer); i < BufferLen; i++)");
        writeln("    Buffer[i]  = ' ';");
        writeln("}");
        writeln();
        writeln("void "+module.name+"SnapVersion(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln("  strncpy(Buffer, "+module.name+"Version, BufferLen);");
        writeln("  for(int i = strlen(Buffer); i < BufferLen; i++)");
        writeln("    Buffer[i]  = ' ';");
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
        writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln("    throw "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln("  return Handle;");
        writeln("}");
        writeln();
        writeln("void t"+module.name+"::Logon(char* UserID, char* Service, char* Host, int32 Timeout)");
        writeln("{");
        writeln("  errorCode = "+module.name+"SnapLogon(&Handle, UserID, Service, Host, Timeout);");
        writeln("  if (errorCode != 0)");
        writeln("  {");
        writeln("    Handle = -1;");
        writeln("    throw errorCode;");
        writeln("  }");
        writeln("  loggedOn = true;");
        writeln("}");
        writeln();
        writeln("void t"+module.name+"::Logoff()");
        writeln("{");
        writeln("  if (loggedOn == false)");
        writeln("    return;");
        writeln("  loggedOn = false;");
        writeln("  errorCode = "+module.name+"SnapLogoff(&Handle);");
        writeln("  if (errorCode != 0)");
        writeln("  {");
        writeln("    Handle = -1;");
        writeln("    throw errorCode;");
        writeln("  }");
        writeln("}");
        writeln();
        writeln("char* t"+module.name+"::ErrBuffer(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln("  "+module.name+"SnapErrBuffer(getHandle(), Buffer, BufferLen-1);");
        writeln("  Buffer[BufferLen-1] = 0;");
        writeln("  return strtrim(Buffer);");
        writeln("}");
        writeln();
        writeln("char* t"+module.name+"::ErrorDesc(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln("  "+module.name+"SnapErrorDesc(errorCode, Buffer, BufferLen-1);");
        writeln("  Buffer[BufferLen-1] = 0;");
        writeln("  return strtrim(Buffer);");
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
    String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "";
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
        writeln("  "+w2+"for (int i_"+n+" = 0; i_"+n+" < "+w5+op.name+"; i_"+n+"++)");
        w1 = w1 + "[i_"+n+"]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
      }
      for (int j=0; j < field.type.arraySizes.size(); j++)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(j);
        n++;
        writeln("    "+w2+"for (int i_"+n+"_"+j+" = 0; i_"+n+"_"+j
                        + " < " + integer.intValue()
                        + "; i_"+n+"_"+j+"++)");
        w1 = w1 + "[i_"+n+"_"+j+"]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
      }
    }
    if (field.isStruct(module))
      writeln("    "+w2+field.name+w1+w3+"Swaps();");
    else if (field.needsSwap())
      writeln("    "+w2+"SwapBytes("+w4+field.name+w1+");");
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
      writeln("  "+prototype.type.cName(false)+" Result;");
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
    writeln("  if (errorCode != "+module.name.toUpperCase()+"_OK)");
    writeln("    throw errorCode;");
    if (prototype.type.typeof != Type.VOID
    ||  prototype.type.reference == Type.BYPTR)
      writeln("  return Result;");
    writeln("}");
    writeln();
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateCClient(Module module, Prototype prototype, int no)
  {
    boolean hasReturn = false;
    String opPtr = "";
    if (prototype.type.reference != Type.BYVAL)
    {
      writeln("#error Only non pointers are allowed as return values");
      errLog.println("#error Only non pointers are allowed as return values");
    }
    if (prototype.type.reference == Type.BYVAL
    &&  prototype.type.typeof != Type.VOID)
      hasReturn = true;
    write("e"+module.name+" "+module.name+"Snap" + prototype.name+"(int32 Handle, int32 Signature");
    if (hasReturn)
        write(", "+prototype.type.cName(false)+"& Result");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(", " + parameter.type.cDef(parameter.name, false));
    }
    writeln(")");
    writeln("{");
    writeln("  if (Signature != "+prototype.signature(true)+")");
    writeln("    return "+module.name.toUpperCase()+"_INV_SIGNATURE;");
    writeln("  if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
    writeln("    return "+module.name.toUpperCase()+"_INV_COOKIE;");
    writeln("  t"+module.name+"SnapCB* snapCB = "+module.name+"CBHandle.Use(Handle);");
    writeln("  try");
    writeln("  {");
    writeln("    snapCB->sendBuffLen = 4;");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action action = (Action) prototype.inputs.elementAt(i);
      Operation op = action.sizeOperation();
      opPtr = "";
      if (op != null) 
      {
        String name = action.getSizeName();
        Field opName = prototype.getParameter(name);
        if (opName != null && opName.type.reference == Type.BYPTR)
          opPtr = "*";
      }
      //  if (op.byPtr(prototype) == true)
      //     opPtr = "*";
      Field field  = action.getParameter(prototype);
      if (field == null)
      {
        writeln("#error "+action.name+" is an undefined input parameter");
        errLog.println("#error "+action.name+" is an undefined input parameter");
        continue;
      }
      if (field.type.reference == Type.BYPTR
      ||  field.type.reference == Type.BYREFPTR)
      {
        if (action.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
            writeln("    snapCB->sendBuffLen += (sizeof(int32)+strlen("+field.name+")+1);");
          else
            writeln("    snapCB->sendBuffLen += sizeof(*"+field.name+");");
        }
        else
        {
          writeln("    snapCB->sendBuffLen += (sizeof(int32)+"+opPtr+op.name+"*sizeof(*"+field.name+"));");
        }
      }
      else
      {
        if (action.hasSize() == false)
          writeln("    snapCB->sendBuffLen += sizeof("+field.name+");");
        else
        {
          writeln("#error "+field.name+" is not a pointer parameter but has a size");
          errLog.println("#error "+field.name+" is not a pointer parameter but has a size");
        }
      }
    }
    writeln("    snapCB->sendBuff = new char[snapCB->sendBuffLen];");
    writeln("    char* ip = snapCB->sendBuff;");
    String w1 = "";
    if (prototype.inputs.size() == 0)
      w1 = "// ";
    writeln("    *(int32*)ip = Signature;");
    writeln("    SwapBytes(*(int32*)ip);");
    writeln("    "+w1+"ip += sizeof(int32);");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      if (i+1 == prototype.inputs.size())
        w1 = "// ";
      Action action = (Action) prototype.inputs.elementAt(i);
      Field field  = action.getParameter(prototype);
      if (field == null)
        continue;
      Operation op = action.sizeOperation();
      if (op != null)
      {
        String name = action.getSizeName();
        Field opName = prototype.getParameter(name);
        if (opName != null && opName.type.reference == Type.BYPTR)
          opPtr = "*";
      }
      generateCSwaps(module, prototype, field, op);
      if (field.type.reference == Type.BYPTR
      ||  field.type.reference == Type.BYREFPTR)
      {
        if (action.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln("    *(int32*)ip = (strlen("+field.name+")+1);");
            writeln("    SwapBytes(*(int32*)ip);");
            writeln("    ip += sizeof(int32);");
            writeln("    memcpy(ip, "+field.name+", (int32)strlen("+field.name+")+1);");
            writeln("    "+w1+"ip += (strlen("+field.name+")+1);");
          }
          else
          {
            writeln("    memcpy(ip, (void*)"+field.name+", (int32)sizeof(*"+field.name+"));");
            writeln("    "+w1+"ip += sizeof(*"+field.name+");");
          }
        }
        else
        {
          writeln("    *(int32*)ip = ("+opPtr+op.name+"*sizeof(*"+field.name+"));");
          writeln("    SwapBytes(*(int32*)ip);");
          writeln("    ip += sizeof(int32);");
          writeln("    memcpy(ip, (void*)"+field.name+", (int32)("+opPtr+op.name+"*sizeof(*"+field.name+")));");
          writeln("    "+w1+"ip += (int32)("+opPtr+op.name+"*sizeof(*"+field.name+"));");
        }
      }
      else
      {
        writeln("    memcpy(ip, (char*)&"+field.name+", (int32)sizeof("+field.name+"));");
        writeln("    "+w1+"ip += sizeof("+field.name+");");
      }
      generateCSwaps(module, prototype, field, op);
    }
    if (prototype.message.length() > 0)
      w1 = prototype.message;
    else
      w1 = ""+no;
    writeln("    snapCB->RPC->Call("+w1+", snapCB->sendBuff, snapCB->sendBuffLen);");
    writeln("    delete [] snapCB->sendBuff;");
    writeln("    snapCB->sendBuff = 0;");
    boolean hasRX     = false;
    w1 = "// ";
    if (prototype.outputs.size() > 0)
      w1 = "";
    if (prototype.outputs.size() > 0 || hasReturn)
    {
      hasRX = true;
      writeln("    ip = (char*)snapCB->RPC->RxBuffer();");
      if (hasReturn)
      {
        writeln("    memcpy(&Result, ip, (int32)sizeof(Result));");
        writeln("    SwapBytes(Result);");
        writeln("    "+w1+"ip += sizeof(Result);");
      }
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        if (i+1 == prototype.outputs.size())
          w1 = "// ";
        Action outAction = (Action) prototype.outputs.elementAt(i);
        Field field  = outAction.getParameter(prototype);
        if (field == null)
        {
          writeln("#error "+outAction.name+" is an undefined input parameter");
          errLog.println("#error "+outAction.name+" is an undefined input parameter");
          continue;
        }
        Operation op = outAction.sizeOperation();
        if (op != null)
        {
          String name = outAction.getSizeName();
          Field opName = prototype.getParameter(name);
          if (opName != null && opName.type.reference == Type.BYPTR)
            opPtr = "*";
        }
        if (field.type.reference == Type.BYPTR
        ||  field.type.reference == Type.BYREFPTR)
        {
          if (outAction.hasSize() == false)
          {
            if (field.type.typeof == Type.CHAR)
            {
              writeln("#error "+outAction.name+" unsized chars cannot be used as output");
              errLog.println("#error "+outAction.name+" unsized chars cannot be used as output");
              continue;
            }
            writeln("    memcpy("+field.name+", ip, sizeof(*"+field.name+"));");
            writeln("    "+w1+"ip += sizeof(*"+field.name+");");
          }
          else
          {
            writeln("    snapCB->recvSize = *(int32*)ip;");
            writeln("    SwapBytes(snapCB->recvSize);");
            writeln("    ip += sizeof(int32);");
            if (field.type.reference == Type.BYREFPTR)
            {
              String s = prototype.getOutputSizeName(field.name);
              Field sf = prototype.getParameter(s);
              String w = "";
              if (sf.type.reference == Type.BYPTR)
                w = "*";
              writeln("    "+field.name+" = new "+field.type.cName(false)+"["+w+s+"];");
            }
            writeln("    memcpy("+field.name+", ip, snapCB->recvSize);");
            writeln("    "+w1+"ip += snapCB->recvSize;");
          }
        }
        else
        {
          writeln("    memcpy(&"+field.name+", ip, sizeof("+field.name+"));");
          writeln("    "+w1+"ip += sizeof("+field.name+");");
        }
        generateCSwaps(module, prototype, field, op);
      }
    }
    if (hasRX)
      writeln("    snapCB->RPC->RxFree();");
    writeln("    return (e"+module.name+")snapCB->RPC->ReturnCode();");
    writeln("  }");
    writeln("  catch(xCept &x)");
    writeln("  {");
    writeln("    snapCB->RPC->ErrBuffer(x.ErrorStr());");
    writeln("    return "+module.name.toUpperCase()+"_SNAP_ERROR;");
    writeln("  }");
    writeln("  catch(...)");
    writeln("  {");
    writeln("    return "+module.name.toUpperCase()+"_SNAP_ERROR;");
    writeln("  }");
    writeln("}");
    writeln();
  }
}
