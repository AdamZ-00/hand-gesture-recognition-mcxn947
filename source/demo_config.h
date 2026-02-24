/**
 * @file    demo_config.h
 * @brief   Paramètres de configuration de la démo d'inférence.
 *
 * Définit le nom de l'exemple, le framework utilisé (TFLite Micro),
 * le seuil de détection (60 %), le nombre de résultats top-N,
 * et le mode verbeux pour le debug console.
 */

/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DEMO_CONFIG_H_
#define _DEMO_CONFIG_H_

#define EXAMPLE_NAME       "CIFAR-10"
#define FRAMEWORK_NAME     "TensorFlow Lite Micro"
#define DETECTION_TRESHOLD 60
#define NUM_RESULTS        1
#define DEMO_VERBOSE       false
#define EOL                "\r\n"

#endif // _DEMO_CONFIG_H_
