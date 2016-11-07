package ca.polymtl.inf4410.tp1.shared;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.HashMap;

public interface ServerInterface extends Remote {
	public Integer generateclientid() throws RemoteException;
    public boolean create(String nom) throws RemoteException;
    public HashMap<String,Integer> list() throws RemoteException;
    public HashMap<String, byte[]> syncLocalDir() throws RemoteException;
    public byte[] get(String nom, byte[] checksum) throws RemoteException;
    public HashMap<Boolean,byte[]> lock(String nom, Integer clientid, byte[] checksum) throws RemoteException;
    public boolean push(String nom, byte[] contenu, Integer clientid) throws RemoteException;
}
