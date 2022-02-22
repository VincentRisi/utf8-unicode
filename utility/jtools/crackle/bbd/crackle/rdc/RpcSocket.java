package bbd.crackle.rdc;

import java.io.DataInputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;

public class RpcSocket
{
  private InetAddress  inetAddress  = null;
  private String       host;
  private final int    port;
  private int          timeout;
  private Socket       socket       = null;
  private ServerSocket serverSocket = null;
  private OutputStream os = null;
  public RpcSocket(String host, String service) throws Throwable
  {
    try
    {
      inetAddress = InetAddress.getByName(host);
    } catch (UnknownHostException e)
    {
      throw new RpcException(String.format("Host: %s Service: %s - UnknownHostException", host, service), e);
    }
    port = Integer.parseInt(service);
  }
  public RpcSocket(String service, int timeout)
  {
    this.timeout = timeout;
    try
    {
      inetAddress = InetAddress.getLocalHost();
      if (inetAddress != null)
        host = inetAddress.getHostName();
    } catch (Throwable ex)
    {
      host = "localhost";
    }
    port = Integer.parseInt(service);
  }
  private RpcSocket(RpcSocket rpc)
  {
    inetAddress = rpc.inetAddress;
    host = rpc.host;
    port = rpc.port;
    socket = rpc.socket;
    try
    {
      socket.setSoTimeout(timeout);
    }
    catch (SocketException e)
    {
      e.printStackTrace();
    }
  }
  public RpcSocket accept() throws Throwable
  {
    if (serverSocket == null)
    {
      serverSocket = new ServerSocket(port);
    }
    socket = serverSocket.accept();
    return new RpcSocket(this);
  }
  public void open() throws Throwable
  {
    socket = new Socket(inetAddress, port);
    socket.setSoTimeout(timeout);
  }
  public void write(byte[] data) throws Throwable
  {
    os = socket.getOutputStream();
    os.write(data);
    os.flush();   
  }
  public byte[] read(int size) throws Throwable
  {
    DataInputStream dis = new DataInputStream(socket.getInputStream());
    int pos = 0;
    byte[] data = new byte[size];
    int no = size;
    for (;;)
    {
      int n = dis.read(data, pos, no);
      if (n == no || n == 0)
        break;
      pos += n;
      no -= n;
    }
    return data;
  }
  public void close() throws Throwable
  {
    socket.close();
  }
  public void setTimeout(int timeout)
  {
    this.timeout = timeout;   
  }
}
