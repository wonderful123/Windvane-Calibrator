#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <numeric>

void setupWiFi();
void setupWebServer();
void handleRoot(AsyncWebServerRequest *request);
void collectAdcData();
float calculateStdDev(const std::vector<int>& values);
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
extern const String html_head;
extern const String js_websocket;
extern const String js_chart;
extern const String html_body;

const char* ssid = "GretaThurnberg";
const char* password = "trump2020";

const int windVanePin = 34;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

std::vector<int> adcValues;
std::vector<int> adcBuffer; // Buffer to store the last N ADC readings
const size_t adcBufferSize = 10; // Size of the buffer

const float stdDevThreshold = 5.0;
bool isSpinning = false;

void setup() {
  Serial.begin(115200);
  
  setupWiFi();
  setupWebServer();

  pinMode(windVanePin, INPUT);
  analogSetAttenuation(ADC_11db);  // Set ADC attenuation to 11 dB
}

void loop() {
  ws.cleanupClients();
  if (isSpinning) {
    collectAdcData();
  }
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}


void collectAdcData() {
  int adcValue = analogRead(windVanePin);
  
  // Add the new reading to the buffer and remove the oldest if buffer is full
  adcBuffer.push_back(adcValue);
  if (adcBuffer.size() > adcBufferSize) {
    adcBuffer.erase(adcBuffer.begin());
  }
  
  // Calculate the standard deviation of the readings in the buffer
  float currentStdDev = calculateStdDev(adcBuffer);

  // Check if the vane has stopped based on standard deviation
  if (currentStdDev < stdDevThreshold) {
    isSpinning = false;
    ws.textAll("Vane stopped. Data collection complete.");
  } else {
    String msg = "ADC Value:" + String(adcValue);
    ws.textAll(msg);
  }
}

float calculateStdDev(const std::vector<int>& values) {
  float mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
  float variance = 0.0;

  for (const auto& val : values) {
    variance += (val - mean) * (val - mean);
  }
  variance /= values.size();

  return std::sqrt(variance);
}

void setupWebServer() {
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    // ... (Previous connect logic)
  } else if(type == WS_EVT_DISCONNECT){
    // ... (Previous disconnect logic)
  } else if(type == WS_EVT_DATA){
    if (strcmp((char*)data, "start") == 0) {
      isSpinning = true;
      ws.textAll("Start spinning the vane clockwise...");
    }
  }
}

void handleRoot(AsyncWebServerRequest *request) {
  String html = html_head + js_websocket + js_chart + html_body;
  request->send(200, "text/html", html);
}

const String html_head = R"(
<!DOCTYPE HTML><html><head>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <script>
    var ws;
    var adcData = [];
    var myChart;
)";

const String js_websocket = R"(
    function setupWebSocket() {
      ws = new WebSocket('ws://' + window.location.hostname + '/ws');
      ws.onopen = function() { console.log('Connected'); };
      ws.onclose = function() { console.log('Disconnected'); };
      ws.onmessage = function(event) {
        if (event.data.startsWith("ADC Value:")) {
          var value = parseInt(event.data.split(":")[1]);
          adcData.push({x: adcData.length, y: value});
          myChart.update();
        } else {
          document.getElementById("output").value += event.data + "\n";
        }
      };
    }
)";

const String js_chart = R"(
    function setupChart() {
      var ctx = document.getElementById('myChart').getContext('2d');
      myChart = new Chart(ctx, {
        type: 'line',
        data: {
          datasets: [{
            label: 'ADC Values',
            data: adcData,
            borderColor: 'rgba(75, 192, 192, 1)',
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            x: { type: 'linear', position: 'bottom' }
          }
        }
      });
    }
)";

const String html_body = R"(
  </script>
</head>
<body onload="setupWebSocket(); setupChart();">
  <button onclick="ws.send('start');">Start</button>
  <textarea id="output" style="width:300px;height:150px;overflow:auto;"></textarea>
  <canvas id="myChart"></canvas>
</body>
</html>
)";