using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AutomaticPIDTuner : MonoBehaviour
{
    public PIDController pidController;

    public bool tuning = false;
    private float startTime;
    private float maxControlValue;
    private float oscillationPeriod;
    private float ultimateGain;

    public void StartTuning()
    {
        tuning = true;
        startTime = Time.time;
        maxControlValue = 0f;
    }

    private void FixedUpdate()
    {
        if (!tuning) return;

        float controlValue = pidController.kP * pidController.previousError.magnitude + pidController.kI * pidController.integral.magnitude + pidController.kD * (pidController.previousError - pidController.previousError).magnitude;

        if (controlValue > maxControlValue)
        {
            maxControlValue = controlValue;
            oscillationPeriod = Time.time - startTime;
            ultimateGain = maxControlValue;
        }

        if (Time.time - startTime > 2 * oscillationPeriod)
        {
            tuning = false;
            pidController.kP = 0.6f * (4 * ultimateGain / (Mathf.PI * oscillationPeriod));
            pidController.kI = 2 * pidController.kP / oscillationPeriod;
            pidController.kD = pidController.kP * oscillationPeriod / 8;
        }
    }
}