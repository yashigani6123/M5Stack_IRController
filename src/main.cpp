#include <M5Stack.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

// Application switch
// #define IRRecvDemo
#define IRSendDemo

// IR Remote Reflective sensor U002
const uint16_t kRecvPin = 5;
const uint16_t kSendPin = 2;

const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC
const uint8_t kTimeout = 50;
#else
const uint8_t kTimeout = 15;
#endif

const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;

#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

IRsend irsend(kSendPin, false, true);
uint16_t rawdata[75] = {3452, 1688,  424, 1274,  424, 1274,  424,  400,  424,  398,
                         424, 1274,  424,  400,  424,  398,  424,  400,  424, 1274,
                         424, 1274,  424, 1274,  424,  398,  424, 1274,  424,  398,
                         424,  398,  424,  400,  424,  398,  424,  398,  424, 1274,
                         424,  400,  424,  398,  424,  398,  424,  398,  424,  398,
                         424, 1274,  424,  400,  424,  400,  424,  400,  424, 1274,
                         424, 1276,  424, 1272,  426, 1272,  424,  400,  424, 1276,
                         422, 1274,  426, 1274,  450}; // Nitori Sealing light ON(FULL)

// ==== IRRecvDemo ====
void setupIRRecvDump(){
  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);

#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif
  irrecv.setTolerance(kTolerancePercentage);
  irrecv.enableIRIn();
}

void runIRRecvDump(){
  if(irrecv.decode(&results)){
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);

    if(results.overflow){
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    }
    Serial.println(D_STR_LIBRARY "  : v" _IRREMOTEESP8266_VERSION_STR "\n");

    if(kTolerancePercentage != kTolerance){
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
    }
    Serial.print(resultToHumanReadableBasic(&results));

    String description = IRAcUtils::resultAcToString(&results);
    if(description.length()){
      Serial.println(D_STR_MESGDESC ": " + description);
    }
    yield();
#if LEGACY_TIMING_INFO
    Serial.println(resultToTimingInfo(&results));
    yield();
#endif
    Serial.println(resultToSourceCode(&results));
    Serial.println();
    yield();
  }
}

// ==== IRSendDemo ====
void setupIRSendDemo(){
  irsend.begin();
}

void runIRSendDemo(){
  Serial.println("Nitori Sealing light");
  irsend.sendRaw(rawdata, 75, 38);
  delay(2000);
}

void setup(){
  M5.begin(true, false, true, false);

  Serial.begin(kBaudRate, SERIAL_8N1);
  while(!Serial){
    delay(50);
  }
  assert(irutils::lowLevelSanityCheck() == 0);

#ifdef IRRecvDemo
  setupIRRecvDump();
#endif

#ifdef IRSendDemo
  setupIRSendDemo();
#endif
}

void loop(){
#ifdef IRRecvDemo
  runIRRecvDump();
#endif

#ifdef IRSendDemo
  runIRSendDemo();
#endif
}