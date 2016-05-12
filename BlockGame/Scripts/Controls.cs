using UnityEngine;
using UnityEngine.UI;
using System.Collections;

/*
	Handles player controls and victory condition
*/

public class Controls : MonoBehaviour {
	
	//dimensions of play field
	public const int X_LENGTH = 5;
	public const int Z_LENGTH = 5;
	
	//number of 1-wire lines
	public const int NUM_LINES = 4;
	
	//playable character
	public GameObject player;
	
	//dummy object containing the "Test" script
	public GameObject dummy;
	
	//text object to display victory message
	public Text vicText;

	//Is the game in an end game state?
	public bool end;
	
	//position of start and end points
	public Vector3 start = new Vector3 (2, 1, 0);
	public Vector3 goal = new Vector3 (2, 5, 4);

	//initializes block height refereance array
	int[,] blocks = new int[X_LENGTH, Z_LENGTH];

	//test script attached to dummy object, initialized in start
	Test blockScript;

	// Use this for initialization
	void Start () {
		blockScript = dummy.GetComponent<Test>();
		vicText.text = "";
		end = false;
	}

	// Update is called once per frame
	void Update () {
		//read player's current position
		Vector3 pos = player.transform.position;

		//checks if in end game
		if ((pos == goal)) {
			if (!end)
				victory ();
			else {
				if (Input.GetKeyDown (KeyCode.Space)) {
					end = false;
					player.transform.position = start;
					vicText.text = "";
				}
			}
		} else {
			move (pos); //if not end and not at goal, check if moving
		}


	}

	void victory() {
		//handles the victory sequence
		
		vicText.text = "VICTORY!\nPress Space to Restart";
		end = true;
		byte[] output = { 0x21, 0x22};
		blockScript.awInput.Write (output, 0, 2);
		Debug.Log (output);
	}

	void move(Vector3 pos) {
		blocks = blockScript.block_cnt; //gets current node counts
		pos.y = blocks [(int)pos.x, (int)pos.z] + 1; //move on top of all blocks on position node
	
		//move right
		if (Input.GetKeyDown (KeyCode.RightArrow)) {
			if (pos.x < (X_LENGTH - 1)) {
				if (Physics.Raycast (pos, Vector3.right, 1)) {
					if (blocks [(int)pos.x + 1, (int)pos.z] == (int)pos.y) {
						pos.x++;
						pos.y++;
					}
				} else {
					pos.x++;
					pos.y = blocks [(int)pos.x, (int)pos.z] + 1;
				}
			}
		}
		//move left
		if (Input.GetKeyDown (KeyCode.LeftArrow)) {
			if (pos.x > 0) {
				if (Physics.Raycast (pos, Vector3.left, 1)) {
					if (blocks [(int)pos.x - 1, (int)pos.z] == (int)pos.y) {
						pos.x--;
						pos.y++;
					}

				} else {
					pos.x--;
					pos.y = blocks [(int)pos.x, (int)pos.z] + 1;
				}
			}
		}
		//move forward
		if (Input.GetKeyDown (KeyCode.UpArrow)) {
			if (pos.z < (Z_LENGTH - 1)) {
				if (Physics.Raycast (pos, Vector3.forward, 1)) {
					if (blocks [(int)pos.x, (int)pos.z + 1] == (int)pos.y) {
						pos.z++;
						pos.y++;
					}
				} else {
					pos.z++;
					pos.y = blocks [(int)pos.x, (int)pos.z] + 1;
				}
			}
		}
		//move back
		if (Input.GetKeyDown (KeyCode.DownArrow)) {
			if (pos.z > 0) {
				if (Physics.Raycast (pos, Vector3.back, 1)) {
					if (blocks [(int)pos.x, (int)pos.z - 1] == (int)pos.y) {
						pos.z--;
						pos.y++;
					}

				} else {
					pos.z--;
					pos.y = blocks [(int)pos.x, (int)pos.z] + 1;
				}
			}
		}
		
		player.transform.position = pos; //change player position to new position
	}
}
