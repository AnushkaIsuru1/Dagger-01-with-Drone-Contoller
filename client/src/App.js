import { React, useEffect, useState } from "react";
import axios from "axios";
var isDisconnected = 0;

function c1(){sendToArduino('Q');}
function c2(){sendToArduino('W');}
function c3(){sendToArduino('E');}
function c4(){sendToArduino('R');}
function c5(){sendToArduino('T');}
function c6(){sendToArduino('Y');}

function T(){
  /*
  radarReady(1)
  var i = 0
  var tt = setInterval(()=>{
    i+=10
    setRadar("GS-1-"+i+"-12-12")
    if(i==180){clearInterval(tt)}
  }, 300) 
  setTimeout(()=>{
    var i = 180
  var tt = setInterval(()=>{
    i-=10
    setRadar("GS-2-"+i+"-12-12")
    if(i==0){clearInterval(tt)}
  }, 300) 
  }, 6000)
  */
}

function T2(){
  
}
function T3(){
  sendToArduino('.');
}
function T4(){
  sendToArduino('-');
  //landed()
  //isDisconnected = 0
}
function btnDisconnect(){sendToArduino('O');}

//======================================================== * * * * *  Send data to Backend  * * * * * ======================================================== 

function sendToArduino(x){  
  Log("O", "transmitArduino", x)

  const func = async () => {  
    try {    
      await axios.post("http://localhost:8080", {cmd: x}).then(resp=>{        
       });      
    } catch (err) {
      console.log(err);      
    }
  }
  func()
}

//======================================================== * * * * *  Retive data from Backend  * * * * * ======================================================== 
setInterval(async()=>{
  try {
  await axios.get("http://localhost:8080/").then(resp=>{    
    const datalist = resp.data.split('\r')
    for(var i = 0; i<datalist.length; i++){
      if(datalist[i]!=""){      
        if(!isDisconnected) {
          uiUpdate(datalist[i])
          Log("I", "recieveArduino",datalist[i])
        } 
        
      }      
    }    
  })
  } catch (err) {
    console.log(err);   
  }
},10)

//======================================================== * * * * *  UI Update  * * * * * ======================================================== 

function uiUpdate(c){
  var onV = ' Deployed',
  offV=' Turned OFF',
  g1=""
  if(c=="PW"){
    incomingData("PW", "Drone Ready", "Drone Connected via BLUTOOTH")
  }
  else  if(c=="R1"){
    componentSwitch("btnPropller", 1)
    incomingData("RT", "Propplers" + onV, "DigitalWrited PIN D007 HIGH")
    Rotor(1);
    updateVoltageAndAmperage()
  }
  else if(c=="R0"){
    componentSwitch("btnPropller", 0)
    incomingData("RF", "Propplers"+offV, "DigitalWrited PIN D007 LOW")
    Rotor(0); 
    updateVoltageAndAmperage()
  }
  else if(c=="L1"){
    componentSwitch("btnLamp", 1)
    incomingData("LT", "Lamp" + onV, "DigitalWrited PIN D004 HIGH")
    updateVoltageAndAmperage()
  }
  else if(c=="L0"){
    componentSwitch("btnLamp", 0)
    incomingData("LF", "Lamp"+offV, "DigitalWrited PIN D004 LOW") 
    updateVoltageAndAmperage()
  }
  else if(c=="X1"){
    componentSwitch("btnLaser", 1)
    incomingData("XT", "Laser" + onV, "DigitalWrited PIN D002 HIGH") 
    updateVoltageAndAmperage()
  }
  else if(c=="X0"){
    componentSwitch("btnLaser", 0)
    incomingData("XF", "Laser"+offV, "DigitalWrited PIN D002 LOW")
    updateVoltageAndAmperage()
  }
  else if(c=="AL1"){    
    incomingData("ALP", "System Offline", "Auto Landing will be Engage")  
    radarReady(1)  
    updateVoltageAndAmperage()
  }
  else if(c=="AL"){    
    incomingData("AL", "Auto Landing Engaged", "Scaning suitable Land")    
    selectScannerButton("btnAutoLand")
    radarReady(1)
  }
  else if(c=="LS"){    
    incomingData("LS", "Land Level Scanner Engaged", "Scaning...")    
    selectScannerButton("btnLandScn")
    radarReady(1)
  }
  else if(c=="PM"){    
    incomingData("PM", "Proximity Map Engaged", "Mapping data of Promity")    
    selectScannerButton("btnProxiMap")
    document.getElementsByClassName("Radarcenter")[0].classList.add("Radarcenter2")
    document.getElementsByClassName("Radarcenter")[0].classList.remove("Radarcenter3")
    document.getElementsByClassName("Radarcenter")[0].children[2].innerHTML = "00"
    radarReady(1)
  }
  else if(c=="GP"){    
    incomingData("GP", "GPWS Engaged", "Ground Proximity Warining System") 
    selectScannerButton("btnGPWS")   
    document.getElementsByClassName("Radarcenter")[0].classList.add("Radarcenter3")
    radarReady(2)
  }
  else if(c=="LH"){    
    incomingData("LH", "Drone Lanched", "All Systems Ready")  
    launched()      
  }
  else if(c=="LD"){    
    incomingData("LD", "Drone Landed", "All Systems Shuted Down")    
    radarReady(0)
    landed()
    setAltitude(0)
  }
  else if(c=="RS"){   

    radarReady(0)
    selectScannerButton(0)
    document.getElementsByClassName("Radarcenter")[0].classList.add("Radarcenter2")
    document.getElementsByClassName("Radarcenter")[0].classList.add("Radarcenter3")
    document.getElementsByClassName("Radarcenter")[0].children[2].innerHTML = ""
    maxContact = 70
  }
  else if(c=="RB"){   
    window.location.href = window.location.href    
  }
  else if(c=="SL"){            
    incomingData("SL", "Lanch Sequence Started", "Drone will Lauch in 15s")  
    document.getElementsByClassName("Radarcenter")[0].classList.add("Radarcenter2")
    document.getElementsByClassName("Radarcenter")[0].classList.add("Radarcenter3")
    document.getElementsByClassName("Radarcenter")[0].children[2].innerHTML = ""
  }
  else if(c=="SD"){            
    incomingData("SL", "Land Sequence Started", "Drone will land in 15s")  
    document.getElementsByClassName("Radarcenter")[0].classList.remove("Radarcenter2")
    document.getElementsByClassName("Radarcenter")[0].classList.remove("Radarcenter3")
    document.getElementsByClassName("Radarcenter")[0].children[2].innerHTML = ""
  }
  else if(c=="OF"){            
    incomingData("OF", "SYSTEM OFFLINE", "Drone Disconnected")  
    alert2("DRONE DISCONNECTED","SYSTEM OFFLINE", "AUTO LANDING Mode will be Engage")
    isDisconnected = 1
  }
  
  g1 = c.substring(0,2)

  if(g1=="AT"){
    setAltitude(c.split("AT")[1])
  }
  else if(g1=="SS"){
    setSensorValue(c.split("SS,")[1])
  }
  else if(g1=="GG"){
    gpws(c)    
  }
  else if(g1=="GS"){
    setRadar(c)    
  }
  else if(g1=="MA"){
    setRadar(c ,0)    
  }
}
//======================================================== * * * * *  Log  * * * * * ======================================================== 

var isLogOpend = 1;

function Log(tp="", funcName="", m1="", m2="", m3=""){

  if(tp==""){tp = "UI_UPDATE"}
  else if(tp=="I"){tp = "INCOMING"}
  else if(tp=="O"){tp = "OUTGOING"}

  var str = ""

  if(isLogOpend){
    str="\n\n\n"
  }
  var sep = ",\t\t\t\t"
  str+="[ "
  str+=tp
  str+=" ]"
  str+=sep

  str+=getTime()
  str+=sep

  str+=funcName
  str+=",\t\t\t\t\t\t"

  str+=m1/*
  str+=sep

  str+=m2
  str+=sep

  str+=m3*/
  str+="\n"

  const func = async () => {  
    try {    
      await axios.put("http://localhost:8080", {str: str}).then(resp=>{        
       });      
    } catch (err) {
      console.log(err);      
    }
  }
  func()
  isLogOpend = 0;
}
