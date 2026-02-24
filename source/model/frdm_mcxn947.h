/**
 * @file    frdm_mcxn947.h
 * @brief   Couche d'abstraction matérielle pour la carte FRDM-MCXN947.
 *
 * Définit les adresses des registres (GPIO, PORT, CTIMER), les constantes PWM
 * pour le pilotage des servomoteurs (période 20 ms, plage 0.5-2.5 ms), les macros
 * de manipulation de bits, et les types/enums pour les doigts et les gestes.
 * Utilisé par tp_servo.c, frdm_mcxn947.c et output_postproc.cpp.
 */

#include <stdint.h>
#include "MCXN947_cm33_core0.h"

#ifdef __cplusplus
extern "C" {
#endif

//Table des vecteurs d'interruption
typedef void (*pointer_to_ISR)(void);
extern pointer_to_ISR vtab[];

#define TIMER_FREQ 12E6
#define PWM_PER (TIMER_FREQ / 50) // Fréquence de PWM à 50Hz (240000)
#define PWM_MOTEUR_0 234000 //(PWM_PER/20) // complément = 228000 (2400000*(20-1)/20) // 230400 pour 0.8 ms 234000 pour 0.5 ms
#define PWM_MOTEUR_90 222000//(PWM_PER/13)// complément = 2220000 (2400000*(20-1,5)/20)
#define PWM_MOTEUR_180 210000 //(PWM_PER/10)// complément = 216000 (2400000*(20-2)/20) // 213600 pour 2.2 ms 210000 pour 2.5 ms

//#define SYSCON0 0x40000000
#define P0_BASE 0x40096000
//#define PORT0_BASE 0x40116000
#define P1_BASE 0x40098000
//#define PORT1_BASE 0x40117000 // Port control
#define P3_BASE 0x4009C000
//#define PORT3_BASE 0x40119000 // Port control
#define PORT0_INT  17


#define LEDRGB_R 10 //sur port P0
#define LEDRGB_G 27 //sur port P0
#define LEDRGB_B 2 // sur port P1
#define SW2 23
#define SW3 6


#define clr_bit(X,Y) (X &= ~((uintmax_t)1<< (Y)))
#define set_bit(X,Y) (X |= ((uintmax_t)1 << (Y) ))
#define tog_bit(X,Y) (X ^= (uintmax_t)1<< (Y))
#define rd_bit(X,Y) ((X >> (Y)) &1)


extern volatile uint32_t *const AHBCLKCTRL0;

extern volatile uint32_t* const SCB_VTOR;
extern volatile uint32_t* const NVIC_ISER0;
extern volatile uint32_t* const NVIC_ISER1;


extern volatile uint32_t *const P0_PDOR;
extern volatile uint32_t* const P0_PDIR;
extern volatile uint32_t *const P0_PDDR;
extern volatile uint32_t* const P0_ISFR0;
extern volatile uint32_t* const P0_ICR6;
extern volatile uint32_t* const PORT0_ICR23;
extern volatile uint32_t *const PORT0_PCR10;
extern volatile uint32_t *const PORT0_PCR23;
extern volatile uint32_t *const PORT0_PCR27;
extern volatile uint32_t *const PORT0_PCR26;
extern volatile uint32_t *const PORT0_PCR25;
extern volatile uint32_t *const PORT0_ICR23;
extern volatile uint32_t *const PORT0_ICR06;
extern volatile uint32_t *const P1_PDOR;
extern volatile uint32_t *const P1_PDDR;
extern volatile uint32_t *const PORT1_PCR2;

extern volatile uint32_t *const PORT1_PCR12;
extern volatile uint32_t *const PORT3_PCR21;

extern const uint32_t LEDR_mask;
extern const uint32_t LEDG_mask;
extern const uint32_t LEDB_mask;
extern const uint32_t SW2_mask;
extern const uint32_t SW3_mask;


extern volatile uint32_t *const AHBCLKCTRL1;

// timer 0
extern volatile uint32_t *const CTIMERCLKSEL0;
extern volatile uint32_t *const CTIMER0CLKDIV;


#define TIMER_FREQ 12E6
//#define CTIMER0_BASE 0x4000C000
#define CTIMER0_INT 31 //Numero d'interruption CTIMER0
extern volatile uint32_t *const CTIMER0_EMR;
extern volatile uint32_t *const CTIMER0_IR ;
extern volatile uint32_t *const CTIMER0_TCR;
extern volatile uint32_t *const CTIMER0_MCR;
extern volatile uint32_t *const CTIMER0_MR0;
extern volatile uint32_t *const CTIMER0_MR1;
extern volatile uint32_t *const CTIMER0_MR2;
extern volatile uint32_t *const CTIMER0_MR3;
extern volatile uint32_t *const CTIMER0_CTCR;
extern volatile uint32_t *const CTIMER0_PWMC ;

// timer 2
extern volatile uint32_t *const CTIMERCLKSEL2;
extern volatile uint32_t *const CTIMER2CLKDIV;

//#define CTIMER2_BASE 0x4000E000
#define CTIMER2_INT 34 //Numero d'interruption CTIMER2
extern volatile uint32_t *const CTIMER2_EMR;
extern volatile uint32_t *const CTIMER2_IR ;
extern volatile uint32_t *const CTIMER2_TCR;
extern volatile uint32_t *const CTIMER2_MCR;
extern volatile uint32_t *const CTIMER2_MR0;
extern volatile uint32_t *const CTIMER2_MR1;
extern volatile uint32_t *const CTIMER2_MR2;
extern volatile uint32_t *const CTIMER2_MR3;
extern volatile uint32_t *const CTIMER2_CTCR;
extern volatile uint32_t *const CTIMER2_PWMC ;

typedef enum doigt {
    POUCE,
    INDEX,
    MAJEUR,
    ANNULAIRE,
    AURICULAIRE
} doigt;

typedef enum geste {
    CINQ ,
	DEUX,
    QUATRE,
    TROIS,
    UN,
    ZERO,
} geste;

void effectuer_geste(int g);
void main_doigt(doigt d, int pos);
void servos_init();
void UpdateServoFromSign(int g_label_num, int g_detected_flag);

void delay(uint32_t ms);

void SW2_handler(void);

void init_leds(void); //Initialisation des GPIO/PORT pour controle des LEDs
void init_switch(void); //Initialisation des GPIO/PORT pour lecture des switchs
void setup_vtab_RAM(void);  // Déclaration de la fonction dans le fichier d'en-tête
//void init_interrupt_sw2(void (*isr)(void));
void init_interrupt_sw2(void*);

void ledg_on(void);  // Allumer la LED verte
void ledg_off(void); // Éteindre la LED verte

int rd_sw2(); //Lecture sw2
int rd_sw3(); //Lecture sw3

void init_interrupt_sw3(void*);
void set_edge_interrupt_sw2(uint8_t falling);

extern volatile uint32_t* const PORT0_ICR06;

#ifdef __cplusplus
}
#endif
