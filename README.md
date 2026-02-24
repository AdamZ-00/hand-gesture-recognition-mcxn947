# Reconnaissance de gestes – Main robotique sur FRDM-MCXN947

Système embarqué de reconnaissance de gestes de la main (chiffres 0 à 5) utilisant un réseau de neurones accéléré par le NPU Neutron du MCX, qui pilote en temps réel une main robotique à 5 servomoteurs.

## Architecture

```
Caméra OV7670 (384×384 RGB565)
       │
       ▼
SmartDMA / EZH (capture par slices de 24 lignes, double buffering)
       │
       ├──► Écran LCD ST7796 (affichage 192×192 + label)
       │
       ▼
Prétraitement (sous-échantillonnage → 96×96 RGB888, soustraction 128)
       │
       ▼
TensorFlow Lite Micro + NPU Neutron (modèle CIFAR-10 retrainé)
       │
       ▼
Post-traitement (top-1, seuil 60%)
       │
       ▼
5 servomoteurs PWM (CTIMER0 + CTIMER2, 50 Hz)
```

## Matériel requis

| Composant | Détail |
|---|---|
| MCU | NXP FRDM-MCXN947 (Cortex-M33, NPU Neutron) |
| Caméra | OV7670 (384×384) |
| Écran | ST7796 LCD (480×320, interfacé via FlexIO) |
| Actionneurs | 5 micro-servomoteurs (un par doigt) |

## Organisation du code

```
source/
├── main.cpp                    # Point d'entrée, initialisation du pipeline
├── demo_config.h               # Paramètres (seuil, framework, etc.)
├── model/
│   ├── model.cpp               # Gestion du modèle TFLite Micro
│   ├── model_cifarnet_ops_npu.cpp  # Op-resolver NPU (Softmax + Neutron)
│   ├── output_postproc.cpp     # Post-traitement et déclenchement des servos
│   ├── get_top_n.cpp           # Extraction du résultat top-N
│   ├── labels.h                # Labels des classes (Cinq, Deux, Quatre, Trois, Un, Zero)
│   ├── frdm_mcxn947.c/h        # BSP : GPIO, LEDs, interruptions, commande servos
│   ├── tp_servo.c              # Init PWM des timers pour les 5 servos
│   ├── debug_image.cpp/h       # Envoi one-shot de l'image sur UART (debug)
│   ├── uart_utils.cpp/h        # Transmission UART bas niveau
│   └── tflite_learn_*          # Modèles TFLite (versions d'entraînement)
└── video/
    ├── cifar10_recognize.cpp   # Boucle d'inférence + conversion RGB565→RGB888
    ├── EZH_Camera.c            # Acquisition caméra via SmartDMA
    ├── display.c               # Affichage LCD + rendu texte
    ├── ov7670.c/h              # Driver caméra OV7670
    └── st7796_lcd.c/h          # Driver LCD ST7796
```

## Classes reconnues

| Index | Label | Geste |
|-------|-------|-------|
| 0 | Cinq | 5 doigts ouverts |
| 1 | Deux | Index + majeur |
| 2 | Quatre | 4 doigts (sans pouce) |
| 3 | Trois | Pouce + index + majeur |
| 4 | Un | Index seul |
| 5 | Zero | Poing fermé |

## Build

Projet MCUXpresso IDE. Importer via `File > Open Projects from File System`, sélectionner le dossier du projet, puis `Build` (Ctrl+B). Flasher via LinkServer Debug.
