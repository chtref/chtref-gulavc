package ca.polymtl.inf4410.tp1.client;

import java.rmi.AccessException;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.math.*;
import java.util.Arrays;

import ca.polymtl.inf4410.tp1.shared.ServerInterface;
import ca.polymtl.inf4410.tp1.shared.Pair;

public class Client {
	
	private String[] _serverList;
	private int[] _serverOccupancy;
	private int[] _serverLimit;
	private int[] _serverReputation;
	private boolean _unsafeMode;
	private ServerInterface[] distantServerStubs;
	
	public static void main(String[] args) {
		String filePath = null;
		_unsafeMode = false;
		
		if (args.length > 0) {
			filePath = args[0];
		}
		
		if (args.length > 1) {
			if(args[1].toLowerCase().equals("true") || args[1].toLowerCase().equals("t")){
				_unsafeMode = true;
			}
		}

		Client client = new Client();
		client.run(filePath);
	}	
	

	public Client() {
		super();

		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}
		
		try{
			
			//read server list file and create list of servers
			File f = new File("serverList.xqt");
			List<String> lines = Files.readLines(f);

			for(int i = 0; i<lines.size(); i++){
				distantServerStubs[i] = loadServerStub(Integer.parseInt(lines.get(i)));
				_serverOccupancy[i] = 0;
				_serverReputation[i] = 0;
			}
		} catch (Exception e){
			System.out.println("Erreur: " + e.getMessage());
		}
	}

	private void run(String path) {
		
		//read calc file
		//separate in appropriate tasks
		//create threads
			//send to available server
			//wait for answer
			//if timeout send to another server
		//validate answer if unsafe mode
		//add to result
		//display result
	}

	private ServerInterface loadServerStub(String hostname) {
		ServerInterface stub = null;

		try {
			Registry registry = LocateRegistry.getRegistry(hostname);
			stub = (ServerInterface) registry.lookup("server");
		} catch (NotBoundException e) {
			System.out.println("Erreur: Le nom '" + e.getMessage()
					+ "' n'est pas défini dans le registre.");
		} catch (AccessException e) {
			System.out.println("Erreur: " + e.getMessage());
		} catch (RemoteException e) {
			System.out.println("Erreur: " + e.getMessage());
		}

		return stub;
	}

}
