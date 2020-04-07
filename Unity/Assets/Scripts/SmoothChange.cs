using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SmoothChange : MonoBehaviour {

    float duration = 5; // This will be your time in seconds.
    float smoothness = 0.02f; // This will determine the smoothness of the lerp. Smaller values are smoother. Really it's the time between updates.
    Color currentColor = Color.white; // This is the state of the color in the current interpolation.

    void Start()
    {
        StartCoroutine("LerpColor");
    }

    IEnumerator LerpColor()
    {
        float progress = 0; //This float will serve as the 3rd parameter of the lerp function.
        float increment = smoothness / duration; //The amount of change to apply.
        while (progress < 1)
        {
            currentColor = Color.Lerp(Color.red, Color.blue, progress);
            progress += increment;
            yield return new WaitForSeconds(smoothness);
        }
    }
}
