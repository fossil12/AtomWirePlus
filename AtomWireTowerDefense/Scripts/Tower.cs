using UnityEngine;
using System.Collections;

public class Tower : MonoBehaviour {

	public int id;
	public string type;

	public Animator animator;

	public float dmg;
	public float delay;
	public float range;
	public float splash;

	// Use this for initialization
	void Start () {
		//automatically shoot if a gun (default) turret
		if (type.Equals ("Gun")) {
			InvokeRepeating ("shoot", 0.1f, delay);
		}
		//the gun turrets have an animation, this allows them to play when enabled (while firing)
		animator = gameObject.GetComponent<Animator> ();
		if (animator != null) {
			animator.enabled = false;
			animator.Play ("Take 001");
		}
	}
	
	// Update is called once per frame
	void Update () {

	}

	public void shoot () {
		/*
			This function is called whenever a tower needs to attack an enemy.
			It checks if enemies are in range and which one is closest to 
			shoot at it, then calls the target's "hit" function to deal damage.
			
			If the tower has a splash radius on its attack it will also hit
			enemies within that distance from its original target.
		*/
		
		GameObject[] targets = GameObject.FindGameObjectsWithTag("CREEP");

		if (targets.Length > 0) {
			

			GameObject nearest = targets [0];
			var n_dist = Vector3.Distance (nearest.transform.position, transform.position);
			
			//find nearest target
			foreach (GameObject creep in targets) {
				
				var dist = Vector3.Distance (creep.transform.position, transform.position);
				if (dist < n_dist) {
					nearest = creep;
					n_dist = dist;
				}	
			}
			//make sure target is in range
			if (n_dist <= range) {
				//if animated, enable shooting animation
				if (animator != null) animator.enabled = true;
				
				//get reference to target's Creep script to call hit with
				Creep target = nearest.GetComponent<Creep> ();
				
				target.hit (dmg); //hit target
				
				//do splash damage
				if (splash > 0){
					foreach (GameObject creep in targets) {
						var dist = Vector3.Distance (creep.transform.position, target.transform.position);
						if ((dist < splash) && (creep != target)) {
							Creep creep_script = creep.GetComponent<Creep> ();
							creep_script.hit (dmg); //hit target's neighbors within splash range
						}
					}
				}
			}
		} else {
			//when not shooting the animation is turned off
			if (animator != null) animator.enabled = false;
		}
	}

				
}
