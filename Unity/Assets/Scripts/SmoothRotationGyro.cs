using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SmoothRotationGyro : MonoBehaviour {

    Transform target;
    public float smoothTime = 0.3F;
    private Vector3 velocity = Vector3.zero;
    public Quaternion targetRotation;

    public Quaternion gyroRot;

    void Update()
    {

        // Smoothly move the camera towards that target position
        //transform.rotation = Vector3.SmoothDamp(transform.rotation, targetRotation, ref velocity, smoothTime);

        transform.rotation = Quaternion.Lerp(transform.rotation, gyroRot, Time.deltaTime * smoothTime);

    }



    public void GyroRotation(Vector3 rot) {
        gyroRot = Quaternion.Euler(rot);
    }
}
