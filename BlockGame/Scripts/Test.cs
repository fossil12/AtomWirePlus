using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class Test : MonoBehaviour {
	
	//dimensions of play field
	public const int X_LENGTH = 5;
	public const int Z_LENGTH = 5;

	public const int NUM_LINES = 4;

	public SerialPort awInput = new SerialPort("COM5", 9600);
	public int[,] block_cnt = new int[X_LENGTH, Z_LENGTH];

	public GameObject block;

	public class Node
	{
		public Node prev;
		public GameObject obj;

	}

	Node[,] blocks = new Node[X_LENGTH, Z_LENGTH];


	// Use this for initialization
	void Start () {
		awInput.Open ();
		for (int i = 0; i < X_LENGTH; i++) {
			for (int j = 0; j < Z_LENGTH; j++) {
				block_cnt [i, j] = 0;
			}
		}
		//goal
		block_cnt[2, 4] = 4;


		//awInput.Write ("Hello World");
	}

	// Update is called once per frame
	void Update () {
		int status_raw;
		int status;
		int line;


		status_raw = awInput.ReadByte ();

		for (int i = 0; i < NUM_LINES; i++) {
			while (status_raw != 0xFF) {
				status_raw = awInput.ReadByte ();
			}

			line = awInput.ReadByte ();

			status_raw = awInput.ReadByte ();
			status = status_raw; //# of Nodes on line

			status_raw = awInput.ReadByte (); //Node ID

			if (status <= 5) {
				for (;block_cnt [2, line] < status; block_cnt [2, line]++) {
					Node temp = blocks [2, line];
					blocks [2, line] = new Node ();
					blocks [2, line].obj = (GameObject)Instantiate (block, new Vector3 (2, block_cnt [2, line] + 1, line), Quaternion.identity);
					blocks [2, line].prev = temp;
				}
				for (; block_cnt [2, line] > status; block_cnt [2, line]--) {
					Node temp = blocks [2, line];
					blocks [2, line] = temp.prev;
					Destroy (temp.obj);
				}
			}
		}
		awInput.BaseStream.Flush();
	}
}
