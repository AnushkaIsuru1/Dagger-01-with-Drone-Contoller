const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const parser = new ReadlineParser({ delimeter : '\r\n'});
var fs = require('fs')
