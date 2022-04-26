/** \file global.h
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
// Global variables, many thingies used throughout the code are kept here.
#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

// Discarding all old "far" qualifiers
#ifndef far
#define far
#endif

// provide strcasecmp to msvc
#ifdef _MSC_VER
#include <string.h>
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#include <cstdio>

constexpr unsigned int coms = 24; // new element included
//#define coms 23

extern bool type_mode; // using type mode flag instead of scroll lock
extern char type_this; // character to type in the keyboard

extern short int nav_a, nav_b;

extern char taking; // Flag: attempt of taking an object.
extern short int carry_type; // Object type being carried (-1 = none).
extern double trackframe; // Number of frames of the docking sequence.
extern char reset_trackframe; // Flag: reset trackframe, please.
extern double tracking; // Increment value to trackframe.
extern char req_end_extra; /* Flag: si richiede la fine dell'attivit
                                 extraveicolare ed il rientro nella nave? */

extern short int alfad, betad; // Angular speed of x and y.
extern short int alfa90, beta90; // Support for some calculations.

extern char fid; // Flag: Orientation of the direction opposite to the current one...
extern char lead; // Flag: Orientation of the leading direction...
extern char orig; // Flag: Orientation towards Sunny (Solicchio)...
extern char comera_m;

extern double spd_x; // x component of the ship's velocity.
extern double spd_y; // y component of the ship's velocity.
extern double spd_z; // z component of the ship's velocity.
extern double spd; // ship's acceleration.

extern double rel_x, rel_y, rel_z; /* Coordinates relative to the docking site,
                               extra-vehicular activities. */

extern short int obj; // Nearest object for taking (-1 = none).
extern char m; // Mouse pointer?
extern char echo; // Echo flags

extern short int carried_pixel; // Pixel being carried (only considered in Sunny (Solicchio)).

extern double disl; // dislivello incontrato da un passo all'altro.

extern short int cursore; // Sulle lavagnette.

extern char explode_count; // Durante gli scoppii.

extern char repeat; // Ripetizione brani dai CD virtuali.
extern char source; // Sorgente di registrazione selezionata.
extern char quality; // Qualit dell'incisione.


extern unsigned char ctk;

extern char subs;

extern double prevpixx;
extern double prevpixz;

extern short int existent_pixeltypes; // Existent pixel types.
extern short int existent_objecttypes; // Existent object types.

/* Dati sui pixels. */

extern short int pixels; // Total nr. of pixels (not initialized).

extern short int far *pixeltype; // Pixel type array, from 0 to existent_pixeltypes-1.

extern unsigned char far *pixel_rot; // Rotation around Sunny flag.

extern float far  *pixel_absd;    // Absolute distance from the observer.
extern double far *pixel_support; // Support for which pixel (in memory).
extern double far *pixel_xdisloc; // Position X.
extern double far *pixel_ydisloc; //     -    Y.
extern double far *pixel_zdisloc; //     -    Z.

// Command definitions.

extern const int ELEMS;
extern const int BUFFERS;
extern const int LIMITE_DISLIVELLI;
extern const int CULLING;

extern const int FRONTIER;
extern const int FRONTIER_M1;
extern const int FRONTIER_M2;
extern const int FRONTIER_M3;
extern const int FRONTIER_COMPL;
extern const int FRONTIER_COMPL_M1;

/// Warning: this constant is likely outdated, do not rely on it
extern const int MEMORIA_RICHIESTA;

/* Descriptions of each pixel type
   (<BUFFERS> tipi caricati per volta, <ELEMS> element per type). */

extern unsigned char far *buffer; // Buffer for retrieving Id's.

extern char loaded_pixeltypes;       // Loaded pixel types.

extern short int far *pixeltype_type;      // What pixel type is defined for that pixel?
extern unsigned char far *pixeltype_elements; // How many elements are defined for that pixel?

extern const char *comspec[];

extern char params[coms];

extern unsigned char far *pixel_elem_t;   // Element types. size = ELEMS*BUFFERS

extern double far *pixel_elem_x; // Coordinates relative to pixel.
extern double far *pixel_elem_y;
extern double far *pixel_elem_z;
extern float  far *pixel_elem_1; // Special parameters (l, h, p, r, orient) ...
extern float  far *pixel_elem_2;
extern float  far *pixel_elem_3;
extern float  far *pixel_elem_4;

/** Text buffer for the TEXT function (40 car.),
 * the buffer contains the text, but can contain the
 * pixel's id if the "%d" symbol is inserted in the text.
 * Text parameters, in numeric order from 0:
 * x, y, z, scalax, scalay, or.x, or.y
 */
extern char far *pixel_elem_b;

// Position of the docking type for each pixel type.
extern float far *docksite_x;  
extern float far *docksite_y;
extern float far *docksite_z;
// Width and Depth.
extern float far *docksite_w;
extern float far *docksite_h;

// Pixel mass
extern float far *pixelmass;

// Files for sottofondi audio.
extern char far *subsignal;

/* Concerning objects. */

extern short int objects; // Total nr. of objects (not initialized).
extern short int _objects; // Variable used when cycling for the total number of objects.

extern short int  *objecttype;      // Object type.
extern double far *relative_x;      // Position X (relative to pixel).
extern double far *relative_y;      // Position Y (relative to pixel).
extern double far *relative_z;      // Position Z (relative to pixel).
extern double far *absolute_x;      // Position X (absolute for Pixel = -1).
extern double far *absolute_y;      // Position Y (absolute for Pixel = -1).
extern double far *absolute_z;      // Position Z (absolute for Pixel = -1).
extern short int  *object_location; // Pixel where the object can be retrieved.

/* IMPORTANT: the object's elevation at the pixel's surface for each object type.
 */

extern double object_elevation[203];
extern double object_collyblockshifting[203];

// Varied support.

extern short int pix; // Pixel pi vicino (ecoscandaglio, attracco automatico...)

extern char extra; // Flag: attivit extraveicolare in corso?

extern double r_x, r_y, r_z; // Posizioni relat. al pixel, per il fotogr. precedente.
extern double dsx, dsy, dsz; // Posizioni reali del docksite del pixel-bersaglio.
extern double cam_xt, cam_yt, cam_zt; // Posizioni <teoriche> dell'osservatore.
extern double _x, _y, _z, _ox, _oy, _oz; // Temporanee nei disegni.

extern double nav_x; // Coordinate di The Fly.
extern double nav_y;
extern double nav_z;

extern double d; // Distanza generica.
extern double k1; // Costanti ausiliarie.
extern int a, b, c; // Contatori ausiliari.

extern int id; // 0, 1, 2 o 3. A seconda della distanza del pixel.
extern int nopix; // Nr. Pixel.

extern char globalvocfile[13]; // sottofondi sui pixels.

extern char fermo_li;

extern int vicini;
extern int sta_suonando;
extern int pixel_sonante;

extern FILE* recfile;

extern double cox, coy, coz;
extern char justloaded;

extern char moving_last_object; // Sta spostando l'ultimo oggetto lasciato.
extern double cfx, cfy, cfz; // Carried-Final-relative-X/Y/Z (dove deve andare /\).

extern const char *source_name[];
extern const char *record_qlty[];

extern unsigned int record_frq[];
extern unsigned int record_filt[];

// Variabili per i testi.

extern char autore_forme[40]; // Nome di chi ha disegnato le forme del PIXELS.DEF
extern char t[80]; // Usata nelle funz. Pixel & Object.


/* Fine variabili di cima - inizio sezione signal processing. */

#define nr_audiofiles 14
//#include <dsp.h>

#define BOM        13
#define DESTROY    12
#define READY      11
#define TASTO      10
#define FLY_OFF     9
#define FLY_ON      8
#define PASSO       7
#define FID         6
#define LASCIARE    5
#define PRENDERE    4
#define DISTACCO    3
#define ATTRACCO    2
#define TARGET      1

#define SOTTOFONDO  0

/* Fine sezione signal processing. */

/*
        La sintassi :

                NR_ELEMENTO, X, Y, Z, [PARAMETRI], BUFFER TESTO.

        Se i parametri sono meno di quattro, inserire degli zeri.
*/

#define FINEPIXEL            0 // Nessun parametro.
#define DETTAGLIO            1 // Nessun parametro.
#define ATTRACCO             2 // 1 = l, 2 = h. Se h < 0, sito circolare di raggio l.
#define BLOCCO_COLLISIONE    3 // 1 = l, 2 = h, 3 = p.
#define SPIRALE              4 // 1 = increm. del raggio per ogni c gradi, 2 = orientamento, 3 = c.
#define PUNTO                5 // Nessun parametro a parte la posizione.
#define LINEA                6 // 1 = x finale, 2 = y finale, 3 = z finale.
#define RETTANGOLO           7 // 1 = l, 2 = h, 3 = orientam.(0=XY/1=XZ/2=YZ).
#define SCATOLA              8 // 1 = l, 2 = h, 3 = p.
#define GRIGLIA              9 // 1 = scala orizzontale, 2 = scala verticale, 3 = cas. per lato., 4 = orientamento.
#define DISEGNO_ELLITTICO   10 // 1 = asse oriz., 2 = asse vert., 3 = orient., 4 = gradi per punto.
#define ONDA                11 // 1 = scala, 2 = ampiezza, 3 = orientamento, 4 = gradi per linea.
#define COLONNA             12 // 1 = raggio base, 2 = raggio sommit, 3 = altezza, 4 = gradi per faccia.
#define ASTERISCO           13 // 1 = raggio (lunghezza di un filamento), 2 = gradi per punto.
#define SFERA               14 // 1 = raggio, 2 = rapporto d'aspetto verticale, 3 = gradi per punto.
#define TESTO               15 // Vedere sez. buffer t.
#define SOTTOSEGNALE        16 // Testo = Nome file di sottofondo digitale.
#define BLOCCO_PROIBITO     17 // Come per i blocchi di collisione, ma neanche gli oggetti possono starci, e l'impedimento non dipende dall'altezza.
#define ELLISSE             18 // 1 = asse oriz., 2 = asse vert., 3 = orient., 4 = gradi per punto.
#define CIAMBELLA           19 // 1 = raggio ciambella, 2 = raggio sezione, 3 = orientamento, 4 = gradi per punto.
#define SFERA_RETICOLARE    20 // 1 = raggio, 2 = rapporto d'aspetto verticale, 3 = gradi per punto.
#define MASSA               21 // 1 = massa del pixel in questione
#define BLOCCO_ELEVATO      22 // Blocco collisione elevato.
#define SOLID_BOX           23 // New Block: like collision high, but also draws a box.

#endif

