/*
 * Deployé sur NodeMCU
 * 
 * Copyright (c) 2023 Gouvernement du Québec
 * Auteur: Julio Cesar Torres (torj01)
 * SPDX-License-Identifier: LiLiQ-R-v.1.1
 * License-Filename: /LICENSE
 */
#include <SoftwareSerial.h>
#include <CRC32.h>

SoftwareSerial port_e_Serial(4, 5);  // Cria uma porta serial virtual nos pinos digitais 6 (RX) e 7 (TX)
// No caso do nodemcu, tx = gpio5 = D1; rx = gpio4 = D2 -> entao, conectar rx ao pin d2, tx ao pin d1; no codigo, referenciar (4,5)

CRC32 crc;


void setup() {
  delay(3000);
  Serial.println("[MASTER] Setup da aplicacao");
  Serial.begin(115200);  // Inicializa a comunicação serial padrão
  port_e_Serial.begin(9600);  // Inicializa a porta serial virtual
  
  // Inicializa o CRC 
  crc.reset();
}

void loop() {

  Serial.println("[MASTER] Loop"); 

  Serial.println("[MASTER] Vérifier si on a reçu des données de SerialComms");
  Serial.print("[MASTER] Bytes available: "); 
  Serial.println(port_e_Serial.available()); 
  
  if(port_e_Serial.available() > 0){

      Serial.print("[MASTER] Bytes available: "); 
      Serial.println(port_e_Serial.available());
      
      String msg = port_e_Serial.readString(); 

      Serial.print("[MASTER] Taille de la reponse:"); 
      Serial.println(msg.length()); 

      Serial.print("[MASTER] Sinal recebido: ");
      Serial.println(msg); 
      if(msg == "0x20"){
          generate();    
          serialFlush(); 
      } else {
          Serial.println("[MASTER] Sinal errado");  
      }
      
  }
  delay(1000);
}

void generate(){
  Serial.println("[MASTER] generate()");

  String mensagem = "didcomm://invite?https://exp-port-e-url-courte.apps.exp.openshift.cqen.ca/J7jrpvie";  // Mensagem a ser enviada

  //String mensagem = "mensagem simples sem qualquer tipo de pontuacao"; 
  // Mensagem a ser enviada
  crc.update((const uint8_t*)mensagem.c_str(), mensagem.length());
  uint32_t calculatedCrc = crc.finalize();

  Serial.println(calculatedCrc);

  Serial.println("[MASTER] mensagem a ser enviada");
  Serial.println(mensagem.lastIndexOf('/')); 
  Serial.println(mensagem.substring(mensagem.lastIndexOf('/')));
  Serial.println(mensagem);  // Envia a mensagem pela porta serial padrão
  Serial.println("[MASTER] sending");

  port_e_Serial.write(mensagem.substring(mensagem.lastIndexOf('/')).c_str());

/*  int msgLen = mensagem.length(); 


  if(msgLen > 64){
      port_e_Serial.write(mensagem.substring(0,63).c_str());  // Envia a primeira parte da mensagem pela porta serial virtual
      port_e_Serial.write(mensagem.substring(64,mensagem.length()).c_str());  // Envia a primeira parte da mensagem pela porta serial virtual
  } else {
      port_e_Serial.write(mensagem.c_str());  // Envia a mensagem total 
  }
*/

  delay(5000);  // Aguarda 3 segundos antes de enviar a próxima mensagem  
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
