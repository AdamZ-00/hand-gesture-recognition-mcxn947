/**
 * @file    video.h
 * @brief   Constantes du pipeline vidéo et prototypes des fonctions d'affichage.
 *
 * Définit les dimensions caméra (384×384), la hauteur des slices DMA (24 lignes),
 * les dimensions du buffer LCD (192×12) et les fonctions d'initialisation
 * du coprocesseur EZH (SmartDMA) et de l'écran LCD ST7796.
 */

/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef VIDEO_VIDEO_H_
#define VIDEO_VIDEO_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAMERA_WIDTH 384
#define CAMERA_HEIGHT 384
#define STRIPE_HEIGHT 24

#define LCD_BUFF_WIDTH 192
#define LCD_WINDOW_HEIGHT 192
#define LCD_BUFF_HEIGHT 12
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
/*******************************************************************************
 * Functions
 ******************************************************************************/
void ezh_start();
void display_init();
void display_show_bg_image();
void display_show_slice(uint32_t g_stripe_index, uint32_t buffer,uint32_t maxidx);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* VIDEO_VIDEO_H_ */
