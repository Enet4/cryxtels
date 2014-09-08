/** \file global.cpp
 *  This file is part of Crystal Pixels.
 *
 *  Crystal Pixels is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Crystal Pixels is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "global.h"

bool type_mode = false; // using type mode flag instead of scroll lock
char type_this = 0; // character to type in the keyboard

/* Variabili di cima per allineamento e risparmio di istruzioni read/write. */

short int nav_a = 0; // Inclinazione di The Fly.
short int nav_b = 0;

char taking = 0; // Flag: tentativo di prelevamento di un oggetto.
short int carry_type = -1; // Tipo oggetto trasportato (-1 = nessuno).
double trackframe = 0; // Nr. fotogramma della sequenza di attracco.
char reset_trackframe = 0; // Flag: azzera trackframe, per favore.
double tracking = 0; // Incremento a trackframe.
char req_end_extra = 0; /* Flag: si richiede la fine dell'attivit
                           extraveicolare ed il rientro nella nave? */

short int alfad = 0, betad = 0; // Velocit angolare su assi x ed y.
short int alfa90, beta90; // Supporti per alcuni calcoli.

char fid = 0; // Flag: Orientamento nella direzione opposta a quella corrente...
char lead = 0; // Flag: Orientamento in direzione d'avanzamento...
char orig = 0; // Flag: Orientamento in direzione del Solicchio...
char comera_m;

double spd_x = 0; // Componente x della velocit della nave.
double spd_y = 0; // Componente y della velocit della nave.
double spd_z = 0; // Componente z della velocit della nave.
double spd = 0; // Accelerazione della nave.

double rel_x, rel_y, rel_z; /* Coordinate relative al docksite per attivit
                               extraveicolare. */

short int obj = -1; // Oggetto pi vicino da prelevare (-1 = nessuno).
char m = 1; // Puntamento da mouse?
char echo = 1; // Ecoscandaglio acceso?

short int carried_pixel = -1; // Pixel trasportato (solo i DONI del Solicchio).

double disl = 0; // dislivello incontrato da un passo all'altro.

short int cursore = 0; // Sulle lavagnette.

char explode_count = 0; // Durante gli scoppii.

char repeat  = 0; // Ripetizione brani dai CD virtuali.
char source  = 0; // Sorgente di registrazione selezionata.
char quality = 0; // Qualit dell'incisione.


unsigned char ctk;

char subs = 1;

double prevpixx;
double prevpixz;

short int existant_pixeltypes = 0; // Tipi di pixel esistenti (letti da disco).
short int existant_objecttypes = 3; // Tipi di oggetto esistenti.

/* Dati sui pixels. */

short int pixels = 0; // Nr. totale dei pixels (non inizializzato).

short int far *pixeltype; // Tipo di pixel, da 0 a existant_pixeltypes-1.

unsigned char far *pixel_rot; // Flag di rotazione attorno al Solicchio.

float far  *pixel_absd;    // Distanza assoluta dall'osservatore.
double far *pixel_support; // Supporto per quel pixel (piccola memoria).
double far *pixel_xdisloc; // Posizione X.
double far *pixel_ydisloc; //     -     Y.
double far *pixel_zdisloc; //     -     Z.

// Definizioni comode.

const int ELEMS             =   100; // Increased from Max. 40
const int BUFFERS           =   100; // Increased from Max. 40
const int LIMITE_DISLIVELLI =   70; // Illimitato
const int CULLING           = 8000; // Illimitato

const int FRONTIER          =            250; // Da 3 a 647.
const int FRONTIER_M1       =   FRONTIER - 1;
const int FRONTIER_M2       =   FRONTIER - 2;
const int FRONTIER_M3       =   FRONTIER - 3;
const int FRONTIER_COMPL    = 650 - FRONTIER;
const int FRONTIER_COMPL_M1 = 649 - FRONTIER;

const int MEMORIA_RICHIESTA = 251901 + 81*ELEMS*BUFFERS + 3*BUFFERS;

//unsigned char far *ctrlkeys = (unsigned char far *) 0x417;

/* Descrizione dei tipi di pixel
   (<BUFFERS> tipi caricati per volta, <ELEMS> elementi per tipo). */

unsigned char far *buffer; // Buffer per la ricerca degli Id.

char loaded_pixeltypes;       // Tipi di pixel caricati.

short int far *pixeltype_type;      // Che tipo di pixel ha questa definizione?
unsigned char far *pixeltype_elements; // Quanti elementi contiene la definizione?

const char *comspec[] = {
"ENDPIXEL", // FINEPIXEL
"DETAIL", // DETTAGLIO
"DOCK", // ATTRACCO
"COLLISION", //BLOCCOCOLLISIONE
"SPIRAL", // SPIRALE
"DOT", // PUNTO
"LINE", // LINEA
"RECTANGLE", // RETTANGOLO
"BOX", // SCATOLA
"GRID", // GRIGLIA
"DOTTEDELLIPSE", // DISEGNOELLITTICO
"WAVE", // ONDA
"COLUMN", // COLONNA
"ASTERISK", // ASTERISCO
"SPHERE", // SFERA
"TEXT", // TESTO
"ASSOCIATEDFILE", // SOTTOFONDO
"FORBIDDEN", // BLOCCOPROIBITO
"ELLIPSE", // ELLISSE
"DONUT", // CIAMBELLA
"GRIDSPHERE", // SFERARETICOLARE
"TOTALMASS", // MASSA
"COLLISIONHIGH", // BLOCCOCOLLISIONEELEVATO
"SOLIDBOX" // new element in English version
};

char params[coms] = {
0,
0,
5,
6,
6,
3,
6,
6,
6,
7,
7,
7,
7,
5,
6,
7,
0,
6,
7,
7,
6,
1,
6,
6,
};

unsigned char far *pixel_elem_t;   // Tipo di elemento.

double far *pixel_elem_x; // Coordinate relative al pixel.
double far *pixel_elem_y;
double far *pixel_elem_z;
float  far *pixel_elem_1; // Parametri speciali (l, h, p, r, orient) ...
float  far *pixel_elem_2;
float  far *pixel_elem_3;
float  far *pixel_elem_4;

char far *pixel_elem_b;   /* Buffer testuale per la funzione TEXT (40 car.),
                             il buffer contiene il testo, ma pu contenere
                             l'id del pixel se nel testo viene inserito
                             il simbolo "%d", insieme ad eventuali caratteri.
                             Parametri per text, in ordine numerico da 0:
                             x, y, z, scalax, scalay, or.x, or.y */

float far *docksite_x;   // Posizione dei siti di attracco per ogni pixeltype.
float far *docksite_y;
float far *docksite_z;
float far *docksite_w;   // Larghezza e Profondit.
float far *docksite_h;

float far *pixelmass; // Massa.

char far *subsignal; // Files per sottofondi audio.

/* Dati sugli oggetti. */

short int objects = 0; // Nr. totale degli oggetti (non inizializzato).
short int _objects; // Variabile usata nei cicli come nr. totale di oggetti.

short int  *objecttype;      // Tipo d'oggetto.
double far *relative_x;      // Posizione X (relativa a quella del Pixel).
double far *relative_y;      // Posizione Y (relativa a quella del Pixel).
double far *relative_z;      // Posizione Z (relativa a quella del Pixel).
double far *absolute_x;      // Posizione X (assoluta per Pixel = -1).
double far *absolute_y;      // Posizione Y (assoluta per Pixel = -1).
double far *absolute_z;      // Posizione Z (assoluta per Pixel = -1).
short int  *object_location; // Pixel su cui si trova l'oggetto.

/* IMPORTANTE: elevazione sulla superficie del pixel
               per ogni tipo di oggetto.
               I primi 3 sono gi definiti a zerozerouno. */

double far object_elevation[203] = { 0.01, 0.01, 0.01 };
double far object_collyblockshifting[203];

// Supporti vari.

short int  pix = 0; // Pixel pi vicino (ecoscandaglio, attracco automatico...)

char extra = 0; // Flag: attivit extraveicolare in corso?

double r_x, r_y, r_z; // Posizioni relat. al pixel, per il fotogr. precedente.
double dsx, dsy, dsz; // Posizioni reali del docksite del pixel-bersaglio.
double cam_xt, cam_yt, cam_zt; // Posizioni <teoriche> dell'osservatore.
double _x, _y, _z, _ox, _oy, _oz; // Temporanee nei disegni.

double nav_x; // Coordinate di The Fly.
double nav_y;
double nav_z;

double d; // Distanza generica.
double k1; // Costanti ausiliarie.
int a, b, c; // Contatori ausiliari.

int id; // 0, 1, 2 o 3. A seconda della distanza del pixel.
int nopix; // Nr. Pixel.

//double ob = 15000, micro_x = 0, micro_y = 0;

char globalvocfile[13] = ".voc"; // sottofondi sui pixels.

char fermo_li = 0;

int vicini = 0;
int sta_suonando = -1;
int pixel_sonante = -1;

int recfile = -1;

double cox = 0, coy = 0, coz = 0;
char justloaded = 5;

char moving_last_object = 0; // Sta spostando l'ultimo oggetto lasciato.
double cfx, cfy, cfz; // Carried-Final-relative-X/Y/Z (dove deve andare /\).

const char *source_name[] = { "SORG. PREIMPOSTATA", "C.D.", "MIC", "LINEA D'INGRESSO" };
const char *record_qlty[] = { "SIMIL-NASTRO", "NASTRO H.Q.", "C.D." };

unsigned record_frq[] = {    15150,        22050,       44100 };
unsigned record_filt[] = {     0,            0,           1 };

// Variabili per i testi.

char autore_forme[40]; // Nome di chi ha disegnato le forme del PIXELS.DEF
char t[80]; // Usata nelle funz. Pixel & Object.

