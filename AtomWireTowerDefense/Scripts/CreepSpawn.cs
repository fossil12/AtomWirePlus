﻿using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class CreepSpawn : MonoBehaviour {

	//number of waves of creeps per game
	public const int NUM_WAVES = 5;
	
	//Creep start and end positions
	public Vector3 startZone;
	public Vector3 endZone;

	//Counts
	public int creepCount;
	public int lives;
	public int wave;

	//HUD Text
	public Text lifeText;
	public Text creepText;
	public Text waveText;
	public Text endText;

	//raptor pre-fab
	public GameObject raptor;

	//Store wave names and enemy types; this implementation only has one
	public string[] waves = new string[NUM_WAVES];
	public GameObject[] creeps = new GameObject[NUM_WAVES];

	// Use this for initialization
	void Start () {
		StartCoroutine("startWave");
		waveText.text = "";
		for (int i = 0; i < NUM_WAVES; i++) {
			waves [i] = "Raptors";
			creeps [i] = raptor;
		}
	}
	
	// Update is called once per frame
	void Update () {
		//update lives and creep count display each frame
		lifeText.text = "Lives: " + lives;
		creepText.text = "Creeps: " + creepCount;
		
		//check if reached end game
		if (lives < 1) {
			endGame (0);
		}
		if ((wave == NUM_WAVES) && (creepCount == 0)) {
			endGame (1);
		}
	}

	IEnumerator startWave() {
		//generates waves of creeps over time until reach final wave
		
		while (wave < NUM_WAVES) {
			creepText.text = "Creeps: " + creepCount;
			if (creepCount == 0) {
				for (int i = 0; i < 10; i++) {
					waveText.text = "Next Wave in " + (10 - i);
					yield return new WaitForSeconds (1);
				}
				wave++;
				waveText.text = "Wave " + wave + ": " + waves [wave - 1];
				creepCount = wave * 10;
				creepText.text = "Creeps: " + creepCount;
				for (int i = 0; i < (wave * 10); i++) {
					yield return new WaitForSeconds (1.5f);
					spawnCreep ();
				}
				waveText.text = "";
			}
			yield return null;
		}
	}

	void spawnCreep () {
		GameObject newCreep = (GameObject)Instantiate (creeps [wave - 1], startZone, creeps [wave - 1].transform.rotation);
	}

	void endGame(int condition) {
		//ends the game
		
		StopCoroutine ("startWave");

		byte[] output = { 0x21, 0x22 };
		PlayerControl controlScript = GameObject.Find ("Control").GetComponent<PlayerControl> ();
		controlScript.awInput.Write (output, 0, 2);

		if (condition == 1) {
			endText.color = Color.yellow;
			endText.text = "VICTORY!";
		} else {
			endText.text = "DEFEAT";
		}
	}
}
