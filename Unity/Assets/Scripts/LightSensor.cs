using UnityEngine;
using System.Collections;
using System;
using MK;

public class LightSensor : MonoBehaviour
{
   // public OSC osc;
   // public string ValueHeader = "/Yorgos";

    [Range(0.0f,4094.0f)]
    public float lightSensor;

    public Color colorIni = Color.white;
    public Color colorFin = Color.red;
    //public float duration = 3f;
    Color lerpedColor = Color.white;
    public MK.Glow.MinMaxRange Threshold;
    public GameObject cam;
    public GameObject prefab;

    public MyMath mm;

    //private float t = 0;
    //private bool flag;

    Renderer _renderer;
    public Renderer[] objs; 

    // Use this for initialization
    void Start()
    {
        //osc.SetAddressHandler(ValueHeader, OnValue);
       
        //_renderer = GetComponent<Renderer>();

    }

    void Update()
    {

       // lerpedColor = Color.Lerp(colorIni, colorFin, Mathf.Lerp(0, 1, lightSensor));
       lerpedColor = mm.colorLerpScale(4094.0f, 2000.0f , 0.0f, 1.0f, lightSensor);
        // _renderer.material.color = lerpedColor;
        foreach (Renderer obj in objs) {
            obj.material.color = lerpedColor;    
        }
        

        //cam.GetComponent<MK.Glow.MinMaxRange>().minValue.Equals( lightSensor);
        Threshold.minValue = Mathf.Lerp(0.5f, 1.0f, lightSensor);


        /*
        if (flag == true)
        {
            t -= Time.deltaTime / duration;
            if (t < 0.01f)
                flag = false;
        }
        else
        {
            t += Time.deltaTime / duration;
            if (t > 0.99f)
                flag = true;
        }
        */

    }


    public void LightSensorValue(float lightVal) {
        lightSensor = lightVal;
    }

    void OnValue(OscMessage message)
    {

        lightSensor = message.GetFloat(0);
        Debug.Log("light is" + lightSensor);

    }


}










