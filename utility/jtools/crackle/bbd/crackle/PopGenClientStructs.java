package bbd.crackle;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

public class PopGenClientStructs extends Generator
{
  private static PrintWriter errLog;
  /**
  * Reads input from stored repository
  */
  public static String description()
  {
    return "Generates Client Structs";
  }
  public static String documentation()
  {
    return "Generates Client Structs";
  }
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
  * - VB Front end interface file
  * - C Header for Server and Client
  * - C Client DLL Marshalling code
  * - C Server Marshalling code
  */
  public static void generate(Module module, String output, PrintWriter outLog)
  {
    errLog = outLog;
    outLog.println(module.name+" version "+module.version);
    generateCClientHeader(module, output, outLog);
  }
  /**
  * Sets up the writer and generates the general stuff
  */
  public static void generateCClientHeader(Module module, String output, PrintWriter outLog)
  {
    try
    {
      outLog.println("Code: "+output+module.name.toLowerCase()+"ClientStructs.h");
      OutputStream outFile = new FileOutputStream(output+module.name.toLowerCase()+"ClientStructs.h");
      PrintWriter outData = new PrintWriter(outFile);
      try
      {
        outData.println("// This code was generated, do not modify it, modify it at source and regenerate it.");
        outData.println("// Mutilation, Spindlization and Bending will result in ...");
        outData.println("#ifndef _"+module.name+"CLIENT_STRUCTS_H_");
        outData.println("#define _"+module.name+"CLIENT_STRUCTS_H_");
        outData.println("#pragma option -b");
        outData.println();
        outData.println("#pragma pack (push,1)");
        PopGen.generateCStructs(module, outData, false);
        outData.println("#pragma pack (pop)");
        outData.println();
        outData.println("#endif");
      }
      finally
      {
        outData.flush();
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
