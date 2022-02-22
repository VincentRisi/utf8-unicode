package bbd.crackle.ubi;

import bbd.crackle.*;
import bbd.crackle.Module;
import bbd.crackle.http.Argument;

import java.io.*;
import java.util.ArrayList;
import java.util.Vector;

public class PopUbi3Bind11 extends Generator
{
  private static final boolean align8byte = true;
  private static PrintWriter errLog;
  private static final boolean simpleBuild = false;
  private static boolean hasInput, hasOutput, hasReturn;
  private static boolean hasBuilder = false;

  public static String description()
  {
    return "Generates Ubi3 Client C Dll and C++ class Code";
  }

  public static String documentation()
  {
    return "Generates Ubi3 Client Code";
  }

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
    outLog.println(module.name + " version " + module.version);
    indent_size = 4;
    generateCModule(module, output, outLog);
  }

  private static void generateFuncDocs(Module module, Prototype prototype, StringBuilder stringBuilder)
  {
    stringBuilder.append("/**\n");
    hasInput = prototype.inputs.size() > 0;
    if (hasInput)
    {
      stringBuilder.append(" * INPUT:\n");
      for (int i = 0; i < prototype.inputs.size(); i++)
      {
        Action input = (Action) prototype.inputs.elementAt(i);
        Operation op = input.sizeOperation();
        stringBuilder.append(format(" *   %s %s\n", input.name, (op != null) ? op.name : ""));
      }
    }
    hasOutput = prototype.outputs.size() > 0;
    if (hasOutput)
    {
      stringBuilder.append(" * OUTPUT:\n");
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Operation op = output.sizeOperation();
        stringBuilder.append(format(" *   %s %s\n", output.name, (op != null) ? op.name : ""));
      }
    }
    String typeName = prototype.type.cNameUbi();
    hasReturn = typeName.contentEquals("void") == false;
    stringBuilder.append(format(" * RETURNS: %s\n", typeName));
    stringBuilder.append(" */\n");
  }

  private static void generateFuncCode(Module module, Prototype prototype, int no, ArrayList<String> mdefs)
  {
    StringBuilder stringBuilder = new StringBuilder();
    generateFuncDocs(module, prototype, stringBuilder);
    if (simpleBuild)
    {
      generateFuncSimpleCode(module, prototype, stringBuilder, mdefs);
      writeln(stringBuilder.toString());
      return;
    }
    writeln(stringBuilder.toString());
    generateFuncMarshalledCode(module, prototype, mdefs, no);
  }

  private static void makeValueRec(String name, int no)
  {
    if (hasBuilder == false)
    {
      writeln(no, "DataBuilderJson _builder_;");
      hasBuilder = true;
    }
    writeln(no, "Value _" + name + "_out_;");
    writeln(no, "_builder_.clear();");
    writeln(no, "" + name + ".BuildData(_builder_);");
    writeln(no, "string _" + name + "_result_;");
    writeln(no, "_builder_.getRecord(_" + name + "_result_);");
    writeln(no, "stringstream _" + name + "_(_" + name + "_result_);");
    writeln(no, "_" + name + "_ >> _" + name + "_out_;");
  }

  private static void genSplitsPySet(Field field, int no)
  {
    switch (field.type.typeof)// == Type.CHAR)
    {
      case Type.CHAR:
      case Type.STRING:
        writeln(1, format("char %s[256];", field.name));
        writeln(1, format("in_data.toch(%s, 256, %d);", field.name, no));
        break;
      case Type.BYTE:
      case Type.BOOLEAN:
      case Type.SHORT:
        writeln(1, format("int16 %s;", field.name));
        writeln(1, format("in_data.tos(&%s, %d);", field.name, no));
        break;
      case Type.INT:
        writeln(1, format("int32 %s;", field.name));
        writeln(1, format("in_data.toi(&%s, %d);", field.name, no));
        break;
      case Type.LONG:
        writeln(1, format("int64 %s;", field.name));
        writeln(1, format("in_data.tol(&%s, %d);", field.name, no));
        break;
      case Type.FLOAT:
      case Type.DOUBLE:
        writeln(1, format("double %s;", field.name));
        writeln(1, format("in_data.tof(&%s, %d);", field.name, no));
        break;
    }
    writeln(1, format("_input_value_[\"%1$s\"] = %1$s;", field.name));
  }

  private static String inout(Field field)
  {
    if (field.isOutput)
      return "&" + field.name;
    return field.name;
  }

  private static void genSplitsSet(Vector<Argument> input_args, Field field, int no)
  {
    switch (field.type.typeof)// == Type.CHAR)
    {
      case Type.CHAR:
      case Type.STRING:
        writeln(1, format("char %s[256];", field.name));
        writeln(1, format("in_data.toch(%s, 256, %d);", field.name, no));
        input_args.addElement(new Argument(field, field.name));
        break;
      case Type.BYTE:
      case Type.BOOLEAN:
      case Type.SHORT:
        writeln(1, format("int16 %s;", field.name));
        writeln(1, format("in_data.tos(&%s, %d);", field.name, no));
        input_args.addElement(new Argument(field, inout(field)));
        break;
      case Type.INT:
        writeln(1, format("int32 %s;", field.name));
        writeln(1, format("in_data.toi(&%s, %d);", field.name, no));
        input_args.addElement(new Argument(field, inout(field)));
        break;
      case Type.LONG:
        writeln(1, format("int64 %s;", field.name));
        writeln(1, format("in_data.tol(&%s, %d);", field.name, no));
        input_args.addElement(new Argument(field, inout(field)));
        break;
      case Type.FLOAT:
      case Type.DOUBLE:
        writeln(1, format("double %s;", field.name));
        writeln(1, format("in_data.tof(&%s, %d);", field.name, no));
        input_args.addElement(new Argument(field, inout(field)));
        break;
    }
  }

  private static void genSplitsInput(Module module, Prototype prototype, Vector<Argument> input_args)
  {
    int no = 0;
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field = input.getParameter(prototype);
      if (field == null)
        continue;
      no++;
    }
    writeln(1, format("Splitter in_data(%d);", no));
    writeln(1, "in_data.read(input.c_str(), '|');");
    no = 0;
    for (int i = 0; i < prototype.inputs.size(); i++)
    {
      Action input = (Action) prototype.inputs.elementAt(i);
      Field field = input.getParameter(prototype);
      if (field == null)
        continue;
      genSplitsSet(input_args, field, no);
      no++;
    }
  }

  private static void generateFuncMarshalledCode(Module module, Prototype prototype, ArrayList<String> mdefs, int no)
  {
    generateMarshallMDef(module, prototype, mdefs);
    hasBuilder = false;
    StringBuilder stringBuilder = new StringBuilder();
    int noInputs = 0;
    boolean hasInput;
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
      noInputs++;
      break;
    }
    hasInput = noInputs > 0;
    if (prototype.type.typeof != Type.VOID)
    {
      if (prototype.type.reference != Type.BYPTR)
        hasResult = true;
      else
        hasOutput = true;
    }
    if (prototype.outputs.size() > 0)
      hasOutput = true;
    stringBuilder.append(format("static void _%s(", prototype.name));
    String comma = "";
    if (hasInput)
    {
      stringBuilder.append("const string _input_");
      comma = ", ";
    }
    if (hasOutput || hasReturn)
    {
      stringBuilder.append(format("%sstring &_output_", comma));
    }
    stringBuilder.append(")");
    writeln(stringBuilder.toString());
    //writeln("void t" + module.name + "Server_http::" + prototype.name + "(string input, string &output)");
    writeln("{");
    if (hasResult)
      writeln(1, "int32 _returnCode_;");
    if (hasInput)
      genInput(module, prototype, input_args);
    if (hasOutput)
      genOutput(module, prototype, output_args);
    String w1 = "";
    write("    ");
    if (hasResult)
      writer.print("_returnCode_ = ");
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
      writeln(1, "Value _output_value_;");
    if (hasResult == true)
      writeln(1, "_output_value_[\"returnCode\"] = _returnCode_;");
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
            writeln(1, format("Value _%s_array_;", field.name));
            writeln(1, format("for (int _o%2$d_=0; _o%2$d_ < %1$s; _o%2$d_++)", op.name, i));
            writeln(1, "{");
            writeln(2, format("%s &rec = %s.arr[_o%d_];", name_of(module, field.type.name), field.name, i));
            makeValueRec("rec", 2);
            writeln(2, format("_%s_array_[_o%d_] = rec_out;", field.name, i));
            writeln(1, "}");
            writeln(1, format("_output_value_[\"%1$s\"] = _%1$s_array_;", field.name));
          } else if (field.type.typeof == Type.CHAR)
          {
            writeln(1, "_output_value_[\"" + field.name + "\"] = " + field.name + ".arr;");
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
              writeln(1, "_output_value_[\"" + field.name + "\"] = " + field.name + "_out;");
            } else
            {
              writeln(1, "_output_value_[\"" + field.name + "\"] = " + field.name + ";");
            }
            continue;
          }
          if (field.type.typeof == Type.USERTYPE)
          {
            writeln(1, format("Value _%s_outarray_;", field.name));
            writeln(1, format("for (int _o%2$d_=0; _o%2$d_ < %1$s; _o%2$d_++)", op.name, i));
            writeln(1, "{");
            writeln(2, format("%s &rec = %s.arr[_o%d_];", name_of(module, field.type.name), field.name, i));
            makeValueRec("rec", 2);
            writeln(2, format("_%s_outarray_[_o%d_] = rec_out;", field.name, i));
            writeln(1, "}");
            writeln(1, format("_output_value_[\"%1$s\"] = _%1$s_outarray_;", field.name));
          } else if (field.type.typeof == Type.CHAR)
          {
            writeln(1, "_output_value_[\"" + field.name + "\"] = " + field.name + ".arr;");
          } else
          {
            writeln(1, "// " + field.name + " as simple array;");
          }
          continue;
        }
        writeln(1, "_output_value_[\"" + field.name + "\"] = " + field.name + ";");
      }
    }
    if (hasOutput == true || hasResult == true)
    {
      writeln(1, "StreamWriterBuilder _swb_;");
      writeln(1, "_output_ = writeString(_swb_, _output_value_);");
    }
    writeln("}");
    writeln();
  }

  private static void genInput(Module module, Prototype prototype, Vector<Argument> input_args)
  {
    //genValidateInputString(prototype);
    writeln(1, "Value _input_value_;");
    writeln(1, "stringstream sstr(_input_);");
    writeln(1, "sstr >> _input_value_;");
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

  private static void generateMarshallMDef(Module module, Prototype prototype, ArrayList<String> mdefs)
  {
    StringBuilder mdef = new StringBuilder();
    mdef.append(format("%2$sm.def(\"%1$s\", &_%1$s", prototype.name, indent(1)));
    String comma = "";
    if (hasInput)
    {
      mdef.append("\"_input_\"_a");
      comma = ", ";
    }
    if (hasOutput)
    {
      mdef.append(format("%s\"_output_\"_a", comma));
    }
    mdef.append(");");
    mdefs.add(mdef.toString());
  }

  private static void genInputStd(Vector<Argument> input_args, Field field, String fieldTypeName)
  {
    if (field.type.typeof == Type.CHAR)
    {
      writeln(1, format("string %s;", field.name));
      writeln(1, format("%1$s = _input_value_[\"%1$s\"]%2$s;", field.name, asType(field)));
      input_args.addElement(new Argument(field, field.name));
    } else if (field.type.typeof == Type.USERTYPE)
    {
      if (hasBuilder == false)
      {
        writeln(1, "DataBuilderJson _builder_;");
        hasBuilder = true;
      }
      writeln(1, format("%s %s;", fieldTypeName, field.name));
      writeln(1, format("Value _%1$s_value_ = _input_value_[\"%1$s\"];", field.name));
      writeln(1, format("_builder_.setValue(_%s_value_);", field.name));
      writeln(1, format("%s.SetData(_builder_);", field.name));
      input_args.addElement(new Argument(field, field.name));
    } else
    {
      writeln(1, format("%s %s;", fieldTypeName, field.name));
      writeln(1, format("%1$s = _input_value_[\"%1$s\"]%2$s;", field.name, asType(field)));
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
        writeln(1, format("%1$s = _input_value_[\"%1$s\"]%2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "(char *)" + field.name + ".c_str()"));
      } else if (field.type.typeof == Type.USERTYPE)
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("Value _%1$s_value_ = _input_value_[\"%1$s\"];", field.name));
        writeln(1, format("builder.setValue(_%s_value_);", field.name));
        writeln(1, format("%s.SetData(builder);", field.name));
        input_args.addElement(new Argument(field, "&" + field.name));
      } else // other std type
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("%1$s = _input_value_[\"%1$s\"]%2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "&" + field.name));
      }
    } else // has size
    {
      if (field.type.typeof == Type.CHAR)
      {
        writeln(1, format("string %s;", field.name));
        writeln(1, format("%1$s = _input_value_[\"%1$s\"]%2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, "(char *)" + field.name + ".c_str()"));
      } else if (field.type.typeof == Type.USERTYPE)
      {
        if (op != null && op.name != null)
        {
          String arrayName = format("InpArray%d", i);
          makeAutoArray(field, fieldTypeName, arrayName, op.name);
          writeln(1, format("Value %1$s_array = _input_value_[\"%1$s\"];", field.name));
          writeln(1, format("for (int i%2$d = 0; i%2$d < %1$s; i%2$d++)", op.name, i));
          writeln(1, "{");
          writeln(2, format("Value _%1$s_value_ = %1$s_array[i%2$d];", field.name, i));
          writeln(2, "builder.clear();");
          writeln(2, format("builder.setValue(_%1$s_value_);", field.name));
          writeln(2, format("%1$s.arr[i%2$d].SetData(builder);", field.name, i));
          writeln(1, "}");
          input_args.addElement(new Argument(field, field.name + ".arr"));
        } else
        {
          writeln(1, format("//not handled byptr has size %s %d", field.name, field.type.typeof));
          writeln(1, format("//not handled byptr %s as list %s no op.name", fieldTypeName, input.name));
        }
      } else // other std type
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("%1$s = _input_value_[\"%1$s\"]%2$s;", field.name, asType(field)));
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
        writeln(1, format("%1$s = _input_value_[\"%1$s\"] %2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, format("(char *)%s.c_str()", field.name)));
      } else if (field.type.typeof == Type.USERTYPE)
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("Value %1$s_array = _input_value_[\"%1$s\"];", field.name));
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
        writeln(1, format("%1$s = _input_value_[\"%1$s\"] %2$s;", field.name, asType(field)));
        input_args.addElement(new Argument(field, format("(char *)%s.c_str()", field.name)));
      } else if (field.type.typeof == Type.USERTYPE)
      {
        writeln(1, format("//not handled byrefptr input unexpected %s %s", fieldTypeName, field.name));
      } else // other std type
      {
        writeln(1, format("%s %s;", fieldTypeName, field.name));
        writeln(1, format("%1$s = _input_value_[\"%1$s\"] %2$s;", field.name, asType(field)));
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


  private static void setInput(Field field, StringBuilder stringBuilder)
  {
    String mod = "";
    String type = "";
    byte reference = field.type.reference;
    if (reference == Type.BYPTR)
      mod = "*";
    else if (reference == Type.BYREF)
      mod = "&";
    else if (reference == Type.BYREFPTR)
      mod = "*&";
    byte typeof = field.type.typeof;
    switch (typeof)
    {
      case Type.CHAR:
        type = "char";
        break;
      case Type.STRING:
        type = "char";
        break;
      case Type.WCHAR:
        type = "wchar";
        break;
      case Type.BYTE:
        type = "int8";
        break;
      case Type.SHORT:
        type = "int16";
        break;
      case Type.INT:
      case Type.BOOLEAN:
        type = "int32";
        break;
      case Type.LONG:
        type = "int64";
        break;
      case Type.FLOAT:
      case Type.DOUBLE:
        type = "double";
        break;
      case Type.VOID:
        type = "void";
        break;
      case Type.USERTYPE:
      default:
        type = field.type.name;
        break;
    }
    stringBuilder.append(format("%s%s%s %4$s = _input_[\"%4$s\"];\n", indent(1), type, mod, field.name));
  }


  private static void setOutput(Field field, StringBuilder stringBuilder)
  {
    String mod = "";
    String type = "";
    byte reference = field.type.reference;
    if (reference == Type.BYPTR)
      mod = "*";
    else if (reference == Type.BYREF)
      mod = "&";
    else if (reference == Type.BYREFPTR)
      mod = "*&";
    byte typeof = field.type.typeof;
    switch (typeof)
    {
      case Type.CHAR:
        type = "char";
        break;
      case Type.STRING:
        type = "char";
        break;
      case Type.WCHAR:
        type = "wchar";
        break;
      case Type.BYTE:
        type = "int8";
        break;
      case Type.SHORT:
        type = "int16";
        break;
      case Type.INT:
      case Type.BOOLEAN:
        type = "int32";
        break;
      case Type.LONG:
        type = "int64";
        break;
      case Type.FLOAT:
      case Type.DOUBLE:
        type = "double";
        break;
      case Type.VOID:
        type = "void";
        break;
      case Type.USERTYPE:
      default:
        type = field.type.name;
        break;
    }
    stringBuilder.append(format("%s%s%s %s;\n", indent(1), type, mod, field.name));
  }

  private static void setOutputSet(Field field, StringBuilder stringBuilder)
  {
    byte reference = field.type.reference;
    byte typeof = field.type.typeof;
  }

  private static void generateSimpleCall(Module module, Prototype prototype, StringBuilder stringBuild, ArrayList<String> mdefs)
  {
    //builder.append(format("%s// call\n", indent(1)));
    StringBuilder mdef = new StringBuilder();
    mdef.append(format("%2$sm.def(\"%1$s\", &_%1$s", prototype.name, indent(1)));
    stringBuild.append(indent(1));
    String returnType = prototype.type.cNameUbi();
    boolean hasReturn = returnType.equals("void") == false;
    if (hasReturn)
      stringBuild.append(format("%s _result = ", returnType));
    stringBuild.append(format("client.%s(", prototype.name));
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      if (i > 0)
        stringBuild.append(", ");
      Field parameter = (Field) prototype.parameters.elementAt(i);
      //builder.append(parameter.type.cDefUbi(parameter.name));
      stringBuild.append(parameter.name);
      mdef.append(format(", \"%s\"_a", parameter.name));
    }
    stringBuild.append(");\n");
    if (hasReturn)
      stringBuild.append(format("%sreturn _result;\n", indent(1)));
    mdef.append(");");
    mdefs.add(mdef.toString());
  }

  private static void generateFuncSimpleCode(Module module, Prototype prototype, StringBuilder stringBuilder, ArrayList<String> mdefs)
  {
    stringBuilder.append(format("static %s _%s(", prototype.type.cNameUbi(), prototype.name));
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      if (i > 0)
        stringBuilder.append(", ");
      Field parameter = (Field) prototype.parameters.elementAt(i);
      stringBuilder.append(parameter.type.cDefBind11(parameter.name));
    }
    stringBuilder.append("{\n");
    generateSimpleCall(module, prototype, stringBuilder, mdefs);
    stringBuilder.append("}\n");
  }

  /**
   * Sets up the writer and generates the general stuff
   */
  private static void generateCModule(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println(format("Code: %sMod.cpp", output + module.name));
      OutputStream outFile = new FileOutputStream(format("%sMod.cpp", output + module.name));
      writer = new PrintWriter(outFile);
      try
      {
        writeln("// This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("// Mutilation, Spindlization and Bending will result in ...");
        writeln("#include <stdio.h>");
        writeln("#include \"machine.h\"");
        writeln(format("#include \"%sclient.h\"", module.name));
        writeln("#include \"pybind11/pybind11.h\"");
        writeln("#include \"Python.h\"");
        writeln("#include \"databuilderjp.h\"");
        writeln("#include \"csvbuilder.h\"");
        writeln();
        writeln("namespace py = pybind11;");
        writeln("using namespace pybind11::literals;");
        writeln();
        writeln(format("static T%s client;", module.name));
        //writeln(format("static DataBuilderJson builder;"));
        writeln();
        writeln("static void _logon(char* service, char* host, int32 timeout=150000)");
        writeln("{");
        writeln("    client.Logon(service, host, timeout);");
        writeln("}");
        writeln();
        writeln("static void _logoff()");
        writeln("{");
        writeln("    client.Logoff();");
        writeln("}");
        writeln();
        ArrayList<String> mdefs = new ArrayList<String>();
        mdefs.add(format("PYBIND11_MODULE(%sMod, m)", module.name));
        mdefs.add("{");
        mdefs.add(format("    m.doc() = \"pybind11 %s client\";", module.name));
        mdefs.add("    m.def(\"logon\", &_logon, \"Connect to server\", \"service\"_a, \"host\"_a, \"timeout\"_a);");
        mdefs.add("    m.def(\"logoff\", &_logoff, \"Disconnect from server\");");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          generateFuncCode(module, prototype, i, mdefs);
        }
        mdefs.add("}");
        for (Object mdef : mdefs) writeln((String) mdef);
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
