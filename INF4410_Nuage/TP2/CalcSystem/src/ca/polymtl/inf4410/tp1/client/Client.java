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
		
		//1er argument contient le chemin vers le fichier de calculs
		if (args.length > 0) {
			filePath = args[0];
		}
		
		//2e argument (optionnel) peut être mis à "t" ou "true" afin d'activer le mode sécurisé 
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
			
			
			_serverLimit = new int[lines.size()];
			distantServerStubs = new ServerInterface[lines.size()];
			
			for(int i = 0; i<lines.size(); i++){
				//initialize server limit for each server at 3, will vary depending on their answer
				String[] info = lines.get(i).split(":");
				distantServerStubs[i] = loadServerStub(info[0],Integer.parseInt(info[1]));
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
				
		try{
			//read calc file
			//read list of calculators
			List<String> lines = Files.readAllLines(Paths.get(path));
			
			//add each task from the file to the uncompletedTasks list
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
		
		//create thread pool
        service = Executors.newFixedThreadPool(2 * distantServerStubs.length);        
        
		
		int global_answer = 0;
		//As long as operations are either not sent or not completed, loop
		while(uncompletedTasks.size() > 0 || sentTasks.size() > 0){
			//If no servers are left, kill.
			if(countAliveServers() <= 0){
				long endTime = System.nanoTime();
				System.out.println("Process stopped after: " + (endTime - startTime));
				System.out.println("All servers failed to respond");
				service.shutdownNow();
				return;
			}
			// Version sécurisée
			// Pour chaque tâche, verifier si celle-ci est complétée par le serveur
			for(int i = 0; i <threadTaskList.size(); ++i){
				if(threadTaskList.get(i).isDone()){
					try{
						final Task partial_answer;
						partial_answer = threadTaskList.get(i).get();
						threadTaskList.remove(i);
						i--;
						
						// On verifie si la tâche est bien répondue (-1 signifie un échec)
						if(partial_answer.answer == -1){							
							// Si on a un échec, on diminue la limite de tâches qu'un serveur peut "handle" (0 étant le minimum et qui signifnie que le serveur ne sera plus utilisable)
							_serverLimit[partial_answer.server] -= 1;
							_serverLimit[partial_answer.server] = Math.max(_serverLimit[partial_answer.server],0);
							
							// On retire les tâches de "sentTasks" et on le remet dans les tâches non-complétées afin de les réessayer plus tard.
							for(Pair<String,Integer> p : partial_answer.task){
								sentTasks.remove(p);							
								uncompletedTasks.add(p);
							}
						} else {
							// Si on a une reussite, on augmente la limite de tâches qu'un serveur peut "handle"
							_serverLimit[partial_answer.server] += 1;
							
							// On ajuste la réponse pour prendre en compte la valeur retournée par le serveur
							global_answer = (global_answer + partial_answer.answer) % 4000;
							
							// On retire les tâches de "sentTasks" et on ne le remet PAS dans les tâches non-complétées: elles sont finies et ne seront pas à refaire.
							for(Pair<String,Integer> p : partial_answer.task){
								sentTasks.remove(p);
							}
						}
					} catch(final InterruptedException ex) {
						ex.printStackTrace();
					} catch(final ExecutionException ex) {
						ex.printStackTrace();
					}
				}
			}
			// Version non-sécurisée
			// Pour chaque tâche, verifier si les deux serveurs de la paire l'ont complétée
			for(int i = 0; i <unsafeThreadList.size(); ++i){
				if(unsafeThreadList.get(i).x.isDone() && unsafeThreadList.get(i).y.isDone()){
					try{
						final Task partial_answer;
						final Task partial_answer2;
						partial_answer = unsafeThreadList.get(i).x.get();
						partial_answer2 = unsafeThreadList.get(i).y.get();
						unsafeThreadList.remove(i);
						i--;
						
						// On verifie si la tâche est bien répondue (-1 signifie un échec)
						if(partial_answer.answer == -1 || partial_answer2.answer == -1){
							// Si on a un échec, on diminue la limite de tâches qu'un serveur peut "handle" (0 étant le minimum et qui signifnie que le serveur ne sera plus utilisable)
							_serverLimit[partial_answer.server] += Math.min(partial_answer.answer, 0);
							_serverLimit[partial_answer.server] = Math.max(_serverLimit[partial_answer.server],0);
							_serverLimit[partial_answer2.server] += Math.min(partial_answer2.answer, 0);
							_serverLimit[partial_answer2.server] = Math.max(_serverLimit[partial_answer2.server],0);
							
							// On retire les tâches de "sentTasks" et on le remet dans les tâches non-complétées afin de les réessayer plus tard.
							for(Pair<String,Integer> p : partial_answer.task){
								sentTasks.remove(p);							
								uncompletedTasks.add(p);
							}
						} else {
							// Si on a une reussite, on augmente la limite de tâches que ces serveurs peuvent "handle"
							_serverLimit[partial_answer.server] += 1;
							_serverLimit[partial_answer2.server] += 1;
							if(partial_answer.answer == partial_answer2.answer){
								// Si les deux réponses retournées correspondent, il s'agit fort probablement de la bonne réponse.
								// La chance que deux mauvaises réponses correspondent est très petit, mais c'est pourquoi on dit que ce système est risqué.
								
								// On ajuste la réponse pour prendre en compte la valeur retournée par le serveur
								global_answer = (global_answer + partial_answer.answer) % 4000;
								
								// On retire les tâches de "sentTasks" et on ne le remet PAS dans les tâches non-complétées: elles sont finies et ne seront pas à refaire.
								for(Pair<String,Integer> p : partial_answer.task){
									sentTasks.remove(p);
								}								
							} else {
								//bad
								
								// Si les réponses ne correspondent pas, il y a nécessairement un des serveurs qui a mal fait son calcul. On le traite comme s'il s'agissait d'un échec
								for(Pair<String,Integer> p : partial_answer.task){
									sentTasks.remove(p);							
									uncompletedTasks.add(p);
								}
							}
							
						}
					} catch(final InterruptedException ex) {
						ex.printStackTrace();
					} catch(final ExecutionException ex) {
						ex.printStackTrace();
					}
				}
			}
		
			// Tant qu'il reste des tâches incomplètes dans notre uncompletedTasks, on crée un thread et on envoie des tâches à des serveurs.
			if(uncompletedTasks.size() > 0){
				
				//Choose available server
				int server = chooseServer();
				
				//Create appropriately sized task
				ArrayList<Pair<String,Integer>> task = new ArrayList<Pair<String,Integer>>(_serverLimit[server]);
				
				// On retire un certain nombre de tâches disponibles de uncompletedTasks et on l'ajoute à sentTasks
				for(int i = 0; i<_serverLimit[server] && uncompletedTasks.size() > 0; ++i){
					Pair<String,Integer> mate = uncompletedTasks.pollFirst();
					task.add(mate);
					sentTasks.add(mate);
				}
				
				if(_unsafeMode){
					
					// On choisie le serveur qui sera l'autre moitié de la paire pour vérifier la réponse
					int server2 = chooseServer();
					while(server2 == server && countAliveServers() > 1){
						server2 = chooseServer();
					}
					
					// On crée notre paire et on leur envoie les tâches					
					Pair<Future<Task>,Future<Task>> pairOfTasks = new Pair<Future<Task>,Future<Task>>();
					pairOfTasks.x = service.submit(new CalcThread(task, distantServerStubs[server], server));
					pairOfTasks.y = service.submit(new CalcThread(task, distantServerStubs[server2], server2));;
					unsafeThreadList.add(pairOfTasks);
					
					
					
				} else {
					// On crée notre thread pour un serveur
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
		
		return (int)(Math.random()*_serverLimit.length);
	}
	
	
	private int countAliveServers(){
		int alive = 0;
		for (int i = 0; i < _serverLimit.length; ++i){
			if(_serverLimit[i] > 0){
				alive++;
			}
		}
		return alive;
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
				// Appele "public int execute(ArrayList<Pair<String, Integer>> listOps)" dans Server.java
				result = _server.execute(_task);
			} catch (RemoteException e) {
				// S'il y a un échec, on retourne -1, qui est un nombre géré par le client. Il sait que -1 signifie un échec et agit en conséquence.
				result = -1;
			}
			return new Task(result, _serverID, _task);
		}
	
	} 

}
