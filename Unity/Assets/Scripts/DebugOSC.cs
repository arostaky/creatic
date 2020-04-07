using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DebugOSC : MonoBehaviour {

    public float LightS;
    public float valueA;
    public float valueB;
    public float valueC;
    public float valueD;
    





    public void PrintValuesL(float l)
    {
        LightS = l;
        Debug.Log("l" + LightS);
    }

    public void PrintValuesA(float a) {
        valueA = a;
        Debug.Log("a"+valueA);
    }

    public void PrintValuesB(float b)
    {
        valueB = b;
        Debug.Log("b" + valueB);
    }
    public void PrintValuesC(float c)
    {
        valueC = c;
        Debug.Log("c" + valueC);
    }
    public void PrintValuesD(float d)
    {
        valueD = d;
        Debug.Log("d" + valueD);
    }
}
