package bbd.crackle.ubi;

import bbd.crackle.*;
import bbd.crackle.Module;

import java.io.PrintWriter;

public class PopUbiGen extends Writer
{
  public static boolean qualifyEnums=false;
  public static PrintWriter errLog = new PrintWriter(System.out);
  /**
  * Sets up the writer and generates the general stuff
  */
  public static void generateCExterns(Module module, PrintWriter writer)
  {
    boolean done = false;
    for (int i = 0; i < module.structures.size(); i++)
    {
      boolean found = false;
      Structure struct1 = (Structure) module.structures.elementAt(i);
      if (struct1.codeType != Structure.NORMAL)
        continue;
      if (struct1.header.length() > 0)
      {
        for (int j = 0; j < i; j++)
        {
          Structure struct2 = (Structure) module.structures.elementAt(j);
          if (struct2.header.compareTo(struct1.header) == 0)
          {
            found = true;
            break;
          }
        }
        if (found == false)
        {
          writeln("#include " + struct1.header);
          done = true;
        }
      }
    }
    if (done)
      writeln();
  }
  /**
  * Generates the data structures defined
  */
  public static void generateCStructs(Module module, PrintWriter writer, boolean doSwap)
  {
    generateCStructs(module, writer, doSwap, true);
  }
  /**
  * Generates the data structures defined
  */
  public static void generateCStructs(Module module, PrintWriter writer, boolean doSwap, boolean allMessages)
  {
    writeln("enum e"+module.name);
    writeln("{ "+module.name.toUpperCase()+"_OK");
    String w0 = "";
    if (module.messageBase > 0)
      w0 = " = "+module.messageBase;
    for (int i = 0; i < module.messages.size(); i++)
    {
      Message message = (Message) module.messages.elementAt(i);
      writeln(", "+module.name.toUpperCase()+"_"+message.name+w0+" // "+message.value);
      w0 = "";
    }
    writeln(", "+module.name.toUpperCase()+"_INV_SIGNATURE");
    writeln(", "+module.name.toUpperCase()+"_INV_COOKIE");
    if (allMessages)
    {
      writeln(", "+module.name.toUpperCase()+"_INV_INIFILE");
      writeln(", "+module.name.toUpperCase()+"_UNCAUGHT_DB_ERROR");
      writeln(", "+module.name.toUpperCase()+"_UNKNOWN_FUNCTION");
      writeln(", "+module.name.toUpperCase()+"_SNAP_ERROR");
      writeln(", "+module.name.toUpperCase()+"_ADDLIST_ERROR");
      writeln(", "+module.name.toUpperCase()+"_ADDLIST_REALLOC_FAILED");
    }
    writeln(", "+module.name.toUpperCase()+"_LAST_LAST");
    writeln("};");
    writeln();
    writeln("extern char *"+module.name+"Errors[];");
    writeln();
    for (int i = 0; i < module.tables.size(); i++)
    {
      Table table = (Table) module.tables.elementAt(i);
      writeln("enum e"+module.name+table.name);
      writeln("{");
      String comma = "  ";
      for (int j = 0; j < table.messages.size(); j++)
      {
        Message message = (Message) table.messages.elementAt(j);
        writeln(comma+table.name.toLowerCase()+message.name+"   // "+message.value);
        comma = ", ";
      }
      writeln(comma+table.name.toLowerCase()+"NoOf");
      writeln("};");
      writeln();
      writeln("extern char *"+module.name+table.name+"[];");
      writeln();
    }
    boolean qualify = module.enumerators.size() > 1 || qualifyEnums;
    if (module.enumerators.size() > 0)
    {
      for (int i = 0; i < module.enumerators.size(); i++)
      {
        Enumerator entry = (Enumerator) module.enumerators.elementAt(i);
        String qualifier = "";
        if (qualify == true)
          qualifier = entry.name + "_";
        writeln("enum " + entry.name);
        String w1 = "{ ";
        for (int j = 0; j < entry.elements.size(); j++)
        {
          String element = (String)entry.elements.elementAt(j);
          writeln(w1 + qualifier + element);
          w1 = ", ";
        }
        writeln("};");
        writeln();
      }
    }
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.codeType != Structure.NORMAL)
        continue;
      if (structure.fields.size() > 0)
      {
        writeln("struct "+structure.name);
        writeln("{");
        int relativeOffset = 0;
        int fillerNo = 0;
        for (int j=0; j < structure.fields.size(); j++)
        {
          Field field = (Field) structure.fields.elementAt(j);
          int n = (relativeOffset+8) % 8;// field.type.paddingSize(false);
          if (n > 0)
          {
            n = 8-n;
            writeln(1, "char filler"+fillerNo+"["+n+"];");
            fillerNo++;
            relativeOffset += n;
          }
          writeln(1, ""+field.type.cDefUbi(field.name)+";");
          relativeOffset += field.type.relativeSize(false);
        }
        int n = (relativeOffset+8) % 8;
        if (n > 0)
        {
          n = 8-n;
          writeln(1, "char filler"+fillerNo+"["+n+"];");
        }
        writeln(1, ""+structure.name+"()");
        writeln(1, "{");
        for (int k = 0; k < structure.fields.size(); k++)
        {
          Field field = (Field)structure.fields.elementAt(k);
          if (field.type.typeof == Type.CHAR)
            writeln(2, "" + field.name + "[0] = 0;");
          else if (field.type.typeof == Type.USERTYPE)
            ;
          else if (field.type.arraySizes.size() == 1)
            writeln(2, "" + field.name + "[0] = 0;");
          else
            writeln(2, "" + field.name + " = 0;");
        }
        writeln(1, "}");
        if (doSwap)
        {
          writeln(1, "void Swaps()");
          writeln(1, "{");
          for (int k = 0; k < structure.fields.size(); k++)
          {
            String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "";
            Field field = (Field)structure.fields.elementAt(k);
            if (field.type.reference == Type.BYPTR
                || field.type.reference == Type.BYREFPTR)
            {
              w3 = "->";
              w4 = "*";
            }
            if (field.needsSwap()
                || field.isStruct(module))
            {
              for (int j = 0; j < field.type.arraySizes.size(); j++)
              {
                Integer integer = (Integer)field.type.arraySizes.elementAt(j);
                writeln(2, "" + w2 + "{for (int k" + j + " = 0; k" + j
                                + " < " + integer.intValue()
                                + "; k" + j + "++)");
                w1 = w1 + "[k" + j + "]";
                w2 = w2 + "  ";
                w5 = w5 + "}";
              }
            }
            if (field.needsSwap())
              writeln(2, "" + w2 + "SwapBytes(" + w4 + field.name + w1 + ");" + w5);
            else if (field.isStruct(module))
              writeln(2, "" + w2 + field.name + w1 + w3 + "Swaps();" + w5);
          }
          writeln(1, "}");
          writeln(1, "#if defined(_DATABUILD_H_)");
          writeln(1, "static int NoBuildFields() {return " + structure.fields.size() + ";}");
          writeln(1, "void _buildAdds(DataBuilder &dBuild)");
          writeln(1, "{");
          for (int k = 0; k < structure.fields.size(); k++)
          {
            Field field = (Field)structure.fields.elementAt(k);
            if (field.type.typeof == Type.BYTE)
            {
              if (field.type.arraySizes.size() == 1)
              {
                Integer integer = (Integer)field.type.arraySizes.elementAt(0);
                writeln(2, "dBuild.addByte(\"" + field.name + "\", " + field.name + ", " + integer.intValue() + ");");
              }
              else
                writeln(2, "dBuild.addByte(\"" + field.name + "\", " + field.name + ");");
            }
            else
              writeln(2, "dBuild.add(\"" + field.name + "\", " + field.name + ");");
          }
          writeln(1, "}");
          writeln(1, "void BuildData(DataBuilder &dBuild, char *name=\"" + structure.name + "\")");
          writeln(1, "{");
          writeln(2, "dBuild.name(name);");
          writeln(2, "_buildAdds(dBuild);");
          writeln(1, "}");
          writeln(1, "void _buildSets(DataBuilder &dBuild)");
          writeln(1, "{");
          for (int k = 0; k < structure.fields.size(); k++)
          {
            Field field = (Field)structure.fields.elementAt(k);
            if (field.type.typeof == Type.BYTE)
            {
              if (field.type.arraySizes.size() == 1)
              {
                Integer integer = (Integer)field.type.arraySizes.elementAt(0);
                writeln(2, "dBuild.setByte(\"" + field.name + "\", " + field.name + ", " + integer.intValue() + ");");
              }
              else
                writeln(2, "dBuild.setByte(\"" + field.name + "\", " + field.name + ", sizeof(" + field.name + "));");
            }
            else
              writeln(2, "dBuild.set(\"" + field.name + "\", " + field.name + ", sizeof(" + field.name + "));");
          }
          writeln(1, "}");
          writeln(1, "void SetData(DataBuilder &dBuild, char *name=\"" + structure.name + "\")");
          writeln(1, "{");
          writeln(2, "dBuild.name(name);");
          writeln(2, "_buildSets(dBuild);");
          writeln(1, "}");
          writeln(1, "#endif");
        }
        writeln(1, "template <class T> void _copyFrom(T& src)");
        writeln(1, "{");
        for (int k = 0; k < structure.fields.size(); k++)
        {
          Field field = (Field)structure.fields.elementAt(k);
          if (field.type.typeof == Type.CHAR)
          {
            writeln(2, "strncpy(" + field.name + ", src." + field.name + ", sizeof(" + field.name + ")-1);");
            writeln(2, "" + field.name + "[sizeof(" + field.name + ")-1] = 0;");
          }
          else if (field.type.typeof == Type.USERTYPE)
            writeln(2, "memcpy(" + field.name + ", src." + field.name + ", sizeof(" + field.name + "));");
          else
            writeln(2, "" + field.name + " = src." + field.name + ";");
        }
        writeln(1, "}");
        writeln(1, "template <class T> void _copyTo(T& src)");
        writeln(1, "{");
        for (int k = 0; k < structure.fields.size(); k++)
        {
          Field field = (Field)structure.fields.elementAt(k);
          if (field.type.typeof == Type.CHAR)
          {
            writeln(2, "strncpy(src." + field.name + ", " + field.name + ", sizeof(src." + field.name + ")-1);");
            writeln(2, "src." + field.name + "[sizeof(src." + field.name + ")-1] = 0;");
          }
          else if (field.type.typeof == Type.USERTYPE)
            writeln(2, "memcpy(src." + field.name + ", " + field.name + ", sizeof(src." + field.name + "));");
          else
            writeln(2, "src." + field.name + " = " + field.name + ";");
        }
        writeln(1, "}");
        writeln("};");
        writeln();
      }
    }
  }
  /**
  * Generates the actions
  */
  private static void generateCActions(Module module, Prototype prototype, PrintWriter writer, String pad)
  {
    writeln(pad+"/**");
    if (prototype.inputs.size() > 0)
    {
      writeln(pad+"* INPUT:");
      for (int i = 0; i < prototype.inputs.size(); i++)
      {
        Action input = (Action) prototype.inputs.elementAt(i);
        Operation op = input.sizeOperation();
        writeln(pad+"*   "+input.name+((op!=null)?" "+op.name:"")+";");
      }
    }
    if (prototype.outputs.size() > 0)
    {
      writeln(pad+"* OUTPUT:");
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Operation op = output.sizeOperation();
        writeln(pad+"*   "+output.name+((op!=null)?" "+op.name:"")+";");
      }
    }
    writeln(pad+"* RETURNS: "+prototype.type.cNameUbi());
    writeln(pad+"*/");
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateCHeader(Module module, Prototype prototype, PrintWriter writer)
  {
    generateCActions(module, prototype, writer, indent(1));
    String w1 = "";
    write(1, ""+prototype.type.cDefUbi(prototype.name)+"(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(w1 + parameter.type.cDefUbi(parameter.name));
      w1 = ", ";
    }
    writeln(");");
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateCInterface(Module module, Prototype prototype, PrintWriter writer)
  {
    generateCActions(module, prototype, writer, indent(1));
    String w1 = "";
    write(1, "virtual " + prototype.type.cDefUbi(prototype.name) + "(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field)prototype.parameters.elementAt(i);
      write(w1 + parameter.type.cDefUbi(parameter.name));
      w1 = ", ";
    }
    writeln(") = 0;");
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateVirtualCHeader(Module module, Prototype prototype, PrintWriter writer)
  {
    generateCActions(module, prototype, writer, indent(1));
    String w1 = "";
    write(1, "virtual " + prototype.type.cDefUbi(prototype.name) + "(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field)prototype.parameters.elementAt(i);
      write(w1 + parameter.type.cDefUbi(parameter.name));
      w1 = ", ";
    }
    writeln(") = 0;");
  }
  /**
  * Generates the implementation code
  */
  public static void generateCImplCode(Module module, Prototype prototype, PrintWriter writer)
  {
    generateCActions(module, prototype, writer, indent(0));
    String w1 = "";
    write(prototype.type.cDefUbi("t"+module.name+"::"+prototype.name)+"(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field) prototype.parameters.elementAt(i);
      write(w1 + parameter.type.cDefUbi(parameter.name));
      w1 = ", ";
    }
    writeln(")");
  }
  /**
  * Generates the implementation code
  */
  public static void generateUbiCImplCode(Module module, Prototype prototype, PrintWriter writer)
  {
    generateCActions(module, prototype, writer, indent(0));
    String w1 = "";
    write(prototype.type.cDefUbi("T" + module.name + "::" + prototype.name) + "(");
    for (int i = 0; i < prototype.parameters.size(); i++)
    {
      Field parameter = (Field)prototype.parameters.elementAt(i);
      write(w1 + parameter.type.cDefUbi(parameter.name));
      w1 = ", ";
    }
    writeln(")");
  }
  /**
  * Generates the prototypes defined
  */
  public static void generateCSwaps(Module module, Prototype prototype, Field field, Operation op, PrintWriter writer)
  {
    String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "", w6 = "";
    int n=0;
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
        writeln(2, ""+w2+"{for (int _i_"+n+op.name+" = 0; _i_"+n+op.name+" < "+w5+op.name+"; _i_"+n+op.name+"++)");
        w1 = w1 + "[_i_"+n+op.name+"]";
        w2 = w2 + "  ";
        if (field.type.reference == Type.BYPTR)
          w3 = ".";
        w4 = "";
        w6 = w6+"}";
      }
      for (int j=0; j < field.type.arraySizes.size(); j++)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(j);
        n++;
        writeln(2, ""+w2+"{for (int i_"+n+j+ " = 0; i_"+n+j
                        + " < " + integer.intValue()
                        + "; i_"+n+j+"++)");
        w1 = w1 + "[i_"+n+j+"]";
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
  public static void generateCTrims(Module module, Prototype prototype, Field field, Operation op, PrintWriter writer)
  {
    String w1 = "", w2 = "", w3 = ".", w4 = "", w5 = "", w6 = "";
    int n=0;
    if (field.needsTrim() == true
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
        writeln(2, ""+w2+"{for (int _i_"+n+op.name+" = 0; _i_"+n+op.name+" < "+w5+op.name+"; _i_"+n+op.name+"++)");
        w1 = w1 + "[_i_"+n+op.name+"]";
        w2 = w2 + "  ";
        if (field.type.reference == Type.BYPTR)
          w3 = ".";
        w4 = "";
        w6 = w6+"}";
      }
      for (int j=1; j < field.type.arraySizes.size(); j++)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(j);
        n++;
        writeln(2, ""+w2+"{for (int i_"+n+j+ " = 0; i_"+n+j
                        + " < " + integer.intValue()
                        + "; i_"+n+j+"++)");
        w1 = w1 + "[i_"+n+j+"]";
        w2 = w2 + "  ";
        w3 = ".";
        w4 = "";
        w6 = w6+"}";
      }
    }
    if (field.isStruct(module))
      writeln(2, ""+w2+field.name+w1+w3+"Trims();"+w6);
    else if (field.needsTrim())
      writeln(2, ""+w2+"TrimTrailingBlanks("+w4+field.name+w1+", sizeof("+w4+field.name+w1+"));"+w6);
    else if (field.type.typeof == Type.USERTYPE)
    {
      //Toolkit.getDefaultToolkit().beep();
      errLog.println("Warning: "+prototype.name+" "+field.name+" is of UserType and may require swapping.");
    }
  }
  public static void generateCStructSwaps(Module module, Prototype prototype, Field field, Operation op, PrintWriter writer)
  {
    if (field.isStruct(module) == true)
    {
      generateCSwaps(module, prototype, field, op, writer);
      generateCTrims(module, prototype, field, op, writer);
    }
  }
  public static void generateCNonStructSwaps(Module module, Prototype prototype, Field field, Operation op, PrintWriter writer)
  {
    if (field.isStruct(module) == false)
    {
      generateCSwaps(module, prototype, field, op, writer);
      generateCTrims(module, prototype, field, op, writer);
    }
  }
}
