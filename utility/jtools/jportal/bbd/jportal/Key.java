/// ------------------------------------------------------------------
/// Copyright (c) 1996, 2004 Vincent Risi in Association 
///                          with Barone Budge and Dominick 
/// All rights reserved. 
/// This program and the accompanying materials are made available 
/// under the terms of the Common Public License v1.0 
/// which accompanies this distribution and is available at 
/// http://www.eclipse.org/legal/cpl-v10.html 
/// Contributors:
///    Vincent Risi
/// ------------------------------------------------------------------
/// System : JPortal
/// $Date: 2004/10/18 13:45:48 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.util.Vector;

/**
*  Keys and Indexes used for the database (if its not primary or unique then it is
*  an index)
*/
public class Key implements Serializable
{
  /**
   * 
   */
  private static final long serialVersionUID = 1L;
  /** Name of index or key */
  public String name;
  /** List of fields used in the index or key */
  public Vector<String> fields;
  public Vector<String> options;
  /** Indicates the primary key */
  public boolean isPrimary;
  /** Indicates the index is unique (not defined if primary key) */
  public boolean isUnique;
  /** Indicates the index is clustered (not defined if primary key) */
  public boolean isClustered;
  /** Contructs with default values */
  public Key()
  {
    name = "";
    fields = new Vector<String>();
    options = new Vector<String>();
    isPrimary = false;
    isUnique = false;
    isClustered = false;
  }
  public void reader(DataInputStream ids) throws IOException
  {
    name = ids.readUTF();
    int noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      fields.addElement(value);
    }
    noOf = ids.readInt();
    for (int i = 0; i < noOf; i++)
    {
      String value = ids.readUTF();
      options.addElement(value);
    }
    isPrimary = ids.readBoolean();
    isUnique = ids.readBoolean();
    isClustered = ids.readBoolean();
  }
  public void writer(DataOutputStream ods) throws IOException
  {
    ods.writeUTF(name);
    ods.writeInt(fields.size());
    for (int i = 0; i < fields.size(); i++)
    {
      String value = fields.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeInt(options.size());
    for (int i = 0; i < options.size(); i++)
    {
      String value = options.elementAt(i);
      ods.writeUTF(value);
    }
    ods.writeBoolean(isPrimary);
    ods.writeBoolean(isUnique);
    ods.writeBoolean(isClustered);
  }
  /** Checks if field is already used */
  public boolean hasField(String s)
  {
    int i;
    for (i = 0; i < fields.size(); i++)
    {
      String name = fields.elementAt(i);
      if (name.equalsIgnoreCase(s))
        return true;
    }
    return false;
  }
}


