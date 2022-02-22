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
/// $Date: 2004/10/18 13:45:46 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;

/**
* This holds the field definition. It also supplies methods for the
* Java format and various SQL formats.
*/
public class Enum implements Serializable
{
  private static final long serialVersionUID = 1L;
  public String name;
  public int value;
  /** constructor ensures fields have correct default values */
  public Enum()
  {
    name = "";
    value = 0;
  }
  public void reader(DataInputStream ids) throws IOException
  {
    name = ids.readUTF();
    value = ids.readInt();
  }
  public void writer(DataOutputStream ods) throws IOException
  {
    ods.writeUTF(name);
    ods.writeInt(value);
  }
}


