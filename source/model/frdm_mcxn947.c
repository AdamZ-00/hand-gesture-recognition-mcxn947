/**
 * @file    frdm_mcxn947.c
 * @brief   BSP et contrôle des servomoteurs pour la carte FRDM-MCXN947.
 *
 * Fournit les fonctions bas niveau : configuration GPIO (LEDs, switchs),
 * table des vecteurs d'interruption en RAM, et surtout le contrôle
 * des 5 servomoteurs via PWM (un par doigt de la main robotique).
 * effectuer_geste() traduit un index de label (0-5) en positions
 * tout-ou-rien des 5 doigts (pouce, index, majeur, annulaire, auriculaire).
 */

#include "frdm_mcxn947.h"

pointer_to_ISR vtab[172]; // Table des vecteurs d'interruption

// Remplacé par accès via SYSCON
// volatile uint32_t *const AHBCLKCTRL0 = (uint32_t*) (SYSCON0 + 0x200);
// volatile uint32_t *const AHBCLKCTRL1 = (uint32_t*) (SYSCON0 + 0x204);

volatile uint32_t *const SCB_VTOR = (uint32_t*) 0xE000ED08;
volatile uint32_t *const NVIC_ISER0 = (uint32_t*) 0xE000E100;
volatile uint32_t *const NVIC_ISER1 = (uint32_t*) 0xE000E104;

// GPIO0, GPIO1, GPIO3 accessibles par GPIOx->PDOR/PDDR
// PORT0, PORT1, PORT3 accessibles par PORTx->PCR[n]

volatile uint32_t *const P0_PDIR = (uint32_t*) (P0_BASE + 0x50); // toujours utilisé en lecture
volatile uint32_t *const PORT0_ICR06 = (uint32_t*) (P0_BASE + 0x80 + 4 * 6);
volatile uint32_t *const PORT0_ICR23 = (uint32_t*) (P0_BASE + 0x80 + 4 * 23);

volatile uint32_t *const P0_ISFR0 = (uint32_t*) (P0_BASE + 0x120);

volatile uint32_t *const CTIMERCLKSEL0 = (uint32_t*) (SYSCON0 + 0x26C); // Clock selection for timer0

volatile uint32_t *const CTIMERCLKSEL2 = (uint32_t*) (SYSCON0 + 0x274); // Clock selection for timer2

#define CTIMER0CLKDIV (SYSCON->CTIMERCLKDIV[0])
#define CTIMER2CLKDIV (SYSCON->CTIMERCLKDIV[2])


// Timer 0
volatile uint32_t *const CTIMER0_IR = (uint32_t*) (CTIMER0_BASE);
volatile uint32_t *const CTIMER0_TCR = (uint32_t*) (CTIMER0_BASE + 4);
volatile uint32_t *const CTIMER0_MCR = (uint32_t*) (CTIMER0_BASE + 0x14);
volatile uint32_t *const CTIMER0_MR0 = (uint32_t*) (CTIMER0_BASE + 0x18);
volatile uint32_t* const CTIMER0_MR1 = (uint32_t*) (CTIMER0_BASE + 0x1C);
volatile uint32_t* const CTIMER0_MR2 = (uint32_t*) (CTIMER0_BASE + 0x20);
volatile uint32_t* const CTIMER0_MR3 = (uint32_t*) (CTIMER0_BASE + 0x24);
volatile uint32_t *const CTIMER0_CTCR = (uint32_t*) (CTIMER0_BASE + 0x70);
volatile uint32_t *const CTIMER0_PWMC = (uint32_t*) (CTIMER0_BASE + 0x74);
volatile uint32_t *const CTIMER0_EMR = (uint32_t*) (CTIMER0_BASE + 0x3C);

// Timer 2
volatile uint32_t *const CTIMER2_IR = (uint32_t*) (CTIMER2_BASE);
volatile uint32_t *const CTIMER2_TCR = (uint32_t*) (CTIMER2_BASE + 4);
volatile uint32_t *const CTIMER2_MCR = (uint32_t*) (CTIMER2_BASE + 0x14);
volatile uint32_t *const CTIMER2_MR0 = (uint32_t*) (CTIMER2_BASE + 0x18);
volatile uint32_t* const CTIMER2_MR1 = (uint32_t*) (CTIMER2_BASE + 0x1C);
volatile uint32_t* const CTIMER2_MR2 = (uint32_t*) (CTIMER2_BASE + 0x20);
volatile uint32_t* const CTIMER2_MR3 = (uint32_t*) (CTIMER2_BASE + 0x24);
volatile uint32_t *const CTIMER2_CTCR = (uint32_t*) (CTIMER2_BASE + 0x70);
volatile uint32_t *const CTIMER2_PWMC = (uint32_t*) (CTIMER2_BASE + 0x74);
volatile uint32_t *const CTIMER2_EMR = (uint32_t*) (CTIMER2_BASE + 0x3C);


void main_doigt(doigt d, int pos) {
    if (pos < 0) pos = 0;
    if (pos > 100) pos = 100;

    switch (d) {
        case POUCE:
            *CTIMER0_MR1 = (pos == 0 ? PWM_MOTEUR_180 : PWM_MOTEUR_0) - 1;
            break;
        case INDEX:
            *CTIMER0_MR2 = (pos == 0 ? PWM_MOTEUR_180 : PWM_MOTEUR_0) - 1;
            break;
        case MAJEUR:
            *CTIMER0_MR3 = (pos == 0 ? PWM_MOTEUR_180 : PWM_MOTEUR_0) - 1;
            break;
        case ANNULAIRE:
            *CTIMER2_MR2 = (pos == 0 ? PWM_MOTEUR_180 : PWM_MOTEUR_0) - 1;
            break;
        case AURICULAIRE:
            *CTIMER2_MR3 = (pos == 0 ? PWM_MOTEUR_180 : PWM_MOTEUR_0) - 1;
            break;
        default:
           break;
    }
}

const int positions_gestes[8][5] = {
    {100, 100, 100, 100, 100}, // CINQ
    {  0, 100, 100,   0,   0}, // DEUX
    {  0, 100, 100, 100, 100}, // QUATRE
    {100, 100, 100,   0,   0}, // TROIS
	{ 0, 100,  0,  0,  0}, // UN
    { 0, 0,  0,  0,  0}, // ZERO
	{ 0, 0,  100,  0,  0}, //default
};

void effectuer_geste(int g) {
    if (g < 0 || g > 5) g = 6; // Par défaut
    for (int i = 0; i < 5; i++) {
        main_doigt((doigt)i, positions_gestes[g][i]);
    }
}

void init_leds(void) {
    SYSCON->AHBCLKCTRL0 |= (1 << 19);  // GPIO0
    SYSCON->AHBCLKCTRL0 |= (1 << 13);  // PORT0
    SYSCON->AHBCLKCTRL0 |= (1 << 20);  // GPIO1
    SYSCON->AHBCLKCTRL0 |= (1 << 14);  // PORT1
    SYSCON->AHBCLKCTRL0 |= (1 << 22);  // GPIO3
    SYSCON->AHBCLKCTRL0 |= (1 << 16);  // PORT3

    PORT0->PCR[27] = 0;                // Réglage du multiplexer
    GPIO0->PDDR |= (1 << LEDRGB_G);   // Configuration en sortie
}

void init_switch() {
    SYSCON->AHBCLKCTRL0 |= (1 << 19); // GPIO0
    SYSCON->AHBCLKCTRL0 |= (1 << 13); // PORT0
    clr_bit(GPIO0->PDDR, SW2);
    clr_bit(GPIO0->PDDR, SW3);
    PORT0->PCR[23] = 0x1000;
    PORT0->PCR[6] = 0x1000;
}

void setup_vtab_RAM() {
    __asm(" CPSID I");
    uintptr_t *v = (uintptr_t*)(*SCB_VTOR);
    for (unsigned int i = 0; i < sizeof(vtab)/sizeof(vtab[0]); i++)
        vtab[i] = (pointer_to_ISR) v[i];
    *SCB_VTOR = (uint32_t) &vtab;
    __asm(" CPSIE I");
}

/*void init_interrupt_sw2(void *funcIT) {
    __asm("CPSID I");
    PORT0->PCR[23] = 0x1000;
    set_bit(*PORT0_ICR23, 19);
    clr_bit(*PORT0_ICR23, 18);
    set_bit(*PORT0_ICR23, 17);
    clr_bit(*PORT0_ICR23, 16);
    vtab[16+PORT0_INT] = funcIT;
    set_bit(*NVIC_ISER0, PORT0_INT);
    __asm("CPSIE I");
}*/

void set_edge_interrupt_sw2(uint8_t falling) {
    *PORT0_ICR23 &= ~0xF;
    *PORT0_ICR23 |= falling ? 0b0101 : 0b1010;
    set_bit(*P0_ISFR0, SW2);
}

/*void init_interrupt_sw3(void *funcIT) {
    __asm("CPSID I");
    PORT0->PCR[6] = 0x1000;
    set_bit(*PORT0_ICR06, 19);
    clr_bit(*PORT0_ICR06, 18);
    set_bit(*PORT0_ICR06, 17);
    clr_bit(*PORT0_ICR06, 16);
    vtab[16 + PORT0_INT] = funcIT;
    set_bit(*NVIC_ISER0, PORT0_INT);
    __asm("CPSIE I");
}*/

int rd_sw3(void) {
    return rd_bit(*P0_PDIR, SW3);
}

void ledg_on(void) {
    clr_bit(GPIO0->PDOR, LEDRGB_G);
}

void ledg_off(void) {
    set_bit(GPIO0->PDOR, LEDRGB_G);
}

void tog_ledg() {
    tog_bit(GPIO0->PDOR, LEDRGB_G);
}

void delay(uint32_t ms) {
    for (int i = 0; i < ms; i++) {}
}
