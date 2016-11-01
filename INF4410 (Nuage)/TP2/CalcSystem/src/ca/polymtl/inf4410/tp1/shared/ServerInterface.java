package ca.polymtl.inf4410.tp1.shared;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ServerInterface extends Remote {
	int execute(Pair<String,Integer>[] listOps) throws RemoteException;
	
}
