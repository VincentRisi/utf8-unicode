package bbd.jportal;

import java.io.Serializable;

/**
 * @author vince
 */
public class PlaceHolderPairs implements Serializable
{
  /**
   *
   */
  private static final long serialVersionUID = 1L;
  public Field field;
  public int pos;

  public PlaceHolderPairs(Field field, int pos)
  {
    this.field = field;
    this.pos = pos;
  }
}
