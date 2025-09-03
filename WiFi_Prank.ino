#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "";
ESP8266WebServer server(80);
DNSServer dns;

void handleRoot() {
  String page = R"rawliteral(
    <!DOCTYPE html><html><head><meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Sambungkan WiFi</title>
    <style>
      body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; }
      .container { margin-top: 50px; }
      img { max-width: 200px; margin-top: 30px; }
      h2 { margin-top: 20px; font-weight: normal; }
      button {
        background-color: #e60000; color: white; border: none;
        padding: 15px 30px; font-size: 18px; border-radius: 8px;
        cursor: pointer; margin-top: 30px;
      }
      button:hover { background-color: #cc0000; }
    </style>
    </head><body>
    <div class='container'>
      <img src='/indihome.png'><br>
      <h2>Ketuk untuk menghubungkan.</h2>
      <button onclick="document.getElementById('audio').play(); this.disabled=true; this.innerText='Terhubung';">Hubungkan</button>
      <audio id='audio'>
        <source src='/prank02.mp3' type='audio/mpeg'>
        Browser tidak mendukung audio.
      </audio>
    </div>
    </body></html>
  )rawliteral";

  server.send(200, "text/html", page);
}

void handleMP3() {
  File mp3File = LittleFS.open("/prank02.mp3", "r");
  if (!mp3File) {
    server.send(404, "text/plain", "File tidak ditemukan");
    return;
  }

  size_t fileSize = mp3File.size();
  String range = server.header("Range");

  if (range.length() > 0) {
    int start = range.indexOf('=') + 1;
    int dash = range.indexOf('-');
    int rangeStart = range.substring(start, dash).toInt();
    int rangeEnd = fileSize - 1;

    if (rangeStart >= fileSize) {
      server.send(416, "text/plain", "Range Invalid");
      mp3File.close();
      return;
    }

    mp3File.seek(rangeStart, SeekSet);
    server.setContentLength(rangeEnd - rangeStart + 1);
    server.sendHeader("Content-Type", "audio/mpeg");
    server.sendHeader("Accept-Ranges", "bytes");
    server.sendHeader("Content-Range", "bytes " + String(rangeStart) + "-" + String(rangeEnd) + "/" + String(fileSize));
    server.send(206);

    uint8_t buffer[1024];
    size_t sent = 0;
    while (mp3File.available() && sent < (rangeEnd - rangeStart + 1)) {
      size_t toRead = min(sizeof(buffer), (rangeEnd - rangeStart + 1 - sent));
      size_t bytesRead = mp3File.read(buffer, toRead);
      server.client().write(buffer, bytesRead);
      sent += bytesRead;
    }
  } else {
    server.sendHeader("Content-Type", "audio/mpeg");
    server.sendHeader("Content-Length", String(fileSize));
    server.sendHeader("Accept-Ranges", "bytes");
    server.streamFile(mp3File, "audio/mpeg");
  }

  mp3File.close();
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid);
  dns.start(53, "*", WiFi.softAPIP());
  LittleFS.begin();
  if (!LittleFS.begin()) {
    Serial.println("Gagal mount LittleFS");
    return;
  }

  server.on("/", handleRoot);
  server.on("/prank02.mp3", HTTP_GET, handleMP3);
  server.on("/indihome.png", HTTP_GET, []() {
    File file = LittleFS.open("/indihome.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  });
  server.onNotFound(handleRoot);
  server.begin();
  Serial.println("Server siap!");
  ArduinoOTA.begin();
}

void loop() {
  dns.processNextRequest();
  server.handleClient();
  ArduinoOTA.handle();
}
