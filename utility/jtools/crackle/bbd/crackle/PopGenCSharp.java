package bbd.crackle;

/**
 * @author vince
 */

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class PopGenCSharp extends Generator
{
  public static String description()
  {
    return "Generates C# Client Code for C++ Server Code";
  }

  public static String documentation()
  {
    return "Generates C# Client Code for C++ Server Code" + "\r\nHandles following pragmas"
        + "\r\n  AlignForSUN - ensure that all fields are on 8 byte boundaries."
        + "\r\n  KeepTee - Keep the dreaded small tee in the structures.";
  }

  private static boolean alignForSun = false;
  private static boolean keepTee = false;
  public static PrintWriter errLog;

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
   * Generates
   */
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    errLog = outLog;
    outLog.println(module.name + " version " + module.version);
    for (int i = 0; i < module.pragmas.size(); i++)
    {
      String pragma = (String) module.pragmas.elementAt(i);
      if (pragma.trim().equalsIgnoreCase("AlignForSUN") == true)
      {
        outLog.println(module.name + ": Align For Sun");
        alignForSun = true;
      } else if (pragma.trim().equalsIgnoreCase("KeepTee") == true)
      {
        outLog.println(module.name + ": Keeping Little tee (hee hee)");
        keepTee = true;
      }
    }
    indent_size = 2;
    generateStructs(module, output, outLog);
    generateInterfaces(module, output, outLog);
    generateClasses(module, output, outLog);
    generateProxies(module, output, outLog);
    //generateIDL2Python(module, output, outLog);
  }

  /**
   * Sets up the writer and generates the general stuff
   */
  public static void generateStructs(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name + "Structs.cs");
      OutputStream outFile = new FileOutputStream(output + module.name + "Structs.cs");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln();
        writeln("using System;");
        generateUsings(module);
        writeln();
        writeln("namespace Bbd.Idl2." + module.name);
        writeln("{");
        generateStructs(module);
        generateEnums(module);
        generateMessageEnums(module);
        generateTableEnums(module);
        writeln("}");
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
  public static void generateInterfaces(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + "I" + module.name + ".cs");
      OutputStream outFile = new FileOutputStream(output + "I" + module.name + ".cs");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln();
        writeln("using System;");
        generateUsings(module);
        writeln();
        writeln("namespace Bbd.Idl2." + module.name);
        writeln("{");
        writeln(1, "public interface I" + module.name + ": IDisposable");
        writeln(1, "{");
        generateInterfaces(module);
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
  public static void generateClasses(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name + ".cs");
      OutputStream outFile = new FileOutputStream(output + module.name + ".cs");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln();
        writeln("using System;");
        writeln("using System.IO;");
        writeln("using Bbd.Idl2;");
        writeln("using Bbd.Idl2.Rpc;");
        generateUsings(module);
        writeln();
        writeln("namespace Bbd.Idl2." + module.name);
        writeln("{");
        writeln(1, "public class " + module.name + ": Popper, I" + module.name);
        writeln(1, "{");
        writeln(2, "private string version = " + module.version + ";");
        writeln(2, "private int signature = " + module.signature + ";");
        writeln(2, "public string Version {get {return version;}}");
        writeln(2, "public int Signature {get {return signature;}}");
        generateMessageLookups(module);
        generateTableLookups(module);
        writeln(2, "public " + module.name + "(Header header, string host, string service, int timeOut)");
        writeln(2, ": base(header, host, service, timeOut)");
        writeln(2, "{}");
        generateCalls(module);
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
  public static void generateProxies(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name + "Proxy.cs");
      OutputStream outFile = new FileOutputStream(output + module.name + "Proxy.cs");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln();
        writeln("using System;");
        writeln("using Bbd.Idl2;");
        writeln("using Bbd.Idl2.Rpc;");
        generateUsings(module);
        writeln();
        writeln("namespace Bbd.Idl2." + module.name);
        writeln("{");
        writeln(1, "public class " + module.name + "Proxy : MarshalByRefObject, I" + module.name);
        writeln(1, "{");
        writeln(2, "protected " + module.name + " server;");
        writeln(2, "public " + module.name + "Proxy(Header header, string host, string service, int timeOut)");
        writeln(2, "{");
        writeln(3, "server = new " + module.name + "(header, host, service, timeOut);");
        writeln(2, "}");
        generateProxies(module);
        writeln(2, "public virtual void Dispose(){} // Not quite sure I like this (VR)");
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
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  /**
   * Generates the module message enum
   */
  public static void generateMessageEnums(Module module)
  {
    writeln(1, "public enum Message");
    writeln(1, "{ OK");
    String w0 = "";
    if (module.messageBase > 0)
      w0 = " = " + module.messageBase;
    for (int i = 0; i < module.messages.size(); i++)
    {
      Message message = (Message) module.messages.elementAt(i);
      writeln(1, ", " + message.name + w0);
      w0 = "";
    }
    writeln(1, ", INV_SIGNATURE");
    writeln(1, ", INV_COOKIE");
    writeln(1, ", INV_INIFILE");
    writeln(1, ", UNCAUGHT_DBERROR");
    writeln(1, ", UNKNOWN_FUNCTION");
    writeln(1, ", SNAP_ERROR");
    writeln(1, ", ADDLIST_ERROR");
    writeln(1, ", ADDLIST_REALLOC_FAILED");
    writeln(1, ", LAST_LAST");
    writeln(1, "}");
  }

  public static void generateMessageLookups(Module module)
  {
    writeln(2, "// Used to be a class method but changed to instance method");
    writeln(2, "// for the publish and subscribe delegates. Requires downcast");
    writeln(2, "// for the same reason.");
    writeln(2, "public override string MessageDesc(Object no)");
    writeln(2, "{");
    writeln(3, "switch ((Message)no)");
    writeln(3, "{");
    writeln(3, "case Message.OK: return \"OK\";");
    for (int i = 0; i < module.messages.size(); i++)
    {
      Message message = (Message) module.messages.elementAt(i);
      writeln(3, "case Message." + message.name + ": return " + message.value + ";");
    }
    writeln(3, "case Message.INV_SIGNATURE: return \"Invalid Signature\";");
    writeln(3, "case Message.INV_COOKIE: return \"Invalid Cookie\";");
    writeln(3, "case Message.INV_INIFILE: return \"Invalid Inifile\";");
    writeln(3, "case Message.UNCAUGHT_DBERROR: return \"Uncaught DBError\";");
    writeln(3, "case Message.UNKNOWN_FUNCTION: return \"Unknown Function\";");
    writeln(3, "case Message.SNAP_ERROR: return \"Snap Error\";");
    writeln(3, "case Message.ADDLIST_ERROR: return \"AddList Error\";");
    writeln(3, "case Message.ADDLIST_REALLOC_FAILED: return \"AddList realloc Failed\";");
    writeln(3, "}");
    writeln(3, "return String.Format(\"MessageDesc for {0} Unknown.\", no);");
    writeln(2, "}");
  }

  /**
   * Generates table messages enum
   */
  public static void generateTableEnums(Module module)
  {
    for (int i = 0; i < module.tables.size(); i++)
    {
      Table table = (Table) module.tables.elementAt(i);
      writeln(2, "public enum " + table.name);
      String comma = "    { ";
      for (int j = 0; j < table.messages.size(); j++)
      {
        Message message = (Message) table.messages.elementAt(j);
        writeln(comma + message.name);
        comma = "    , ";
      }
      writeln(comma + table.name.toLowerCase() + "NoOf");
      writeln(2, "}");
    }
  }

  public static void generateTableLookups(Module module)
  {
    for (int i = 0; i < module.tables.size(); i++)
    {
      Table table = (Table) module.tables.elementAt(i);
      writeln(2, "public static string " + table.name + "Value(" + table.name + " no)");
      writeln(2, "{");
      writeln(3, "switch (no)");
      writeln(3, "{");
      for (int j = 0; j < table.messages.size(); j++)
      {
        Message message = (Message) table.messages.elementAt(j);
        writeln(3, "case " + table.name + "." + message.name + ": return " + message.value + ";");
      }
      writeln(3, "}");
      writeln(3, "return String.Format(\"" + table.name + "Value for {0} Unknown.\", no);");
      writeln(2, "}");
    }
  }

  public static void generateEnums(Module module)
  {
    for (int i = 0; i < module.enumerators.size(); i++)
    {
      Enumerator entry = (Enumerator) module.enumerators.elementAt(i);
      writeln(1, "public enum " + entry.name);
      String w1 = "  { ";
      for (int j = 0; j < entry.elements.size(); j++)
      {
        String element = (String) entry.elements.elementAt(j);
        writeln(w1 + element);
        w1 = "  , ";
      }
      writeln(1, "}");
    }
  }

  public static void generateUsings(Module module)
  {
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.fields.size() > 0)
        continue;
      if (structure.header.toLowerCase().indexOf(".sh") == structure.header.length() - 4)
      {
        writeln("using Bbd.Idl2.DBPortal;");
        break;
      }
    }
  }

  public static void generateStructs(Module module)
  {
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.fields.size() <= 0)
        continue;
      if (structure.name.compareTo(module.name) == 0)
        continue;
      writeln(1, "[Serializable()]");
      writeln(1, "public class " + dropTee(structure.name, module.name));
      writeln(1, "{");
      int relativeOffset = 0;
      int fillerNo = 0;
      for (int j = 0; j < structure.fields.size(); j++)
      {
        Field field = (Field) structure.fields.elementAt(j);
        int n = relativeOffset % field.type.paddingSize(true);
        if (n > 0)
        {
          n = field.type.paddingSize(true) - n;
          writeln(2, "[Field(Size=" + n + ")] byte[] _fill" + fillerNo + " = new byte[" + n + "];");
          fillerNo++;
          relativeOffset += n;
        }
        //writeln(2, "" + field.type.csAttribute(field.isStruct(module)) + "public "
        //    + field.type.csStructDef(field.name, field.isStruct(module)) + ";");
        String vardef = field.type.csStructDef(field.name, field.isStruct(module));
        String propdef = field.type.csStructDef(makeProp(field.name), false);
        String attribute = field.type.csAttribute(field.isStruct(module));
        writeln(2, format("public %s {get {return %4$s;} set{%4$s=value;}} %2$spublic %3$s;", propdef, attribute, vardef, field.name));
        relativeOffset += field.type.relativeSize(true);
      }
      int n = relativeOffset % 8;
      if (n > 0)
      {
        n = 8 - n;
        writeln(2, "[Field(Size=" + n + ")] byte[] _fill" + fillerNo + " = new byte[" + n + "];");
      }
      writeln(1, "}");
    }
  }

  private static String makeProp(String name)
  {
    char first = name.charAt(0);
    if (first == '_')
      return name.substring(1);
    String f = name.substring(0, 1);
    if (first >= 'A' && first <= 'Z')
      return f.toLowerCase()+name.substring(1);
    return f.toUpperCase()+name.substring(1);
  }

  public static void generateCalls(Module module)
  {
    for (int i = 0; i < module.prototypes.size(); i++)
    {
      Prototype prototype = (Prototype) module.prototypes.elementAt(i);
      if (prototype.codeType != Prototype.RPCCALL)
        continue;
      generateCall(module, prototype, i);
    }
  }

  public static Vector parameterList;

  public static boolean generateCommon(Module module, Prototype prototype, int no, String extra1)
  {
    Parameter pd = null;
    parameterList = new Vector();
    boolean hasReturn = prototype.type.reference == Type.BYVAL && prototype.type.typeof != Type.VOID;
    write("    " + extra1 + (hasReturn ? prototype.type.netName() : "void") + " " + prototype.name + "(");
    String comma = "";
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      pd = new Parameter();
      Field field = (Field) prototype.parameters.elementAt(i);
      pd.field = field;
      for (int j = 0; j < prototype.inputs.size(); j++)
      {
        pd.input = (Action) prototype.inputs.elementAt(j);
        if (pd.input.name.compareTo(field.name) == 0)
        {
          pd.isInput = true;
          pd.inputNo = j;
          pd.hasInputSize = pd.input.hasSize();
          pd.inputSizeValue = pd.input.getSizeName();
          break;
        }
      }
      for (int j = 0; j < prototype.outputs.size(); j++)
      {
        pd.output = (Action) prototype.outputs.elementAt(j);
        if (pd.output.name.compareTo(field.name) == 0)
        {
          pd.isOutput = true;
          pd.outputNo = j;
          pd.hasOutputSize = pd.output.hasSize();
          pd.outputSizeValue = pd.output.getSizeName();
          break;
        }
      }
      writeln(comma);
      write("        ");
      if (pd.isInput && pd.isOutput)
        write("ref ");
      else if (pd.isOutput)
        write("out ");
      else if (field.type.reference == Type.BYREF || field.type.reference == Type.BYPTR
          || field.type.reference == Type.BYREFPTR)
        write("ref ");
      pd.type = dropTee(field.type.netName(), module.name);
      pd.name = lowerFirst(field.name);
      write(pd.type);
      if (field.type.reference == Type.BYREFPTR && pd.type.compareTo("string") != 0)
      {
        pd.isArray = true;
        write("[] ");
      } else if ((pd.hasInputSize || pd.hasOutputSize) && pd.type.compareTo("string") != 0)
      {
        pd.isArray = true;
        write("[] ");
      } else
        write(" ");
      write(pd.name);
      parameterList.addElement(pd);
      comma = ",";
    }
    return hasReturn;
  }

  public static void generateCall(Module module, Prototype prototype, int no)
  {
    Parameter pd = new Parameter();
    boolean hasReturn = generateCommon(module, prototype, no, "public ");
    String sNo = "" + no;
    writeln(")");
    writeln(2, "{");
    writeln(3, "Init(" + (prototype.message.length() > 0 ? prototype.message : sNo) + ", \"" + module.name + "\", \""
        + prototype.name + "\");");
    writeln(3, "try");
    writeln(3, "{");
    String align8 = "false";
    if (alignForSun == true)
      align8 = "true";
    writeln(4, "Add(" + prototype.signature(true) + ", " + align8 + ");");
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      for (int j = 0; j < parameterList.size(); j++)
      {
        pd = (Parameter) parameterList.elementAt(j);
        if (pd.inputNo == i)
          break;
      }
      writeln(4, "Add(" + pd.name + ", " + align8 + ");");
    }
    writeln(4, "Call();");
    if (hasReturn)
    {
      writeln(4, "" + prototype.type.netName() + " result = new " + prototype.type.netName() + "();");
      writeln(4, "result = (" + prototype.type.netName() + ") Get(result, " + align8 + ");");
    }
    for (int i = 0; i < prototype.outputs.size(); i++)
    {
      for (int j = 0; j < parameterList.size(); j++)
      {
        pd = (Parameter) parameterList.elementAt(j);
        if (pd.outputNo == i)
          break;
      }
      if (pd.isOutput == true && pd.isInput == false)
      {
        if (pd.isArray)
        {
          writeln(4, "" + pd.name + " = new " + pd.type + "[" + lowerFirst(pd.outputSizeValue) + "];");
          writeln(4, "for (int i=0; i<" + lowerFirst(pd.outputSizeValue) + "; i++)");
          writeln(5, "" + pd.name + "[i] = new " + pd.type + "();");
        } else if (pd.type.compareTo("string") != 0)
          writeln(4, "" + pd.name + " = new " + pd.type + "();");
      }
      String pdType = pd.type + (pd.isArray == true ? "[]" : "");
      if (pd.type.compareTo("string") != 0)
        writeln(4, "" + pd.name + " = (" + pdType + ") Get(" + pd.name + ", " + align8 + ");");
      else
        writeln(4, "" + pd.name + "= GetString(" + pd.outputSizeValue + ", " + align8 + ");");
    }
    if (hasReturn)
      writeln(4, "return result;");
    writeln(3, "}");
    writeln(3, "catch (RpcException ex)");
    writeln(3, "{");
    writeln(4, "OnError(ex, (int)Message.INV_SIGNATURE);");
    writeln(4, "if (ex.ErrorCode != 0)");
    writeln(4, "{");
    writeln(5, "throw new RpcException(ex.ErrorCode,");
    writeln(6, "MessageDesc((Message)ex.ErrorCode)");
    writeln(7, "+ System.Environment.NewLine");
    writeln(7, "+ ex.Message,");
    writeln(6, "ex.InnerException);");
    writeln(4, "}");
    writeln(4, "throw; // else just rethrow");
    writeln(3, "}");
    writeln(3, "finally");
    writeln(3, "{");
    writeln(4, "Done();");
    writeln(3, "}");
    writeln(2, "}");
  }

  public static void generateInterfaces(Module module)
  {
    for (int i = 0; i < module.prototypes.size(); i++)
    {
      Prototype prototype = (Prototype) module.prototypes.elementAt(i);
      if (prototype.codeType != Prototype.RPCCALL)
        continue;
      generateInterface(module, prototype, i);
    }
  }

  public static void generateInterface(Module module, Prototype prototype, int no)
  {
    generateCommon(module, prototype, no, "");
    writeln(");");
  }

  public static void generateProxies(Module module)
  {
    for (int i = 0; i < module.prototypes.size(); i++)
    {
      Prototype prototype = (Prototype) module.prototypes.elementAt(i);
      if (prototype.codeType != Prototype.RPCCALL)
        continue;
      generateProxy(module, prototype, i);
    }
  }

  public static void generateProxy(Module module, Prototype prototype, int no)
  {
    Parameter pd = new Parameter();
    boolean hasReturn = generateCommon(module, prototype, no, "public ");
    writeln(")");
    writeln(2, "{");
    if (hasReturn == true)
      write("      return ");
    else
      write("      ");
    write("server." + prototype.name + "(");
    String comma = "";
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      pd = new Parameter();
      Field parameter = (Field) prototype.parameters.elementAt(i);
      pd.field = parameter;
      for (int j = 0; j < prototype.inputs.size(); j++)
      {
        pd.input = (Action) prototype.inputs.elementAt(j);
        if (pd.input.name.compareTo(parameter.name) == 0)
        {
          pd.isInput = true;
          pd.inputNo = j;
          pd.hasInputSize = pd.input.hasSize();
          pd.inputSizeValue = pd.input.getSizeName();
          break;
        }
      }
      for (int j = 0; j < prototype.outputs.size(); j++)
      {
        pd.output = (Action) prototype.outputs.elementAt(j);
        if (pd.output.name.compareTo(parameter.name) == 0)
        {
          pd.isOutput = true;
          pd.outputNo = j;
          pd.hasOutputSize = pd.output.hasSize();
          pd.outputSizeValue = pd.output.getSizeName();
          break;
        }
      }
      write(comma);
      if (pd.isInput && pd.isOutput)
        write("ref ");
      else if (pd.isOutput)
        write("out ");
      else if (parameter.type.reference == Type.BYREF || parameter.type.reference == Type.BYPTR
          || parameter.type.reference == Type.BYREFPTR)
        write("ref ");
      pd.name = lowerFirst(parameter.name);
      write(pd.name);
      parameterList.addElement(pd);
      comma = ", ";
    }
    writeln(");");
    writeln(2, "}");
  }

  public static String lowerFirst(String in)
  {
    String result = in.substring(0, 1).toLowerCase() + in.substring(1);
    return result;
  }

  public static String dropTee(String in, String modName)
  {
    String result = in;
    if (in.compareTo("char") == 0)
    {
      result = "string";
    } else if (keepTee == false)
    {
      if (result.charAt(0) == 't')
        result = result.substring(1);
    }
    return result;
  }

  public static void generateIDL2Python(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name + "Idl2.py");
      OutputStream outFile = new FileOutputStream(output + module.name + "Idl2.py");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("# This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("# Mutilation, Spindlization and Bending will result in ...");
        generateIDL2PythonCall(module);
      } finally
      {
        writer.flush();
        outFile.close();
      }
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  public static void generateIDL2PythonCall(Module module)
  {
    writeln();
    writeln("class Structure(object):");
    writeln(1, "__slots__  = ['name', 'codeType', 'fields']");
    writeln(1, "def __init__(self, name, codeType, fields):");
    writeln(2, "self.name = name");
    writeln(2, "self.codeType = codeType");
    writeln(2, "self.fields = fields");
    writeln();
    writeln("class Prototype(object):");
    writeln(1, "__slots__  = ['name', 'type', 'parameters']");
    writeln(1, "def __init__(self, name, type, parameters):");
    writeln(2, "self.name = name");
    writeln(2, "self.type = type");
    writeln(2, "self.parameters = parameters");
    writeln();
    writeln("class Field(object):");
    writeln(1, "__slots__ = ['name','type','isInput','isOutput']");
    writeln(1, "def __init__(self, name, type, isInput=False, isOutput=False):");
    writeln(2, "self.name = name");
    writeln(2, "self.type = type");
    writeln(2, "self.isInput = isInput");
    writeln(2, "self.isOutput = isOutput");
    writeln();
    writeln("class Type(object):");
    writeln(1, "__slots__ = ['name', 'typeof', 'reference']");
    writeln(1, "def __init__(self, name, typeof, reference):");
    writeln(2, "self.name = name");
    writeln(2, "self.typeof = typeof");
    writeln(2, "self.reference = reference");
    writeln();
    writeln("class Proc(object):");
    writeln(1, "__slots__ = ['name', 'struct', 'table', 'type', 'extended', 'input', 'output']");
    writeln(1, "def __init__(self, name, struct, table, type, extended, input, output):");
    writeln(2, "self.name = name");
    writeln(2, "self.struct = struct");
    writeln(2, "self.table = table");
    writeln(2, "self.type = type");
    writeln(2, "self.extended = extended");
    writeln(2, "self.input = input");
    writeln(2, "self.output = output");
    writeln();
    writeln("USERTYPE =  0");
    writeln("BOOLEAN  =  1");
    writeln("CHAR     =  2");
    writeln("SHORT    =  3");
    writeln("LONG     =  4");
    writeln("FLOAT    =  5");
    writeln("DOUBLE   =  6");
    writeln("VOID     =  7");
    writeln("BYTE     =  8");
    writeln("INT      =  9");
    writeln("STRING   = 10");
    writeln();
    writeln("BYVAL    =  1");
    writeln("BYPTR    =  2");
    writeln("BYREF    =  3");
    writeln("BYPTRPTR =  4");
    writeln("BYREFPTR =  5");
    writeln("ARRAYED  =  6");
    writeln();
    writeln("SINGLE   =  1");
    writeln("MULTIPLE =  2");
    writeln("ACTION   =  3");
    writeln();
    writeln("NORMAL   =  0");
    writeln("PUBLIC   =  1");
    writeln("PRIVATE  =  2");
    writeln("PROTECTED=  3");

    writeln();
    writeln("structures = []");
    writeln("prototypes = []");
    writeln("procs      = []");
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.fields.size() == 0)
        continue;
      generateStructure(structure);
    }
    for (int i = 0; i < module.prototypes.size(); i++)
    {
      Prototype prototype = (Prototype) module.prototypes.elementAt(i);
      if (prototype.codeType != Prototype.RPCCALL)
        continue;
      generatePrototype(prototype);
    }
  }

  public static void generateStructure(Structure structure)
  {
    writeln();
    write("structures.append(Structure('" + structure.name + "', " + structType(structure.codeType) + ",");
    if (structure.fields.size() == 0)
    {
      writeln(" [] ) )");
      return;
    }
    writeln();
    String square = "[";
    for (int i = 0; i < structure.fields.size(); i++)
    {
      Field field = (Field) structure.fields.elementAt(i);
      writeln(
          "  " + square + " Field('" + field.name + "', Type('" + field.type.name + "', " + fieldType(field.type.typeof)
              + ", " + refType(field.type.reference) + "))" + (i + 1 < structure.fields.size() ? "," : " ] ) )"));
      square = " ";
    }
  }

  public static void generatePrototype(Prototype prototype)
  {
    writeln();
    write("prototypes.append(Prototype('" + prototype.name + "',");
    write(" Type('" + prototype.type.name + "', " + fieldType(prototype.type.typeof) + ", "
        + refType(prototype.type.reference) + "), ");
    if (prototype.parameters.size() == 0)
    {
      writeln(" [] ) )");
      return;
    }
    writeln();
    String square = "[";
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field field = (Field) prototype.parameters.elementAt(i);
      writeln(1, "" + square + " Field('" + field.name + "', Type('" + field.type.name + "', "
          + fieldType(field.type.typeof) + ", " + refType(field.type.reference) + "), "
          + (field.isInput == true ? "True" : "False") + ", " + (field.isOutput == true ? "True" : "False") + ")"
          + (i + 1 < prototype.parameters.size() ? "," : " ] ) )"));
      square = " ";
    }
  }

  public static String fieldType(int no)
  {
    switch (no)
    {
    case 0:
      return "USERTYPE";
    case 1:
      return "BOOLEAN";
    case 2:
      return "CHAR";
    case 3:
      return "SHORT";
    case 4:
      return "LONG";
    case 5:
      return "FLOAT";
    case 6:
      return "DOUBLE";
    case 7:
      return "VOID";
    case 8:
      return "BYTE";
    case 9:
      return "INT";
    case 10:
      return "STRING";
    }
    return "???";
  }

  public static String refType(int no)
  {
    switch (no)
    {
    case 1:
      return "BYVAL";
    case 2:
      return "BYPTR";
    case 3:
      return "BYREF";
    case 4:
      return "BYPTRPTR";
    case 5:
      return "BYREFPTR";
    case 6:
      return "ARRAYED";
    }
    return "???";
  }

  public static String structType(int no)
  {
    switch (no)
    {
    case 0:
      return "NORMAL";
    case 1:
      return "PUBLIC";
    case 2:
      return "PRIVATE";
    case 3:
      return "PROTECTED";
    }
    return "???";
  }
}