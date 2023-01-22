using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HexapodLeg : MonoBehaviour
{
    [SerializeField] float length1;
    [SerializeField] float length2;

    [SerializeField] Transform bone1;
    [SerializeField] Transform bone2;

    [SerializeField] Transform target;
    [SerializeField] Transform preview;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 delta = target.position - bone1.position;
        preview.position = bone1.position + delta;

        float length3 = Mathf.Sqrt(delta.x * delta.x + delta.y * delta.y);

        if (length3 > length1 + length2 || length3 < Mathf.Abs(length1 - length2))
        {
            Debug.Log("could not reach target");
            return;
        }

        float angle1 = Mathf.Acos((length2 * length2 - length1 * length1 - length3 * length3) / (-2 * length3 * length1)) * Mathf.Rad2Deg;
        float angle2 = Mathf.Acos((length3 * length3 - length2 * length2 - length1 * length1) / (-2 * length2 * length1)) * Mathf.Rad2Deg;

        angle1 = 90 - (angle1 + Mathf.Asin(delta.y / length3) * Mathf.Rad2Deg);
        angle2 = 180 - angle2;


        Debug.Log("Start");

        Debug.Log(angle1);
        Debug.Log(angle2);

        bone1.rotation = Quaternion.Euler(0, 0, angle1);
        bone2.localRotation = Quaternion.Euler(0, 0, angle2);
    }
}
