/**
 * @file    main.cpp
 * @brief   Point d'entrée du système de reconnaissance de gestes.
 *
 * Initialise les périphériques (horloge, caméra OV7670, écran LCD, servomoteurs)
 * puis lance la boucle d'inférence CIFAR-10 qui pilote la main robotique.
 * Le pipeline complet : capture caméra → prétraitement → inférence TFLite/NPU → actionnement servos.
 */

/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_init.h"
#include "demo_config.h"
#include "demo_info.h"
#include "fsl_debug_console.h"
#include "image.h"
#include "image_utils.h"
#include "model.h"
#include "output_postproc.h"
#include "timer.h"
#include "video.h"
#include "ov7670.h"
#include "frdm_mcxn947.h"
#include "uart_utils.h"



int main(void)
{
    BOARD_Init();
    TIMER_Init();


    DEMO_PrintInfo();


    Ov7670_Init();

    display_init();

    ezh_start();

    servos_init();

    cifar10_recognize();

    while(1)
    {

    }
}
