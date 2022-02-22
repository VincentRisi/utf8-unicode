///
/// System : JPortal
/// $Source: /main/nedcor/cvsroot/eclipse/workspace/jportal/bbd/jportal/Prime.java,v $
/// $Author: vince $
/// $Date: 2004/10/18 13:45:49 $
/// $Revision: 411.1 $
///

package bbd.jportal.old;

import java.util.Date;

public class Prime
{
  public static void main(String[] args)
  {
    int[] Primes = new int[100000];
    int DivIndex;
    int Index;
    int NoPrimes;
    Date d1, d2;

    Index = 3;
    NoPrimes = 0;
    Primes[0] = 3;

    d1 = new Date();
    for(;NoPrimes < 100000;)
    {
      for(DivIndex = 0; DivIndex <= NoPrimes; DivIndex++)
      {
        if (Primes[DivIndex] > Index/2)
        {
          NoPrimes++;
          Primes[NoPrimes] = Index;
          if(NoPrimes % 1000 == 0)
          {
             d2 = new Date();
             System.out.println(""+ NoPrimes + "th Prime : " + Primes[NoPrimes] + " Time : " + (d2.getTime() - d1.getTime()));
          }
          break;
        }
        if (Index % Primes[DivIndex] == 0)
          break;
      }
      Index += 2;
    }
    d2 = new Date();
    System.out.println(""+ NoPrimes + "th Prime : " + Primes[NoPrimes] + " Time : " + (d2.getTime() - d1.getTime()));
  }
}

