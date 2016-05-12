using UnityEngine;
using System.Collections;
using System.IO.Ports;

/*
	Controls the tower construction, destruction and use
	by interfacing with AtomWire through the serial port.
	Similar to the "Test" script in the block game, as 
	they recieve the exact smae inputs.
	This one uses all the inputs, but only uses one node 
	per line.
*/

public class PlayerControl : MonoBehaviour
{

	//dimensions of play field
	public const int X_LENGTH = 10;
	public const int Z_LENGTH = 10;

	public const int NUM_LINES = 4; //number of 1-wire lines
	public const int NUM_TOWER_TYPES = 2; //number of available tower types
	public Vector3[] twr_pos = new Vector3[NUM_LINES]; //the four available tower positions

	public SerialPort awInput = new SerialPort ("COM5", 9600); //initialize serial port on COM5

	public int[] block_cnt = new int[NUM_LINES]; //number of blocks per line

	public GameObject[] tower_pre = new GameObject[NUM_TOWER_TYPES]; //stores the tower type pre-fabs

	public GameObject[] towers = new GameObject[NUM_LINES]; //stores the tower on each line

	int[] postpone_destroy = new int[NUM_LINES]; //part of reducing flickering if node not detected when it should be

	// Use this for initialization
	void Start ()
	{
		awInput.Open (); //open serial port
		//initialize block count, frame count
		for (int i = 0; i < NUM_LINES; i++) {
			block_cnt [i] = 0;
			postpone_destroy [i] = 0;
		}
		//set tower positions
		twr_pos [0] = new Vector3 (30, 5, 20);
		twr_pos [1] = new Vector3 (30, 5, 80);
		twr_pos [2] = new Vector3 (70, 5, 20);
		twr_pos [3] = new Vector3 (70, 5, 80);
	}

	// Update is called once per frame
	void Update ()
	{
		/*
			Serial Input Format:
				byte line #
				byte # of Nodes (not needed yet, just for reference)
				byte Node ID
				byte Node Type
				byte Node Input
		*/
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

			if ((status > 0) && (status_raw != 0xFF)) {
				
				if (towers [line] != null) {
					Tower tower = towers [line].GetComponent<Tower> ();
					if (tower != null) {
						if (tower.id == status_raw) {
								
							awInput.ReadByte (); //Node Type

							status_raw = awInput.ReadByte (); //Node Input
							updateTwr (towers [line], status_raw);
						} else {
							int type = awInput.ReadByte (); //Node Type
							Destroy (towers [line]);
							towers [line] = buildTwr (status_raw, type, line);

							status_raw = awInput.ReadByte (); //Node Input
							updateTwr (towers [line], status_raw);
						}
					}
				} else {
					int type = awInput.ReadByte (); //Node Type

					towers [line] = buildTwr (status_raw, type, line);

					status_raw = awInput.ReadByte (); //Node Input
					updateTwr (towers [line], status_raw);
				}

				block_cnt [line] = status;

			} else if ((towers [line] != null) && (block_cnt [line] > status)) {
				if (Time.frameCount > (postpone_destroy [line] + 4)) {
					Destroy (towers [line]);
					block_cnt [line] = status;
				}
				postpone_destroy [line] = Time.frameCount;
			}
		}

		awInput.BaseStream.Flush ();
	}

	void updateTwr (GameObject tower_obj, int status)
	{
		//Updates tower status or shoots based on inputs
		
		Tower tower = tower_obj.GetComponent<Tower> ();
		if (status > 0) {
			tower.shoot ();
		}
	}

	GameObject buildTwr (int id, int type, int line)
	{
		//initializes new towers
		
		GameObject new_twr = (GameObject)Instantiate (tower_pre [type], twr_pos [line], tower_pre [type].transform.rotation);

		if ((line == 2) || (line == 3)) {
			new_twr.transform.rotation = Quaternion.Euler (0, -90, 0);
		}

		Tower tower = new_twr.GetComponent<Tower> ();
		tower.id = id;

		if (type == 0) {
			tower.delay = 0.5f;
			tower.dmg = 10.0f;
			tower.range = 50.0f;
			tower.type = "Gun";
			tower.splash = 0;
		} else if (type == 1) {
			tower.delay = 2f;
			tower.dmg = 20.0f;
			tower.range = 50.0f;
			tower.type = "Magic";
			tower.splash = 10.0f;
		}

		return new_twr;
	}
}
