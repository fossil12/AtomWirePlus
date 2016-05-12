using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Creep : MonoBehaviour {

	//currently max health is constant at 100
	public const float MAX_HEALTH = 100.0f;

	//stores spawner script to refer to and change its public variables
	public GameObject spawner;
	public CreepSpawn spawnScript;

	//creep's health and health bar textures
	public float health;
	public Texture emptyHealth;
	public Texture fullHealth;

	//current position, if needed
	public Vector3 pos;

	// Use this for initialization
	void Start () {

		pos = transform.position;
		spawner = GameObject.Find ("Spawner");
		spawnScript = spawner.GetComponent<CreepSpawn> ();
		health = MAX_HEALTH;
	}
	
	// Update is called once per frame
	void Update () {

		//kill the creep if it's dead, otherwise move forward
		if (health <= 0) {
			spawnScript.creepCount--;
			Destroy (this.gameObject);
		} else {

			transform.position += Vector3.back;

			if (pos.z < spawnScript.endZone.z) {
				spawnScript.lives--;
				transform.position = spawnScript.startZone;
			}
			
			//dbug attack, to ensure victory if necessary
			//all creeps on screen will be injured halfway if spacebar is pressed
			if (Input.GetKeyDown (KeyCode.Space)) {
				hit (50.0f);
			}
			pos = transform.position;
		}
	}

	void OnGUI ()
	{
		//update health bar
		
		Vector3 screenPos = Camera.main.WorldToScreenPoint (pos);
		GUI.BeginGroup (new Rect (screenPos.x - 50, Screen.height - screenPos.y + 10, 100, 15));
		GUI.DrawTexture (new Rect (0, 0, 100, 15), emptyHealth, ScaleMode.StretchToFill);

		GUI.BeginGroup (new Rect (0, 0, 100 * (float)health / (float)MAX_HEALTH, 15));
		GUI.DrawTexture (new Rect (0, 0, 100, 15), fullHealth, ScaleMode.StretchToFill);
		GUI.EndGroup ();

		GUI.EndGroup ();
	}

	public void hit(float dmg) {
		//deal damage
		
		health -= dmg;

	}
}
