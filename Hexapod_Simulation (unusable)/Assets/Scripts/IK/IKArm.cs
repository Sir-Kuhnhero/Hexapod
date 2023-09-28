using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IKArm : MonoBehaviour
{
    public Transform target;
    public Transform[] bones;
    public float[] boneLengths;
    public float damping = 1f;

    void Update()
    {
        if (target == null || bones.Length == 0)
            return;

        // Calculate the position of the end-effector
        Vector3 endEffectorPos = bones[bones.Length - 1].position + bones[bones.Length - 1].forward * boneLengths[bones.Length - 1];

        // Iterate through the bones backwards, from the last bone to the first one
        for (int i = bones.Length - 2; i >= 0; i--)
        {
            // Save the current position and rotation of the bone
            Vector3 bonePos = bones[i].position;
            Quaternion boneRot = bones[i].rotation;

            // Align the bone with the target position
            bones[i].LookAt(target);

            // Calculate the position of the end-effector taking into account the rotation of the parent bones
            endEffectorPos = bones[i].TransformPoint(Vector3.forward * boneLengths[i]);

            // Interpolate the position and rotation of the bone towards the target
            bones[i].position = Vector3.Lerp(bonePos, endEffectorPos, damping * Time.deltaTime);
            bones[i].rotation = Quaternion.Lerp(boneRot, bones[i].rotation, damping * Time.deltaTime);
        }
    }
}

