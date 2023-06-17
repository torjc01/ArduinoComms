#include <SoftwareSerial.h>

#define ON 0
#define OFF 1
String DEEPLINK_ROOT = "didcomm://invite?https://exp-port-e-url-courte.apps.exp.openshift.cqen.ca";

SoftwareSerial port_e_Serial(6, 7);  // Cria uma porta serial virtual nos pinos digitais 6 (RX) e 7 (TX)

const String GEN = "0x20"; 

int state;

void setup() {
    Serial.begin(115200);  
    Serial.println("[SLAVE] Setup da aplicacao");
    port_e_Serial.begin(9600);

    state = OFF;   
}

void loop() {
  
    // Si le code n'existe pas, faire la demande d'un nouveau code 

    Serial.println("[SLAVE] Loop");
    if (state == OFF){
        Serial.println("[SLAVE] Alors, on est à OFF");

        Serial.println("[SLAVE] Faire la demande de génération d'un adresse...");
        int bytesSent = port_e_Serial.write(GEN.c_str());
        delay(5000);
        Serial.print("[SLAVE] Demande faite. Bytes envoyés: ");
        Serial.println(bytesSent);

        // Verifica se há dados disponíveis na porta serial virtual
        Serial.println("[SLAVE] Vérifier s'il y a de réponse a la demande dans la porte serial virtuelle");
        if (port_e_Serial.available()) {
            
            Serial.print("[SLAVE] Bytes available: "); 
            Serial.println(port_e_Serial.available()); 
            
            Serial.println("[SLAVE] Virtual serial est disponible");
            String mensagem = port_e_Serial.readString();  // Lê a mensagem recebida pela porta serial virtual

            delay(3000);
            Serial.print("[SLAVE] Taille de la reponse:"); 
            Serial.println(mensagem.length()); 

            
            // Realiza alguma ação com a mensagem recebida pela porta serial virtual
            Serial.print("[SLAVE] Mensagem recebida (Porta Serial Virtual): ");
            Serial.println(mensagem);
            Serial.print("Adresse à faire broadcast: ");
            Serial.println(DEEPLINK_ROOT + mensagem);
            state = ON;
            delay(1000);
        } else {
          Serial.println("[SLAVE] Pas de données reçus");
        }
        
    }
    delay(3000);
}
