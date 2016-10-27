package ca.polymtl.inf4410.tp1.server;

import java.rmi.ConnectException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

import ca.polymtl.inf4410.tp1.shared.ServerInterface;
import ca.polymtl.inf4410.tp1.server.Operations;

public class Server implements ServerInterface {
	
	private double _badSever;
	private int _serverCapacity;

	public static void main(String[] args) {
		Server server = new Server();
		server.run();
	}

	public Server() {
		super();
		//read server conf file to set badServer & capacity
	}

	private void run() {
	
		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}

		try {
			ServerInterface stub = (ServerInterface) UnicastRemoteObject
					.exportObject(this, 0);

			Registry registry = LocateRegistry.getRegistry();
			registry.rebind("server", stub);
			System.out.println("Server ready.");
		} catch (ConnectException e) {
			System.err
					.println("Impossible de se connecter au registre RMI. Est-ce que rmiregistry est lancé ?");
			System.err.println();
			System.err.println("Erreur: " + e.getMessage());
		} catch (Exception e) {
			System.err.println("Erreur: " + e.getMessage());
		}
	}
	
	
	private boolean acceptTask(int numTasks){
	
		if(numTasks <= _serverCapacity){
			return true;
		}
		
		double refusalRate = (double)(numTasks - _serverCapacity)/(4.0 * _serverCapacity);
		if(Math.random() > refusalRate){
			return true;
		}
		
		return false;
	
	}

	/*
	 * Méthode accessible par RMI.
	 */
	 
	//Return value:
	//null = timeout / server dead
	//-1 = error / server full, need to reassign
	//[0, infinity+ = "good" answer
	@Override
	public int execute(Pair<String,int>[] listOps) throws RemoteException {
		//verify if available
		//execute operations
		return 0;
	}
	
}
