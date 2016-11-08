package ca.polymtl.inf4410.tp1.shared;

import java.util.ArrayList;

public class Task {
	public Integer answer;
	public int server;
	public ArrayList<Pair<String,Integer>> task;
	
	public Task(Integer _answer, int _server, ArrayList<Pair<String,Integer>> _task){
		task = _task;
		server = _server;
		answer = _answer;
	}

}
