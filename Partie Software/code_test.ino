// Définir la broche de la LED interne sur GPIO 22
const int LED_BUILTIN = 22;

void setup()
{
  // Initialiser la broche de la LED comme sortie
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // Allumer la LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Attendre 1 seconde

  // Éteindre la LED
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Attendre 1 seconde
}