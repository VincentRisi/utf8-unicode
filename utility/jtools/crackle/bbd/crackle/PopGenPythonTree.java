package bbd.crackle;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Vector;

public class PopGenPythonTree extends Generator
{
  public static String description()
  {
    return "Generates Python Tree Code";
  }
  public static String documentation()
  {
    return "Generates Python Tree Code";
  }
  /**
  * Reads input from stored repository
  */
  public static void main(String[] args)
  {
    try
    {
      PrintWriter outLog = new PrintWriter(System.out);
      for (int i = 0; i < args.length; i++)
      {
        outLog.println(args[i] + ": Generate ... ");
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
  private static PrintWriter _outLog;
  /**
  * Generates
  */
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    _outLog = outLog;
    outLog.println(module.name + " version " + module.version);
    generate_tree(module, output, outLog);
  }
  public PopGenPythonTree()
  {
  }
  private static void generate_field(Field field, String pId)
  {
    writeln(pId + ".name = '" + field.name + "'");
    writeln(pId + ".type = Class()");
    writeln(pId + ".type.name = '" + field.type.name + "'");
    writeln(pId + ".type.typeof = " + field.type.typeof);
    writeln(pId + ".type.reference = " + field.type.reference);
    writeln(pId + ".type.isUnsigned = " + (field.type.isUnsigned ? "True" : "False"));
    writeln(pId + ".type.arraySizes = []");
    for (int i = 0; i < field.type.arraySizes.size(); i++)
    {
      Integer no = (Integer)field.type.arraySizes.elementAt(i);
      writeln(pId + ".type.arraySizes.append(" + no.intValue() + ")");
    }
    if (field.input != null)
    {
      writeln(pId + ".input = Class()");
      writeln(pId + ".input.name = '" + field.input.name + "'");
      writeln(pId + ".input.operations = []");
      for (int i = 0; i < field.input.operations.size(); i++)
      {
        Operation operation = (Operation)field.input.operations.elementAt(i);
        writeln("_op = Class()");
        writeln(pId + ".input.operations.append(_op)");
        writeln("_op.name = '" + operation.name + "'");
        writeln("_op.code = " + operation.code);
        writeln("_op.isConstant = " + (operation.isConstant ? "True" : "False"));
        if (operation.field != null)
          generate_field(operation.field, "_op");
        else
          writeln("_op.field = None");
      }
    }
    else
      writeln(pId + ".input = None");
    if (field.output != null)
    {
      writeln(pId + ".output = Class()");
      writeln(pId + ".output.name = '" + field.output.name + "'");
      writeln(pId + ".output.operations = []");
      for (int i = 0; i < field.output.operations.size(); i++)
      {
        Operation operation = (Operation)field.output.operations.elementAt(i);
        writeln("_op = Class()");
        writeln(pId + ".output.operations.append(_op)");
        writeln("_op.name = '" + operation.name + "'");
        writeln("_op.code = " + operation.code);
        writeln("_op.isConstant = " + (operation.isConstant ? "True" : "False"));
        if (operation.field != null)
          generate_field(operation.field, "_op");
        else
          writeln("_op.field = None");
      }
    }
    else
      writeln(pId + ".output = None");
    writeln(pId + ".isInput = " + (field.isInput ? "True" : "False"));
    writeln(pId + ".isOutput = " + (field.isOutput ? "True" : "False"));
    writeln(pId + ".hasSize = " + (field.hasSize ? "True" : "False"));
  }
  private static String fix(String line)
  {
    String result = line.replaceAll("\r","").replaceAll("\n", "");
    if (result.contains("'"))
      return format("\"\"\"%s\"\"\"", result);
    return format("'''%s'''", result);
  }
  private static void generate_tree(Module module, String output, PrintWriter outLog)
  {
    try
    {
      String sourceName = module.name.toLowerCase() + "_tree";
      outLog.println("Code: " + output + sourceName + ".py");
      OutputStream outFile = new FileOutputStream(output + sourceName + ".py");
      writer = new PrintWriter(outFile);
      try
      {
        writeln("STRUCTURE_TYPE = {0:'NORMAL',1:'PUBLIC',2:'PRIVATE',3:'PROTECTED'}");
        writeln("FIELD_TYPE = {0:'USERTYPE',1:'BOOLEAN',2:'CHAR',3:'SHORT',4:'LONG',5:'FLOAT',6:'DOUBLE',7:'VOID',8:'BYTE',9:'INT',10:'STRING'}");
        writeln("REFERENCE_TYPE = {0:'BYVAL',1:'BYPTR',2:'BYREF',3:'BYPTRPTR',4:'BYREFPTR',5:'ARRAYED'}");
        writeln("PROTOTYPE_TYPE = {0:'RPCCALL',1:'PUBLIC',2:'PRIVATE',3:'PROTECTED'}");
        writeln("OPERATION_TYPE = {1:'SIZE',2:'DYNAMIC'}");
        writeln();
        writeln("class Class: pass");
        writeln("module = Class()");
        writeln("module.sourceName= r'" + module.sourceName + "'");
        writeln("module.name = '" + module.name + "'");
        writeln("module.version = '" + module.version + "'");
        writeln("module.packageName = '" + module.packageName + "'");
        writeln("module.signature = " + module.signature);
        writeln("module.countOfHashes = " + module.countOfHashes);
        writeln("module.messageBase = " + module.messageBase);
        writeln("module.codeLine = " + module.codeLine);
        writeln("module.codeStart = " + module.codeStart);
        writeln("module.messageStart= " + module.messageStart);
        writeln("module.messages = []");
        for (int i = 0; i < module.messages.size(); i++)
        {
          Message message = (Message)module.messages.elementAt(i);
          writeln();
          writeln("_i = Class()");
          writeln("module.messages.append(_i)");
          writeln("_i.name = '" + message.name + "'");
          writeln("_i.value = '" + message.value + "'");
        }
        writeln("module.tables = []");
        for (int i = 0; i < module.tables.size(); i++)
        {
          Table table = (Table)module.tables.elementAt(i);
          writeln();
          writeln("_i = Class()");
          writeln("module.tables.append(_i)");
          writeln("_i.name = '" + table.name + "'");
          writeln("_i.messages = []");
          for (int j = 0; j < table.messages.size(); j++)
          {
            Message message = (Message)table.messages.elementAt(j);
            writeln("_j = Class()");
            writeln("_i.messages.append(_j)");
            writeln("_j.name = '" + message.name + "'");
            writeln("_j.value = '" + message.value + "'");
          }
        }
        writeln("module.structures = []");
        for (int i = 0; i < module.structures.size(); i++)
        {
          Structure structure = (Structure)module.structures.elementAt(i);
          writeln();
          writeln("_i = Class()");
          writeln("module.structures.append(_i)");
          writeln("_i.name = '" + structure.name + "'");
          writeln("_i.header = '" + structure.header + "'");
          writeln("_i.categories = []");
          writeln("_i.fields = []");
          writeln("_i.codeType = " + structure.codeType);
          writeln("_i.code = []");
          writeln("_i.codeLine = " + structure.codeLine);
          writeln("_i.start = " + structure.start);
          for (int j = 0; j < structure.categories.size(); j++)
          {
            String category = (String)structure.categories.elementAt(j);
            writeln("_i.categories.append('" + category + "'");
          }
          for (int j = 0; j < structure.fields.size(); j++)
          {
            Field field = (Field)structure.fields.elementAt(j);
            writeln("_j = Class()");
            writeln("_i.fields.append(_j)");
            generate_field(field, "_j");
          }
          for (int j = 0; j < structure.code.size(); j++)
          {
            String code = (String)structure.code.elementAt(j);
            writeln("_i.code.append(" + fix(code) +")");
          }
          writeln("_i.python = []");
          for (int j = 0; j < structure.python.size(); j++)
          {
            String code = (String)structure.python.elementAt(j);
            writeln("_i.python.append(" + fix(code) +")");
          }
        }
        writeln("module.enumerators = []");
        for (int i = 0; i < module.enumerators.size(); i++)
        {
          Enumerator enumerator = (Enumerator)module.enumerators.elementAt(i);
          writeln();
          writeln("_i = Class()");
          writeln("module.enumerators.append(_i)");
          writeln("_i.name = '" + enumerator.name + "'");
          writeln("_i.elements = []");
          for (int j = 0; j < enumerator.elements.size(); j++)
            writeln("_i.elements.append(\"\"\"" + enumerator.elements.elementAt(j) + "\"\"\")");
        }
        writeln("module.prototypes = []");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype)module.prototypes.elementAt(i);
          writeln();
          writeln("_i = Class()");
          writeln("module.prototypes.append(_i)");
          writeln("_i.name = '" + prototype.name + "'");
          writeln("_i.message = '" + prototype.message + "'");
          writeln("_i.type = Class()");
          writeln("_i.type.name = '" + prototype.type.name + "'");
          writeln("_i.type.typeof = " + prototype.type.typeof);
          writeln("_i.type.reference = " + prototype.type.reference);
          writeln("_i.type.isUnsigned = " + (prototype.type.isUnsigned ? "True" : "False"));
          writeln("_i.type.arraySizes = []");
          for (int j = 0; j < prototype.type.arraySizes.size(); j++)
          {
            Integer no = (Integer)prototype.type.arraySizes.elementAt(j);
            writeln("_i.type.arraySizes.append(" + no.intValue() + ")");
          }
          writeln("_i.parameters = []");
          for (int j = 0; j < prototype.parameters.size(); j++)
          {
            Field field = (Field)prototype.parameters.elementAt(j);
            writeln("_j = Class()");
            writeln("_i.parameters.append(_j)");
            generate_field(field, "_j");
          }
          writeln("_i.inputs = []");
          for (int j = 0; j < prototype.inputs.size(); j++)
          {
            Action action = (Action)prototype.inputs.elementAt(j);
            writeln("_j = Class()");
            writeln("_i.inputs.append(_j)");
            writeln("_j.name = '" + action.name + "'");
            writeln("_j.operations = []");
            for (int k = 0; k < action.operations.size(); k++)
            {
              Operation operation = (Operation)action.operations.elementAt(k);
              writeln("_op = Class()");
              writeln("_j.operations.append(_op)");
              writeln("_op.name = '" + operation.name + "'");
              writeln("_op.code = " + operation.code);
              writeln("_op.isConstant = " + (operation.isConstant ? "True" : "False"));
              if (operation.field != null)
                generate_field(operation.field, "_op");
              else
                writeln("_op.field = None");
            }
          }
          writeln("_i.outputs = []");
          for (int j = 0; j < prototype.outputs.size(); j++)
          {
            Action action = (Action)prototype.outputs.elementAt(j);
            writeln("_j = Class()");
            writeln("_i.outputs.append(_j)");
            writeln("_j.name = '" + action.name + "'");
            writeln("_j.operations = []");
            for (int k = 0; k < action.operations.size(); k++)
            {
              Operation operation = (Operation)action.operations.elementAt(k);
              writeln("_op = Class()");
              writeln("_j.operations.append(_op)");
              writeln("_op.name = '" + operation.name + "'");
              writeln("_op.code = " + operation.code);
              writeln("_op.isConstant = " + (operation.isConstant ? "True" : "False"));
              if (operation.field != null)
                generate_field(operation.field, "_op");
              else
                writeln("_op.field = None");
            }
          }
          writeln("_i.categories = []");
          for (int j = 0; j < prototype.categories.size(); j++)
          {
            String category = (String)prototype.categories.elementAt(j);
            writeln("_i.categories.append('" + category + "'");
          }
          writeln("_i.codeType = " + prototype.codeType);
          writeln("_i.code = []");
          for (int j = 0; j < prototype.code.size(); j++)
          {
            String code = (String)prototype.code.elementAt(j);
            writeln("_i.code.append(" + fix(code) +")");
          }
          writeln("_i.codeLine = " + prototype.codeLine);
          writeln("_i.start = " + prototype.start);
          writeln("_i.python = []");
          for (int j = 0; j < prototype.python.size(); j++)
          {
            String code = (String)prototype.python.elementAt(j);
            writeln("_i.python.append(" + fix(code) +")");
          }
        }
        writeln("module.pragmas = []");
        for (int i = 0; i < module.pragmas.size(); i++)
        {
          String pragma = (String)module.pragmas.elementAt(i);
          writeln("module.pragmas.append(" + fix(pragma) +")");
        }
        writeln("module.code = []");
        for (int i = 0; i < module.code.size(); i++)
        {
          String code = (String)module.code.elementAt(i);
          writeln("module.code.append(" + fix(code) +")");
        }
        writeln("module.python = []");
        for (int i = 0; i < module.python.size(); i++)
        {
          String code = (String)module.python.elementAt(i);
          writeln("module.python.append(" + fix(code) +")");
        }
        writeln("module.imports = []");
        for (int i = 0; i < module.imports.size(); i++)
        {
          String imp = (String)module.imports.elementAt(i);
          writeln("module.imports.append(" + fix(imp) +")");
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
    }
    catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }
}


