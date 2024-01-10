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