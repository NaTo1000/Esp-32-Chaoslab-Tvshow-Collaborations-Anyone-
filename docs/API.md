# ESP Chas TV API Documentation

This document describes the web API and control interfaces for the ESP Chas TV project.

## Web Interface

### Base URL
```
http://192.168.4.1
```
(When connected to ESP_CHAS_TV WiFi network)

## Endpoints

### GET `/`
**Description**: Main control interface

**Returns**: HTML page with:
- Current show status (Live/Off Air)
- Viewer count
- Current scene name
- Control buttons
- Device information

**Example Response**: Interactive web page with TV show controls

---

### GET `/start`
**Description**: Start the TV show

**Action**:
- Sets `showRunning = true`
- Turns on built-in LED
- Sets current scene to "Opening"
- Logs "🎬 Show started!" to serial

**Returns**: Redirect to `/` (HTTP 303)

---

### GET `/stop`
**Description**: Stop the TV show

**Action**:
- Sets `showRunning = false`
- Turns off built-in LED
- Sets current scene to "Ended"
- Logs "⏹️ Show stopped!" to serial

**Returns**: Redirect to `/` (HTTP 303)

---

### GET `/scene1`
**Description**: Change to Scene 1

**Action**:
- Sets current scene to "Scene 1: Introduction"
- Logs scene change to serial

**Returns**: Redirect to `/` (HTTP 303)

---

### GET `/scene2`
**Description**: Change to Scene 2

**Action**:
- Sets current scene to "Scene 2: Main Act"
- Logs scene change to serial

**Returns**: Redirect to `/` (HTTP 303)

---

### GET `/scene3`
**Description**: Change to Scene 3

**Action**:
- Sets current scene to "Scene 3: Finale"
- Logs scene change to serial

**Returns**: Redirect to `/` (HTTP 303)

---

## State Variables

### `showRunning` (boolean)
- `true`: Show is currently running
- `false`: Show is off air

### `viewerCount` (integer)
- Increments each time someone visits the main page
- Tracks total page views since ESP32 started

### `currentScene` (String)
- Default: "Welcome"
- Changes based on scene buttons
- Options:
  - "Welcome"
  - "Opening"
  - "Scene 1: Introduction"
  - "Scene 2: Main Act"
  - "Scene 3: Finale"
  - "Ended"

## WiFi Configuration

### Access Point Mode

**SSID**: `ESP_CHAS_TV`  
**Password**: `chaoslab2024`  
**IP Address**: `192.168.4.1`  
**Gateway**: `192.168.4.1`  
**Subnet**: `255.255.255.0`

### Changing WiFi Credentials

Edit in `src/main.cpp`:
```cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```

## Serial Interface

### Baud Rate
```
115200
```

### Messages

#### Startup
```
🎬 ESP Chas TV Starting...
================================
📡 Starting WiFi Access Point...
✅ AP Started! IP address: 192.168.4.1
📱 Connect to WiFi: ESP_CHAS_TV
🔑 Password: chaoslab2024
🌐 Web server started!
================================
🎭 Ready for collaboration!
================================
```

#### Runtime
- `🎬 Show started!` - When `/start` is triggered
- `⏹️ Show stopped!` - When `/stop` is triggered
- `🎭 Changed to Scene X` - When scene changes

## LED Behavior

### Built-in LED (GPIO 2)

**When show is running** (`showRunning = true`):
- Blinks every 500ms (on/off)

**When show is stopped** (`showRunning = false`):
- Off

## Extending the API

### Adding New Endpoints

1. **Define handler function**:
```cpp
void handleNewEndpoint() {
  // Your code here
  server.sendHeader("Location", "/");
  server.send(303);
}
```

2. **Register in `setup()`**:
```cpp
server.on("/newendpoint", handleNewEndpoint);
```

### Adding New State Variables

```cpp
// Global scope
int newVariable = 0;

// In handler
void handleNewAction() {
  newVariable++;
  // Use the variable
}

// Display in HTML
html += "New Value: " + String(newVariable);
```

## Integration Examples

### Using cURL

```bash
# Start the show
curl http://192.168.4.1/start

# Change to scene 2
curl http://192.168.4.1/scene2

# Stop the show
curl http://192.168.4.1/stop
```

### Using Python

```python
import requests

base_url = "http://192.168.4.1"

# Start show
response = requests.get(f"{base_url}/start")

# Get status
response = requests.get(base_url)
print(response.text)

# Stop show
requests.get(f"{base_url}/stop")
```

### Using JavaScript (Node.js)

```javascript
const fetch = require('node-fetch');

const baseUrl = 'http://192.168.4.1';

// Start show
fetch(`${baseUrl}/start`)
  .then(res => console.log('Show started'))
  .catch(err => console.error(err));

// Change scene
fetch(`${baseUrl}/scene1`)
  .then(res => console.log('Scene changed'))
  .catch(err => console.error(err));
```

## Security Considerations

### Current Implementation

⚠️ **Warning**: The current implementation is for demonstration and learning purposes.

**Limitations**:
- No authentication
- Open WiFi access (password-protected but shared)
- No HTTPS/encryption
- No rate limiting
- No input validation

### Recommended for Production

If deploying in a production environment:

1. **Add authentication**:
   - Basic Auth
   - Token-based authentication
   - OAuth integration

2. **Enable HTTPS**:
   - Use TLS/SSL
   - Generate certificates

3. **Implement rate limiting**:
   - Prevent abuse
   - Limit requests per client

4. **Add input validation**:
   - Sanitize inputs
   - Prevent injection attacks

5. **Use stronger WiFi security**:
   - WPA3 if supported
   - Unique passwords per device
   - Hidden SSID option

## Future API Enhancements

Potential additions for the API:

- **POST endpoints** for submitting data
- **JSON responses** for API-only clients
- **WebSocket support** for real-time updates
- **RESTful architecture** for better organization
- **API versioning** for backward compatibility
- **MQTT integration** for IoT messaging
- **Authentication system** for secure access

## Troubleshooting

### Can't access API
- Ensure connected to `ESP_CHAS_TV` WiFi
- Verify ESP32 is powered and running
- Check serial monitor for IP address
- Try `http://192.168.4.1` exactly

### API responds slowly
- WiFi signal strength
- Too many simultaneous connections
- ESP32 processing power limits

### Endpoints not working
- Check serial monitor for errors
- Verify endpoint spelling
- Try restarting ESP32

---

**Questions about the API? Open an issue!** 📡✨
