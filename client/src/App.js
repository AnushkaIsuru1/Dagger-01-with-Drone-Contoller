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

function getTime(){
  let dateObject = new Date();

  let date = ("0" + dateObject.getDate()).slice(-2);
  let month = ("0" + (dateObject.getMonth() + 1)).slice(-2);
  let year = dateObject.getFullYear();

  let hours = ("0" + dateObject.getHours()).slice(-2) 
  let minutes = ("0" + dateObject.getMinutes()).slice(-2)
  let seconds = ("0" + dateObject.getSeconds()).slice(-2)

  return(year + "-" + month + "-" + date + " " + hours + ":" + minutes + ":" + seconds);
}
var oldGpwsAngle = 0,maxContact = 70;

function setRadar(s, t=1){
  var str = s.split('-'), li,li2,spot,len , angleS, rotateF
    if(maxContact>str[3] * 1 && str[3] > 30){maxContact = str[3]}
    if(maxContact>str[4] * 1 && str[4] > 30) {maxContact = str[4]}
    
    if(str[1]%2==1){
      angleS = 180;
      rotateF =  str[2] * -1
      if(str[2]==180){        
        rotateF = -210
        setTimeout(()=>{
          document.getElementsByClassName("radarIndicator")[0].style.transition = '0s'
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(0deg) rotate('+ 0 +'deg)'       
             
        },250)
        setTimeout(()=>{         
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(0deg) rotate('+ 0 +'deg)'
          document.getElementsByClassName("radarIndicator")[0].style.transition = 'all .5s'
        },360)

        if(!t){
          CountDown("RadarcenterText",maxContact, 500);          
        }        
      }
    }else{
      angleS = 0;
      rotateF = (180 - str[2]) * -1 
      if(str[2]==0){        
        rotateF = -210
        setTimeout(()=>{
          document.getElementsByClassName("radarIndicator")[0].style.transition = '0s'
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(180deg) rotate('+ 0 +'deg)'       
             
        },250)
        setTimeout(()=>{         
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(180deg) rotate('+ 0 +'deg)'
          document.getElementsByClassName("radarIndicator")[0].style.transition = 'all .5s'
        },360)

        if(!t){
          CountDown("RadarcenterText",maxContact, 500);          
        } 
      }
    }
    document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY('+ angleS +'deg) rotate('+ rotateF +'deg)'
    

  li2 =  document.getElementById("redarSpotsUL").children[((str[2] - 10 )/10)]
  li = document.getElementById("redarSpotsUL").children[((str[2]*1 + 170 )/10)]

  
  spot = [0,3,2,1,0][str[1]]
  if(t){
    if(str[3]!=0){
      li.children[spot].children[0].classList.add("radarSpotR")
      li.children[spot].children[0].children[1].innerHTML = str[3]
    }
    else{
      li.children[spot].children[0].classList.remove("radarSpotR")
      li.children[spot].children[0].children[1].innerHTML = 0
    }
    if(str[4]!=0){
      li2.children[spot].children[0].classList.add("radarSpotR")
      li2.children[spot].children[0].children[1].innerHTML = str[4]
    }
    else{
      li2.children[spot].children[0].classList.remove("radarSpotR")
      li2.children[spot].children[0].children[1].innerHTML = 0
    }
  }else{
    var lc,rc, ra1 = 50, ra2 = 40;
    li.children[spot].children[0].classList.remove("radarSpotR")
    li.children[spot].children[0].classList.remove("radarSpotG")
    li.children[spot].children[0].classList.remove("radarSpotY")

    li2.children[spot].children[0].classList.remove("radarSpotR")
    li2.children[spot].children[0].classList.remove("radarSpotG")
    li2.children[spot].children[0].classList.remove("radarSpotY")

    lc = rc = "radarSpotG"
    if(str[3]<ra1){lc = "radarSpotY"}
    if(str[3]<ra2){lc = "radarSpotR"}

    if(str[4]<ra1){rc = "radarSpotY"}
    if(str[4]<ra2){rc = "radarSpotR"}

    li.children[spot].children[0].classList.add(lc)
    li.children[spot].children[0].children[1].innerHTML = str[3]

    li2.children[spot].children[0].classList.add(rc)
    li2.children[spot].children[0].children[1].innerHTML = str[4]

    if(str[2]==170){
      document.getElementById("redarSpotsUL").children[17].children[spot].children[0].classList.add(lc)
      document.getElementById("redarSpotsUL").children[17].children[spot].children[0].children[1].innerHTML = str[3]

      document.getElementById("redarSpotsUL").children[35].children[spot].children[0].classList.add(rc)
      document.getElementById("redarSpotsUL").children[35].children[spot].children[0].children[1].innerHTML = str[4]
    }
  }
 
}
function gpws(s){
  var str = s.split(','), li,len , angleS, rotateF;
  if(str[1]>30){    
    if(!oldGpwsAngle){
      angleS = 180;
      rotateF = (90 - str[1])
      if(str[1]==140){
        oldGpwsAngle =1
        rotateF = (60 - str[1])
        setTimeout(()=>{
          document.getElementsByClassName("radarIndicator")[0].style.transition = '0s'
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(0deg) rotate('+ 50 +'deg)'       
             
        },250)
        setTimeout(()=>{         
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(0deg) rotate('+ 50 +'deg)'
          document.getElementsByClassName("radarIndicator")[0].style.transition = 'all .5s'
        },360)
      }
    }else if(oldGpwsAngle){
      angleS = 0;
      rotateF = (str[1] - 90)
      if(str[1]==40){
        oldGpwsAngle =0
        rotateF = (str[1] - 120)
        setTimeout(()=>{
          document.getElementsByClassName("radarIndicator")[0].style.transition = '0s'
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(180deg) rotate('+ 50 +'deg)'          
        },250)
        setTimeout(()=>{         
          document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(180deg) rotate('+ 50 +'deg)'
          document.getElementsByClassName("radarIndicator")[0].style.transition = 'all .5s'
        },360)
      }
    }
    document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY('+ angleS +'deg) rotate('+ rotateF +'deg)'
    

  li = document.getElementById("gpwsrSpotsUL").children[((str[1] - 40 )/10)]
  len = str[2];
  
  if(20<len && len <= 40){ 
    li.children[3].children[0].classList.add("SpotShow") 
  }
  else{
    li.children[3].children[0].classList.remove("SpotShow")     
  }

  if(40<len && len <= 60){ 
    li.children[2].children[0].classList.add("SpotShow") 
  }
  else{
    li.children[2].children[0].classList.remove("SpotShow")     
  }

  if(60<len && len <= 80){ 
    li.children[1].children[0].classList.add("SpotShow") 
  }
  else{
    li.children[1].children[0].classList.remove("SpotShow")     
  }  
  if(80<len && len <= 100){ 
    li.children[0].children[0].classList.add("SpotShow") 
  }
  else{
    li.children[0].children[0].classList.remove("SpotShow")     
  }
  
  if(40<len && len <= 60){ 
    alert2("THREAT DETECTED", "GPWS FOUND IMPACT", "<span class='special'>Pull UP</span> or <span class='special'>Auto Filot</span> mode will be Engage")
  }
} 
}
function radarReady(x=0){
  //document.getElementsByClassName("radarIndicatorCont")[0].style.transform = ''
  var ul,len
  ul = document.getElementById("gpwsrSpotsUL").children;
    len = ul.length;
    for (var i = 0 ; i<len; i++){   
        
        var spots = ul[i].children;
        
        for (var j = 0 ; j<5; j++){
            try{
                spots[j].children[0].classList.remove("SpotShow");
            }catch(e){
                
            }
            
        }
    }

    ul = document.getElementById("redarSpotsUL").children;
    len = ul.length;
    for (var i = 0 ; i<len; i++){   
        
        var spots = ul[i].children;
        
        for (var j = 0 ; j<5; j++){
            try{
                spots[j].children[0].classList.remove("radarSpotR"); 
                spots[j].children[0].classList.remove("radarSpotY");
                spots[j].children[0].classList.remove("radarSpotG");
            }catch(e){
                
            }
            
        }
    }
  if(x==1){
    document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(180deg) rotate(0deg)'
    document.getElementsByClassName("radarIndicator")[0].classList.add("radarIndicatorShow");
    document.getElementsByClassName("radarIndicator")[0].classList.remove("radarIndicatorGPWS");
    document.getElementById("redarGrid").children[35].children[0].innerHTML = 350;
    document.getElementById("redarGrid").children[34].children[0].innerHTML = 340;
    document.getElementById("redarGrid").children[33].children[0].innerHTML = 330;
    document.getElementById("redarGrid").children[32].children[0].innerHTML = 320;
    document.getElementById("redarGrid").children[31].children[0].innerHTML = 310;
    document.getElementsByClassName("Radarcenter")[0].children[2].innerHTML = ""
  }else if(x==2){
    document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(180deg) rotate(50deg)'
    document.getElementsByClassName("radarIndicator")[0].classList.add("radarIndicatorShow");
    document.getElementsByClassName("radarIndicator")[0].classList.add("radarIndicatorGPWS");
    document.getElementById("redarGrid").children[35].children[0].innerHTML = -10;
    document.getElementById("redarGrid").children[34].children[0].innerHTML = -20;
    document.getElementById("redarGrid").children[33].children[0].innerHTML = -30;
    document.getElementById("redarGrid").children[32].children[0].innerHTML = -40;
    document.getElementById("redarGrid").children[31].children[0].innerHTML = -50;
    document.getElementsByClassName("Radarcenter")[0].children[2].innerHTML = ""

  }else{
    
    document.getElementsByClassName("radarIndicator")[0].classList.remove("radarIndicatorShow");
    document.getElementsByClassName("radarIndicator")[0].classList.remove("radarIndicatorGPWS");
    document.getElementById("redarGrid").children[35].children[0].innerHTML = 350;
    document.getElementById("redarGrid").children[34].children[0].innerHTML = 340;
    document.getElementById("redarGrid").children[33].children[0].innerHTML = 330;
    document.getElementById("redarGrid").children[32].children[0].innerHTML = 320;
    document.getElementById("redarGrid").children[31].children[0].innerHTML = 310;
    document.getElementsByClassName("Radarcenter")[0].children[2].innerHTML = ""
    document.getElementsByClassName("radarIndicator")[0].style.transform = 'rotateY(0deg) rotate(0deg)'
    //document.getElementsByClassName("radarIndicatorCont")[0].style.transform = 'rotateY(180deg)'
  }
  
}