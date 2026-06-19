#include "webSocket.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
    background-color: #98EDE6; 
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }

  .card {
    background-color: #FFFFFF;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }

  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Air Quality Monitoring</h2>
      <p>Temperature: <span id="temp">--</span> &#8451;</p>
      <p>Humidity: <span id="humi">--</span> &#37;</p>
      <p>CO2: <span id="co2">--</span> ppm</p>
      <p>VOC Index: <span id="voc">--</span></p>
      <p>Date: <span id="date">--</span></p>
      <p>Time: <span id="time">--</span></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
     let values = event.data.split(",");

document.getElementById("temp").innerHTML = values[0];
document.getElementById("humi").innerHTML = values[1];
document.getElementById("co2").innerHTML  = values[2];
document.getElementById("voc").innerHTML  = values[3];
document.getElementById("date").innerHTML = values[4];
document.getElementById("time").innerHTML = values[5];
    
  }
 
  function onLoad(event) {
    initWebSocket();

  }
  
</script>
</body>
</html>
)rawliteral";




void notifyClients()
{
    String msg =
      String(g_temp,1) + "," +
      String(g_humidity,1) + "," +
      String(g_co2,0) + "," +
      String(g_voc) + "," +
      dateValue + "," +
      timeValue;

    ws.textAll(msg);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
 {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;  
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
      notifyClients();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void getData()
{
    // SCD30 ->
  if (scd30.dataReady())
    {
        if (scd30.read())
        {
            g_temp = scd30.temperature;
            g_humidity = scd30.relative_humidity;
            g_co2  = scd30.CO2;
        }
    }

    // RTC ->
    DateTime now = rtc.now();
    
    sprintf(g_date,"%04d/%02d/%02d",
            now.year(),
            now.month(),
            now.day());

    dateValue = String(g_date);


    sprintf(g_time,"%02d:%02d:%02d",
            now.hour(),
            now.minute(),
            now.second());

    timeValue = String(g_time);


    // SGP ->
    uint16_t srawVoc;
    uint16_t error;

    error = sgp40.measureRawSignal(
                0x8000,
                0x6666,
                srawVoc);

    if (!error)
    {
        g_voc = vocAlgorithm.process(srawVoc);
    }
}