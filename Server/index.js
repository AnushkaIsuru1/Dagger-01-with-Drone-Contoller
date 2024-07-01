const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const parser = new ReadlineParser({ delimeter: '\r\n' });
var fs = require('fs')

var express = require('express');
var cors = require('cors');
const app = express()


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

var ss = ""

port.pipe(parser);
parser.on('data', (data) => {
  console.log(data.toString());
  ss += data.toString()
});


app.use(express.json())
app.use(cors())

app.get("/", (req, res) => {
  res.json(
    ss
  )
  ss = ""
})

app.post("/", async (req, res) => {
  try {
    port.write(req.body.cmd)
    res.status(201).json({ status: 'Success' })
  } catch (w) {
    console.log(w)
  }
})


app.put("/", async (req, res) => {
  fs.appendFile("log.csv", req.body.str, (err) => {
    if (err) throw err;
  });
  res.status(201).json({ status: 'Success' })
})


app.listen(8080, () => {
  console.log("Connected to backend")
})