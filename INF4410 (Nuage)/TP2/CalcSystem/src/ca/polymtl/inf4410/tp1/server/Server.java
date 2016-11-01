package ca.polymtl.inf4410.tp1.server;

import java.rmi.ConnectException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.*;
import java.io.File;
import java.nio.file.*;

import ca.polymtl.inf4410.tp1.shared.ServerInterface;
import ca.polymtl.inf4410.tp1.server.Operations;
import ca.polymtl.inf4410.tp1.shared.Pair;

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
		try{
			
			//read server config file
			List<String> lines = Files.readAllLines(Paths.get("serverConfig.xqt"));

			_badSever = Double.parseDouble(lines.get(0));
			_serverCapacity = Integer.parseInt(lines.get(1));
			
		} catch (Exception e){
			System.out.println("Erreur: " + e.getMessage());
		}
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
	 
	//Return values
	//-1 = error / server full, need to reassign
	//[0, infinity+ = "good" answer
	@Override
	public int execute(Pair<String,Integer>[] listOps) throws RemoteException {
		//verify if available
		if(acceptTask(listOps.length)){
			int result = 0;
			for(int i = 0; i<listOps.length; ++i){
				if(listOps[i].x.toLowerCase().equals("prime")){
					result += Operations.prime(listOps[i].y.intValue());
				} else if (listOps[i].x.toLowerCase().equals("pell")){
					result += Operations.pell(listOps[i].y.intValue());
				} else {
					//panic
					System.out.println("Unsupported operation in task. Aborting.");
					return -1;
				}
			}
			return result; //random number, chosen by a fair dice roll
		} else {
			return -1; //Operations refusées
		}
	}
	
}
