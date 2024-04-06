using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System;

public class DataScript : MonoBehaviour
{
    [SerializeField]
    TMP_Text PPM_Data;
    [SerializeField]
    TMP_Text Therm1_Data;
    [SerializeField]
    TMP_Text Thermocouple_Data;
    [SerializeField]
    TMP_Text Therm2_Data;
    [SerializeField]
    TMP_Text RPM_Data;
    [SerializeField]
    bleListener listener;
    // Start is called before the first frame update

    // Update is called once per frame
    void Update()
    {
        PPM_Data.text = "Smoke Content(PPM): " +  listener.smoke;
        Therm1_Data.text = "Ambient Temperature(C): " + listener.temp1;
        Thermocouple_Data.text = "Thermocouple Temperature(C): " + listener.tcouple;
        Therm2_Data.text = "Thermistor Temperature(C): " + listener.temp2;
        RPM_Data.text = "Fan Speed(RPM): " + listener.RPM;

    }
}
