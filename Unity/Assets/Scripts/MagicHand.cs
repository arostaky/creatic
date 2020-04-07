using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MagicHand : MonoBehaviour
{
    [Range(0,100)]
    public int RightHandSpeed;
    [Range(0, 100)]
    public int LeftHandSpeed;
    public float Conductive ;
    public float Extruder = 0.01f;
    OSC osc;
    float LastRightHandSpeed = 0f;
    float LastLeftHandSpeed = 0f;

    int LastFlicker = 0;
    public int Flicker = 0;

    public string LeftHeader = "/LeftVibe";
    public string RightHeader = "/RightVibe";
    public string ConductHeader = "/ConductHand";
    public string FlickerHeader = "/Flicker";

    public GameObject avatar;





    // Start is called before the first frame update
    void Start()
    {

        osc = gameObject.GetComponent<OSC>();
        osc.SetAddressHandler(ConductHeader, OnConductValue);
    }

    // Update is called once per frame
    
    void Update()
    {
        if (LastLeftHandSpeed != LeftHandSpeed)
        {

            SendRightVibe();
        }

        if (LastLeftHandSpeed != LeftHandSpeed)
        {

            SendLeftVibe();


        }
        if (LastFlicker !=Flicker)
        {

            SendFlicker();

        }



        if (Conductive > 0)
        {
            Extruder = Mathf.Lerp(0, 0.25f, Conductive);
        }
        else
        {
            Extruder = 0.01f;
            Conductive = 0;
        }

        Conductive = Mathf.Lerp(0, 1, Conductive);
        avatar.GetComponent<Renderer>().sharedMaterial.SetFloat("_Factor", Extruder);

    }

    void SendLeftVibe ()
    {
        LastLeftHandSpeed = LeftHandSpeed;
        Debug.Log("Left Hand: " + LeftHandSpeed);
        OscMessage Message = new OscMessage();
        Message.address = LeftHeader;
        Message.values.Add(LeftHandSpeed);
        osc.Send(Message);
    }

    void SendRightVibe()
    {
        Debug.Log("Right Hand: " + RightHandSpeed);
        LastRightHandSpeed = RightHandSpeed;
        OscMessage Message = new OscMessage();
        Message.address = RightHeader;
        Message.values.Add(RightHandSpeed);
        osc.Send(Message);
    }


    


    void OnConductValue (OscMessage message)
    {

        Conductive = message.GetInt(0);
        if (Conductive != 0)
        {
            Extruder = Conductive;
        }
        else {
            Extruder = 0.01f;
        }
        

    }

    
    void SendFlicker() {
        Debug.Log("Flicker: " + Flicker);
        LastFlicker = Flicker;
        OscMessage Message = new OscMessage();
        Message.address = FlickerHeader;
        Message.values.Add(Flicker);
        osc.Send(Message);
    }

}
