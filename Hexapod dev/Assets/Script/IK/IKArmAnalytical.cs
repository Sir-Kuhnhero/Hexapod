using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IKArmAnalytical : MonoBehaviour
{
    [SerializeField] float length1, length2, length3;

    [SerializeField] float min1, min2, min3;
    [SerializeField] float max1, max2, max3;

    [SerializeField] Transform bone1, bone2, bone3;

    [SerializeField] Transform target;
    [SerializeField] Transform preview;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 delta = target.position - bone2.position;
        preview.position = bone2.position + delta;

        float lengthAbs = Mathf.Sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);


        float angle1 = Mathf.Atan(delta.z / delta.x) * Mathf.Rad2Deg * -1;
        float angle2;
        float angle3;

        if (lengthAbs > length2 + length3 || lengthAbs < Mathf.Abs(length2 - length3))
        {
            Debug.Log("could not reach target");

            angle2 = 90 - Mathf.Asin(delta.y / lengthAbs) * Mathf.Rad2Deg;
            angle3 = 0;
        }
        else
        {
            angle2 = Mathf.Acos((length3 * length3 - length2 * length2 - lengthAbs * lengthAbs) / (-2 * lengthAbs * length2)) * Mathf.Rad2Deg;
            angle3 = Mathf.Acos((lengthAbs * lengthAbs - length3 * length3 - length2 * length2) / (-2 * length3 * length2)) * Mathf.Rad2Deg;
            
            angle2 = 90 - (angle2 + Mathf.Asin(delta.y / lengthAbs) * Mathf.Rad2Deg);
            angle3 = 180 - angle3;
        }

        if (delta.x > 0)
        {
            angle1 -= 180;
        }


        if (angle1 < min1 || angle1 > max1 || angle2 < min2 || angle2 > max2 || angle3 < min3 || angle3 > max3)
        {
            Debug.Log("could not reach target");
        }

        angle1 = Mathf.Clamp(angle1, min1, max1);
        angle2 = Mathf.Clamp(angle2, min2, max2);
        angle3 = Mathf.Clamp(angle3, min3, max3);




        //bone1.rotation = Quaternion.Euler(90, angle1, 0);
        bone2.localRotation = Quaternion.Euler(0, angle1, angle2);
        bone3.localRotation = Quaternion.Euler(0, 0, angle3);
    }
}
