/*
 * Created on Jan 7, 2004
 */
package bbd.crackle.rpc;

/**
 * @author vince
 */
public interface Rpc
{
	Object call(int message, int signature, Object object) throws Throwable;
	Object call(int message, int signature) throws Throwable;
	Rpc open();
	Object read();
	void write(Object object);
	void close();
  void setUserId(String userId);
  String getUserId();
  void setLocationId(String locationId);
  String getLocationId();
}
