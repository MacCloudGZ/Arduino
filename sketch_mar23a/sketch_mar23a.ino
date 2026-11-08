#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "Zenffer";
const char* password = "00000000";

#define LED_PIN 4
#define NUM_LEDS 10

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

WiFiServer server(80);

int r = 0, g = 0, b = 0; // current RGB values
int brightness = 255;    // 0..255 global brightness scaler

static void setColor(int red, int green, int blue) {
  red = constrain(red, 0, 255);
  green = constrain(green, 0, 255);
  blue = constrain(blue, 0, 255);

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

static int readParamInt(const String& query, char key, int fallbackValue) {
  int kPos = query.indexOf(key);
  if (kPos < 0) return fallbackValue;

  int valueStart = kPos + 1;
  int nextPos = query.length();
  for (int i = valueStart; i < (int)query.length(); i++) {
    char ch = query[i];
    // Stop at the next parameter key or the end delimiter.
    if (ch == 'r' || ch == 'g' || ch == 'b' || ch == 'l' || ch == '&') {
      nextPos = i;
      break;
    }
  }
  if (nextPos <= valueStart) return fallbackValue;
  return query.substring(valueStart, nextPos).toInt();
}

// Keep the HTML in one place for easier editing.
static const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
</head>
<body>
  <div class="container">
    <div class="row">
      <h1>ESP Color Picker</h1>
    </div>
    <a class="btn btn-primary btn-lg mt-3" href="#" id="change_color" role="button">Change Color</a>

    <div class="mt-4">
      <div class="mb-3">
        <div>R: <span id="rVal">0</span></div>
        <input type="range" min="0" max="255" value="0" id="r" class="custom-range" />
      </div>

      <div class="mb-3">
        <div>G: <span id="gVal">255</span></div>
        <input type="range" min="0" max="255" value="255" id="g" class="custom-range" />
      </div>

      <div class="mb-3">
        <div>B: <span id="bVal">0</span></div>
        <input type="range" min="0" max="255" value="0" id="b" class="custom-range" />
      </div>

      <div class="mb-2">
        <div>Brightness: <span id="lVal">255</span></div>
        <input type="range" min="0" max="255" value="255" id="l" class="custom-range" />
      </div>
    </div>
  </div>

  <script>
    function update() {
      const r = document.getElementById('r').value;
      const g = document.getElementById('g').value;
      const b = document.getElementById('b').value;
      const l = document.getElementById('l').value;

      document.getElementById('rVal').innerText = r;
      document.getElementById('gVal').innerText = g;
      document.getElementById('bVal').innerText = b;
      document.getElementById('lVal').innerText = l;

      document.getElementById("change_color").href =
        "?r" + r + "g" + g + "b" + b + "l" + l + "&";
    }

    ['r', 'g', 'b', 'l'].forEach(id => {
      document.getElementById(id).addEventListener('input', update);
    });

    update();
  </script>
</body>
</html>
)HTML";

void setup() {
  Serial.begin(115200);

  strip.begin();
  // On boot: show off (black)
  setColor(0, 0, 0);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String requestLine;
  String line;
  bool gotRequestLine = false;

  // Read until the blank line that ends HTTP headers.
  while (client.connected()) {
    if (!client.available()) {
      delay(1);
      continue;
    }

    char c = client.read();
    if (c == '\r') continue;

    if (c == '\n') {
      if (line.length() == 0) break; // end of headers
      if (!gotRequestLine) {
        requestLine = line;
        gotRequestLine = true;
      }
      line = "";
    } else {
      line += c;
    }
  }

  // Parse query from: GET /?r201g32b255& HTTP/1.1
  int qStart = requestLine.indexOf("/?");
  if (qStart >= 0) {
    int qEnd = requestLine.indexOf(' ', qStart);
    String query = requestLine.substring(qStart + 2, qEnd > 0 ? qEnd : requestLine.length());

    // Parse slider values from: /?r<val>g<val>b<val>l<val>&
    int nr = readParamInt(query, 'r', r);
    int ng = readParamInt(query, 'g', g);
    int nb = readParamInt(query, 'b', b);
    int nl = readParamInt(query, 'l', brightness);

    r = constrain(nr, 0, 255);
    g = constrain(ng, 0, 255);
    b = constrain(nb, 0, 255);
    brightness = constrain(nl, 0, 255);

    // Brightness scales each channel (0..255).
    int rr = (r * brightness) / 255;
    int gg = (g * brightness) / 255;
    int bb = (b * brightness) / 255;
    setColor(rr, gg, bb);
  }

  // Web response.
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=utf-8");
  client.println("Connection: close");
  client.println();

  client.print(PAGE);

  client.stop();
}