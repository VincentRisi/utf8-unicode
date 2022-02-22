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

import java.io.PrintWriter;

public class Padder
{
  int relativeOffset;
  int fillerNo;
  public void padC(int padSize, int fieldSize, PrintWriter outData)
  {
     int n = relativeOffset % padSize;
     if (n > 0)
     {
       n = padSize-n;
       outData.println("  char   FILL"+fillerNo+"["+n+"];");
       fillerNo++;
       relativeOffset += n;
     }
     relativeOffset += fieldSize;
  }
  public void padVB(int padSize, int fieldSize, PrintWriter outData)
  {
    int n = relativeOffset % padSize;
    if (n > 0)
    {
      n = padSize-n;
      fillerNo++;
      outData.println("  FILL"+fillerNo+" as String * "+n);
      relativeOffset += n;
    }
    relativeOffset += fieldSize;
  }
  public void incOffset(int value)
  {
    relativeOffset += value;
  }
}

