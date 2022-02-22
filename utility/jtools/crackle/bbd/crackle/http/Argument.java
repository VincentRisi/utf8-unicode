package bbd.crackle.http;

import bbd.crackle.Field;

public class Argument
{
  public String useAs;
  public Field field;

  public Argument(Field field, String useAs)
  {
    this.field = field;
    this.useAs = useAs;
  }
}
