using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AutomaticPIDPTuner : MonoBehaviour
{
    public PIDPController pidpController;

    public float oscillationPeriod;
    public float ultimateGain;
    public bool start = false;

    private bool tuning = false;
    private float startTime;
    private float maxControlValue;

    private void Update()
    {
        if (start)
        {
            start = false;
            StartTuning();
        }
    }

    private void StartTuning()
    {
        tuning = true;
        startTime = Time.time;
        maxControlValue = 0f;
    }

    private void FixedUpdate()
    {
        if (!tuning) return;

        float controlValue = pidpController.kP * pidpController.previousError.magnitude + pidpController.kI * pidpController.integral.magnitude + pidpController.kD * (pidpController.previousError - pidpController.previousError).magnitude + pidpController.kPreview * pidpController.preview.magnitude;

        if (controlValue > maxControlValue)
        {
            maxControlValue = controlValue;
            oscillationPeriod = Time.time - startTime;
            ultimateGain = maxControlValue;
        }

        if (Time.time - startTime > 2 * oscillationPeriod)
        {
            tuning = false;
            pidpController.kP = 0.6f * (4 * ultimateGain / (Mathf.PI * oscillationPeriod));
            pidpController.kI = 2 * pidpController.kP / oscillationPeriod;
            pidpController.kD = pidpController.kP * oscillationPeriod / 8;
            pidpController.kPreview = pidpController.kP / (4 * Mathf.Sqrt(ultimateGain));
        }

        Debug.Log("tuning");
    }
}