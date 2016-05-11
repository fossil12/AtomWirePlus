using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Creep : MonoBehaviour {

	public const float MAX_HEALTH = 100.0f;

	public GameObject spawner;
	public CreepSpawn spawnScript;

	public float health;
	public Texture emptyHealth;
	public Texture fullHealth;

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


		if (health <= 0) {
			spawnScript.creepCount--;
			Destroy (this.gameObject);
		} else {

			transform.position += Vector3.back;

			if (pos.z < spawnScript.endZone.z) {
				spawnScript.lives--;
				transform.position = spawnScript.startZone;
			}

			if (Input.GetKeyDown (KeyCode.Space)) {
				hit (50.0f);
			}
			pos = transform.position;
		}
	}

	void OnGUI ()
	{
		Vector3 screenPos = Camera.main.WorldToScreenPoint (pos);
		GUI.BeginGroup (new Rect (screenPos.x - 50, Screen.height - screenPos.y + 10, 100, 15));
		GUI.DrawTexture (new Rect (0, 0, 100, 15), emptyHealth, ScaleMode.StretchToFill);

		GUI.BeginGroup (new Rect (0, 0, 100 * (float)health / (float)MAX_HEALTH, 15));
		GUI.DrawTexture (new Rect (0, 0, 100, 15), fullHealth, ScaleMode.StretchToFill);
		GUI.EndGroup ();

		GUI.EndGroup ();
	}

	public void hit(float dmg) {
		health -= dmg;

	}
}
