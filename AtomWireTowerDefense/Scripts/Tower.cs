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
		if (type.Equals ("Gun")) {
			InvokeRepeating ("shoot", 0.1f, delay);
		}
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
		
		GameObject[] targets = GameObject.FindGameObjectsWithTag("CREEP");

		if (targets.Length > 0) {
			

			GameObject nearest = targets [0];
			var n_dist = Vector3.Distance (nearest.transform.position, transform.position);

			foreach (GameObject creep in targets) {
				
				var dist = Vector3.Distance (creep.transform.position, transform.position);
				if (dist < n_dist) {
					nearest = creep;
					n_dist = dist;
				}	
			}
			if (n_dist <= range) {
				if (animator != null) animator.enabled = true;

				Creep target = nearest.GetComponent<Creep> ();
				if (splash > 0){
					foreach (GameObject creep in targets) {
						var dist = Vector3.Distance (creep.transform.position, target.transform.position);
						if ((dist < splash) && (creep != target)) {
							Creep creep_script = creep.GetComponent<Creep> ();
							creep_script.hit (dmg);
						}
					}
				}
				target.hit (dmg);
			}
		} else {
			if (animator != null) animator.enabled = false;
		}
	}

				
}
