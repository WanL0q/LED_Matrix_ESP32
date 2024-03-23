#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include "ESP32-VirtualMatrixPanel-I2S-DMA.h" 
#include <CAN.h>
#include <SD.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
#include <Arduino.h>
// Pin LED Screen configuration
constexpr int8_t R_1 = 17;
constexpr int8_t G_1 = 16;
constexpr int8_t B_1 = 4;
constexpr int8_t R_2 = 13;
constexpr int8_t G_2 = 2;
constexpr int8_t B_2 = 15;
constexpr int8_t A = 33;
constexpr int8_t B = 32;
constexpr int8_t C = 26;
constexpr int8_t D = -1;
constexpr int8_t E = -1;
constexpr int8_t LAT = 27;
constexpr int8_t OE = 12;
constexpr int8_t CLK = 14;

// Pin CAN configuration
#define TX_GPIO_NUM   22  // Connects to CTX
#define RX_GPIO_NUM   21  // Connects to CRX

// Panel configuration
constexpr uint16_t px = 64;           // Number of pixels wide of each INDIVIDUAL panel module. 
constexpr uint16_t py = 32;           // Number of pixels tall of each INDIVIDUAL panel module.
constexpr uint16_t nrow = 1;          // Number of ROWS of chained INDIVIDUAL PANELS
constexpr uint16_t ncol = 1;          // Number of INDIVIDUAL PANELS per ROW
constexpr uint16_t offset = px * ((nrow * ncol) - 1);       // Offset for calculating pixel location

// Colors
constexpr uint16_t white = 65535;
constexpr uint16_t black = 0;

// The path of the file on the SD card.
// Declare frames_vector as in the video. file
std::vector<std::vector<long long>> frames_vector;

// DMA and virtual display objects
MatrixPanel_I2S_DMA* dma_display = nullptr;
VirtualMatrixPanel* virtual_display = nullptr;

bool Receive_check = 0;
bool Tim_check = 0;
uint8_t emoji_mode = 0;
uint8_t num_vid = 0;
uint8_t error = 0;
unsigned long pre_mil = 0;
const unsigned long interval = 1000;
hw_timer_t * timer = NULL;
uint16_t count =0;
File root;
void sendCANFrame(){
  CAN.beginExtendedPacket(0x00113A);
  CAN.write(emoji_mode);
  CAN.write(num_vid);
  CAN.write(error);
  CAN.endPacket();
}
void IRAM_ATTR sendCANFrameCallback() {
  Tim_check =1;
}
void setup() {
  Serial.begin(115200);
  // Set the pins
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  // Initialize CAN bus with 500 kbps transmission rate
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  } else {
    Serial.println("CAN Initialized");
  }
  CAN.onReceive(onReceive);
  if (!SD.begin(5)) {
    Serial.println("Unable to boot SD card");
    error = 1;
  }
  CAN.filterExtended(0x0000013a);

  // Initialize DMA display
  HUB75_I2S_CFG::i2s_pins PINS = {R_1, G_1, B_1, R_2, G_2, B_2, A, B, C, D, E, LAT, OE, CLK}; 
  HUB75_I2S_CFG mxconfig(
    px * 2,            // DO NOT CHANGE THIS 
    py / 2,            // DO NOT CHANGE THIS 
    nrow * ncol,       // DO NOT CHANGE THIS 
    PINS               // Custom pins
  );
  mxconfig.clkphase = false; 
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->setBrightness8(255);    // range is 0-255, 0 - 0%, 255 - 100%
  dma_display->begin();
  dma_display->clearScreen();
  delay(500);
  // Initialize virtual display based on DMA display
  virtual_display = new VirtualMatrixPanel((*dma_display), nrow, ncol, px, py);
  virtual_display->setPhysicalPanelScanRate(FOUR_SCAN_32PX_HIGH);

  // Initialize timer 0
  timer = timerBegin(0, 80, true); // timer number 0, frequency division 80 (1 MHz), no scaling
  timerAttachInterrupt(timer, &sendCANFrameCallback, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer); 
}
void loop() {
  if (Tim_check) {
    sendCANFrame();
    Tim_check = 0; 
  }
  if (Receive_check) {
    receivedCANFrame();
    Receive_check = 0;
  }
  for (auto f : frames_vector) {
    if (Tim_check){
      sendCANFrame();
      Tim_check = 0;
    }
    if(Receive_check){
      receivedCANFrame();
      Receive_check = 0;
      if (error == 0) break;
    }
    display_frame(f);
    delay(50);
  }
  
}
void onReceive(int packetSize) {
  Receive_check = 1;
}
void receivedCANFrame(){
  SD.end();
  if (!SD.begin(5)) { 
    Serial.println("Unable to boot SD card");
    error=1;
  } else {
    num_vid = countVideos(SD.open("/"));
    uint8_t fileNumber;
    while (CAN.available()) {
      fileNumber += CAN.read();
    }
    String filePath = "/video" + String(fileNumber) + ".bin";
    Serial.println(filePath);
    if (SD.exists(filePath)) {
      Serial.println("File exists!");
      readFramesFromFilebin(fileNumber);
    } else {
      Serial.println("File not found on SD card");
      error = 2;
    }
  }
}
void readFramesFromFilebin(uint8_t fileNumber) {
  String filePath = "/video" + String(fileNumber) + ".bin";
  File file = SD.open(filePath.c_str());
  emoji_mode = fileNumber;
  error = 0;
  frames_vector.clear();
  while (file.available()) {
    std::vector<long long> frame;
    // Read data from file and add to vector
    for (int i = 0; i < 32; ++i) {
      long long value;
      file.read((uint8_t*)&value, sizeof(value));
      frame.push_back(value);
    }
    // Add vector to frames_vector
    frames_vector.push_back(frame);
  }
  file.close();
}
uint8_t countVideos(File dir) {
  uint8_t count = 0;
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }
    if (isVideoFile(entry.name())) {
      count++;
    }
    entry.close();
  }
  dir.rewindDirectory();
  return count;
}
bool isVideoFile(String filename) {
  return filename.endsWith(".bin");
}
void display_row(long long col, int row) {
  std::bitset<64> binary(col);
  for (int i = 63; i >= 0; --i) {
    virtual_display->drawPixel(63 - i, row, binary[i] ? white : black);
  }
}
void display_frame(std::vector<long long> frame) {
  for (int i = 0; i < frame.size(); i++) {
    display_row(frame[i], i);
  }
}

