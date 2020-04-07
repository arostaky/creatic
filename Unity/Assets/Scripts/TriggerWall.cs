using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TriggerWall : MonoBehaviour {

    int lastFlicker = 0;
    public int flicker = 0;
    int lastGloves = 0;
    public int gloves = 0;

    public OSC osc;

    public string glovesHeaderL = "/LeftVibe";
    public string glovesHeaderR = "/RightVibe";
    public string flickerHeader = "/Flicker";

    public Rigidbody Assistant;



    void Update() {
        if (lastFlicker != flicker)
        {

            SendFlicker();

        }
        if (lastGloves != gloves)
        {

            SendGloves();

        }
    }


    void OnTriggerEnter(Collider other) {
        if(other.tag == "LeftHand" || other.tag == "RightHand" || other.tag == "Hips")
        {
            StartCoroutine(SetFlicker());
        }
    }

    IEnumerator SetFlicker()
    {
        flicker = 1;
        gloves = 1000;
        Debug.Log("hit");
        SendFlicker();
        SendGloves();
        Assistant.mass = flicker;
        Assistant.angularDrag = gloves;

        yield return new WaitForSeconds(2);
        flicker = 0;
        gloves = 0;

    }


    /*
    void OnTriggerExit(Collider other)
    {
        if (other.tag == "LeftHand" || other.tag == "RightHand" || other.tag == "Hips")
        {
            flicker = 0;
            gloves = 0;
            Debug.Log("hit OUT");
            SendFlicker();
            SendGloves();
        }
    }
    */



    void SendFlicker()
    {
  
        lastFlicker = flicker;
        OscMessage Message = new OscMessage();
        Message.address = flickerHeader;
        Message.values.Add(flicker);
        osc.Send(Message);
    }

    void SendGloves()
    {

        lastGloves = gloves;
        OscMessage Message1 = new OscMessage();
        OscMessage Message2 = new OscMessage();
        Message1.address = glovesHeaderL;
        Message2.address = glovesHeaderR;
        Message1.values.Add(gloves);
        Message2.values.Add(gloves);
        osc.Send(Message1);
        osc.Send(Message2);
    }

}


