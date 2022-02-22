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
/// $Date: 2004/10/18 13:45:47 $
/// $Revision: 411.1 $ // YMM.Revision
/// ------------------------------------------------------------------

package bbd.jportal;
import java.io.PrintWriter;
import java.util.Vector;

public abstract class Generator
{
  public static void generate(Database database, String output, PrintWriter outLog)
  {
  }
  public static String description()
  {
    return "This description should be implemented in the generator";
  }
  public static String documentation()
  {
    return "This documentation should be implemented in the generator";
  }
  public static Vector<Flag> flags()
  {
    return null;
  }
  public static boolean toBoolean(Object value) 
  {
    String s = value.toString();
	  return s.equalsIgnoreCase("true");
  }
}
