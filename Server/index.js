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
