package bbd.crackle;

import java.io.Serializable;
import java.util.Vector;

/**
*
*/
public class Table implements Serializable
{
  private static final long serialVersionUID = 1L;
  public String name;
  public Vector messages;
  public Table()
  {
    name  = "";
    messages = new Vector();
  }
}

