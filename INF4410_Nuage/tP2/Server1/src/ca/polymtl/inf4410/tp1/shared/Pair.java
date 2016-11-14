package ca.polymtl.inf4410.tp1.shared;

import java.io.Serializable;

public class Pair<X, Y> implements Serializable{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -9059878201727784714L;
	
	public X x; 
	public Y y; 
	public Pair(X x, Y y) { 
		this.x = x; 
		this.y = y; 
	} 
	
	public Pair() {
		
	}

	public boolean equals(Pair<X, Y> p2){
		return ((x.equals(p2.x)) && y.equals(p2.y));
		
	}
}
