using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class Action : MonoBehaviour
{

    float speed;
    float x, z;
    public Animator anim;
    public Rigidbody rb;
    bool isItUp = true;
    Quaternion originalRotation;

    // Use this for initialization
    void Start()
    {
        anim = this.GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {


        z = Input.GetAxis("Vertical") * Time.deltaTime * 2.0f;
        if (z < 0)
        {
            z = 0;
        }
        x = Input.GetAxis("Horizontal") * Time.deltaTime * 150.0f;
        transform.Translate(0, 0, z);
        transform.Rotate(0, x, 0);

        anim.SetFloat("speed", z);

        if (Input.GetKeyDown(KeyCode.Space))
        {
            anim.SetTrigger("rest");
        }


    }

    void FixedUpdate()
    {


        if (Input.GetButtonDown("Jump"))
        {
            originalRotation = rb.transform.rotation;
            rb.AddRelativeForce(Vector3.forward * 400); //= new Vector3(0, 0, 10);
            StartCoroutine(ResetPosition());
        }
        if (isItUp == false)
        {
            rb.transform.rotation = Quaternion.Lerp(rb.transform.rotation, originalRotation, Time.deltaTime * 5.0f);
            if (rb.transform.rotation == originalRotation)
            {
                isItUp = true;
            }
        }
    }


    IEnumerator ResetPosition()
    {
        yield return new WaitForSeconds(6);
        isItUp = false;
    }


}
