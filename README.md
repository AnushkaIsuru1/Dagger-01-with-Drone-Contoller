# Drone Controller App with DAGGER - 01
<br>

<p align="center">
  <img width="100%"  src="/Drone Contoller App.jpg" alt="Drone Contoller App">
</p>


## Overview

Dagger - 01 is a drone model created using Arduino as part of a university project. Developed with my colleagues. The primary purpose of the app was to create an application to control the drone via a computer, integrating advanced functionalities like finding suitable landing areas, identifying impacts, auto-landing, and GPWS (Ground Proximity Warning System).

By overcoming challenges like Arduino's single-thread limitation and Bluetooth communication issues, I created a functional and visually appealing control system for the drone, fulfilling both technical requirements and my desire to design a sci-fi user interface.
<br>

### Stack

  - **1. React** - Develop Front-end
  - **2. Node JS** - Develop Back-end
  - **3. Arduino** - Programme Drone Model
<hr>

## DAGGER - 01
<p align="center">
  <img width="100%"  src="DAGGER - 01 (A Drone Model).jpg" alt="DAGGER - 01 (A Drone Model)">
</p>

### <a href="https://youtu.be/2T9WsozWct8">Watch this project in Youtube</a>
<hr>

## Special Features

### 01. Multi-threading for Arduino

Arduino's lack of multi-threading posed a challenge as the drone's process scanner could not respond to IO operations concurrently. To overcome this, a feedback loop was implemented, allowing the scanner to temporarily save its state and resume from the last saved state after receiving feedback.

```js
....
void feedbackLoop(){
  inp='\n';
  if(Serial.available()){
    inp = Serial.read();
    inputCounter = 0;
  }else{
    inputCounter++;
    if((inputCounter == (6000  * 1000)) && isFlying){      
      offline();
    }
  }
  ...
}
...

switch (inp)
  {
    case 'GPWS':
    ScannerStateReset();
    topServoForGPWS(0);
    rotateServoSameDirection(90) ;
    Serial.println("GPWS");
    break;
  }
...
```

### 02. Logging

I implemented logging functions in the back-end to track:

- Commands sent to the drone
- Actions taken by the drone in response to commands
- Procedure of actions
- Sensor reading

#### Log Sample:
| Direction | Timestamp           | Source          | Data        |
|---|---|---|---|
| INCOMING  | 2023-07-24 17:53:41 | recieveArduino  | PW          |
| INCOMING  | 2023-07-24 17:53:53 | recieveArduino  | SL          |
| OUTGOING  | 2023-07-24 17:53:53 | transmitArduino | LAUNCH      |
| INCOMING  | 2023-07-24 17:53:55 | recieveArduino  | X1          |
| INCOMING  | 2023-07-24 17:54:15 | recieveArduino  | SS 90       |
| INCOMING  | 2023-07-24 17:54:16 | recieveArduino  | 0\t\t54      |
| INCOMING  | 2023-07-24 17:54:16 | recieveArduino  | AT54        |
| INCOMING  | 2023-07-24 17:56:24 | recieveArduino  | GS-1-10-0-0 |
| INCOMING  | 2023-07-24 17:56:25 | recieveArduino  | GS-1-20-0-0 |
| INCOMING  | 2023-07-24 17:56:25 | recieveArduino  | GS-1-30-0-0 |
| INCOMING  | 2023-07-24 17:56:24 | recieveArduino  | 53\t\t53      |
| INCOMING  | 2023-07-24 17:56:24 | recieveArduino  | AT53        |

<hr>

## Backend

  The back-end, developed in Node.js, facilitated communication between the computer and the drone model via Bluetooth. Since React does not support direct communication with Bluetooth devices, the Node.js server handled all Bluetooth interactions, relaying information between the drone and the front-end application.
  
```js
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const parser = new ReadlineParser({ delimeter : '\r\n'});
var fs = require('fs')

const config = {
  path: 'COM5',
  baudRate: 9600,
  dataBits: 8,
  parity: 'none',
  stopBits: 1,
  autoOpen: false,
};

const port = new SerialPort(config);
port.open((err) => {
  if (err) {
    console.log(err.messages);
  }
});

port.pipe(parser);
parser.on('data', (data) => {
  console.log(data.toString());  
  ...
});
...
```
<hr>

## <a href="https://youtu.be/2T9WsozWct8">Watch this project in Youtube</a>

<br>
