package bbd.crackle;

import java.io.Serializable;

/**
*
*/
public class Operation implements Serializable
{
  private static final long serialVersionUID = 1L;
  public static final byte
      SIZE = 1
  ,   DYNAMIC = 2
  ;
  public String name;
  public byte code;
  public boolean isConstant;
  public Field field;
  public Operation()
  {
    code = 0;
    name = "";
    isConstant = false;
    field = null;
  }
  //public boolean byPtr(Prototype prototype)
  //{
  //  if (isConstant == true || code != SIZE) return false;
  //  if (field == null)
  //  {
  //    for (int i=0; i<prototype.parameters.size(); i++)
  //    {
  //      Field f = (Field)prototype.parameters.elementAt(i);
  //      if (f.name.compareTo(name) == 0)
  //        return f.type.typeof == Type.BYREFPTR || f.type.typeof == Type.BYPTR;
  //    }
  //    return false;
  //  }    
  //  return field.type.typeof == Type.BYREFPTR || field.type.typeof == Type.BYPTR;
  //}
}
