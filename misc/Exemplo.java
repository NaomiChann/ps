package dungeonhack;

import java.util.Random;

public final class Die 
{
	// Attributes:
	private int _sides;
	private int _value;
	private Random _generator;

	// Constructors:
	public Die ( int sides ) 
	{
		this._sides = sides;
		this._generator = new Random();
		this.roll(); // roll it so it gets a value.
	}


/*
===================
Die 
-------------------
Receives a number of sides and creates the die, rolling it so it gets a value.
===================
*/

	public Die ( int sides, int seed ) 
	{
		this._sides = sides;
		this._generator = new Random( seed );
		this.roll();
	}

/*
===================
Die 
-------------------
The generator number defined in Die(int sides) is used to set a specific seed in this function, this seed will guarantee that every die creation is unique. It also rolls the die to get a value.
===================
*/

	//Setters:
	public int setSides ( int sides ) 
	{
		this._sides = sides;
		return _sides;
	}

	//Getters:
	public int getSides () 
	{
		return _sides;
	}

	public int getValue ()
	{
		return _value;
	}

	//functions:
	public int Roll () 
	{
		_value = _generator.nextInt( _sides ) + 1;
		return _value;
	}

/*
===================
Roll
-------------------
Rolls the dice and returns an int.
===================
*/
}