# Notes et remarques sur la partie software

## Aide pour Platform.io

Nécessité de commencer le code en définissant l'environnement :

```
[env:lolin32_lite]
platform = espressif32
board = lolin32_lite

; change microcontroller
board_build.mcu = esp32

; change MCU frequency
board_build.f_cpu = 240000000L
```

Pour plus de détails sur cette mise en oeuvre, voir [ici](https://docs.platformio.org/en/latest/boards/espressif32/lolin32.html).

## Reflexion sur microcontroleur - Effort

[Architecture détaillée](https://github.com/ericmassol/BEI_N7_2024_2025/blob/main/Partie%20Hardware/architecture_detaille.png)
