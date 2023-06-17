#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7);  // Cria uma porta serial virtual nos pinos digitais 6 (RX) e 7 (TX)

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial padrão
  mySerial.begin(115200);  // Inicializa a porta serial virtual
}

void loop() {
  String mensagem = "Ola Dispositivo 2!";  // Mensagem a ser enviada

  Serial.println(mensagem);  // Envia a mensagem pela porta serial padrão
  mySerial.println(mensagem);  // Envia a mensagem pela porta serial virtual
  delay(1000);  // Aguarda 1 segundo antes de enviar a próxima mensagem
}
