#include <Arduino.h>
#include "sauvegardeValeur.h"
#include <stdint.h>

// Définition des ports utilisés
#define PIN_CAPTEUR_EFFORT A0 // Capteur d'effort
#define PIN_ACK_OPERATEUR 4   // Autorisation opérateur
#define PIN_ACK_SEPARATION 5  // Autorisation séparation
#define PIN_MISE_A_FEU 6      // Sortie pour la mise à feu
#define PIN_LED_OPERATEUR 8   // LED pour afficher l'état de l'autorisation opérateur
#define PIN_LED_SEPARATION 7  // LED pour afficher l'état de l'autorisation séparation

// Définition des constantes
#define SEUIL_EFFORT 10  // Seuil d'effort pour déclencher la mise à feu
#define TEMPS_ATTENTE 10 // Temps d'attente avant la mise à feu une fois la charge utile au sol

// Variables globales
int temps = 0;                        // Compteur de temps pour la mise à feu
volatile bool maf = false;            // Booléen de mise à feu
volatile bool ack_operateur = false;  // Autorisation de l'opérateur une fois la charge utile en descente
volatile bool ack_separation = false; // Autorisation de séparation obtenu lors de la séparation avec le ballon

// Fonction pour vérifier les conditions de mise à feu
bool maf_deventement(float effort, bool ack_operateur, bool ack_separation, int &temps)
{
  if (effort < SEUIL_EFFORT && ack_operateur && ack_separation)
  {
    if (temps > TEMPS_ATTENTE)
    {
      digitalWrite(PIN_MISE_A_FEU, HIGH); // Envoyer le signal de mise à feu
      return true;
    }
    else
    {
      digitalWrite(PIN_MISE_A_FEU, LOW);
      return false;
    }
  }
  else
  {
    temps = 0; // Réinitialiser le compteur de temps
    digitalWrite(PIN_MISE_A_FEU, LOW);
    return false;
  }
}

void setup()
{
  // Configuration des ports en entrée
  pinMode(PIN_CAPTEUR_EFFORT, INPUT);
  pinMode(PIN_ACK_OPERATEUR, INPUT);
  pinMode(PIN_ACK_SEPARATION, INPUT);

  // Configuration du port de sortie pour la mise à feu
  pinMode(PIN_MISE_A_FEU, OUTPUT);
  digitalWrite(PIN_MISE_A_FEU, LOW); // Initialiser le signal de mise à feu à LOW

  // Configuration des LEDs pour le debug
  pinMode(PIN_LED_OPERATEUR, OUTPUT);
  pinMode(PIN_LED_SEPARATION, OUTPUT);
}

void loop()
{
  // Lire la valeur de l'effort mesuré par le capteur
  int effort = analogRead(PIN_CAPTEUR_EFFORT);

  // Incrémentation du compteur de temps
  temps++;

  // Lire l'état des autorisations et afficher sur les LEDs de debug
  ack_operateur = digitalRead(PIN_ACK_OPERATEUR);
  digitalWrite(PIN_LED_OPERATEUR, ack_operateur); // LED ON si autorisation opérateur est accordée

  ack_separation = digitalRead(PIN_ACK_SEPARATION);
  digitalWrite(PIN_LED_SEPARATION, ack_separation); // LED ON si autorisation séparation est accordée

  // Vérifier les conditions de mise à feu
  maf = maf_deventement(effort, ack_operateur, ack_separation, temps);

  // Attendre 1 seconde avant de recommencer la boucle
  delay(1000);
}
