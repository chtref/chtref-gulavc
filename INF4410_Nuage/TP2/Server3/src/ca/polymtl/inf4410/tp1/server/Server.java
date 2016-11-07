package ca.polymtl.inf4410.tp1.server;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.rmi.ConnectException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.List;

import ca.polymtl.inf4410.tp1.shared.Pair;
import ca.polymtl.inf4410.tp1.shared.ServerInterface;

public class Server implements ServerInterface {
	
	private double _badSever;
	private int _serverCapacity;
	private int _port;

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
			_port = Integer.parseInt(lines.get(2));
			
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

			Registry registry = LocateRegistry.getRegistry(_port);
			registry.rebind("server", stub);
			System.out.println("Server ready.");
		} catch (ConnectException e) {
			System.err.println("Impossible de se connecter au registre RMI. Est-ce que rmiregistry est lancé ?");
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
	public int execute(ArrayList<Pair<String, Integer>> listOps) throws RemoteException {
		//verify if available
		if(acceptTask(listOps.size())){
			int result = 0;
			for(int i = 0; i<listOps.size(); ++i){
				if(listOps.get(i).x.toLowerCase().equals("prime")){
					result = (result + Operations.prime(listOps.get(i).y.intValue())) % 4000;
				} else if (listOps.get(i).x.toLowerCase().equals("pell")){
					result = (result + Operations.pell(listOps.get(i).y.intValue())) % 4000;
				} else {
					//panic
					System.out.println("Unsupported operation in task. Aborting.");
					return -1;
				}
			}
			System.out.println("ezmath");
			return result; //random number, chosen by a fair dice roll
		} else {
			System.out.println("I ain't doin that shit");
			return -1; //Operations refusées
		}
	}
	
}
