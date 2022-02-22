package bbd.crackle.ubi;

import bbd.crackle.*;
import bbd.crackle.Module;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class PopUbiClient extends Generator
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
  * - C Header for Client
  * - C Client
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
    indent_size = 4;
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
      outLog.println("Code: "+output+module.name.toLowerCase()+"client.h");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"client.h");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln("#ifndef _"+module.name+"CLIENT_H_");
        writeln("#define _"+module.name+"CLIENT_H_");
        writeln("#include \"machine.h\"");
        writeln();
        writeln("#include \"swapbytes.h\"");
        writeln();
        writeln("extern char *"+module.name+"Version;");
        writeln("extern int32 "+module.name+"Signature;");
        PopUbiGen.generateCExterns(module, writer);
        writeln();
        PopUbiGen.generateCStructs(module, writer, true);
        writeln();
        writeln("class T"+module.name+"Interface");
        writeln("{");
        writeln("public:");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          PopUbiGen.generateCInterface(module, prototype, writer);
        }
        writeln("};");
        writeln();
        writeln("class T" + module.name + " : public T" + module.name + "Interface");
        writeln("{");
        writeln("public:");
        writeln(1, "int32 Handle;");
        writeln(1, "e"+module.name+" errorCode;");
        writeln(1, "bool loggedOn;");
        writeln(1, "char fErrBuffer[4096];");
        writeln(1, "char fErrorDesc[256];");
        writeln("private:");
        writeln(1, "static T" + module.name + "* instance;");
        writeln("public:");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          PopUbiGen.generateCHeader(module, prototype, writer);
        }
        writeln(1, "static T" + module.name + "* Instance() {return instance;}");
        writeln(1, "T" + module.name + "() {loggedOn = false; instance=this;}");
        writeln(1, "~T"+module.name+"() {if (loggedOn) Logoff();}");
        writeln(1, "int32 getHandle();");
        writeln(1, "void Logon(char* Service, char* Host, int32 Timeout=150000);");
        writeln(1, "void Logoff();");
        writeln(1, "char* ErrBuffer() {return ErrBuffer(fErrBuffer, sizeof(fErrBuffer));}");
        writeln(1, "char* ErrorDesc() {return ErrorDesc(fErrorDesc, sizeof(fErrorDesc));}");
        writeln(1, "char* ErrorDescForNo(e"+module.name+" ErrorNo) { return ErrorDescForNo(ErrorNo, fErrorDesc, sizeof(fErrorDesc));}");
        writeln(1, "char* ErrorDescForNo(e"+module.name+" ErrorNo, char *Buffer, int32 BufferLen);");
        writeln(1, "char* ErrBuffer(char *Buffer, int32 BufferLen);");
        writeln(1, "char* ErrorDesc(char *Buffer, int32 BufferLen);");
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
  private static void generateCClient(Module module, String output, PrintWriter outLog)
  {
    try
    {
      String w1 = "";
      outLog.println("Code: "+output+module.name.toLowerCase()+"client.cpp");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"client.cpp");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#include \"machine.h\"");
        writeln("#include \"xcept.h\"");
        writeln("#include \"xstring.h\"");
        writeln();
        writeln("char *"+module.name+"Version = "+module.version+";");
        writeln("int32 "+module.name+"Signature = "+module.signature+";");
        writeln();
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln();
        writeln("#include \"swapbytes.h\"");
        writeln("#include \"ubiclient.h\"");
        writeln("#include \"handles.h\"");
        writeln();
        writeln("#include \""+module.name.toLowerCase()+"client.h\"");
        writeln();
        writeln("char *"+module.name+"Errors[] = ");
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
        if (alignForSun == true)
        {
          writeln("inline int32 alignData(int32 size)");
          writeln("{");
          writeln(1, "int32 n = size % 8;");
          writeln(1, "if (n > 0) size += (8-n);");
          writeln(1, "return size;");
          writeln("}");
          writeln();
        }
        writeln("struct T"+module.name+"UbiCB");
        writeln("{");
        writeln(1, "UbiClient* RPC;");
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
                  writeln(1, ""+parameter.type.cDefUbiRefPAsP(prototype.name+parameter.name)+";");
                else
                  writeln(1, ""+parameter.type.cDefUbi(prototype.name+parameter.name)+";");
              }
              if (prototype.hasOutputSize(parameter.name))
                retrievals.addElement(prototype.name+parameter.name);
              if (prototype.hasInputSize(parameter.name))
                submittals.addElement(prototype.name+parameter.name);
            }
          }
        }
        writeln(1, "T"+module.name+"UbiCB(char* Service, char* Host, int32 Timeout)");
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
        writeln(2, "RPC = new UbiClient(Host, Service, Timeout);");
        writeln(1, "}");
        writeln(1, "~T"+module.name+"UbiCB()");
        writeln(1, "{");
        writeln(2, "delete RPC;");
        writeln(1, "}");
        writeln("};");
        writeln();
        writeln("const int CookieStart = 2329;");
        writeln("const int NoCookies = 32;");
        writeln("static tHandle<T"+module.name+"UbiCB*, CookieStart, NoCookies> "+module.name+"CBHandle;");
        writeln();
        writeln("e"+module.name+" "+module.name+"SnapLogon(int32* Handle, char* Service, char* Host, int32 Timeout)");
        writeln("{");
        writeln(1, "try");
        writeln(1, "{");
        writeln(2, "*Handle = "+module.name+"CBHandle.Create(new T"+module.name+"UbiCB(Service, Host, Timeout));");
        writeln(2, "return "+module.name.toUpperCase()+"_OK;");
        writeln(1, "}");
        writeln(1, "catch (xCept &x)");
        writeln(1, "{");
        writeln(2, "*Handle = -1;");
        writeln(2, "return "+module.name.toUpperCase()+"_SNAP_ERROR;");
        writeln(1, "}");
        writeln("}");
        writeln();
        writeln("e"+module.name+" "+module.name+"SnapLogoff(int32* Handle)");
        writeln("{");
        writeln(1, "if (*Handle < CookieStart || *Handle >= CookieStart+NoCookies)");
        writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln(1, ""+module.name+"CBHandle.Release(*Handle);");
        writeln(1, "*Handle = -1;");
        writeln(1, "return "+module.name.toUpperCase()+"_OK;");
        writeln("}");
        writeln();
        writeln("void " + module.name + "SnapThrowOnError(int32 Handle, char *file=0, int line=0)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return;");
        writeln(1, "T" + module.name + "UbiCB* ubiCB = " + module.name + "CBHandle.Use(Handle);");
        writeln(1, "if (ubiCB->RPC->ErrSize())");
        writeln(1, "{");
        writeln(2, "xUbiException error(file, line, ubiCB->RPC->ErrBuffer());");
        writeln(2, "ubiCB->RPC->ErrFree();");
        writeln(2, "throw error;");
        writeln(1, "}");
        writeln(1, "return;");
        writeln("}");
        writeln();
        writeln("e" + module.name + " " + module.name + "SnapErrBuffer(int32 Handle, char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln(1, "T"+module.name+"UbiCB* ubiCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "if (ubiCB->RPC->ErrSize())");
        writeln(2, "strncpy(Buffer, ubiCB->RPC->ErrBuffer(), BufferLen);");
        writeln(1, "else");
        writeln(2, "strcpy(Buffer, \"\");");
        writeln(1, "for(int32 i = strlen(Buffer); i < BufferLen; i++)");
        writeln(2, "Buffer[i]  = ' ';");
        writeln(1, "return "+module.name.toUpperCase()+"_OK;");
        writeln("}");
        writeln();
        writeln("void "+module.name+"SnapErrorDesc(e"+module.name+" RC, char *Buffer, int32 BufferLen)");
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
        writeln(1, "int32 n = (int32)"+w1+";");
        writeln(1, "strncpy(Buffer, "+module.name+"Errors[n], BufferLen-(strlen(W1)+1));");
        writeln(1, "if (RC == "+module.name.toUpperCase()+"_LAST_LAST)");
        writeln(2, "strcat(Buffer, W1);");
        writeln(1, "for(int32 i = strlen(Buffer); i < BufferLen; i++)");
        writeln(2, "Buffer[i]  = ' ';");
        writeln("}");
        writeln();
        writeln("void "+module.name+"SnapVersion(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, "strncpy(Buffer, "+module.name+"Version, BufferLen);");
        writeln(1, "for(int32 i = strlen(Buffer); i < BufferLen; i++)");
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
        writeln("T" + module.name + "* T" + module.name + "::instance;");
        writeln("int32 T"+module.name+"::getHandle()");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "throw "+module.name.toUpperCase()+"_INV_COOKIE;");
        writeln(1, "return Handle;");
        writeln("}");
        writeln();
        writeln("void T"+module.name+"::Logon(char* Service, char* Host, int32 Timeout)");
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
        writeln("void T"+module.name+"::Logoff()");
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
        writeln("char* T"+module.name+"::ErrBuffer(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, ""+module.name+"SnapErrBuffer(getHandle(), Buffer, BufferLen-1);");
        writeln(1, "Buffer[BufferLen-1] = 0;");
        writeln(1, "return strtrim(Buffer);");
        writeln("}");
        writeln();
        writeln("char* T"+module.name+"::ErrorDesc(char *Buffer, int32 BufferLen)");
        writeln("{");
        writeln(1, ""+module.name+"SnapErrorDesc(errorCode, Buffer, BufferLen-1);");
        writeln(1, "Buffer[BufferLen-1] = 0;");
        writeln(1, "return strtrim(Buffer);");
        writeln("}");
        writeln();
        writeln("char* T"+module.name+"::ErrorDescForNo(e"+module.name+" ErrorNo, char *Buffer, int32 BufferLen)");
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
  private static int n=0;
  private static void generateCSwaps(Module module, Prototype prototype, Field field, Operation op)
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
  private static void generateCClientImp(Module module, Prototype prototype)
  {
    String w1 = "";
    write(prototype.type.cDefUbi("T"+module.name+"::"+prototype.name)+"(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(w1 + parameter.type.cDefUbi(parameter.name));
      w1 = ", ";
    }
    writeln(")");
    writeln("{");
    if (prototype.type.typeof != Type.VOID
    ||  prototype.type.reference == Type.BYPTR)
      writeln(1, ""+prototype.type.cNameUbi()+" Result;");
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
    writeln(1, "" + module.name + "SnapThrowOnError(getHandle(), __FILE__, __LINE__);");
    writeln(1, "if (errorCode != " + module.name.toUpperCase() + "_OK)");
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
  private static void generateCClient(Module module, Prototype prototype, int no)
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
    write("e"+module.name+" "+module.name+"Snap" + prototype.name+"(int32 Handle, int32 Signature");
    if (hasReturn)
        write(", "+prototype.type.cNameUbi()+"& Result");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(", " + parameter.type.cDefUbi(parameter.name));
    }
    writeln(")");
    writeln("{");
    writeln(1, "if (Signature != "+prototype.signature(true)+")");
    writeln(2, "return "+module.name.toUpperCase()+"_INV_SIGNATURE;");
    writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
    writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
    writeln(1, "T"+module.name+"UbiCB* ubiCB = "+module.name+"CBHandle.Use(Handle);");
    writeln(1, "try");
    writeln(1, "{");
    if (alignForSun == true)
      writeln(2, "ubiCB->sendBuffLen = alignData(4);");
    else
      writeln(2, "ubiCB->sendBuffLen = 4;");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field  = input.getParameter(prototype);
      String opExtra = "";
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
              writeln(2, "ubiCB->sendBuffLen += alignData(alignData(sizeof(int32))+strlen(" + field.name + ")+1);");
            else
              writeln(2, "ubiCB->sendBuffLen += (sizeof(int32)+strlen(" + field.name + ")+1);");
          }
          else
          {
            if (alignForSun == true)
              writeln(2, "ubiCB->sendBuffLen += alignData(sizeof(*" + field.name + "));");
            else
              writeln(2, "ubiCB->sendBuffLen += sizeof(*" + field.name + ");");
          }
        }
        else
        {
          Operation op = input.sizeOperation();
          Field opField = prototype.getParameter(op.name);
          if (opField != null && opField.type.reference == Type.BYPTR)
            opExtra = "*";
          if (alignForSun == true)
            writeln(2, "ubiCB->sendBuffLen += alignData(alignData(sizeof(int32))+" + opExtra + op.name + "*sizeof(*" + field.name + "));");
          else
            writeln(2, "ubiCB->sendBuffLen += (sizeof(int32)+" + opExtra + op.name + "*sizeof(*" + field.name + "));");
        }
      }
      else
      {
        if (input.hasSize() == false)
        {
          if (alignForSun == true)
            writeln(2, "ubiCB->sendBuffLen += alignData(sizeof("+field.name+"));");
          else
            writeln(2, "ubiCB->sendBuffLen += sizeof("+field.name+");");
        }
        else
        {
          writeln("#error "+field.name+" is not a pointer parameter but has a size");
          errLog.println("#error "+field.name+" is not a pointer parameter but has a size");
        }
      }
    }
    writeln(2, "ubiCB->sendBuff = new char[ubiCB->sendBuffLen];");
    writeln(2, "char* ip = ubiCB->sendBuff;");
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
      Operation op = input.sizeOperation();
      generateCSwaps(module, prototype, field, op);
      if (field.type.reference == Type.BYPTR
      || field.type.reference == Type.BYREFPTR)
      {
        if (input.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln(2, "*(int32*)ip = (strlen(" + field.name + ")+1);");
            writeln(2, "SwapBytes(*(int32*)ip);");
            if (alignForSun == true)
              writeln(2, "ip += alignData(sizeof(int32));");
            else
              writeln(2, "ip += sizeof(int32);");
            writeln(2, "memcpy(ip, " + field.name + ", (int32)strlen(" + field.name + ")+1);");
            if (alignForSun == true)
              writeln(2, "" + w1 + "ip += alignData(strlen(" + field.name + ")+1);");
            else
              writeln(2, "" + w1 + "ip += (strlen(" + field.name + ")+1);");
          }
          else
          {
            writeln(2, "memcpy(ip, (void*)" + field.name + ", (int32)sizeof(*" + field.name + "));");
            if (alignForSun == true)
              writeln(2, "" + w1 + "ip += alignData(sizeof(*" + field.name + "));");
            else
              writeln(2, "" + w1 + "ip += sizeof(*" + field.name + ");");
          }
        }
        else
        {
          String opExtra = "";
          Field opField = prototype.getParameter(op.name);
          if (opField != null && opField.type.reference == Type.BYPTR)
            opExtra = "*";
          if (field.type.typeof != Type.CHAR)
            submittals.addElement(field);
          writeln(2, "*(int32*)ip = (" + opExtra + op.name + "*sizeof(*" + field.name + "));");
          writeln(2, "SwapBytes(*(int32*)ip);");
          if (alignForSun == true)
            writeln(2, "ip += alignData(sizeof(int32));");
          else
            writeln(2, "ip += sizeof(int32);");
          writeln(2, "memcpy(ip, (void*)" + field.name + ", (int32)(" + opExtra + op.name + "*sizeof(*" + field.name + ")));");
          if (alignForSun == true)
            writeln(2, "" + w1 + "ip += alignData(int32(" + opExtra + op.name + "*sizeof(*" + field.name + ")));");
          else
            writeln(2, "" + w1 + "ip += (int32)(" + opExtra + op.name + "*sizeof(*" + field.name + "));");
        }
      }
      else
      {
        writeln(2, "memcpy(ip, (char*)&" + field.name + ", (int32)sizeof(" + field.name + "));");
        if (alignForSun == true)
          writeln(2, "" + w1 + "ip += alignData(sizeof(" + field.name + "));");
        else
          writeln(2, "" + w1 + "ip += sizeof(" + field.name + ");");
      }
      generateCSwaps(module, prototype, field, op);
    }
    if (prototype.message.length() > 0)
      w1 = prototype.message;
    else
      w1 = ""+no;
    writeln(2, "ubiCB->RPC->Call("+w1+", ubiCB->sendBuff, ubiCB->sendBuffLen);");
    writeln(2, "delete [] ubiCB->sendBuff;");
    writeln(2, "ubiCB->sendBuff = 0;");
    boolean hasRX     = false;
    w1 = "// ";
    if (prototype.outputs.size() > 0)
      w1 = "";
    if (prototype.outputs.size() > 0 || hasReturn)
    {
      hasRX = true;
      writeln(2, "if (ubiCB->RPC->RxSize())");
      writeln(2, "{");
      writeln(3, "ip = (char*)ubiCB->RPC->RxBuffer();");
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
            writeln(3, "ubiCB->recvSize = *(int32*)ip;");
            writeln(3, "SwapBytes(ubiCB->recvSize);");
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
              writeln(3, ""+field.name+" = new "+field.type.cNameUbi()+"["+w+s+"];");
            }
            writeln(3, "memcpy("+field.name+", ip, ubiCB->recvSize);");
            if (alignForSun == true)
              writeln(3, ""+w1+"ip += alignData(ubiCB->recvSize);");
            else
              writeln(3, ""+w1+"ip += ubiCB->recvSize;");
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
      writeln(3, "ubiCB->RPC->RxFree();");
      writeln(2, "}");
    }
    writeln(2, "return (e"+module.name+")ubiCB->RPC->ReturnCode();");
    writeln(1, "}");
    writeln(1, "catch(xCept &x)");
    writeln(1, "{");
    writeln(2, "ubiCB->RPC->ErrBuffer(x.ErrorStr());");
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
        writeln("void "+module.name+"Snap"+prototype.name+parameter.name+"Prepare(int32 Handle, int32 Size)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return;");
        writeln(1, "T"+module.name+"UbiCB* ubiCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "ubiCB->"+prototype.name+parameter.name+" = new "
                            +parameter.type.cNameUbi()+" [Size];");
        allocated.addElement(parameter.name);
        writeln("}");
        writeln();
        write("int32 "+module.name+"Snap"+prototype.name+parameter.name+"Fill(int32 Handle");
        write(", " + parameter.type.cDefUbi("Rec"));
        writeln(", int Index)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return 0;");
        writeln(1, "T"+module.name+"UbiCB* ubiCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "ubiCB->"+prototype.name+parameter.name+"[Index] = *Rec;");
        writeln(1, "return 1;");
        writeln("}");
        writeln();
      }
    }
    if (retrievals.size() > 0 || submittals.size() > 0)
    {
      write("e"+module.name+" "+module.name+"Snap" + prototype.name + "Start(int32 Handle, int32 Signature");
      if (hasReturn)
        write(", "+prototype.type.cNameUbi()+"& Result");
      for (int i = 0; i < prototype.parameters.size(); i++)
      {
        Field parameter = (Field) prototype.parameters.elementAt(i);
        if (parameter.type.typeof != Type.CHAR
        && (parameter.type.reference == Type.BYPTR
        ||  parameter.type.reference == Type.BYREFPTR))
          if (prototype.hasOutputSize(parameter.name)
          ||  prototype.hasInputSize(parameter.name))
            continue;
        write(", " + parameter.type.cDefUbi(parameter.name));
      }
      writeln(")");
      writeln("{");
      writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
      writeln(2, "return "+module.name.toUpperCase()+"_INV_COOKIE;");
      writeln(1, "T"+module.name+"UbiCB* ubiCB = "+module.name+"CBHandle.Use(Handle);");
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
        writeln(1, "ubiCB->"+prototype.name+parameter.name+" = new "
                        +parameter.type.cNameUbi()+"["+w+s+"];");
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
          write(", ubiCB->"+prototype.name+parameter.name);
        else
          write(", "+parameter.name);
      }
      writeln(");");
      for (int i = 0; i < submittals.size(); i++)
      {
        Field parameter = (Field) submittals.elementAt(i);
        writeln(1, "delete [] ubiCB->"+prototype.name+parameter.name+";");
        writeln(1, "ubiCB->"+prototype.name+parameter.name+" = 0;");
      }
      writeln(1, "return result;");
      writeln("}");
      writeln();
    }
    if (retrievals.size() > 0)
    {
      for (int i = 0; i < retrievals.size(); i++)
      {
        Field parameter = (Field) retrievals.elementAt(i);
        write("int32 "+module.name+"Snap"+prototype.name+parameter.name+"Next(int32 Handle");
        write(", " + parameter.type.cDefUbiRefPAsP("Rec"));
        writeln(", int32 Index)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return 0;");
        writeln(1, "T"+module.name+"UbiCB* ubiCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "*Rec = ubiCB->"+prototype.name+parameter.name+"[Index];");
        writeln(1, "return 1;");
        writeln("}");
        writeln();
        writeln("void "+module.name+"Snap"+prototype.name+parameter.name+"Done(int32 Handle)");
        writeln("{");
        writeln(1, "if (Handle < CookieStart || Handle >= CookieStart+NoCookies)");
        writeln(2, "return;");
        writeln(1, "T"+module.name+"UbiCB* ubiCB = "+module.name+"CBHandle.Use(Handle);");
        writeln(1, "if (ubiCB->"+prototype.name+parameter.name+")");
        writeln(2, "delete [] ubiCB->"+prototype.name+parameter.name+";");
        writeln(1, "ubiCB->"+prototype.name+parameter.name+" = 0;");
        writeln("}");
        writeln();
      }
    }
  }
}
