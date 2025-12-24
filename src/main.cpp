/**
 * ESP Chas TV - Chaoslab TV Show Collaboration
 * Main ESP32 Program
 * 
 * This is the main entry point for the ESP32 Chaoslab TV Show project.
 * It sets up WiFi, web server, and display capabilities for interactive
 * TV show experiences.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Configuration
const char* ssid = "ESP_CHAS_TV";           // Default AP name
const char* password = "chaoslab2024";       // Default AP password

// Web server on port 80
WebServer server(80);

// LED Pin (built-in LED)
const int LED_PIN = 2;

// State variables
bool showRunning = false;
int viewerCount = 0;
String currentScene = "Welcome";

/**
 * Handle root web page
 */
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>ESP Chas TV Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; padding: 20px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; }";
  html += "h1 { font-size: 2.5em; margin-bottom: 10px; }";
  html += ".card { background: rgba(255,255,255,0.1); backdrop-filter: blur(10px); padding: 20px; border-radius: 15px; margin: 20px auto; max-width: 500px; box-shadow: 0 8px 32px rgba(0,0,0,0.3); }";
  html += ".status { font-size: 1.2em; margin: 15px 0; }";
  html += ".button { background: #4CAF50; color: white; border: none; padding: 15px 32px; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 5px; cursor: pointer; border-radius: 8px; transition: all 0.3s; }";
  html += ".button:hover { background: #45a049; transform: scale(1.05); }";
  html += ".stop { background: #f44336; }";
  html += ".stop:hover { background: #da190b; }";
  html += ".info { background: #2196F3; }";
  html += ".info:hover { background: #0b7dda; }";
  html += "</style></head><body>";
  html += "<h1>🎬 ESP Chas TV</h1>";
  html += "<h3>Chaoslab TV Show Collaboration</h3>";
  html += "<div class='card'>";
  html += "<div class='status'>📺 Show Status: <strong>" + String(showRunning ? "LIVE" : "Off Air") + "</strong></div>";
  html += "<div class='status'>👥 Viewers: <strong>" + String(viewerCount) + "</strong></div>";
  html += "<div class='status'>🎭 Current Scene: <strong>" + currentScene + "</strong></div>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>Controls</h2>";
  html += "<a href='/start'><button class='button'>▶️ Start Show</button></a>";
  html += "<a href='/stop'><button class='button stop'>⏹️ Stop Show</button></a><br>";
  html += "<a href='/scene1'><button class='button info'>Scene 1</button></a>";
  html += "<a href='/scene2'><button class='button info'>Scene 2</button></a>";
  html += "<a href='/scene3'><button class='button info'>Scene 3</button></a>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<p>🔌 Device: ESP32</p>";
  html += "<p>📡 IP: " + WiFi.softAPIP().toString() + "</p>";
  html += "</div>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
  viewerCount++;
}

/**
 * Start the show
 */
void handleStart() {
  showRunning = true;
  digitalWrite(LED_PIN, HIGH);
  currentScene = "Opening";
  Serial.println("🎬 Show started!");
  
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
 * Stop the show
 */
void handleStop() {
  showRunning = false;
  digitalWrite(LED_PIN, LOW);
  currentScene = "Ended";
  Serial.println("⏹️ Show stopped!");
  
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
 * Change to scene 1
 */
void handleScene1() {
  currentScene = "Scene 1: Introduction";
  Serial.println("🎭 Changed to Scene 1");
  
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
 * Change to scene 2
 */
void handleScene2() {
  currentScene = "Scene 2: Main Act";
  Serial.println("🎭 Changed to Scene 2");
  
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
 * Change to scene 3
 */
void handleScene3() {
  currentScene = "Scene 3: Finale";
  Serial.println("🎭 Changed to Scene 3");
  
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
 * Handle 404 errors
 */
void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

/**
 * Setup function - runs once at startup
 */
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n🎬 ESP Chas TV Starting...");
  Serial.println("================================");
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Start WiFi Access Point
  Serial.println("📡 Starting WiFi Access Point...");
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("✅ AP Started! IP address: ");
  Serial.println(IP);
  Serial.print("📱 Connect to WiFi: ");
  Serial.println(ssid);
  Serial.print("🔑 Password: ");
  Serial.println(password);
  
  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/start", handleStart);
  server.on("/stop", handleStop);
  server.on("/scene1", handleScene1);
  server.on("/scene2", handleScene2);
  server.on("/scene3", handleScene3);
  server.onNotFound(handleNotFound);
  
  // Start web server
  server.begin();
  Serial.println("🌐 Web server started!");
  Serial.println("================================");
  Serial.println("🎭 Ready for collaboration!");
  Serial.println("================================\n");
}

/**
 * Main loop - runs continuously
 */
void loop() {
  // Handle web server requests
  server.handleClient();
  
  // LED blink effect when show is running
  if (showRunning) {
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 500) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      lastBlink = millis();
    }
  }
  
  delay(10);
}
