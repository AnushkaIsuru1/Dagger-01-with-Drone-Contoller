
function App() {
  return (
    <div className="App">
      <Headers>
        <ul>
          <li>Setting</li>
          <li>Map</li>
          <li>Reboot</li>
          <li>Shut Down</li>
          <li>Discconnect</li>
        </ul>
      </Headers>
      <main>
        <div className="Left"></div>
        <div className="RadarCont"></div>
        <button className="btn1">LAND SCANNER</button>
        <button className="btn1">Altitude Map</button>
        <button className="btn2">Auto Land</button>
        <button className="btn2"></button>
        <div className="right"></div>

        
      </main>
    </div>
  );
}

export default App;
