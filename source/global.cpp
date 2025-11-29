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
#include <cstdio>

bool type_mode = false; // using type mode flag instead of scroll lock
char type_this = 0; // character to type in the keyboard

/* Variabili di cima per allineamento e risparmio di istruzioni read/write. */

i16 nav_a = 0; // Inclinazione di The Fly.
i16 nav_b = 0;

/// persistent distance to Sunny,
/// calculated each frame
double dsol = 0;

char bkecho = 1; // bk-up per l'ecoscandaglio
/// triggered when the player is close enough to Sunny,
/// so that the palette is tweaked accordingly
bool yel = false;

int bki = -1; // Button pressed.

char blink = 0; // flag per lampeggìo.

/// tracker for whether the thrust key is held
bool thrust_keyhold = false;
/// tracker for whether the back key is held
bool back_keyhold = false;

/// tracker for right click holding duration
int rclick = -1;

u8 taking = 0; // Flag: tentativo di prelevamento di un oggetto.
ObjectId carry_type = -1; // Tipo oggetto trasportato (-1 = nessuno).
double trackframe = 0; // Nr. fotogramma della sequenza di attracco.
u8 reset_trackframe = 0; // Flag: azzera trackframe, per favore.
double tracking = 0; // Incremento a trackframe.
u8 req_end_extra = 0; /* Flag: si richiede la fine dell'attività
                           extraveicolare ed il rientro nella nave? */

i16 v_alpha = 0, v_beta = 0; // Velocità angolare su assi x ed y.
i16 alpha90, beta90; // Supporti per alcuni calcoli.

u8 fid = 0; // Flag: Orientamento nella direzione opposta a quella corrente...
u8 lead = 0; // Flag: Orientamento in direzione d'avanzamento...
u8 orig = 0; // Flag: Orientamento in direzione del Solicchio...
u8 comera_m;

double spd_x = 0; // Componente x della velocità della nave.
double spd_y = 0; // Componente y della velocità della nave.
double spd_z = 0; // Componente z della velocità della nave.
double spd = 0; // Accelerazione della nave.

double rel_x, rel_y, rel_z; /* Coordinate relative al docksite per attivit
                               extraveicolare. */

ObjectId obj = -1; // Oggetto più vicino da prelevare (-1 = nessuno).
u8 m = 1; // Puntamento da mouse?
u8 echo = 1; // Ecoscandaglio acceso?

PixelId carried_pixel = -1; // Pixel trasportato (solo i DONI del Solicchio).

double disl = 0; // dislivello incontrato da un passo all'altro.

u16 cursore = 0; // Sulle lavagnette.

u8 explode_count = 0; // Durante gli scoppii.

char repeat  = 0; // Ripetizione brani dai CD virtuali.
char source  = 0; // Sorgente di registrazione selezionata.
char quality = 0; // Qualità dell'incisione.


unsigned char ctk;

char subs = 1;

double prevpixx;
double prevpixz;

u16 existent_pixeltypes = 0; // Tipi di pixel esistenti (letti da disco).
u16 existent_objecttypes = 3; // Tipi di oggetto esistenti.

/* Dati sui pixels. */

u16 pixels; // Nr. totale dei pixels (non inizializzato).

PixelTypeId* pixeltype; // Tipo di pixel, da 0 a existent_pixeltypes-1.

u8* pixel_rot; // Flag di rotazione attorno al Solicchio.

float  *pixel_absd;    // Distanza assoluta dall'osservatore.
double *pixel_support; // Supporto per quel pixel (piccola memoria).
double *pixel_xdisloc; // Posizione X.
double *pixel_ydisloc; //     -     Y.
double *pixel_zdisloc; //     -     Z.

// Definizioni comode.

/// Number of elements 
/// (Increased from Max. 40)
const int ELEMS             =   100;
/// Number of buffers 
/// (Increased from Max. 40)
const int BUFFERS           =   100; // Increased from Max. 40
const int LIMITE_DISLIVELLI =   70; // Illimitato
const int CULLING           = 8000; // Illimitato

const int FRONTIER          =            250; // Da 3 a 647.
const int FRONTIER_M1       =   FRONTIER - 1;
const int FRONTIER_M2       =   FRONTIER - 2;
const int FRONTIER_M3       =   FRONTIER - 3;
const int FRONTIER_COMPL    = 650 - FRONTIER;
const int FRONTIER_COMPL_M1 = 649 - FRONTIER;

// Warning: this constant is likely outdated, do not rely on it
const int MEMORIA_RICHIESTA = 251901 + 81*ELEMS*BUFFERS + 3*BUFFERS;

//unsigned char *ctrlkeys = (unsigned char *) 0x417;

/* Descrizione dei tipi di pixel
   (<BUFFERS> tipi caricati per volta, <ELEMS> elementi per tipo). */

u8 *buffer; // Buffer per la ricerca degli Id.

u8 loaded_pixeltypes;       // Tipi di pixel caricati.

PixelTypeId *pixeltype_type;      // Che tipo di pixel ha questa definizione?
u8 *pixeltype_elements; // Quanti elementi contiene la definizione?

const char* comspec[COMS] = {
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

/// The number of parameters expected for each primitive in a pixel definition.
const u8 params[COMS] = {
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

u8* pixel_elem_t;   // Tipo di elemento.

double *pixel_elem_x; // Coordinate relative al pixel.
double *pixel_elem_y;
double *pixel_elem_z;
float  *pixel_elem_1; // Parametri speciali (l, h, p, r, orient) ...
float  *pixel_elem_2;
float  *pixel_elem_3;
float  *pixel_elem_4;

char *pixel_elem_b;   /* Buffer testuale per la funzione TEXT (40 car.),
                             il buffer contiene il testo, ma può contenere
                             l'id del pixel se nel testo viene inserito
                             il simbolo "%d", insieme ad eventuali caratteri.
                             Parametri per text, in ordine numerico da 0:
                             x, y, z, scalax, scalay, or.x, or.y */

float *docksite_x;   // Posizione dei siti di attracco per ogni pixeltype.
float *docksite_y;
float *docksite_z;
float *docksite_w;   // Larghezza e Profondità.
float *docksite_h;

float *pixelmass; // Massa.

char *subsignal; // Files per sottofondi audio.

/* Dati sugli oggetti. */

u16 objects; // Nr. totale degli oggetti (non inizializzato).
u16 _objects; // Variabile usata nei cicli come nr. totale di oggetti.

ObjectTypeId  *objecttype;      // Tipo d'oggetto.
double *relative_x;      // Posizione X (relativa a quella del Pixel).
double *relative_y;      // Posizione Y (relativa a quella del Pixel).
double *relative_z;      // Posizione Z (relativa a quella del Pixel).
double *absolute_x;      // Posizione X (assoluta per Pixel = -1).
double *absolute_y;      // Posizione Y (assoluta per Pixel = -1).
double *absolute_z;      // Posizione Z (assoluta per Pixel = -1).
PixelId  *object_location; // Pixel su cui si trova l'oggetto.

double object_elevation[203] = { 0.01, 0.01, 0.01 };
double object_collyblockshifting[203];

// Supporti vari.

PixelId pix = 0; // Pixel più vicino (ecoscandaglio, attracco automatico...)

u8 EVA_in_progress = 0; // Flag: attività extraveicolare in corso?

double r_x, r_y, r_z; // Posizioni relat. al pixel, per il fotogr. precedente.
double dsx, dsy, dsz; // Posizioni reali del docksite del pixel-bersaglio.
double cam_xt, cam_yt, cam_zt; // Posizioni <teoriche> dell'osservatore.
double _x, _y, _z, _ox, _oy, _oz; // Temporanee nei disegni.

double nav_x; // Coordinate di The Fly.
double nav_y;
double nav_z;

double d; // Distanza generica.
double k0, k1; // Costanti ausiliarie.
int a, b, c; // Contatori ausiliari.

int id; // 0, 1, 2 o 3. A seconda della distanza del pixel.
/// A general purpose variable, usually for keeping a Pixel ID.
/// 
/// This variable may be used by some functions to know which pixel
/// is to be considered.
int  nopix;

//double ob = 15000, micro_x = 0, micro_y = 0;

char globalvocfile[13] = ".voc"; // sottofondi sui pixels.

char fermo_li = 0;

int vicini = 0;
int sta_suonando = -1;
PixelId pixel_sonante = -1;

FILE* recfile = nullptr;

double cox = 0, coy = 0, coz = 0;
u8 justloaded = 5;

u8 moving_last_object = 0; // Sta spostando l'ultimo oggetto lasciato.
double cfx, cfy, cfz; // Carried-Final-relative-X/Y/Z (dove deve andare /\).

const char *source_name[] = { "SORG. PREIMPOSTATA", "C.D.", "MIC", "LINEA D'INGRESSO" };
const char *record_qlty[] = { "SIMIL-NASTRO", "NASTRO H.Q.", "C.D." };

unsigned record_frq[] = {    15150,        22050,       44100 };
unsigned record_filt[] = {     0,            0,           1 };

// Variabili per i testi.

char autore_forme[40]; // Nome di chi ha disegnato le forme del PIXELS.DEF
char t[80]; // Usata nelle funz. Pixel & Object.

