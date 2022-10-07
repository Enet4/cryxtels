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
#include "primitives.h"

/// Identifier for a pixel in the game
/// (from 0 to 32768, -1 means no pixel or not defined).
using PixelId = i16;

/// Identifier for an object in the game
/// (from 0 to 32768, -1 means no object or not defined).
using ObjectId = i16;

/// Identifier for a pixel type in the game
/// (from 0 to 32768, -1 means no pixel type or not defined).
using PixelTypeId = i16;

/// Identifier for an object type in the game (from 0 to 32768, -1 means no object type).
using ObjectTypeId = i16;

/// The total number of different pixel primitives.
/// (updated from the original 23, to include SOLIDBOX)
constexpr unsigned int COMS = 24;

extern bool type_mode; // using type mode flag instead of scroll lock
extern char type_this; // character to type in the keyboard

/// alpha angle of the Fly
extern i16 nav_a;
/// beta angle of the Fly
extern i16 nav_b;

/// persistent distance to Sunny,
/// calculated each frame
extern double dsol;

/// Whether the echo probe is on or off
extern char bkecho;

/// triggered when the player is close enough to Sunny,
/// so that the palette is tweaked accordingly
extern bool yel;

/// Button pressed
extern int bki;

/// Flag for the radar flashes
extern char blink;

/// tracker for whether the thrust key is held
extern bool thrust_keyhold;
/// tracker for whether the back key is held
extern bool back_keyhold;

/// tracker for right click holding duration
extern int rclick;


extern u8 taking; // Flag: attempt of taking an object.
extern ObjectId carry_type; // Object type being carried (-1 = none).
extern double trackframe; // Number of frames of the docking sequence.
extern u8 reset_trackframe; // Flag: reset trackframe, please.
extern double tracking; // Increment value to trackframe.
/// Counter for the number of frames of the Fly reentering sequence.
extern u8 req_end_extra;

extern i16 alfad, betad; // Angular speed of x and y.
extern i16 alfa90, beta90; // Support for some calculations.

extern u8 fid; // Flag: Orientation of the direction opposite to the current one...
extern u8 lead; // Flag: Orientation of the leading direction...
extern u8 orig; // Flag: Orientation towards Sunny (Solicchio)...

// Mouse flags
extern u8 m;
/// Previous mouse flags
extern u8 comera_m;

extern double spd_x; // x component of the ship's velocity.
extern double spd_y; // y component of the ship's velocity.
extern double spd_z; // z component of the ship's velocity.
extern double spd; // ship's acceleration.

extern double rel_x, rel_y, rel_z; /* Coordinates relative to the docking site,
                               extra-vehicular activities. */

extern ObjectId obj; // Nearest object for taking (-1 = none).
extern u8 echo; // Echo flags

extern PixelId carried_pixel; // Pixel being carried (only considered in Sunny (Solicchio)).

extern double disl; // dislivello incontrato da un passo all'altro.

/// Cursor position on a text board.
extern u16 cursore;

/// Counter for explosion sequence.
extern u8 explode_count;

extern char repeat; // Ripetizione brani dai CD virtuali.
extern char source; // Sorgente di registrazione selezionata.
extern char quality; // Qualità dell'incisione.


extern unsigned char ctk;

extern char subs;

extern double prevpixx;
extern double prevpixz;

/// Total number of existent pixel types
/// according to the pixel definitions.
extern u16 existent_pixeltypes;
// Total number of existent object types
/// according to the pixel definitions.
extern u16 existent_objecttypes;

/* Dati sui pixels. */

extern u16 pixels; // Total nr. of pixels (not initialized).

extern PixelTypeId far *pixeltype; // Pixel type array, from 0 to existent_pixeltypes-1.

extern u8 far *pixel_rot; // Rotation amount around Sunny flag.

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

extern u8* buffer; // A multi-purpose piece of memory.

/// Number of loaded pixel types.
extern u8 loaded_pixeltypes;

extern PixelTypeId* pixeltype_type;      // What pixel type is defined for that pixel?
extern u8*  pixeltype_elements; // How many elements are defined for that pixel type?

/// The names of the primitives.
extern const char *comspec[COMS];

extern const u8 params[COMS];

extern u8 far *pixel_elem_t;   // Element types. size = ELEMS*BUFFERS

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

/// X coordinate of each pixel's dock site
extern float far *docksite_x;
/// Y coordinate of each pixel's dock site
extern float far *docksite_y;
/// Z coordinate of each pixel's dock site
extern float far *docksite_z;
/// Dock site width for each pixel.
extern float far *docksite_w;
/// Dock site height for each pixel.
extern float far *docksite_h;

// Pixel mass
extern float far *pixelmass;

// Files for sottofondi audio.
extern char far *subsignal;

/* Concerning objects. */

extern u16 objects; // Total nr. of objects (not initialized).
extern u16 _objects; // Variable used when cycling for the total number of objects.

extern ObjectTypeId  *objecttype;      // Object type.
extern double far *relative_x;      // Position X (relative to pixel).
extern double far *relative_y;      // Position Y (relative to pixel).
extern double far *relative_z;      // Position Z (relative to pixel).
extern double far *absolute_x;      // Position X (absolute for Pixel = -1).
extern double far *absolute_y;      // Position Y (absolute for Pixel = -1).
extern double far *absolute_z;      // Position Z (absolute for Pixel = -1).
extern PixelId  *object_location; // Pixel where the object can be retrieved.

/* IMPORTANT: the object's elevation at the pixel's surface for each object type.
 */

extern double object_elevation[203];
extern double object_collyblockshifting[203];

// Varied support.

/// ID of the current closest pixel.
extern PixelId pix;

/// Flag: 1 = extravehicular activity (walking on a pixel); 0 = on Fly
extern u8 extra;

extern double r_x, r_y, r_z; // Posizioni relat. al pixel, per il fotogr. precedente.
extern double dsx, dsy, dsz; // Posizioni reali del docksite del pixel-bersaglio.
extern double cam_xt, cam_yt, cam_zt; // Posizioni <teoriche> dell'osservatore.
extern double _x, _y, _z, _ox, _oy, _oz; // Temporanee nei disegni.

/// X coordinate of The Fly
extern double nav_x;
/// Y coordinate of The Fly
extern double nav_y;
/// Z coordinate of The Fly
extern double nav_z;

extern double d; // Distanza generica.
extern double k1; // Costanti ausiliarie.
extern int a, b, c; // Contatori ausiliari.

extern int id; // 0, 1, 2 o 3. A seconda della distanza del pixel.
extern int nopix; // Nr. Pixel.

extern char globalvocfile[13]; // sottofondi sui pixels.

extern char fermo_li;

/// whether the sound emitting object is close
extern int vicini;
extern int sta_suonando;
/// ID of the pixel emitting sound.
extern PixelId pixel_sonante;

extern FILE* recfile;

extern double cox, coy, coz;
extern u8 justloaded;

/// Whether the last object moved is still being dropped.
extern u8 moving_last_object; // Sta spostando l'ultimo oggetto lasciato.
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

