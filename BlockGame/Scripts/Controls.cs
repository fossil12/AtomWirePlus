using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Controls : MonoBehaviour {
	
	//dimensions of play field
	public const int X_LENGTH = 5;
	public const int Z_LENGTH = 5;
	
	public const int NUM_LINES = 3;
	
	public GameObject player;

	public GameObject dummy;

	public Text vicText;

	public bool end;

	public Vector3 start = new Vector3 (2, 1, 0);
	//public Vector3 goal = new Vector3 (2, 5, 4);
	public Vector3 goal = new Vector3 (2, 2, 2);

	int[,] blocks = new int[X_LENGTH, Z_LENGTH];

	Test blockScript;

	// Use this for initialization
	void Start () {
		blockScript = dummy.GetComponent<Test>();
		vicText.text = "";
		end = false;
	}

	// Update is called once per frame
	void Update () {
		Vector3 pos = player.transform.position;

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
			move (pos);
		}


	}

	void victory() {
		vicText.text = "VICTORY!\nPress Space to Restart";
		end = true;
		byte[] output = { 0x21, 0x22};
		blockScript.awInput.Write (output, 0, 2);
		Debug.Log (output);
	}

	void move(Vector3 pos) {
		blocks = blockScript.block_cnt;
		pos.y = blocks [(int)pos.x, (int)pos.z] + 1;
	
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

		player.transform.position = pos;
	}
}
