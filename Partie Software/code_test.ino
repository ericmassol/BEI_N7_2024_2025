#include <Arduino.h>

// Définir la broche de la LED interne sur GPIO 22
const int LED_test = 22;

void setup()
{
  // Initialiser la broche de la LED comme sortie
  pinMode(LED_test, OUTPUT);
}

void loop()
{
  // Allumer la LED
  digitalWrite(LED_test, HIGH);
  delay(1000); // Attendre 1 seconde

  // Éteindre la LED
  digitalWrite(LED_test, LOW);
  delay(1000); // Attendre 1 seconde
}