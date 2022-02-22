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
/// $Date: 2004/01/06 12:00:23 $
/// $Revision: 409.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.Serializable;

/**
 * @author vince
 */
public class Flag implements Serializable
{
  /**
   * 
   */
  private static final long serialVersionUID = 1L;
  public String name;
  public Object value;
  public String description;
  public Flag(String name, Object value, String description)
  {
    this.name = name;
    this.value = value;
    this.description = description;
  }
}