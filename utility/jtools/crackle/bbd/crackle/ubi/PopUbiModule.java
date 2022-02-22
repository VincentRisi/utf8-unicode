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

public class PopUbiModule extends Generator
{
  public static String description()
  {
    return "Generates Ubiquitous Module Code (AIX|SUN|NT)";
  }
  public static String documentation()
  {
    return "Generates Ubiquitous Module Code (AIX|SUN|NT)"
     + "\r\nHandles following pragmas"
     + "\r\n  AlignForSUN - ensure that all fields are on 8 byte boundaries."
      ;
  }
  private static void setupPragmaVector()
  {
    if (pragmaVector == null) 
    {
      pragmaVector = new Vector();
      pragmaVector.addElement(new Pragma("AlignForSun", false, "Ensure that all fields are on 8 byte boundaries."));
    }
  }
  {
    setupPragmaVector();
  }
  private static boolean alignForSun;
  private static void setPragmas(Module module)
  {
    // Ensure these are in the same order as above
    setupPragmaVector();
    int no=0;
    alignForSun = pragmaVector.elementAt(no++).value;
    for (int i = 0; i < module.pragmas.size(); i++)
    {
      String pragma = (String) module.pragmas.elementAt(i);
      if (pragma.trim().equalsIgnoreCase("AlignForSUN") == true)
        alignForSun = true;
    }
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
  /**
  * Generates
  * - C Header for Server
  * - C Server Marshaling code
  */
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    indent_size = 4;
    errLog = outLog;
    outLog.println(module.name+" version "+module.version);
    setPragmas(module);
    generateCHeader(module, output, outLog);
    if (generateCModule(module, output, outLog))
      generateCCode(module, output, outLog);
  }
  /**
  * Sets up the writer and generates the general stuff
  */
  private static String nameOf(String fullName)
  {
    int n = fullName.lastIndexOf('/');
    if (n > 0)
      return fullName.substring(n+1);
    n = fullName.lastIndexOf('\\');
    if (n > 0)
      return fullName.substring(n+1);
    return fullName;
  }
  private static void generateCHeader(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: "+output+module.name.toLowerCase()+".h");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+".h");
      writer = new PrintWriter(outFile);
      try
      {
        boolean hasProtected = false;
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#ifndef _" + module.name + "H_");
        writeln("#define _" + module.name + "H_");
        writeln();
        writeln("#include \"swapbytes.h\"");
        writeln("#include \"ubidata.h\"");
        writeln("#include \"ubimodule.h\"");
        //writeln("#include \"logfile.h\"");
        //writeln("#include \"pgapi.h\"");
        writeln("#include \"pgaspect.h\"");
        writeln("#include \"handles.h\"");
        writeln();
        PopUbiGen.generateCExterns(module, writer);
        writeln("class I" + module.name/* + ": public PgAspect"*/);
        writeln("{");
        //writeln("protected: PgAspect* aspect;");
        writeln("public:");
        for (int i = 0; i < module.structures.size(); i++)
        {
          Structure structure = (Structure)module.structures.elementAt(i);
          if (structure.codeType != Structure.PUBLIC)
            continue;
          for (int j = 0; j < structure.fields.size(); j++)
          {
            Field field = (Field)structure.fields.elementAt(j);
            writeln("  " + field.type.cDefUbi(field.name) + ";");
          }
        }
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType == Prototype.PRIVATE
          || prototype.codeType == Prototype.PROTECTED)
            continue;
          PopUbiGen.generateVirtualCHeader(module, prototype, writer);
        }
        if (hasProtected == true)
        {
          writeln("protected:");
          for (int i = 0; i < module.structures.size(); i++)
          {
            Structure structure = (Structure)module.structures.elementAt(i);
            if (structure.codeType != Structure.PROTECTED)
              continue;
            for (int j = 0; j < structure.fields.size(); j++)
            {
              Field field = (Field)structure.fields.elementAt(j);
              writeln("  " + field.type.cDefUbi(field.name) + ";");
            }
          }
          for (int i = 0; i < module.prototypes.size(); i++)
          {
            Prototype prototype = (Prototype)module.prototypes.elementAt(i);
            if (prototype.codeType != Prototype.PROTECTED)
              continue;
            PopUbiGen.generateVirtualCHeader(module, prototype, writer);
          }
        }
        writeln("};");
        writeln();
        writeln("class T" + module.name + " : public I" + module.name);
        writeln("{");
        for (int i = 0; i < module.structures.size(); i++)
        {
          Structure structure = (Structure)module.structures.elementAt(i);
          if (structure.codeType == Structure.PROTECTED)
            hasProtected = true;
          if (structure.codeType != Structure.PRIVATE)
            continue;
          for (int j = 0; j < structure.fields.size(); j++)
          {
            Field field = (Field)structure.fields.elementAt(j);
            writeln("  " + field.type.cDefUbi(field.name) + ";");
          }
        }
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType == Prototype.PROTECTED)
            hasProtected = true;
          if (prototype.codeType != Prototype.PRIVATE)
            continue;
          PopUbiGen.generateCHeader(module, prototype, writer);
        }
        writeln("protected:");
        writeln("  int instance;");
        writeln("  tLogFile* logFile;");
        writeln("  TJConnector* connect;");
        writeln("public:");
        writeln("  void Setup(tLogFile* logFile= 0, TJConnector* connect = 0) {this->logFile = logFile; this->connect = connect;}");
        writeln("  typedef tHandle<T" + module.name + "*, 100, 100> T" + module.name + "Handle;");
        writeln("  static T" + module.name + "Handle " + module.name + "Handle;");
        writeln("  T" + module.name + "(){instance = " + module.name + "Handle.Create(this);Setup();}");
        writeln(" ~T" + module.name + "(){" + module.name + "Handle.Release(instance, 0);}");
        writeln("  static T" + module.name + "* Instance(int instance=100)");
        writeln("  {");
        writeln("    T" + module.name + "* result = (T" + module.name + "*) " + module.name + "Handle.Use(instance);");
        writeln("    return result;");
        writeln("  }");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType == Prototype.PRIVATE
          || prototype.codeType == Prototype.PROTECTED)
            continue;
          PopUbiGen.generateCHeader(module, prototype, writer);
        }
        if (hasProtected == true)
        {
          writeln("protected:");
          for (int i = 0; i < module.structures.size(); i++)
          {
            Structure structure = (Structure)module.structures.elementAt(i);
            if (structure.codeType != Structure.PROTECTED)
              continue;
            for (int j = 0; j < structure.fields.size(); j++)
            {
              Field field = (Field)structure.fields.elementAt(j);
              writeln("  " + field.type.cDefUbi(field.name) + ";");
            }
          }
          for (int i = 0; i < module.prototypes.size(); i++)
          {
            Prototype prototype = (Prototype)module.prototypes.elementAt(i);
            if (prototype.codeType != Prototype.PROTECTED)
              continue;
            PopUbiGen.generateCHeader(module, prototype, writer);
          }
        }
        writeln("};");
        writeln();
        writeln("class T" + module.name + "Module : public UbiModule");
        writeln("{");
        writeln("public:");
        writeln("  T" + module.name + " *" + module.name + ";");
        writeln("  T" + module.name + "Module();");
        writeln("  ~T" + module.name + "Module();");
        writeln("  static UbiMessage messages[];");
        writeln("  UbiData* Dispatch(const UbiData *input);");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          writeln("  int32 " + prototype.name + "(char *ip);");
        }
        writeln("  UbiMessage* Messages(int &noOf);");
        writeln("  bool HasMessage(int reqID, int signature);");
        writeln("  void Setup(PgAspect *aspect) {" + module.name + "->Setup(aspect->logFile, aspect->connect);}");
        writeln("  void Clear();");
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
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    }
    catch (Throwable e)
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
  private static void generateCCode(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: "+output+module.name.toLowerCase()+".cpp");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+".cpp");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#include \"machine.h\"");
        writeln("#include \"ubixcept.h\"");
        writeln();
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln("#include <string.h>");
        writeln("#include <assert.h>");
        writeln();
        writeln("#include \""+module.name.toLowerCase()+".h\"");
        writeln();
        writeln("T" + module.name + "::T" + module.name + "Handle T" + module.name + "::" + module.name + "Handle;");
        writeln();
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.code.size() > 0)
          {
            PopUbiGen.generateUbiCImplCode(module, prototype, writer);
            writeln("{");
            if (prototype.code.size() > 0 && prototype.codeLine > 0)
              writeln("/** #line "+prototype.codeLine+" \""+nameOf(module.sourceName.toLowerCase())+"\" **/");
            for (int j = 0; j < prototype.code.size(); j++)
            {
              String codeLine = (String) prototype.code.elementAt(j);
              write(codeLine);
            }
            writeln("}");
            writeln();
          }
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
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    }
    catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }
  private static String toHex(String value)
  {
    int n = new Integer(value).intValue();
    return toHex(n);
  }
  private static String toHex(int value)
  {
    String result = Integer.toHexString(value);
    while (result.length() < 8)
      result = "0" + result;
    result = "0x" + result.toUpperCase();
    return result;
  }
  private static void generateReqIDTable(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: "+output+module.name.toLowerCase()+"ReqID.txt");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"ReqID.txt");
      writer = new PrintWriter(outFile);
      try
      {
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          if (prototype.message.length() > 0)
            writeln("'" + module.name + "', " + toHex(prototype.message) + ", '" + prototype.name + "'");
          else
            writeln("'" + module.name + "', " + toHex(i) + ", '" + prototype.name + "'");
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
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
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
  private static boolean generateCModule(Module module, String output, PrintWriter outLog)
  {
    boolean hasCode = false;
    try
    {
      outLog.println("Code: "+output+module.name.toLowerCase()+"module.cpp");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"module.cpp");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("#include \"machine.h\"");
        writeln("#include <stdio.h>");
        writeln("#include <stdlib.h>");
        writeln("#include <string.h>");
        writeln("#include \"" + module.name.toLowerCase() + ".h\"");
        writeln();
        if (alignForSun == true)
        {
          writeln("inline int32 alignData(int32 size)");
          writeln("{");
          writeln("  int n = size % 8;");
          writeln("  if (n > 0) size += (8-n);");
          writeln("  return size;");
          writeln("}");
          writeln();
        }
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
        String comma = "{ ";
        writeln("UbiMessage T" + module.name + "Module::messages[] = ");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          String reqID = toHex(i);
          if (prototype.message.length() > 0)
            reqID = toHex(prototype.message);
          writeln(comma + " UbiMessage(" + reqID + ", " + toHex((int)prototype.signature(true)) + ") // "+ prototype.name);
          comma = ", ";
        }
        writeln("};");
        writeln();
        writeln("T" + module.name + "Module::T" + module.name + "Module()");
        writeln("{");
        writeln("  " + module.name + " = new T" + module.name + "();");
        writeln("  output = new UbiData();");
        writeln("}");
        writeln();
        writeln("T"+module.name+"Module::~T"+module.name+"Module()");
        writeln("{");
        writeln("}");
        writeln();
        writeln("extern \"C\" OS_DECLSPEC UbiModule* " + module.name + "construct()");
        writeln("{");
        writeln("  UbiModule *result = new T" + module.name + "Module();");
        writeln("  return result;");
        writeln("}");
        writeln("");
        writeln("extern \"C\" OS_DECLSPEC void " + module.name + "destruct(UbiModule *& module)");
        writeln("{");
        writeln("  delete module;");
        writeln("  module = 0;");
        writeln("}");
        writeln();
        writeln("UbiData* T" + module.name + "Module::Dispatch(const UbiData *input)");
        writeln("{");
        writeln("  char* ip = (char*)input->data;");
        if (alignForSun == true)
          writeln("  ip += alignData(sizeof(int32));");
        else
          writeln("  ip += sizeof(int32);");
        writeln("  switch (input->reqID)");
        writeln("  {");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          if (prototype.code.size() > 0)
            hasCode = true;
          if (prototype.message.length() > 0)
            write("    case "+toHex(prototype.message)+":");
          else
            write("    case "+toHex(i)+":");
          write(" result = "+prototype.name+"(ip);");
          writeln(" break;");
        }
        writeln("  default:");
        writeln("    return output;");
        writeln("  }");
        writeln("  output->Use((unsigned char *)replyBody, replySize, result);");
        writeln("  return output;");
        writeln("}");
        writeln();
        writeln("void T" + module.name + "Module::Clear()");
        writeln("{");
        writeln("  output->Clear();");
        writeln("}");
        writeln();
        writeln("UbiMessage* T" + module.name + "Module::Messages(int &noOf)");
        writeln("{");
        writeln("  noOf = sizeof(messages) / sizeof(UbiMessage);");
        writeln("  return messages;");
        writeln("}");
        writeln();
        writeln("bool T" + module.name + "Module::HasMessage(int reqID, int signature)");
        writeln("{");
        writeln("  switch (reqID)");
        writeln("  {");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          if (prototype.message.length() > 0)
            write("    case " + toHex(prototype.message) + ":");
          else
            write("    case " + toHex(i) + ":");
          writeln(" return signature == " + toHex((int)prototype.signature(true)) + "; // "+ prototype.name);
        }
        write("    default: return false;");
        writeln("  }");
        writeln("}");
        writeln();
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          generateCModule(module, prototype, i);
        }
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
      System.out.println(e1);
      System.out.flush();
      e1.printStackTrace();
    }
    catch (Throwable e)
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
  private static void generateCModule(Module module, Prototype prototype, int no)
  {
    writeln("int T" + module.name + "Module::" + prototype.name + "(char *ip)");
    writeln("{");
    writeln("  int resultCode = 0;");
    writeln("  try");
    writeln("  {");
    writeln("    replySize = 0;replyBody = 0;");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      boolean addUp = i + 1 != prototype.inputs.size();
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field  = input.getParameter(prototype);
      if (field == null)
        continue;
      byte savedReference = field.type.reference;
      if (field.type.reference == Type.BYPTR
      ||  field.type.reference == Type.BYREFPTR)
      {
        if (input.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln("    recvSize = *(int32*)ip;");
            writeln("    SwapBytes(recvSize);");
            if (alignForSun == true)
              writeln("    ip += alignData(sizeof(int32));");
            else
              writeln("    ip += sizeof(int32);");
            writeln("    char* "+field.name+" = ip;");
            if (addUp)
            {
              if (alignForSun == true)
                writeln("    ip += alignData(recvSize);");
              else
                writeln("    ip += recvSize;");
            }
          }
          else
          {
            writeln("    "+field.type.cNameUbi()+"* "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
            if (addUp)
            {
              if (alignForSun == true)
                writeln("    ip += alignData(sizeof(*"+field.name+"));");
              else
                writeln("    ip += sizeof(*"+field.name+");");
            }
          }
        }
        else
        {
          writeln("    recvSize = *(int32*)ip;");
          writeln("    SwapBytes(recvSize);");
          if (alignForSun == true)
            writeln("    ip += alignData(sizeof(int32));");
          else
            writeln("    ip += sizeof(int32);");
          writeln("    "+field.type.cNameUbi()+"* "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
          if (addUp)
          {
            if (alignForSun == true)
              writeln("    ip += alignData(recvSize);");
            else
              writeln("    ip += recvSize;");
          }
        }
      }
      else
      {
        writeln("    "+field.type.cNameUbi()+"* "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
        if (addUp)
        {
          if (alignForSun == true)
            writeln("    ip += alignData(sizeof(*"+field.name+"));");
          else
            writeln("    ip += sizeof(*"+field.name+");");
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
    boolean hasresult = false;
    if (prototype.type.typeof != Type.VOID
    ||  prototype.type.reference == Type.BYPTR)
    {
      if (prototype.type.reference != Type.BYPTR)
      {
        if (alignForSun == true)
          writeln("    replySize += alignData((int32)sizeof("+prototype.type.cNameUbi()+")); // result Size");
        else
          writeln("    replySize += (int32)sizeof("+prototype.type.cNameUbi()+"); // result Size");
        hasOutput = true;
        hasresult = true;
      }
    }
    int Variants = 0;
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      Action output = (Action) prototype.outputs.elementAt(i);
      Field field  = output.getParameter(prototype);
      if (field == null)
        continue;
      hasOutput = true;
      Operation op = output.sizeOperation();
      if (field.type.reference == Type.BYPTR)
      {
        if (output.hasSize() == false)
        {
          if (field.type.typeof == Type.CHAR)
          {
            writeln("#error unsized char* on output");
            errLog.println("#error unsized char* on output");
          }
          else
          {
            if (alignForSun == true)
              writeln("    replySize += alignData(sizeof("+field.type.cNameUbi()+")); // size "+field.name);
            else
              writeln("    replySize += sizeof("+field.type.cNameUbi()+"); // size "+field.name);
          }
        }
        else
        {
          String w1 = "";
          if (op.isConstant == false)
            w1 = "*";
          if (alignForSun == true)
            writeln("    replySize += alignData(sizeof(int32));  // size of Block");
          else
            writeln("    replySize += sizeof(int32);  // size of Block");
          if (alignForSun == true)
            writeln("    replySize += alignData("+w1+""+op.name+" * sizeof("+field.type.cNameUbi()+")); // size "+field.name);
          else
            writeln("    replySize += ("+w1+""+op.name+" * sizeof("+field.type.cNameUbi()+")); // size "+field.name);
        }
      }
      else if (field.type.reference == Type.BYREFPTR)
      {
        if (output.hasSize() == false)
        {
          writeln("#error unsized BYREFPTR on output");
          errLog.println("#error unsized BYREFPTR on output");
        }
        else
        {
          Variants++;
          if (alignForSun == true)
            writeln("    replySize += alignData(sizeof("+field.type.cNameUbi()+"*)); // Variant "+field.name);
          else
            writeln("    replySize += sizeof("+field.type.cNameUbi()+"*); // Variant "+field.name);
        }
      }
      else
      {
        if (alignForSun == true)
          writeln("    replySize += alignData(sizeof("+field.type.cNameUbi()+")); // size "+field.name);
        else
          writeln("    replySize += sizeof("+field.type.cNameUbi()+"); // size "+field.name);
      }
    }
    if (hasOutput)
    {
      if (Variants > 0)
      {
        writeln("    int32 Variants["+Variants+"];");
        writeln("    int32 VariantSize["+Variants+"];");
        Variants = 0;
      }
      writeln("    replyBody = new char [replySize];");
      writeln("    memset(replyBody, 0, replySize);");
      writeln("    ip = replyBody;");
    }
    if (prototype.type.typeof != Type.VOID
    ||  prototype.type.reference == Type.BYPTR)
    {
      if (prototype.type.reference != Type.BYPTR)
      {
        writeln("    "+prototype.type.cNameUbi()+"* result = ("+prototype.type.cNameUbi()+"*) ip;");
        if (prototype.outputs.size() > 0)
        {
          if (alignForSun == true)
            writeln("    ip += alignData(sizeof("+prototype.type.cNameUbi()+"));");
          else
            writeln("    ip += sizeof("+prototype.type.cNameUbi()+");");
        }
      }
    }
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      boolean addUp = i + 1 != prototype.outputs.size();
      Action output = (Action) prototype.outputs.elementAt(i);
      Field field  = output.getParameter(prototype);
      if (field == null)
        continue;
      Operation op = output.sizeOperation();
      Action input = prototype.getInputAction(field.name);
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
              writeln("    memcpy(ip, "+field.name+", sizeof("+field.type.cNameUbi()+"));");
              writeln("    "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
            }
            else
              writeln("    "+field.type.cNameUbi()+"* "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
            if (addUp)
            {
              if (alignForSun == true)
                writeln("    ip += alignData(sizeof("+field.type.cNameUbi()+"));");
              else
                writeln("    ip += sizeof("+field.type.cNameUbi()+");");
            }
          }
        }
        else
        {
          String w1 = "";
          if (op.isConstant == false)
            w1 = "*";
          writeln("    *(int32*)ip = ("+w1+""+op.name+" * sizeof("+field.type.cNameUbi()+"));");
          writeln("    SwapBytes(*(int32*)ip);");
          if (alignForSun == true)
            writeln("    ip += alignData(sizeof(int32));");
          else
            writeln("    ip += sizeof(int32);");
          if (input != null)
          {
            writeln("    memcpy(ip, "+field.name+", ("+w1+op.name+" * sizeof("+field.type.cNameUbi()+")));");
            writeln("    "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
          }
          else
            writeln("    "+field.type.cNameUbi()+"* "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
          if (addUp)
          {
            if (alignForSun == true)
              writeln("    ip += alignData("+w1+op.name+" * sizeof("+field.type.cNameUbi()+"));");
            else
              writeln("    ip += ("+w1+op.name+" * sizeof("+field.type.cNameUbi()+"));");
          }
        }
      }
      else if (field.type.reference == Type.BYREFPTR)
      {
        if (output.hasSize() == false)
          continue;
        else
        {
          if (op.isConstant == false)
          {
          } else
            writeln("#error Constant Size with BYREFPTR output");
          writeln("    Variants["+(Variants++)+"] = (int32)(ip-replyBody);");
          if (input != null)
            writeln("    ("+field.type.cNameUbi()+"*)*ip = "+field.name+";");
          else
            writeln("    "+field.type.cNameUbi()+"** "+field.name+" = ("+field.type.cNameUbi()+"**)ip;");
          if (addUp)
          {
            if (alignForSun == true)
              writeln("    ip += alignData(sizeof("+field.type.cNameUbi()+"*));");
            else
              writeln("    ip += sizeof("+field.type.cNameUbi()+"*);");
          }
        }
      }
      else
      {
        if (input != null)
        {
          writeln("    memcpy(ip, "+field.name+", sizeof("+field.type.cNameUbi()+"));");
          writeln("    "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
        }
        else
          writeln("    "+field.type.cNameUbi()+"* "+field.name+" = ("+field.type.cNameUbi()+"*)ip;");
        if (addUp)
        {
          if (alignForSun == true)
            writeln("    ip += alignData(sizeof("+field.type.cNameUbi()+"));");
          else
            writeln("    ip += sizeof("+field.type.cNameUbi()+");");
        }
      }
    }
    writeln("    try");
    writeln("    {");
    String w1 = "", w2 = "";
    write("      ");
    if (hasresult)
      write("*result = ");
    write(module.name+"->"+prototype.name+"(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field field = (Field) prototype.parameters.elementAt(i);
      if (field.type.reference == Type.BYPTR)
        w2 = "";
      else
        w2 = "*";
      write(w1 + w2 + field.name);
      w1 = ", ";
    }
    writeln(");");
    writeln("    }");
    writeln("    catch (int rc)");
    writeln("    {");
    writeln("      resultCode = rc;");
    writeln("    }");
    if (hasresult && prototype.needsSwap())
      writeln("    SwapBytes(*result);");
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      Action output = (Action) prototype.outputs.elementAt(i);
      Field field  = output.getParameter(prototype);
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
      writeln("    // We are going to setup a new replyBody because of Variants");
      writeln("    char *oldreplyBody = replyBody;");
      writeln("    ip = replyBody;");
      writeln("    int32 tail = replySize;");
      writeln("    // Calc new replySize for Contiguous memory from Disjoint memory");
      writeln("    replySize = 0;");
      if (prototype.type.typeof != Type.VOID
      ||  prototype.type.reference == Type.BYPTR)
      {
        if (prototype.type.reference != Type.BYPTR)
        {
          if (alignForSun == true)
            writeln("    replySize += alignData((int32)sizeof("+prototype.type.cNameUbi()+")); // result Size");
          else
            writeln("    replySize += (int32)sizeof("+prototype.type.cNameUbi()+"); // result Size");
          hasOutput = true;
          hasresult = true;
        }
      }
      int VariantSize = 0;
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field  = output.getParameter(prototype);
        if (field == null)
          continue;
        hasOutput = true;
        Operation op = output.sizeOperation();
        if (field.type.reference == Type.BYPTR
        ||  field.type.reference == Type.BYREFPTR)
        {
          if (output.hasSize() == false)
          {
            if (field.type.typeof != Type.CHAR)
            {
              if (alignForSun == true)
                writeln("    replySize += alignData(sizeof("+field.type.cNameUbi()+")); // size "+field.name);
              else
                writeln("    replySize += sizeof("+field.type.cNameUbi()+"); // size "+field.name);
            }
          }
          else
          {
            w1 = "";
            if (op.isConstant == false)
              w1 = "*";
            if (alignForSun == true)
            {
              writeln("    replySize += alignData(sizeof(int32));  // size of Block");
              writeln("    replySize += alignData("+w1+""+op.name+" * sizeof("+field.type.cNameUbi()+")); // size "+field.name);
            }
            else
            {
              writeln("    replySize += sizeof(int32);  // size of Block");
              writeln("    replySize += ("+w1+""+op.name+" * sizeof("+field.type.cNameUbi()+")); // size "+field.name);
            }
            if (field.type.reference == Type.BYREFPTR)
              writeln("    VariantSize["+(VariantSize++)+"] = ("+w1+""+op.name+" * sizeof("+field.type.cNameUbi()+"));");
          }
        }
        else
        {
          if (alignForSun == true)
            writeln("    replySize += alignData(sizeof("+field.type.cNameUbi()+")); // size "+field.name);
          else
            writeln("    replySize += sizeof("+field.type.cNameUbi()+"); // size "+field.name);
        }
      }
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field  = output.getParameter(prototype);
        if (field == null)
          continue;
        Operation op = output.sizeOperation();
        generateCNonStructSwaps(module, prototype, field, op);
      }
      writeln("    replyBody = new char [replySize];");
      writeln("    memset(replyBody, 0, replySize);");
      writeln("    char *op = replyBody;");
      writeln("    int32 pos = 0;");
      writeln("    {");
      writeln("      for (int i = 0; i < "+Variants+"; i++)");
      writeln("      {");
      writeln("        int32 size = Variants[i]-pos;");
      writeln("        if (size > 0)");
      writeln("        {");
      writeln("          memcpy(op, ip, size);");
      if (alignForSun == true)
      {
        writeln("          ip += alignData(size);");
        writeln("          op += alignData(size);");
        writeln("          tail -= alignData(size);");
      }
      else
      {
        writeln("          ip += size;");
        writeln("          op += size;");
        writeln("          tail -= size;");
      }
      writeln("        }");
      writeln("        memcpy(op, &VariantSize[i], sizeof(VariantSize[i])); ");
      writeln("        SwapBytes(*(int32*)op);");
      if (alignForSun == true)
        writeln("        op += alignData(sizeof(VariantSize[i]));");
      else
        writeln("        op += sizeof(VariantSize[i]);");
      writeln("        if (VariantSize[i] > 0)");
      writeln("        {");
      writeln("          char **block = (char **)ip;");
      writeln("          memcpy(op, *block, VariantSize[i]);");
      writeln("          free(*block);");
      writeln("        }");
      if (alignForSun == true)
      {
        writeln("        op += alignData(VariantSize[i]);");
        writeln("        ip += alignData(sizeof(char *));");
        writeln("        tail -= alignData(sizeof(char *));");
      }
      else
      {
        writeln("        op += VariantSize[i];");
        writeln("        ip += sizeof(char *);");
        writeln("        tail -= sizeof(char *);");
      }
      if (alignForSun == true)
        writeln("        pos = alignData(Variants[i])+alignData(sizeof(char *));");
      else
        writeln("        pos = Variants[i]+sizeof(char *);");
      writeln("      }");
      writeln("    }");
      writeln("    if (tail > 0)");
      writeln("      memcpy(op, ip, tail);");
      writeln("    delete [] oldreplyBody;");
    }
    else
    {
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field  = output.getParameter(prototype);
        if (field == null)
          continue;
        Operation op = output.sizeOperation();
        generateCNonStructSwaps(module, prototype, field, op);
      }
    }
    writeln("  }");
    writeln("  catch (int rc)");
    writeln("  {");
    writeln("    return rc;");
    writeln("  }");
    writeln("  return resultCode;");
    writeln("}");
    writeln();
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateCSwaps(Module module, Prototype prototype, Field field, Operation op)
  {
    String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "", w6 = "", w7 = "", w8 = "", w9 = "";
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
        w8 = "{";
        w9 = "}";
        Field opField = prototype.getParameter(op.name);
        if (opField != null
        && (opField.type.reference == Type.BYPTR
        ||  opField.type.reference == Type.BYREFPTR))
          w5 = "*";
        writeln("    "+w2+w8+"for (int i = 0; i < "+w5+op.name+"; i++)");
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
      for (int j=0; j < field.type.arraySizes.size(); j++)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(j);
        writeln("    "+w2+"for (int i" + j + " = 0; i" + j
                        + " < " + integer.intValue()
                        + "; i" + j + "++)");
        w1 = w1 + "[i" + j + "]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
      }
    }
    if (field.isStruct(module))
      writeln("    "+w2+w6+field.name+w7+w1+w3+"Swaps();"+w9);
    else if (field.needsSwap())
      writeln("    "+w2+"SwapBytes("+w4+field.name+w1+");"+w9);
    else if (field.type.typeof == Type.USERTYPE)
      errLog.println("Warning: "+prototype.name+" "+field.name+" is of UserType and may require swapping.");
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
