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
import ca.polymtl.inf4410.tp1.shared.Task;

public class Client {
	
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
		
		long startTime = System.nanoTime();
		
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
        ArrayList<Future<Task>> threadTaskList = new ArrayList<Future<Task>>();
        ArrayList<Pair<Future<Task>,Future<Task>>> unsafeThreadList = new ArrayList<Pair<Future<Task>,Future<Task>>>();

        service = Executors.newFixedThreadPool(2 * distantServerStubs.length);        
        
		
		int global_answer = 0;
		while(uncompletedTasks.size() > 0 || sentTasks.size() > 0){
			
			for(int i = 0; i <threadTaskList.size(); ++i){
				if(threadTaskList.get(i).isDone()){
					try{
						final Task partial_answer;
						partial_answer = threadTaskList.get(i).get();
						threadTaskList.remove(i);
						i--;
						
						if(partial_answer.answer == -1){
							//server error
							_serverLimit[partial_answer.server] -= 1;
							for(Pair<String,Integer> p : partial_answer.task){
								sentTasks.remove(p);							
								uncompletedTasks.add(p);
							}
						} else {
							//validate
							_serverLimit[partial_answer.server] += 1;
							global_answer = (global_answer + partial_answer.answer) % 4000;
							for(Pair<String,Integer> p : partial_answer.task){
								sentTasks.remove(p);
							}
						}
					} catch(final InterruptedException ex) {
						
						// TODO
						
						
						
						ex.printStackTrace();
					} catch(final ExecutionException ex) {
						ex.printStackTrace();
					}
				}
			}
			
			for(int i = 0; i <unsafeThreadList.size(); ++i){
				if(unsafeThreadList.get(i).x.isDone() && unsafeThreadList.get(i).y.isDone()){
					try{
						final Task partial_answer;
						final Task partial_answer2;
						partial_answer = unsafeThreadList.get(i).x.get();
						partial_answer2 = unsafeThreadList.get(i).y.get();
						unsafeThreadList.remove(i);
						i--;
						
						if(partial_answer.answer == -1 || partial_answer2.answer == -1){
							//server error
							_serverLimit[partial_answer.server] += Math.min(partial_answer.answer, 0);
							_serverLimit[partial_answer2.server] += Math.min(partial_answer2.answer, 0);
							for(Pair<String,Integer> p : partial_answer.task){
								sentTasks.remove(p);							
								uncompletedTasks.add(p);
							}
						} else {
							//validate
							_serverLimit[partial_answer.server] += 1;
							_serverLimit[partial_answer2.server] += 1;
							if(partial_answer.answer == partial_answer2.answer){
								//good								
								global_answer = (global_answer + partial_answer.answer) % 4000;
								for(Pair<String,Integer> p : partial_answer.task){
									sentTasks.remove(p);
								}								
							} else {
								//bad
								for(Pair<String,Integer> p : partial_answer.task){
									sentTasks.remove(p);							
									uncompletedTasks.add(p);
								}
							}
							
						}
					} catch(final InterruptedException ex) {
						
						// TODO

						ex.printStackTrace();
					} catch(final ExecutionException ex) {
						ex.printStackTrace();
					}
				}
			}
		
			
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
				
				if(_unsafeMode){
					int server2 = chooseServer();
					while(server2 == server && distantServerStubs.length > 1){
						server2 = chooseServer();
					}
					
					Pair<Future<Task>,Future<Task>> pairOfTasks = new Pair<Future<Task>,Future<Task>>();
					pairOfTasks.x = service.submit(new CalcThread(task, distantServerStubs[server], server));
					pairOfTasks.y = service.submit(new CalcThread(task, distantServerStubs[server2], server2));;
					unsafeThreadList.add(pairOfTasks);
					
					
					
				} else {
					//create thread for task
					threadTaskList.add(service.submit(new CalcThread(task, distantServerStubs[server], server)));
				}
				
				
				
			}
		}
		long endTime = System.nanoTime();
		System.out.println("Time: " + (endTime - startTime));
		System.out.println("Answer: " + global_answer);
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
	
	private class CalcThread implements Callable<Task> {
		
		public ArrayList<Pair<String, Integer>> _task;
		private ServerInterface _server;
		private int _serverID;
		
		public CalcThread(ArrayList<Pair<String, Integer>> task, ServerInterface server, int serverID){
			
			_task = task;
			_server = server;
			_serverID = serverID;
		}
		
		public Task call() {
			int result = 0;
			
			try {
				result = _server.execute(_task);
			} catch (RemoteException e) {
				//e.printStackTrace();
				result = -1;
			}
			
			return new Task(result, _serverID, _task);
		}
	
	} 

}
