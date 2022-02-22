package bbd.crackle;

import java.io.Serializable;
import java.util.Vector;

/**
*
*/
public class Structure implements Serializable
{
  private static final long serialVersionUID = 1L;
  public static final byte
    NORMAL     = 0
  , PUBLIC     = 1
  , PRIVATE    = 2
  , PROTECTED  = 3
  ;
  public String name;
  public String header;
  public Vector categories;
  public Vector fields;
  public byte codeType;
  public Vector code;
  public Vector python;
  public int codeLine;
  public int start;
  public Structure()
  {
    name = "";
    header = "";
    categories = new Vector();
    fields = new Vector();
    codeType = NORMAL;
    code = new Vector();
    python = new Vector();
    codeLine = 0;
    start = 0;
  }
  public boolean hasSwap()
  {
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      if (field.needsSwap())
        return true;
    }
    return false;
  }
  public int dotNetSize()
  {
    int result = 0;
    for (int i = 0; i < fields.size(); i++)
    {
      Field field = (Field) fields.elementAt(i);
      result += field.dotNetSize();
    }
    return result;
  }
  public String toString()
  {
    return name;
  }
}
