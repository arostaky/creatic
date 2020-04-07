using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ConductivityGloves : MonoBehaviour {

    public GameObject avatar;
    public PerlinAnimateMesh[] walls;

    public float Conductive;
    public float Extruder = 0.01f;

    public MyMath mm;


    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {

        //Conductive = Mathf.Lerp(0, 1, Conductive);
        avatar.GetComponent<Renderer>().sharedMaterial.SetFloat("_Factor", Extruder);
    }

    public void ConductivityReceive( float con)
    {
        Conductive = mm.lerpScale(2.0f, 800.0f, 0.0f, 0.5f, con);

        foreach (PerlinAnimateMesh obj in walls)
        {
            obj.AnimateMesh(Conductive);
        }
    }
}
