package ca.polymtl.inf4410.tp1.client;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.rmi.AccessException;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import ca.polymtl.inf4410.tp1.shared.Pair;
import ca.polymtl.inf4410.tp1.shared.ServerInterface;

public class Client {
	
	private static String[] _serverList;
	private static int[] _serverOccupancy;
	private static int[] _serverLimit;
	private static int[] _serverReputation;
	private static boolean _unsafeMode = false;
	private static ServerInterface[] distantServerStubs;
	
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
			List<String> lines = Files.readAllLines(Paths.get("serverList.xqt"));
			
			_serverOccupancy = new int[lines.size()];
			_serverLimit = new int[lines.size()];
			_serverReputation = new int[lines.size()];
			distantServerStubs = new ServerInterface[lines.size()];
			
			for(int i = 0; i<lines.size(); i++){
				String[] info = lines.get(i).split(":");
				distantServerStubs[i] = loadServerStub(info[0],Integer.parseInt(info[1]));
				_serverOccupancy[i] = 0;
				_serverReputation[i] = 0;
				_serverLimit[i] = 3;
			}
		} catch (Exception e){
			System.out.println("Erreur 1: " + e.getMessage());
		}
	}

	private void run(String path) {
		
		LinkedList<Pair<String,Integer>> uncompletedTasks = new LinkedList<Pair<String,Integer>>();
		LinkedList<Pair<String, Integer>> sentTasks = new LinkedList<Pair<String,Integer>>();
		//read calc file		
		try{
			
			//read list of calculators
			List<String> lines = Files.readAllLines(Paths.get(path));
			
			for(String s : lines){
				String[] parts = s.split(" ");
				Pair<String,Integer> pair = new Pair<String,Integer>(parts[0], Integer.parseInt(parts[1]));
				uncompletedTasks.add(pair);
			}
			
		} catch (Exception e){
			System.out.println("Erreur 2: " + e.getMessage());
		}
		
		
		final ExecutorService service;
        Future<Integer> threadTask;

        service = Executors.newFixedThreadPool(2 * distantServerStubs.length);        
        
		
		int global_answer = 0;
		while(uncompletedTasks.size() > 0 || sentTasks.size() > 0){
			
			if(uncompletedTasks.size() > 0){
				//Choose available server
				int server = chooseServer();
				//Create appropriately sized task
				ArrayList<Pair<String,Integer>> task = new ArrayList<Pair<String,Integer>>(_serverLimit[server]);
				for(int i = 0; i<_serverLimit[server] && uncompletedTasks.size() > 0; ++i){
					Pair<String,Integer> mate = uncompletedTasks.pollFirst();
					task.add(mate);
					sentTasks.add(mate);
				}
				
				//create thread for task
				threadTask = service.submit(new CalcThread(task, distantServerStubs[server]));
				try{
					final int partial_answer;
					partial_answer = threadTask.get();
					
					if(partial_answer == -1){
						//server error
						_serverLimit[server] -= 1;
						for(Pair<String,Integer> p : task){
							sentTasks.remove(p);							
							uncompletedTasks.add(p);
						}
					} else {
						//validate
						_serverLimit[server] += 1;
						global_answer = (global_answer + partial_answer) % 4000;
						for(Pair<String,Integer> p : task){
							sentTasks.remove(p);
						}
					}
				} catch(final InterruptedException ex) {
					for(Pair<String,Integer> p : task){
						uncompletedTasks.add(p);
						sentTasks.remove(p);
					}
				} catch(final ExecutionException ex) {
					ex.printStackTrace();
				}
			}
		}
		System.out.println(global_answer);
		service.shutdownNow();
	}

	private ServerInterface loadServerStub(String hostname,int port) {
		ServerInterface stub = null;
		
		try {
			Registry registry = LocateRegistry.getRegistry(hostname,port);
			stub = (ServerInterface) registry.lookup("server");
		} catch (NotBoundException e) {
			System.out.println("Erreur: Le nom '" + e.getMessage()
					+ "' n'est pas défini dans le registre.");
		} catch (AccessException e) {
			System.out.println("Erreur 3: " + e.getMessage());
		} catch (RemoteException e) {
			System.out.println("Erreur 4: " + e.getMessage());
		}

		return stub;
	}
	
	//Chooses which server to send the next task to
	private int chooseServer(){
		
		return (int)(Math.random()*_serverOccupancy.length);
	}
	
	private class CalcThread implements Callable<Integer> {
		
		private ArrayList<Pair<String, Integer>> _task;
		private ServerInterface _server;
		
		public CalcThread(ArrayList<Pair<String, Integer>> task, ServerInterface server){
			
			_task = task;
			_server = server;
		}
		
		public Integer call() {
			int result = 0;
			
			try {
				result = _server.execute(_task);
			} catch (RemoteException e) {
				e.printStackTrace();
				result = -1;
			}
			
			return Integer.valueOf(result);
		}
	
	} 

}
