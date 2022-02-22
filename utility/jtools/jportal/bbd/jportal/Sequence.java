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
/// $Date: 2004/10/18 13:45:50 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;

import java.io.Serializable;

public class Sequence implements Serializable
{
  /**
   * 
   */
  private static final long serialVersionUID = 1L;
  public String  name;
  public int     minValue;
  public int     maxValue;
  public int     increment;
  public boolean cycleFlag;
  public boolean orderFlag;
  public int     startWith;
  /** Code starts at line */
  public int     start;
  public Sequence()
  {
    name        = "";
    minValue    = 0;
    maxValue    = 999999999;
    increment   = 1;
    cycleFlag   = true;
    orderFlag   = true;
    startWith   = 1;
    start       = 0;
  }
}

