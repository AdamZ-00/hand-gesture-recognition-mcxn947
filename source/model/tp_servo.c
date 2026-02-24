/**
 * @file    tp_servo.c
 * @brief   Initialisation des timers PWM pour les 5 servomoteurs.
 *
 * Configure CTIMER0 (3 canaux PWM sur P0.25/P0.26/P0.27 → pouce, index, majeur)
 * et CTIMER2 (2 canaux PWM sur P1.12/P3.21 → annulaire, auriculaire) à 50 Hz
 * avec une horloge FRO 12 MHz. La période PWM est de 20 ms (240 000 cycles).
 */

#include "frdm_mcxn947.h"

//uint8_t P0_SW2 = 0; // Variable partagée
//uint8_t P0_SW3 = 0; // Variable partagée

/*void isr_P0_sw(void) {
    if (rd_bit(*P0_ISFR0, SW2)) {
        set_bit(*P0_ISFR0, SW2);
        P0_SW2 = 1;
    }
    if (rd_bit(*P0_ISFR0, SW3)) {
        set_bit(*P0_ISFR0, SW3);
        P0_SW3 = 1;
    }
}*/

void servos_init() {
    //init_leds();
    //init_switch();
    //setup_vtab_RAM();

    /*__asm(" CPSID I");
    init_interrupt_sw2(isr_P0_sw);
    init_interrupt_sw3(isr_P0_sw);
    __asm(" CPSIE I");*/

    //ledg_off();

    // TIMER 0
    SYSCON->AHBCLKCTRL1 |= (1 << 26);       // Horloge CTIMER0
    SYSCON->CTIMERCLKSEL[0] = 0b100;        // FRO 12MHz

    // Configure CTIMERCLKDIV bits for CTIMER0 and CTIMER2
    SYSCON->CTIMERCLKDIV[0] = 0;      // Pas de division
    SYSCON->CTIMERCLKDIV[2] = 0;      // Pas de division

    CTIMER0->TCR = 2;    // Reset
    CTIMER0->CTCR = 0;   // Mode Timer

    CTIMER0->MR[0] = PWM_PER - 1;
    CTIMER0->MR[1] = 0;
    CTIMER0->MR[2] = 0;
    CTIMER0->MR[3] = 0;
    CTIMER0->MCR = (1 << 1);      // Reset on MR0
    CTIMER0->PWMC = (1 << 1) | (1 << 2) | (1 << 3);

    PORT0->PCR[25] = (4 << 8);
    PORT0->PCR[26] = (4 << 8);
    PORT0->PCR[27] = (4 << 8);

    CTIMER0->TCR = 1;    // Start timer

    // TIMER 2
    SYSCON->AHBCLKCTRL1 |= (1 << 22);       // Horloge CTIMER2
    SYSCON->CTIMERCLKSEL[2] = 0b100;        // FRO 12MHz

    // Note : le diviseur de CTIMER2 est déjà configuré ci-dessus

    CTIMER2->TCR = 2;
    CTIMER2->CTCR = 0;

    CTIMER2->MR[0] = PWM_PER - 1;
    CTIMER2->MR[2] = 0;
    CTIMER2->MR[3] = 0;
    CTIMER2->MCR = (1 << 1);
    CTIMER2->PWMC = (1 << 2) | (1 << 3);

    PORT1->PCR[12] = (4 << 8);
    PORT3->PCR[21] = (4 << 8);

    CTIMER2->TCR = 1;
}
