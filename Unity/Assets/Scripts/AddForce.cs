using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AddForce : MonoBehaviour {

    public float thrust;
    Rigidbody rb;
    public Collider other;


	// Use this for initialization
	void Start () {
        rb = GetComponent<Rigidbody>();
	}
	


    void OnTriggerEnter(Collider other) {
        if (other) {
            //other.GetComponent<Rigidbody>().AddForce(Vector3.up * thrust, ForceMode.Acceleration);
            rb.AddExplosionForce(5, transform.forward * thrust, 5);

        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other)
        {
            Debug.Log("exit");
        }
    }

}
