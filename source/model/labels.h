/**
 * @file    labels.h
 * @brief   Labels de classification des gestes de la main.
 *
 * Définit les noms des classes reconnues par le modèle CIFAR-10 retrainé.
 * L'ordre des labels doit correspondre aux indices de sortie du modèle.
 * Classes : Cinq, Deux, Quatre, Trois, Un, Zero.
 */

/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LABELS_H_
#define _LABELS_H_

static const char* labels[] = {
		"Cinq", "Deux", "Quatre", "Trois", "Un", "Zero"
				"          "
};

#endif // _LABELS_H_
