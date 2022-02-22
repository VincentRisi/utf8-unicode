package bbd.crackle.http;

import bbd.crackle.*;
import bbd.crackle.Module;
import bbd.crackle.http.PopHTTP;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.Reader;
import java.util.HashMap;
import java.util.Map;

public class PopHTTPOpenApiFull extends Generator
{
  public static String description()
  {
    return "Generates HTTP OpenApi Restful 3.0.0 YAML (AIX|LINUX|WINDOWS)";
  }

  public static String documentation()
  {
    return "Generates HTTP OpenApi Restful 3.0.0 YAML (AIX|LINUX|WINDOWS)";
  }

  private static PrintWriter outLog;

  public static void main(String[] args)
  {
    try
    {
      outLog = new PrintWriter(System.out);
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
  private static String urlPrefix = "..";
  private static String compSqlSub = "/yaml3";
  private static String description;
  public static void generate(Module module, String output, PrintWriter inOutLog)
  {
    outLog = inOutLog;
    outLog.println(module.name + " version " + module.version);
    loadProperties(module, output);
    getOpenApiPragmas(module);
    generateOpenApi(module, output, outLog);
  }
  private static void getOpenApiPragmas(Module module)
  {
    for (int i = 0; i < module.pragmas.size(); i++)
    {
      String pragma = (String) module.pragmas.elementAt(i);
      pragma = pragma.trim();
      int n = pragma.indexOf("openapi:");
      if (n != 0)
        continue;
      pragma = pragma.substring(8).trim();
      n = pragma.indexOf("urlPrefix:");
      if (n == 0)
      {
        urlPrefix = pragma.substring(10).trim();
        if (urlPrefix.charAt(0) == '$')
          urlPrefix = getProperty(urlPrefix.substring(1), "..");
        continue;
      }
      n = pragma.indexOf("compSqlSub:");
      if (n == 0)
      {
        compSqlSub = pragma.substring(11).trim();
        if (compSqlSub.charAt(0) == '$')
          compSqlSub = getProperty(compSqlSub.substring(1), "/yaml3");
        continue;
      }
      n = pragma.indexOf("description:");
      if (n == 0)
      {
        description = pragma.substring(12).trim();
        continue;
      }
    }
  }
  private static void generateOpenApi(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: " + output + module.name.toLowerCase() + "_openapi.yaml");
      OutputStream outFile = new FileOutputStream(output + module.name.toLowerCase() + "_openapi.yaml");
      writer = new PrintWriter(outFile);
      indent_size = 2;
      try
      {
        writeln("#%YAML 1.2");
        writeln("#---");
        writeln("# This code was generated, do not modify it, modify it at source and regenerate it.");
        writeln("openapi: \"3.0.0\"");
        writeln("info:");
        writeln(1, "title: " + module.name);
        if (description != null)
          writeln(1, "description: " + description);
        writeln(1, "version: " + module.version);
        generateComponents(module);
        writeln("paths:");
        for (int i = 0; i < module.prototypes.size(); i++)
        {
          Prototype prototype = (Prototype) module.prototypes.elementAt(i);
          if (prototype.codeType != Prototype.RPCCALL)
            continue;
          generateRequest(module, prototype);
          generateResponse(module, prototype);
        }
        writeln("#...");
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
  private static final Map<String, String> done = new HashMap<String, String>();
  private static boolean fileExists(String fileName)
  {
    try
    {
      File f = new File(fileName);
      return f.exists();
    }
    catch (Exception e)
    {
      outLog.println(format("%s - Exception", fileName));
      return false;
    }
  }
  private static void generateComponents(Module module)
  {
    writeln("components:");
    writeln(1, "schemas:");
    writeln(2, "tError:");
    writeln(3, "type: object");
    writeln(3, "properties:");
    writeln(4, "error:");
    writeln(5, "type: string");
    writeln(5, "maxLength: 4096");
    generateOpenApiComponents(module);
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.codeType != Structure.NORMAL)
        continue;
      if (structure.fields.size() > 0)
        continue;
      String table = structure.header.substring(1);
      if (table.endsWith(".h\""))
        continue;
      String structName = format(" %s:", structure.name);
      if (done.containsKey(structName) == true)
        continue;
      int end = table.indexOf('.');
      table = table.substring(0, end);
      String name = format("%s%s/%s", urlPrefix, compSqlSub, table);
      String yaml3File = format("%s.yaml3", name);
      try
      {
        if (fileExists(yaml3File) == false)
          yaml3File = format("%s.yaml", name);
        done.put(structName, table);
        Reader input = new FileReader(yaml3File);
        BufferedReader reader = new BufferedReader(input);
        String line = null;
        int state = 0;
        while ((line = reader.readLine()) != null)
        {
          switch (state)
          {
          case 0:
            if (line.contains(structName) == true)
            {
              state = 1;
              writeln(line);
            }
            break;
          case 1:
            if (line.compareTo("...") == 0 || line.charAt(4) != ' ')
              state = 2;
            else
              writeln(line);
            break;
          }
          if (state == 2)
            break;
        }
        reader.close();
        input.close();
      } catch (IOException e)
      {
        outLog.println(format("%s - IOException", yaml3File));
      }
    }
  }
  private static String lastPath = "";

  // allows post put patch options head trace - disallows get delete
  private static void generateRequest(Module module, Prototype prototype)
  {
    boolean doRequestBody = false;
    OpenApi openApi = PopHTTP.getOpenApi(module, prototype);
    String path = openApi.path;
    String operation = openApi.getType();
    String tags = openApi.tags == null ? path : openApi.tags;
    if (lastPath.compareTo(path) != 0)
    {
      lastPath = path;
      writeln(1, format("/%s:", path));
    }
    writeln(2, format("%s:", operation));
    writeln(3, "tags:");
    writeln(4, format("- %s", tags));
    writeln(3, format("operationId: %s", prototype.name));
    writeln(3, format("summary: %s", prototype.name));
    if (doRequestBody == true)
      generateRequestBody(module, prototype);
    else
      generateIn(module, prototype, path);
  }
  private static void generateRequestBody(Module module, Prototype prototype)
  {
    if (prototype.inputs.size() > 0)
    {
      writeln(3, "requestBody:");
      writeln(4, "required: true");
      writeln(4, "content:");
      writeln(5, "application/json:");
      writeln(6, "schema:");
      writeln(7, format("title: %s_request", prototype.name));
      writeln(7, "type: object");
      writeln(7, "properties: # inputs");
      for (int i = 0; i < prototype.inputs.size(); i++)
      {
        Action input = (Action) prototype.inputs.elementAt(i);
        Operation op = input.sizeOperation();
        Field field = input.getParameter(prototype);
        if (field == null)
          continue;
        writeln(8, format("%s:", field.name));
        openapiParameter(op, field, 9);
      }
      boolean done = false;
      for (int i = 0; i < prototype.inputs.size(); i++)
      {
        Action input = (Action) prototype.inputs.elementAt(i);
        Field field = input.getParameter(prototype);
        if (field == null)
          continue;
        switch (field.type.typeof)
        {
        case Type.BYTE:
        case Type.BOOLEAN:
        case Type.INT:
        case Type.SHORT:
        case Type.LONG:
        case Type.FLOAT:
        case Type.DOUBLE:
          if (done == false)
          {
            done = true;
            writeln(7, "required:");
          }
          writeln(8, format("- %s", field.name));
          break;
        default:
          break;
        }
      }
    }
  }
  // allows any /{x} - query /x - path
  private static void generateIn(Module module, Prototype prototype, String path)
  {
    boolean usePath = path.indexOf('{') > 0;
    if (prototype.inputs.size() > 0)
    {
      writeln(3, "parameters:");
      for (int i = 0; i < prototype.inputs.size(); i++)
      {
        Action input = (Action) prototype.inputs.elementAt(i);
        Operation op = input.sizeOperation();
        Field field = input.getParameter(prototype);
        if (field == null)
          continue;
        writeln(4, format("- name: %s", field.name));
        if (usePath == true)
          writeln(5, "in: path");
        else
          writeln(5, "in: query");
        writeln(5, format("description: %s", field.name));
        writeln(5, "required: true");
        writeln(5, "schema:");
        openapiParameter(op, field, 6);
      }
    }
  }

  private static void generateResponse(Module module, Prototype prototype)
  {
    boolean hasResult = false;
    boolean hasOutput = false;
    if (prototype.type.typeof != Type.VOID)
    {
      if (prototype.type.reference != Type.BYPTR)
        hasResult = true;
      else
        hasOutput = true;
    }
    if (prototype.outputs.size() > 0)
      hasOutput = true;
    writeln(3, "responses:");
    if (hasResult == true || hasOutput == true)
    {
      writeln(4, "'200':");
      writeln(5, "description: Success");
      writeln(5, "content:");
      writeln(6, "application/json:");
      writeln(7, "schema:");
      writeln(8, format("title: %s_response", prototype.name));
      writeln(8, "type: object");
      writeln(8, "properties:");
      if (hasResult == true)
      {
        writeln(9, "returnCode:");
        writeln(10, "type: integer");
        writeln(10, "format: int32");
      }
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Operation op = output.sizeOperation();
        Field field = output.getParameter(prototype);
        if (field == null)
          continue;
        writeln(9, format("%s:", field.name));
        openapiParameter(op, field, 10);
      }
      boolean requiredHeaderDone = false;
      if (hasResult == true)
      {
        writeln(8, "required:");
        writeln(9, "- returnCode");
        requiredHeaderDone = true;
      }
      for (int i = 0; i < prototype.outputs.size(); i++)
      {
        Action output = (Action) prototype.outputs.elementAt(i);
        Field field = output.getParameter(prototype);
        if (field == null)
          continue;
        if (requiredHeaderDone == false)
        {
          writeln(8, "required:");
          requiredHeaderDone = true;
        }
        writeln(9, format("- %s", field.name));
      }
    } else
    {
      writeln(4, "'204':");
      writeln(5, "description: No response here");
    }
    writeln(4, "'400':");
    writeln(5, "description: Bad Request");
    writeln(5, "content:");
    writeln(6, "application/json:");
    writeln(7, "schema:");
    writeln(8, "$ref: '#/components/schemas/tError'");
  }

  private static void openapiParameter(Operation op, Field field, int ind)
  {
    switch (field.type.typeof)
    {
    case Type.USERTYPE:
      if (field.type.reference == Type.BYREFPTR)
      {
        if (op != null)
        {
          writeln(ind, "type: array");
          writeln(ind, "items:");
          writeln(ind + 1, format("$ref: '#/components/schemas/%s'", field.type.name));
        } else
          writeln(ind, format("$ref: '#/components/schemas/%s'", field.type.name));
      } else if (field.type.reference == Type.BYPTR)
      {
        if (op != null)
        {
          writeln(ind, "type: array");
          writeln(ind, "items:");
          writeln(ind + 1, format("$ref: '#/components/schemas/%s'", field.type.name));
        } else
          writeln(ind, format("$ref: '#/components/schemas/%s'", field.type.name));
      } else
        writeln(ind, format("$ref: '#/components/schemas/%s'", field.type.name));
      break;
    case Type.CHAR:
    case Type.WCHAR:
    case Type.STRING:
      writeln(ind, "type: string");
      if (field.type.arraySizes.size() > 0)
      {
        Integer integer = (Integer) field.type.arraySizes.elementAt(0);
        writeln(ind, format("maxLength: %d", integer.intValue() - 1));
      }
      break;
    case Type.BYTE:
      writeln(ind, "type: integer");
      writeln(ind, "format: int8");
      break;
    case Type.BOOLEAN:
    case Type.INT:
      writeln(ind, "type: integer");
      writeln(ind, "format: int32");
      break;
    case Type.SHORT:
      writeln(ind, "type: integer");
      writeln(ind, "format: int16");
      break;
    case Type.LONG:
      writeln(ind, "type: integer");
      writeln(ind, "format: int64");
      break;
    case Type.FLOAT:
    case Type.DOUBLE:
      writeln(ind, "type: number");
      writeln(ind, "format: double");
      break;
    case Type.VOID:
    default:
      break;
    }
  }

  private static void generateOpenApiComponents(Module module)
  {
    try
    {
      for (int i = 0; i < module.structures.size(); i++)
      {
        Structure structure = (Structure) module.structures.elementAt(i);
        if (structure.codeType != Structure.NORMAL)
          continue;
        if (structure.fields.size() > 0)
        {
          writeln(2, format("%s:", structure.name));
          writeln(3, "type: object");
          writeln(3, "properties: # with fields");
          int ind = 4;
          for (int j = 0; j < structure.fields.size(); j++)
          {
            Field field = (Field) structure.fields.elementAt(j);
            writeln(ind, format("%s:", field.name));
            switch (field.type.typeof)
            {
            case Type.CHAR:
            case Type.WCHAR:
            case Type.STRING:
              writeln(ind + 1, "type: string");
              if (field.type.arraySizes.size() > 0)
              {
                Integer integer = (Integer) field.type.arraySizes.elementAt(0);
                writeln(ind + 1, format("maxLength: %d", integer.intValue() - 1));
              }
              break;
            case Type.BYTE:
              writeln(ind + 1, "type: integer");
              writeln(ind + 1, "format: int8");
              break;
            case Type.BOOLEAN:
            case Type.INT:
              writeln(ind + 1, "type: integer");
              writeln(ind + 1, "format: int32");
              break;
            case Type.SHORT:
              writeln(ind + 1, "type: integer");
              writeln(ind + 1, "format: int16");
              break;
            case Type.LONG:
              writeln(ind + 1, "type: integer");
              writeln(ind + 1, "format: int64");
              break;
            case Type.FLOAT:
            case Type.DOUBLE:
              writeln(ind + 1, "type: number");
              writeln(ind + 1, "format: double");
              break;
            case Type.USERTYPE:
              writeln(ind + 1, format("$ref: '#/definitions/%s'", field.type.name));
              break;
            case Type.VOID:
              writeln(ind + 1, "$ type: void");
              break;
            default:
              break;
            }
          }
          writeln(3, "required:");
          for (int j = 0; j < structure.fields.size(); j++)
          {
            Field field = (Field) structure.fields.elementAt(j);
            switch (field.type.typeof)
            {
            case Type.BYTE:
            case Type.BOOLEAN:
            case Type.INT:
            case Type.SHORT:
            case Type.LONG:
            case Type.FLOAT:
            case Type.DOUBLE:
              writeln(4, format("- %s", field.name));
              break;
            default:
              break;
            }
          }
          continue;
        }
      }
    } catch (Throwable e)
    {
      System.out.println(e);
      System.out.flush();
      e.printStackTrace();
    }
  }

  public static void generateCStructs(Module module, boolean doSwap)
  {
    for (int i = 0; i < module.structures.size(); i++)
    {
      Structure structure = (Structure) module.structures.elementAt(i);
      if (structure.codeType != Structure.NORMAL)
        continue;
      if (structure.fields.size() > 0)
      {
        writeln("struct " + structure.name + "_http : public " + structure.name);
        writeln("{");
        writeln(1, "void BuildData(DataBuilder &dBuild, char *name=\"+structure.name+\")");
        writeln(1, "{");
        writeln(2, "dBuild.name(name);");
        writeln(2, "dBuild.count(" + structure.fields.size() + "); // requires _DATABUILD2_H_");
        for (int j = 0; j < structure.fields.size(); j++)
        {
          Field field = (Field) structure.fields.elementAt(j);
          String name = field.name;
          switch (field.type.typeof)
          {
          case Type.USERTYPE:
            break;
          case Type.CHAR:
            writeln(2, "dBuild.add(\"" + name + "\", " + name + ", sizeof(" + name + "), \"\");");
            break;
          case Type.BYTE:
          case Type.BOOLEAN:
          case Type.INT:
          case Type.SHORT:
          case Type.LONG:
          case Type.FLOAT:
          case Type.DOUBLE:
            writeln(2, "dBuild.add(\"" + name + "\", " + name + ", \"\");");
            break;
          case Type.VOID:
            break;
          case Type.STRING:
            break;
          case Type.WCHAR:
            break;
          default:
            break;
          }
        }
        writeln(1, "}");
        writeln(1, "void SetData(DataBuilder &dBuild, char *name=\"+structure.name+\")");
        writeln(1, "{");
        writeln(2, "dBuild.name(name);");
        writeln(2, "dBuild.count(" + structure.fields.size() + "); // requires _DATABUILD2_H_");
        for (int j = 0; j < structure.fields.size(); j++)
        {
          Field field = (Field) structure.fields.elementAt(j);
          String name = field.name;
          switch (field.type.typeof)
          {
          case Type.USERTYPE:
            break;
          case Type.CHAR:
            writeln(2, "dBuild.set(\"" + name + "\", " + name + ", sizeof(" + name + "), \"\");");
            break;
          case Type.BYTE:
          case Type.BOOLEAN:
            writeln(2, "dBuild.set(\"" + name + "\", " + name + ", sizeof(" + name + "), \"\");");
            break;
          case Type.INT:
          case Type.SHORT:
          case Type.LONG:
          case Type.FLOAT:
          case Type.DOUBLE:
            writeln(2, "dBuild.set(\"" + name + "\", " + name + ", sizeof(" + name + "), \"\");");
            break;
          case Type.VOID:
            break;
          case Type.STRING:
            break;
          case Type.WCHAR:
            break;
          default:
            break;
          }
        }
        writeln(1, "}");
      }
    }
    writeln("};");
    writeln();
  }
}
