package bbd.jportal;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;

public class Value implements Serializable
{
  private static final long serialVersionUID = 1L;
  public String key;
  public String value;
  public Value()
  {
    key = "";
    value = "";
  }
  public void reader(DataInputStream ids) throws IOException
  {
    key = ids.readUTF();
    value = ids.readUTF();
  }
  public void writer(DataOutputStream ods) throws IOException
  {
    ods.writeUTF(key);
    ods.writeUTF(value);
  }
}
