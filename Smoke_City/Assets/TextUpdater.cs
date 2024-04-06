using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class TextUpdater : MonoBehaviour
{
    // Start is called before the first frame update
    
    public bleListener listner;
    public TMP_Text text;
    
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //text.text = "Data: " +listner.GetComponent<bleListener>().temp;
    }
}
