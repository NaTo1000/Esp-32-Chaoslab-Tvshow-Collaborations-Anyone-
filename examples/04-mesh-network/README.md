# Mesh Network Communication

Build a self-organizing, self-healing mesh network where ESP32 devices relay messages for each other.

## 🕸️ What is Mesh Networking?

A mesh network is a decentralized network where:
- **No central hub** - Every device can route messages
- **Self-healing** - If one path fails, messages find another route
- **Extended range** - Messages hop between devices
- **Scalable** - Add more nodes for greater coverage

Think of it like the internet, but with your ESP32 devices!

## 🎯 Why Mesh for TV Shows?

### 1. Massive Coverage
One device covers 250m, but 10 devices in mesh can cover kilometers!

### 2. Reliability
If one device fails, network keeps working through other paths.

### 3. Mobile Nodes
Performers can carry ESP32 nodes that stay connected while moving.

### 4. Audience Network
Hundreds of audience devices can communicate through mesh.

### 5. No Infrastructure
Works anywhere - no WiFi router or cellular needed!

## 🏗️ Architecture

```
     [Node A] -------- [Node B]
         |                |
         |                |
     [Node C] -------- [Node D] -------- [Node E]
                          |
                      [Node F]
```

Message from A to F might go: A → B → D → F

## 📦 Hardware Required

- 3 or more ESP32 boards (more = better mesh!)
- USB cables for programming
- Optional: External antennas for better range
- Optional: Batteries for mobile nodes

## 🔧 Implementation Approaches

### Approach 1: ESP-NOW Mesh (Recommended for beginners)

Uses ESP-NOW as transport, custom routing logic.

**Pros**:
- Fast (low latency)
- No external libraries needed
- Good range (250m)

**Cons**:
- Manual routing implementation
- Limited to ESP32 devices

### Approach 2: painlessMesh Library

Established mesh library for ESP32.

**Pros**:
- Proven, stable
- Auto-routing included
- Easy to use

**Cons**:
- WiFi-based (more power)
- Can interfere with other WiFi

### Approach 3: LoRa Mesh (Maximum range)

Mesh network over LoRa radio.

**Pros**:
- Extremely long range (10km+)
- Low power
- Works in remote areas

**Cons**:
- Requires LoRa hardware
- Slower data rate
- More complex

### Approach 4: Meshtastic Protocol

Open-source mesh protocol optimized for range and reliability.

**Pros**:
- Mature protocol
- Tested in real-world
- Community support
- Encryption built-in

**Cons**:
- Requires LoRa hardware
- Steeper learning curve

## 🚀 Quick Start: painlessMesh

### Installation

Add to `platformio.ini`:
```ini
lib_deps = 
    painlessmesh/painlessMesh@^1.5.0
```

### Basic Example

```cpp
#include <painlessMesh.h>

#define MESH_PREFIX     "ChaosMesh"
#define MESH_PASSWORD   "chaoslab2024"
#define MESH_PORT       5555

Scheduler userScheduler;
painlessMesh mesh;

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("📨 From %u: %s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("✅ New node connected: %u\n", nodeId);
}

void setup() {
  Serial.begin(115200);
  
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  
  Serial.println("🕸️ Mesh network started!");
}

void loop() {
  mesh.update();
  
  // Send broadcast message every 10 seconds
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 10000) {
    String msg = "Hello from node " + String(mesh.getNodeId());
    mesh.sendBroadcast(msg);
    lastSend = millis();
  }
}
```

## 🎭 TV Show Use Cases

### 1. Multi-Stage Festival

```
[Main Stage] -------- [Side Stage A]
      |                     |
[Production] -------- [Side Stage B]
      |
[Lighting Booth] --- [Sound Booth]
```

All stages coordinate through mesh network.

### 2. Mobile Performers

Dancers/actors wear ESP32 nodes:
- Costume effects synchronized
- Position tracking
- Emergency communications
- Interactive lighting

### 3. Audience Participation

Every audience member has a node:
- Voting propagates through mesh
- Wave effects (Mexican wave, but with LEDs!)
- Collective decision-making
- Creates "audience cloud"

### 4. Redundant Communication

Primary: WiFi  
Backup 1: ESP-NOW mesh  
Backup 2: LoRa mesh  

Show must go on!

### 5. Extended Range Coverage

Cover entire festival grounds with strategic node placement.

## 💡 Advanced Features

### 1. Message Routing

```cpp
// Send to specific node
uint32_t targetNode = 123456;
mesh.sendSingle(targetNode, "Hello target!");

// Broadcast to all
mesh.sendBroadcast("Hello everyone!");

// Reply to sender
void receivedCallback(uint32_t from, String &msg) {
  mesh.sendSingle(from, "Got your message!");
}
```

### 2. Node Roles

```cpp
enum NodeRole {
  MASTER,      // Control center
  STAGE,       // Stage device
  LIGHTING,    // Light controller
  SOUND,       // Sound device
  MOBILE       // Performer/audience
};

NodeRole myRole = STAGE;

// Include role in messages
String msg = String(myRole) + ":" + "Status OK";
mesh.sendBroadcast(msg);
```

### 3. Time Synchronization

```cpp
uint32_t networkTime = mesh.getNodeTime();

// Schedule event at specific time
uint32_t eventTime = networkTime + 5000000; // 5 seconds

void checkSchedule() {
  if (mesh.getNodeTime() >= eventTime) {
    // Execute synchronized action!
  }
}
```

### 4. Network Topology

```cpp
// Get connected nodes
std::list<uint32_t> nodes = mesh.getNodeList();

Serial.println("Connected nodes:");
for (uint32_t node : nodes) {
  Serial.println(node);
}

// Visualize network
void printTopology() {
  Serial.println(mesh.subConnectionJson());
}
```

## 📊 Performance Metrics

### painlessMesh (WiFi-based)
- **Range per hop**: ~50-100m indoors, 250m outdoors
- **Latency**: 50-200ms per hop
- **Max nodes**: 50+ (tested)
- **Data rate**: Good (1-2 Mbps)

### LoRa Mesh
- **Range per hop**: 2-10km (line of sight)
- **Latency**: 500-2000ms per hop
- **Max nodes**: 100+ (theoretical)
- **Data rate**: Low (~250 bytes/sec)

## 🔒 Security

### 1. Encryption

painlessMesh has built-in encryption:
```cpp
// Already encrypted with MESH_PASSWORD
// Use strong password!
#define MESH_PASSWORD   "y0ur$tr0ngP@ssw0rd"
```

### 2. Node Authentication

```cpp
// Whitelist of allowed nodes
std::set<uint32_t> allowedNodes = {123456, 234567, 345678};

void receivedCallback(uint32_t from, String &msg) {
  if (allowedNodes.find(from) == allowedNodes.end()) {
    Serial.println("⛔ Unauthorized node!");
    return;
  }
  // Process message
}
```

### 3. Message Signing

```cpp
#include <mbedtls/md.h>

// Sign messages with secret key
String signMessage(String msg) {
  // Implement HMAC signing
  return msg + ":" + generateHMAC(msg);
}
```

### 4. Meshtastic Barrier + House-of-Mirrors Defense

Use when a node appears to be force-seeking addresses or probing routes:

```cpp
// Conceptual pseudocode; Meshtastic does not ship these helpers—implement them in your firmware
const int MAX_TRUSTED_HOPS = 6; // tighten for small/private meshes, loosen for long routes
const String BROADCAST_DEST = "ALL"; // adjust to whatever your payload uses for broadcast

// Placeholder hooks to implement in firmware (custom wrappers; not base Meshtastic API)
bool rateExceeded(uint32_t nodeId);
String extractDest(const String& msg);
int extractHopLimit(const String& msg);
void quarantine(uint32_t nodeId);
void injectDecoy(uint32_t nodeId, int hops);
int randomHops(int minHop, int maxHop);

bool looksLikeForceSeek(uint32_t from, const String& dest, int hopLimit) {
  return (dest.equals(BROADCAST_DEST) || hopLimit > MAX_TRUSTED_HOPS) && rateExceeded(from);
}

void receivedCallback(uint32_t from, String &msg) {
  // extractDest/extractHopLimit: parse your mesh payload metadata
  if (looksLikeForceSeek(from, extractDest(msg), extractHopLimit(msg))) {
    quarantine(from);                              // custom extension (implement ACL/block logic)
    injectDecoy(from, randomHops(2, MAX_TRUSTED_HOPS)); // custom extension (implement decoys)
    return;
  }

  // normal handling
}
```

- **Barrier**: quarantine or rate-limit the node and require re-auth.
- **House-of-Mirrors**: return decoy topology so reconnaissance burns time while the real mesh stays hidden.

## 🐛 Troubleshooting

### Nodes not connecting

**Check**:
1. Same MESH_PREFIX and PASSWORD
2. Same MESH_PORT
3. WiFi channels (mesh auto-negotiates)
4. Power supply stable
5. Distance between nodes (<100m to start)

### Messages not routing

**Solutions**:
1. Ensure nodes are connected (check node list)
2. Add intermediate nodes for longer paths
3. Check message size (keep small)
4. Monitor for dropped packets

### Network unstable

**Fixes**:
1. Reduce broadcast frequency
2. Implement message deduplication
3. Add error handling
4. Update painlessMesh library

## 🎯 Best Practices

1. **Start simple** - Test with 3 nodes before scaling
2. **Name your nodes** - Easier debugging
3. **Monitor health** - Track connected nodes
4. **Limit broadcasts** - Don't spam the network
5. **Test failure scenarios** - Turn off nodes, see if mesh heals

## 🌟 Real-World Example

### Festival Coordination System

```cpp
#include <painlessMesh.h>

#define MESH_PREFIX     "FestivalMesh"
#define MESH_PASSWORD   "festival2024"
#define MESH_PORT       5555

painlessMesh mesh;
String nodeName = "Stage_Main";

void receivedCallback(uint32_t from, String &msg) {
  // Parse command
  if (msg == "SHOW_START") {
    startShow();
  } else if (msg.startsWith("SCENE:")) {
    int scene = msg.substring(6).toInt();
    changeScene(scene);
  }
}

void setup() {
  Serial.begin(115200);
  
  Scheduler userScheduler;
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  
  // Announce presence
  mesh.sendBroadcast(nodeName + " online");
}

void loop() {
  mesh.update();
  
  // Send heartbeat
  static unsigned long lastHB = 0;
  if (millis() - lastHB > 30000) {
    mesh.sendBroadcast(nodeName + " alive");
    lastHB = millis();
  }
}
```

## 📚 Resources

- [painlessMesh Documentation](https://gitlab.com/painlessMesh/painlessMesh)
- [Meshtastic Project](https://meshtastic.org/)
- [ESP-MESH (ESP-IDF)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/mesh.html)

## 🚀 Next Steps

1. **Test with 3 nodes** - Get basic mesh working
2. **Add more nodes** - See mesh auto-organize
3. **Implement show control** - Send cues through mesh
4. **Add visualization** - Create network map
5. **Test reliability** - Disconnect nodes, watch mesh heal

---

**Build resilient, scalable networks that can't be taken down!** 🕸️✨
