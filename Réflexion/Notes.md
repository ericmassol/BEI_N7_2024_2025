
## Notes de réflexion sur le projet

### Séance 1

#### Partie Alimentation 

Contrairement au groupe de l'année dernière, nous nous intéressons uniquement à la panne avance et pas la panne retard donc nous avons certaines contraintes supplémentaires pour garantir notre système contre la simple panne.

En effet, avec une architecture résistante à la panne retard, ils avaient dû dédoubler leur architecture ce qui simplifiait certaines problématiques (mais peut aussi être embêtant si architecture identique comme vu en cours).

Dans notre cas, on devrait donc aussi ajouter la résistance à la simple panne vis à vis du système de chauffage. Cela signifie lui ajouter une source d'alimentation supplémentaire. On le détaillera dans notre architecture mais on ne le mettra pas en oeuvre en utilisant une hypothèse simplificatrice à cause des délais imposés.

De plus, on peut proposer d'autres capteurs et actionneurs pour le projet ainsi qu'un plan de développement pour la suite mais cela servira pour les améliorations et ne sera pas mis en pratique de suite (à cause des délais de nouveau).