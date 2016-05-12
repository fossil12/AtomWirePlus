using UnityEngine;
using System.Collections;
using System.IO.Ports;

/*
	Script to read and process info from the AtomWire master
	Creates playing field sized arrays for future implementations 
	with a larger number of lines
	Change the COM port to match the one the Arduino connects to
*/

public class Test : MonoBehaviour {
	
	//dimensions of play field
	public const int X_LENGTH = 5;
	public const int Z_LENGTH = 5;

	public const int NUM_LINES = 4; //number of 1-wire lines

	public SerialPort awInput = new SerialPort("COM5", 9600); //create serial port instance on COM5
	public int[,] block_cnt = new int[X_LENGTH, Z_LENGTH]; //node count on each line array

	public GameObject block; //block pre-fab

	//create node object for linked list of block objeccts 
	//to add and remove them easily from each line
	public class Node
	{
		public Node prev;
		public GameObject obj;

	}

	//stores block linked lists for each line
	Node[,] blocks = new Node[X_LENGTH, Z_LENGTH];


	// Use this for initialization
	void Start () {
		awInput.Open (); //Open the serial port
		for (int i = 0; i < X_LENGTH; i++) {
			for (int j = 0; j < Z_LENGTH; j++) {
				block_cnt [i, j] = 0; //initialize block counts to zero
			}
		}
		//goal
		block_cnt[2, 4] = 4;
		
		/*
			In the future, a proper impementation will iterate through all 
			X_LENGTH by Z_LENGTH lines, excluding the goal, which would be 
			kept constant, to update blocks. For now in update it only goes 
			through the center four from the player to the goal.
		*/
	}

	// Update is called once per frame
	void Update () {
		int status_raw;//temporary input storage
		int status; //#nodes on line
		int line; //line #


		status_raw = awInput.ReadByte ();
		
		//attempts to check all lines for nodes
		for (int i = 0; i < NUM_LINES; i++) {
			//wait for next new set of inputs
			while (status_raw != 0xFF) { 
				status_raw = awInput.ReadByte ();
			}
			
			line = awInput.ReadByte ();

			status_raw = awInput.ReadByte ();
			status = status_raw; //# of Nodes on line

			status_raw = awInput.ReadByte (); //Node ID
			
			//if valid # nodes input (5 nodes is current limit), create or destroy blocks
			if (status <= 5) {
				//create nodes
				for (;block_cnt [2, line] < status; block_cnt [2, line]++) {
					Node temp = blocks [2, line];
					blocks [2, line] = new Node ();
					blocks [2, line].obj = (GameObject)Instantiate (block, new Vector3 (2, block_cnt [2, line] + 1, line), Quaternion.identity);
					blocks [2, line].prev = temp;
				}
				//destroy nodes
				for (; block_cnt [2, line] > status; block_cnt [2, line]--) {
					Node temp = blocks [2, line];
					blocks [2, line] = temp.prev;
					Destroy (temp.obj);
				}
			}
		}
		//remove processed input from buffer to ensure only new input is read
		awInput.BaseStream.Flush();
	}
}
