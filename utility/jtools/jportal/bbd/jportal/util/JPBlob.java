package bbd.jportal.util;

import java.sql.SQLException;

public class JPBlob
{
  private int used;
  private final int maxSize;
  private byte[] buffer;
  public JPBlob(int maxSize)
  {
    this.maxSize = maxSize;
    buffer = new byte[4];
    buffer[0] = buffer[1] = buffer[2] = buffer[3] = 0;
    used = 4;
  }
  public JPBlob()
  {
    this(0x80000);
  }
  public void setBytes(byte[] data)
  {
    int size = data.length;
    used = size + 4;
    if (used > maxSize)
    {
      size = maxSize - 4;
      used = maxSize;
    }
    buffer = new byte[used];
    for (int i = 0; i < size; i++)
      buffer[i + 4] = data[i];
    setSize(size);
  }
  private void setSize(int size)
  {
    buffer[0] = (byte) ((size & 0xFF000000) >>> 24);
    buffer[1] = (byte) ((size & 0x00FF0000) >> 16);
    buffer[2] = (byte) ((size & 0x0000FF00) >> 8);
    buffer[3] = (byte) (size & 0x000000FF);
  }
  public int length()
  {
    return ((buffer[0]& 0x000000FF) << 24)
         + ((buffer[1]& 0x000000FF) << 16)
         + ((buffer[2]& 0x000000FF) << 8)
         + (buffer[3]& 0x000000FF);
  }
  public byte[] getBytes()
  {
    int size = length();
    byte[] result = new byte[size];
    for (int i = 0; i < size; i++)
      result[i] = buffer[i + 4];
    return result;
  }
  public ByteArrayBlob getBlob() throws SQLException
  {
    byte[] result = new byte[used];
    for (int i = 0; i < used; i++)
      result[i] = buffer[i];
    ByteArrayBlob blob = new ByteArrayBlob(result);
    return blob;
  }
  public void setBlob(ByteArrayBlob blob) throws SQLException
  {
	int size = (int)blob.length();
	byte[] data = blob.getBytes(1, size);
    buffer = new byte[size];
    for (int i=4; i<size; i++)
      buffer[i] = data[i];
    setSize(size-4);
  }
}
