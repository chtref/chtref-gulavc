package ca.polymtl.inf4410.tp1.shared;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;

public interface ServerInterface extends Remote {
	int execute(ArrayList<Pair<String, Integer>> listOps) throws RemoteException;
	
}
