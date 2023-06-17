// ----------------------------
// Standard Libraries
// ----------------------------

// NFC
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"

// Software Serial
#include <SoftwareSerial.h>

// -----------------------------
// Fichiers header personnalisés
// -----------------------------
#include "arduino_secrets.h"
#include "thingProperties.h"

// Header  NFC 
#include "emulatetag.h"
#include "NdefMessage.h"

// -----------------------------
// Variables globales 
// -----------------------------

// Objets de la librairie PN532
PN532_SPI pn532spi(SPI, 10);
EmulateTag nfc(pn532spi);

// Objets de la librairie NDEF
uint8_t ndefBuf[NFC_BUFFER_SIZE];
NdefMessage message;
int messageSize;

// Identificateur du tag. Doit avoir 3 bytes de long
uint8_t uid[3] = { 0x12, 0x34, 0x56 };

// char DEEPLINK_ROOT[90] = "didcomm://invite?https://exp-port-e-url-courte.apps.exp.openshift.cqen.ca";
// char DEEPLINK_ROOT[90] = "didcom://invite?https://github.com";

// Objet de la librairie SoftwareSerial
SoftwareSerial porteSerial(6, 7);  // Cria uma porta serial virtual nos pinos digitais 6 (RX) e 7 (TX)

// Variable qui fait track de l'état de l'adresse de l'application
int etatAdresse; 

/**
 *  
 */
void setup(){
    Serial.begin(115200); 
    porteSerial.begin(9600);

    Serial.println("[NFC] Setup de l'application..."); 

    etatAdresse = ADDRESS_NON_DISP;
}

/**
 * 
 */
void loop(){
    if(etatAdresse == ADDRESS_NON_DISP){
        Serial.println("[NFC] Adresse non dispo. Demander génération...");
        broadcast(genererAdresse());
    }
    delay(DELAY_M);
}

String genererAdresse(){
    Serial.println("[NFC] Génération de l'adresse en cours...");
    //int bytesSent = porteSerial.write(SIG_GEN);
    porteSerial.write(SIG_GEN);

    String addr = "";
    delay(DELAY_L+DELAY_M); 

    if(porteSerial.available() > 0){
        addr = porteSerial.readStringUntil('\n');
        Serial.print("[NFC] Addr généré: "); 
        Serial.println(addr);
        delay(DELAY_M);
    }else {
        Serial.println("[NFC] Pas de temps disponible pour recevoir...");
    }

    etatAdresse = ADDRESS_DISP; 
    Serial.println(addr.length());
    return addr;
    //return DEEPLINK_ROOT + addr;
    // return DEEPLINK_ROOT + '\0';
}

void broadcast(String param){
    Serial.println("[NFC] Broadcasting l'adresse");
    Serial.print("[NFC] Parameter: ");
    Serial.println(param.c_str()); 
    Serial.println(param.c_str());
    Serial.println(param.length());
    message = NdefMessage(); 

    //message.addUriRecord("didcomm://invite?https://exp-port-e-url-courte.apps.exp.openshift.cqen.ca/fg5Qk3XU"); 
    // message.addUriRecord(param);
    message.addUriRecord("https://shortnsweet.link/BY7cN239");
    // message.addUriRecord("https://e.apps.exp.openshift.cqen.ca/fg5Qk3XU");

    messageSize = message.getEncodedSize();
    if (messageSize > sizeof(ndefBuf)) {
        Serial.println("[NFC] ndefBuf est trop petit");
        while (1) { }
    }

    /*char bd[512] = {ndefBuf};
    Serial.println(bd); */
    message.encode(ndefBuf); 

    // commenter la command si l'on ne veut pas du ndef message
    nfc.setNdefFile(ndefBuf, messageSize);
    
    nfc.setUid(uid); // uid doit avoir 3 bytes!
    
    nfc.init();

    nfc.emulate(); // start emulation (blocks l'exécution du loop du firmware)
    
    if(nfc.writeOccured()){
        
        Serial.println("[NFC] \nWrite occured !"); 
        uint8_t* tag_buf; uint16_t length;
        nfc.getContent(&tag_buf, &length);
        NdefMessage msg = NdefMessage(tag_buf, length);
        msg.print();
    }
    Serial.println("[NFC] Message livré au telephone");
    delay(DELAY_S);
    etatAdresse = ADDRESS_NON_DISP;
}

/**
 * 
 */
void serialFlush(){

    while(porteSerial.available() > 0) {
        char t = porteSerial.read();
    }
}
