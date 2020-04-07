 using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VibrationTrigger : MonoBehaviour {

    public MagicHand magichand;

    void Start() {
        
    }

    public void OnCollisionEnter(Collision colin) {
        Collision cin = colin;
        CollisionInFunc(colin);

    }

    public void CollisionInFunc(Collision cin) {
        if (cin.collider.tag == "Hips")
        {
            Debug.Log("Hips collided!");
            magichand.LeftHandSpeed = 100;
            magichand.RightHandSpeed = 100;
            magichand.Flicker = 1;
        }
        if (cin.collider.tag == "LeftPart")
        {
            magichand.LeftHandSpeed = 100;
            magichand.Flicker = 1;
            Debug.Log("Left part collided!");
        }
        if (cin.collider.tag == "RightPart")
        {
            magichand.RightHandSpeed = 100;
            magichand.Flicker = 1;
            Debug.Log("Right part collided!");
        }
    }

    
    public void OnCollisionExit(Collision colout) {
        Collision cout = colout;
        CollisionOutFunc(cout);
    }

    public void CollisionOutFunc(Collision cout) {
        if (cout.collider.tag == "Hips")
        {
            Debug.Log("Hips exited collision!");
            magichand.LeftHandSpeed = 0;
            magichand.RightHandSpeed = 0;
            magichand.Flicker = 0;
        }
        if (cout.collider.tag == "LeftPart")
        {
            magichand.LeftHandSpeed = 0;
            magichand.Flicker = 0;
            Debug.Log("Left part exited collision!");
        }
        if (cout.collider.tag == "RightPart")
        {
            magichand.RightHandSpeed = 0;
            magichand.Flicker = 0;
            Debug.Log("Right part exited collision!");
        }
    }
    
}
