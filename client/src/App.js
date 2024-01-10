import { React, useEffect, useState } from "react";
import axios from "axios";
var isDisconnected = 0;

function c1(){sendToArduino('Q');}
function c2(){sendToArduino('W');}
function c3(){sendToArduino('E');}
function c4(){sendToArduino('R');}
function c5(){sendToArduino('T');}
function c6(){sendToArduino('Y');}
