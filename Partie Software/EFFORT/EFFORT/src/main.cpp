#include <Arduino.h>
#include "sauvegardeValeur.h"
#include <stdint.h>

// Définir le seuil d'effort pour déclencher la mise à feu
const int seuil_effort = 10; // Ici le seuil est arbitraire car on ne possède pas de données réalistes

// Variables d'état pour la mise à feu et les autorisations
volatile bool maf = false;            // Booléen de mise à feu 
volatile bool ack_operateur = false;  // Autorisation de l'opérateur une fois la charge utile en descente
volatile bool ack_separation = false; // Autorisation de séparation obtenu lors de la séparation avec le ballon

// Fonction pour vérifier les conditions de mise à feu
bool maf_deventement(float effort, bool ack_operateur, bool ack_separation) {
  // Si l'effort est inférieur au seuil et que les deux autorisations sont accordées
  if (effort < seuil_effort && ack_operateur && ack_separation) {
    digitalWrite(6, HIGH); // Envoyer le signal de mise à feu
    return true;
  } else {
    return false;
  }
}

void setup() {
  // Configuration des ports d'entrée pour une carte Arduino Uno
  pinMode(A0, INPUT);  // Capteur d'effort
  pinMode(7, INPUT);  // Autorisation opérateur
  pinMode(8, INPUT);  // Autorisation séparation

  // Configuration du port de sortie pour la mise à feu
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW); // Initialiser le signal de mise à feu à LOW
}

void loop() {
  // Lire la valeur de l'effort mesuré par le capteur
  int effort = analogRead(A0);

  // Lire l'état des autorisations si elles ne sont pas déjà accordées
  if (!ack_operateur) {
    ack_operateur = digitalRead(7); // Autorisation opérateur
  }

  if (!ack_separation) {
    ack_separation = digitalRead(8); // Autorisation séparation
  }

  // Vérifier les conditions de mise à feu si elle n'a pas déjà été déclenchée
  if (!maf) {
    maf = maf_deventement(effort, ack_operateur, ack_separation);
  }

  // // Sauvegarder la valeur de l'effort
  // static uint16_t currentAddress = 0; // Adresse actuelle pour la sauvegarde (static pour conserver la valeur entre les appels)
  // const uint16_t dataSize = 32;      // Taille des données à sauvegarder

  // // Vérifier que l'adresse ne déborde pas (sécurité contre les buffer overflows)
  // if (currentAddress + dataSize <= MAX_ADDRESS) { // MAX_ADDRESS doit être défini selon la mémoire disponible
  //   sauvegarderValeur(effort, currentAddress); // Sauvegarder la valeur
  //   currentAddress += dataSize; // Mettre à jour l'adresse pour la prochaine sauvegarde
  // } else {
  //   // Gérer le débordement de mémoire (par exemple, réinitialiser l'adresse ou signaler une erreur)
  //   currentAddress = 0; // Réinitialisation de l'adresse (ou autre logique de gestion d'erreur)
  // }

  // Attendre 1 seconde avant de recommencer la boucle
  delay(1000);
}