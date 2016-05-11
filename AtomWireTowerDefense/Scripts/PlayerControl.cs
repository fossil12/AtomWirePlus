using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class PlayerControl : MonoBehaviour
{

	//dimensions of play field
	public const int X_LENGTH = 10;
	public const int Z_LENGTH = 10;

	public const int NUM_LINES = 4;
	public const int NUM_TOWER_TYPES = 2;
	public Vector3[] twr_pos = new Vector3[NUM_LINES];

	public SerialPort awInput = new SerialPort ("COM5", 9600);

	public int[] block_cnt = new int[NUM_LINES];

	public GameObject[] tower_pre = new GameObject[NUM_TOWER_TYPES];

	public GameObject[] towers = new GameObject[NUM_LINES];

	int[] postpone_destroy = new int[NUM_LINES];

	// Use this for initialization
	void Start ()
	{
		awInput.Open ();
		for (int i = 0; i < NUM_LINES; i++) {
			block_cnt [i] = 0;
			postpone_destroy [i] = 0;
		}
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
		Tower tower = tower_obj.GetComponent<Tower> ();
		if (status > 0) {
			tower.shoot ();
		}
	}

	GameObject buildTwr (int id, int type, int line)
	{
		
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
