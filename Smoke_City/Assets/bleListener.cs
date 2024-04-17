using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.IO;
using System;
using UnityEngine.UI;
using Unity.Mathematics;
using UnityEditor.Rendering;



public class bleListener : MonoBehaviour
{
    SerialPort serial = new SerialPort("COM5", 9600);
    string filename = "";
    public string smoke;
    public string temp1;
    public string tcouple;
    public string temp2;
    public string RPM;
    public char[] buffer;
    float timeSent;
    public float timeDelay = .01f;
    public Button zeroButton;
    public Button fiftyButton;
    public Button fullButton;
    string sendMessage = "z";
    float logTime = 0;
    public string data = "";
    // Start is called before the first frame update
    void Start(){
        zeroButton.onClick.AddListener(zeroSpeed);
        fiftyButton.onClick.AddListener(halfSpeed);
        fullButton.onClick.AddListener(fullSpeed);
        filename = Application.dataPath + "/LogFile"+ System.DateTime.Now.ToOADate() + ".txt";
        
        if(!serial.IsOpen){
            serial.Open();
        }
        serial.ReadTimeout = 20;
        serial.WriteTimeout = 20;
        //buffer = new char[30];
        //serial.ReadTimeout=500;
    }
    
    void OnEnable(){
        Application.logMessageReceived += Log;
    }
    void OnDisable() {
        Application.logMessageReceived -= Log;
    }
    
    // Update is called once per frame
    
    void Update()
    {
        
        /*
        if(serial!=null && Time.time-timeSent>=timeDelay){
            serial.Write(sendMessage);
            //Debug.Log("Trying to send");
        }
        */
        //&& Time.time-timeSent>=timeDelay
        if(serial!=null && Time.time-timeSent>=timeDelay){
            try
            {
                data = serial.ReadLine(); 
                //Debug.Log(temp);
            }
            catch (Exception e)
            {
                // no-op, just to silence the timeouts.
                // (my arduino sends 12-16 byte packets every 0.1 secs)
            }
            timeSent = Time.time;
            string[] words = data.Split(' ');
            if(words.Length==5){
                smoke = words[0];
                temp1 = words[1];
                tcouple = words[2];//.Substring(2);
                temp2 = words[3];
                RPM = words[4];
            }
            
        }
            //serial.Read(buffer,0,30);
            //Debug.Log(buffer);
        if(Time.time - logTime>=10){
            Debug.Log(data);
            logTime=Time.time;
        }
       
        
    
    }
    public void zeroSpeed(){
        sendMessage = "z";
        serial.Write(sendMessage);
        //Debug.Log("turning off");
    }
    public void halfSpeed(){
        
        sendMessage = "h";
        serial.Write(sendMessage);
    }
    public void fullSpeed(){
        sendMessage = "f";
        serial.Write(sendMessage);
    }
    public void Log(string logString, string stackTrace, LogType type){
        TextWriter tw = new StreamWriter(filename,true);
        tw.WriteLine("[" + System.DateTime.Now + "]" + logString);
        tw.Close();
    }
}
