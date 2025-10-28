/** \file cryxtels.cpp
 * Crystal Pixels - main source file
 * File created in 12th of September of 1994.
 * Initial version: 1.00
 *
 * Modern version of Crystal Pixels was started in August of 2013.
 *
 * This software is licensed under the GNU GPL v3.

    (c) 1994-95-96-97 Alessandro Ghignola and Fottifoh,
                      both being the same person.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.


        You should have received a copy of the GNU General Public License
        along with the program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <random>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <cmath>
#include <cstring>
#include <errno.h>
#include <cassert>
#include "global.h"
#include "primitives.h"
#include "fast3d.h"
#include "text3d.h"
#include "input.h"
#include "draw.h"
#include "sdl_exception.h"

#include "SDL.h"
#include "conf.h"

using namespace std;

constexpr int TICKS_IN_A_SECOND = 1000;
constexpr int TICKS_PER_FRAME = TICKS_IN_A_SECOND / FRAMES_PER_SECOND;
constexpr int INTRO_TICKS_PER_FRAME = TICKS_IN_A_SECOND / INTRO_FRAMES_PER_SECOND;

// dummy function (nullify effect)
inline void play (long) {}

/// initialize some parts of cryxtels
inline void init_start();

/// perform a broad range of needed memory allocations
/// \return false if any allocation fails
inline bool allocation_farm();

/// read the command line arguments and do stuff
void read_args(int argc, char** argv, char& flag, char& sit);

// Aggiorna il buffer dei pixels caricati.
/// Updates buffer with loaded pixels:
/// Search for the pixel type of ID `typ`
/// and load it if it is not loaded yet.
void cerca_e_carica (PixelTypeId typ);

/// build the cosmos
void build_cosm(char& flag);

/// Update
void Aggiornamento ();

/// Update pixels' absd value (distance to observer)
void dists ();

// Assegnazione posizioni a pixels gravitanti.
/// Assign positions to gravitating pixels.
void rot ();

/// Fade out effect of the display.
void fade (u8 speed = 1);

/// Docking effects.
void dock_effects ();

/// Save state
void save_situation (char i);

/// Load state
void load_situation (char i, bool skip_fade = false);

/// Terminate program in case PIXELS.DEF contains a syntax error
void fail_pixel_def (int el, int pix);

/// redefinition of random(int),
/// which probably became deprecated or existed in an outdated library
int random(int max_num);

/// Terminate some stuff
void alfin (char arc);

/// Take a snapshot into a BMP file.
void snapshot ();

// Auxiliary variables for collyblock function
char onblock; // Osservatore almeno su un blocco di collisione.
char dropping; // Si stanno lasciando cadere oggetti (fase iniziale).
double lastblock_real_x; // Servono alla funzione lascia_cadere.
double lastblock_real_z;
double max_y;

/* Modifica la posizione dell'osservatore se c' eventuale collisione
   con la zona data dal parallelepipedo definito con i parametri,
   relativi all'origine di un Crystal Pixel. */
/// Modify the observer's position based on the eventual collision
/// With the zone given by a box, relative to the origin of a cryxtel.
void collyblock (double cx, double cy, double cz,
                 double hx, double hy, double hz,
                 char blocktype);

/* Calcola le zone di collisione per il pixel vicino e per gli oggetti su
   di esso. Tenta di prelevare l'oggetto pi vicino. */
/// Calculates the collision zone by neighbour pixel and its objects.
/// It tries to get the nearest object.
void CollyZone (int nopix, char objectblocks);

// Disegna la forma di un Crystal Pixel o di un Oggetto.
/// Draws a Cryxtel's, or an object's, model
void Pixel (int typ);

/// Draws an object
void Object (int tipo);

// Funzione che toglie un oggetto dal microcosmo e lo dá all'utente.
/// Function that takes an object from the microcosm and gives it to the user.
void preleva_oggetto (int nr_ogg); // not implemented

/// Update objects' data over the pixels
void Oggetti_sul_Pixel (char oblige); // not implemented

// Funzione che lascia al suolo l'oggetto prelevato.
/// Function that leaves the taken object on the ground.
void lascia_cadere ();

/// Thrust / Walk Forward
void ispd ();

/// Stop Thrust / Walk Back
void dspd ();

/// Dock.
void attracco ();

/// Dock/Undock (calls attraco() in case of dock).
void undock ();

/// Enter/Leave The Fly
void extra_stop_extra ();

/// Look for angles starting from coordinates relative to the observer. (alpha & beta)
// Per cercare angoli partendo da coordinate relative all'osservatore.
void find_alphabeta();

/// Invert direction
void fid_on ();

/// Point towards velocity
void lead_on ();

/// Point towards Sunny
void orig_on ();

/// Generate the color palette
void tinte (u8 satu);

/// Explode
void scoppia (int nr_ogg, double potenza, int var);

/// close file driver?
void chiudi_filedriver ();

/// Fottifoh object model
extern i8 fotty[277];

/// Run one frame of intro loop logic
bool intro_loop();

/// Run one frame of main loop logic
bool main_loop();

int main(int argc, char** argv)
{
    cout << "Crystal Pixels" << endl
        << " version 3.15 originally made by Alessandro Ghignola in 1997" << endl
        << "     ported to modern systems by Eduardo Pinho in 2013" << endl;

    char flag = 0;
    //char sit = 'S'; // < the original would auto-load the default game save (Alex's save)
    char sit = 0;
    try {
        init_start();

        update_ctrlkeys();

        read_args(argc, argv, flag, sit);

        load_pixels_def();

        build_cosm(flag);

        cout << "\n" << endl;

        // set the display
        init_video();
        tinte (0);

        // Configure input

    } catch (int err) {
        cerr << "Error code: " << err << endl;
        return -1;
    } catch (sdl_exception& e) {
        cerr << "Error #" << e.code << " : " << e.what() << endl;
        return -1;
    }

    // Don't run the intro if a game is loaded by the user.
    bool run_intro = (argc!=2);

    cam_y = -4680; alpha = 90;
    ox = 0; oy = 0; oz = 0;

    sprintf (t, "%s'S MICROCOSM", autore_forme);

    //while (!tasto_premuto()&&!mpul) {
    // ...
    //}
    //while (!tasto_premuto() && !mpul) {
    while (run_intro) {
        run_intro = intro_loop();
    }

//    ignesci: //pop_audiofile ();
//    ignentra: //push_audiofile ("ECHO");

    if (flag)
            load_situation(sit, true);
    else {
            cam_z = -20000;
            fade (3);
    }

    if (grab_mouse) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    // discard relativa mouse movements up to now
    SDL_GetRelativeMouseState(&mdltx, &mdlty);

    // Ciclo principale.
    bool running = true;
	do {
        running = main_loop();
    } while (running);

    alfin (1);

    cout << "Quitting." << endl;
    return 0;
}

/**
 * The game introduction loop logic, called once per frame.
 * 
 * @return true if the introduction should continue normally,
 * false to end the introduction
 */
bool intro_loop() {
    static const int FOTTY_VIEWPORT_UPPER = HEIGHT*65/200;
    static const int FOTTY_VIEWPORT_LOWER = HEIGHT*135/200;

    //if (!sbp_stat) play (0);
    u32 sync = SDL_GetTicks(); //clock();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_RESIZED:
            break;
            case SDL_QUIT:
                alfin (1);
                exit(0);
            break;
            case SDL_KEYDOWN:
            case SDL_MOUSEBUTTONDOWN:
                return false; // Get out of here.
            default:
                break;
        }
    }

    Txt ("CRYSTAL PIXELS", -77, 0, 100, 3, 4, 270, 0);
    Txt ("WRITTEN BETWEEN 1994 AND 1997", -112, 0, 80, 2, 4, 270, 0);
    Txt ("BY ALESSANDRO GHIGNOLA.", -78, 0, 60, 2, 4, 270, 0);
    Txt (t, (1-(double)strlen(t)) * 6, 0, -60, 3, 4, 270, 0); // microcosm author
    Txt ("MODERN VERSION IN 2013-2022", -104, 0, -84, 2, 4, 270, 0);

    // rotate camera and approach text
    if (beta<360) {
        cam_y += 25;
        beta += 2;
        darken_once();
    }
    // once the text is up close:
    else {
        // place a fottifoh in front of the camera
        c = (c+1)%360;
        i16 tmp = beta; beta = c;
        cam_y += 140;
        Object (0);
        cam_y -= 140;
        beta = tmp;

        u16 time = (SDL_GetTicks()/INTRO_TICKS_PER_FRAME) & 0xFFFF; // WIDTH;
        u32 cx = WIDTH*50;
        do {
            if (time < WIDTH*HEIGHT) {
                // pixel value outside fottifoh row range
                if (time < WIDTH*FOTTY_VIEWPORT_UPPER + 4 ||
                    time >= WIDTH*FOTTY_VIEWPORT_LOWER + 4)
                {
                    video_buffer[time] >>= 1;
                }
            }
            if (cx >= WIDTH*50/2) {
                time +=  WIDTH + 1;
            } else {
                time += WIDTH - 1;
            }
        } while (--cx > 0);

        auto di = &video_buffer[0];
        cx = HEIGHT*WIDTH; // all space
        do {
            // fade out effect
            if (*di != 0) {
                *di -= 1;
            }
            di++;
        } while (--cx > 0);
    }
    Render();

    unsigned long cticks = SDL_GetTicks();
    while (sync + INTRO_TICKS_PER_FRAME > cticks) {
        SDL_Delay(3);
        cticks = SDL_GetTicks();
    }

    return true;
}

/// The main loop logic, called once per frame.
///
/// @return true if the main loop should continue normally,
/// false to end the program gracefully
bool main_loop() {
    bool running = true;
    u32 sync = SDL_GetTicks();

    int i = -1;

        /*
	    if (blink) {
            sync = clock();
        } */
        // Effetti del sistema di virata.
        alpha+=v_alpha;
        if (alpha<0) alpha+=360;
        if (alpha>=360) alpha-=360;
        beta+=v_beta;
        if (beta<0) beta+=360;
        if (beta>=360) beta-=360;

        // Movimento manuale.
        spd_x -= (double)spd * 0.01 * tsin[beta] * tcos[alpha];
        spd_z += (double)spd * 0.01 * tcos[beta] * tcos[alpha];
        spd_y += (double)spd * 0.01 * tsin[alpha];

        // Movimento inerziale.
        cam_x += spd_x;
        cam_y += spd_y;
        cam_z += spd_z;

        rot (); /* GRAVITAZIONE (SPOSTAMENTO PIXELS ROTANTI). */

        dock_effects (); // Legame d'attracco.

        /* Calcolo distanze assolute
            dei pixels dall'osservatore. */
        dists ();

        /* if (EVA_in_progress) {
                rx = pixel_xdisloc[pix] + rel_x - cam_x;
                ry = pixel_ydisloc[pix] + rel_y - cam_y;
                rz = pixel_zdisloc[pix] + rel_z - cam_z;
                pixel_absd[pix] = sqrt (rx*rx+ry*ry+rz*rz);
            } */

        if (!trackframe&&!explode_count) {
            pix = 0;
            for (u16 p = 0; p < pixels ; p++)
                if (pixel_absd[pix]>pixel_absd[p]) {
                    if (carried_pixel>-1 && p==pixels-1) break;
                    pix = p;
                }
        }
        //gap = pixel_absd[pix] / 500 + 5;

        r_x = rel_x;
        r_z = rel_z;

        // Sezione interazione da mouse.
        if (m && !req_end_extra) {

            mpul = 0;
            mouse_input ();

            if (m && grab_mouse) {
                mx += mdltx;
                my += mdlty;
            }

            bool update_angles = true;

            if (!EVA_in_progress) {
                if (trackframe) {
                    my = 0;
                    if (trackframe==23)
                        mx = (360-nav_b) * 5;
                    else {
                        mx -= 2*mdltx;
                        if (mx>1799) mx -= 1800;
                        if (mx<0) mx += 1800;
                        beta = mx / 5;
                    }
                    update_angles = false;
                }
            }

            if (update_angles) {
                mx -= 2*mdltx;
                my -= 2*mdlty;

                while (mx>=1800) mx -= 1800;
                while (mx<0) mx += 1800;
                while (my>=1800) my -= 1800;
                while (my<0) my += 1800;

                beta = mx / 5;
                alpha = my / 5;
            }
        }

        // Mod keys array update
        update_ctrlkeys();

        // Sezione interazione da tastiera.
        //if (fermo_li) goto no_keys;
        bool quit_now = false;

        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent)) {
            switch (sdlevent.type) {
              case SDL_WINDOWEVENT_RESIZED:
                break;
              case SDL_QUIT:
                quit_now = true;
                break;
              case SDL_KEYUP: if (sdlevent.key.state == SDL_RELEASED) {
                i = sdlevent.key.keysym.sym;
                switch (i) {
                    case keymap_thrust:
                        thrust_keyhold = false;
                        break;
                    case keymap_back:
                        back_keyhold = false;
                        break;
                }
                i = 0;
              case SDL_KEYDOWN: if (sdlevent.key.state == SDL_PRESSED) {
                i = sdlevent.key.keysym.sym;
                switch (i) {
/*                  case 0xa1: ob /= 1.05; if (ob<15000) ob = 15000; break;
                    case 0x9d: micro_y -= 0.001; break;
                    case 0x9b: micro_y += 0.001; break;
                    case 0xa0: micro_x += 0.001; break;
                    case 0x98: micro_x -= 0.001; break;
                    case 0x99: ob *= 1.05; break; */
                    case SDLK_RETURN:
                        if (ctrlkeys[0]&16) {
                            toggle_fullscreen();
                            tinte(0);
                        }
                        break;
                    case keymap_up:
                        if ((ctrlkeys[0]&3) || type_mode || fermo_li || m) break;
                        if ((trackframe!=0)&&!EVA_in_progress) break;
                        v_alpha--;
                        if (v_alpha<-3) v_alpha=-3;
                        break;
                    case keymap_down:
                        if ((ctrlkeys[0]&3) || type_mode || fermo_li || m) break;
                        if (trackframe&&!EVA_in_progress) break;
                        v_alpha++;
                        if (v_alpha>3) v_alpha=3;
                        break;
                    case keymap_right:
                        if ((ctrlkeys[0]&3) || type_mode || fermo_li || m) break;
                        if (trackframe&&!EVA_in_progress) break;
                        v_beta--;
                        if (v_beta<-3) v_beta=-3;
                        break;
                    case keymap_left:
                        if ((ctrlkeys[0]&3) || type_mode || fermo_li || m) break;
                        if (trackframe&&!EVA_in_progress) break;
                        v_beta++;
                        if (v_beta>3) v_beta=3;
                        break;
                    case keymap_thrust: //141: // Acc
                        if ((ctrlkeys[0]&3) || type_mode || fermo_li || m) break;
                        thrust_keyhold = true;
                        break;
                    case keymap_back: //145:
                        if ((ctrlkeys[0]&3) || type_mode || fermo_li || m) break;
                        back_keyhold = true;
                        break;
                    case SDLK_F1: // 59:
                        fid_on (); // invert
                        break;
                    case SDLK_F2: // 60:
                        lead_on (); // point at velocity
                        break;
                    case SDLK_F3: // 61:
                        extra_stop_extra (); // leave/enter The Fly
                        break;
                    case SDLK_F4: // 62:
                        undock (); // dock / undock
                        break;
                    case SDLK_F5: // 63:
                        orig_on (); // point towards Sunny
                        break;
                    case SDLK_F6: // 64:
                        snapshot (); // take a snapshot
                        break;
                    case SDLK_F7: // previously scroll lock
                        if (!EVA_in_progress)
                            type_mode = false;
                        else
                            type_mode = !type_mode; // enter text typing mode
                        break;
                    case SDLK_PAGEUP: // pick up or create object
                        if (carry_type==-1) {
                            if (dsol>15000)
                                taking = 1; // pick up an object
                            else {
                                if (_objects<MAX_OBJECTS) // create a new object
                                    carry_type = existent_objecttypes - 1;
                            }
                        }
                        else {
                            if (dsol<15000) {
                                if (carry_type>0) // change the object being created
                                    carry_type--;
                                else // create a new object
                                    carry_type = existent_objecttypes - 1;
                            }
                        }
                        break;
                    case SDLK_PAGEDOWN: // drop the object
                        if (carry_type!=-1) lascia_cadere ();
                        break;
                    case SDLK_e: // Echo toggle
                        // ignore if any shift key is held
                        if (ctrlkeys[0]&3) break;
                        // ignore in text typing mode
                        if (type_mode) break;
                        echo ^= 1;
                        break;
                    case SDLK_x: // The Fly view toggle
                        // ignore if any shift key is held
                        if (ctrlkeys[0]&3) break;
                        // ignore in text typing mode
                        if (type_mode) break;
                        echo ^= 2;
                        break;
                    case SDLK_TAB: /* 9 */ // Toggle climbing
                        // ignore if any shift key is held
                        if (ctrlkeys[0]&3) break;
                        echo ^= 4;
                        break;
                    case SDLK_i: // Information toggle
                        // ignore if any shift key is held
                        if (ctrlkeys[0]&3) break;
                        // ignore in text typing mode
                        if (type_mode) break;
                        echo^=8;
                        break;
                    case SDLK_m: // Keyboard/Mouse control toggle
                        // ignore if any shift key is held
                        if (ctrlkeys[0]&3) break;
                        // ignore in text typing mode
                        if (type_mode) break;

                        mpul = 0;
                        m = 1 - m;
                        v_alpha = 0;
                        v_beta = 0;
                        mx = beta * 5;
                        my = alpha * 5;
                        if (grab_mouse) {
                            SDL_SetRelativeMouseMode(m ? SDL_TRUE : SDL_FALSE);

                            // fetch mouse delta to discard past movement
                            SDL_GetRelativeMouseState(&mdltx, &mdlty);
                        }
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                    }
                    // Text Cursor
                    switch (i) {
                        case SDLK_RIGHT: //77:
                            if (cursore<511) cursore++;
                            break;
                        case SDLK_LEFT: //75:
                            if (cursore>0) cursore--;
                            break;
                        case SDLK_DOWN: //80:
                            if (cursore<480) cursore += 32;
                            break;
                        case SDLK_UP: //72:
                            if (cursore>31) cursore -= 32;
                            break;
                    }

                    // Loading and saving
                    if ((i>=SDLK_a && i<=SDLK_z) || i == SDLK_ASTERISK) {
                        if (ctrlkeys[0]&1) {
                            load_situation(i);
                        } else if (ctrlkeys[0]&2) {
                            save_situation(i);
                        }
                    }
////                }
//                else {
                    /* Text typing something */
                    if (type_mode) {
                        if (i >= SDLK_a && i <= SDLK_z) {
                            i = i + 'A' - SDLK_a;
                        }
                        if (i>=32 && i<=96) {
                            type_this = i;
                        }
                    }
                    /*
                    if (ctrlkeys[0]&64) { // caps lock
                        if (ctrlkeys[0]&3) {
                            if (i>='a'&&i<='z')
                                i -= 32;
                        }
                        else {
                            if (i>='A'&&i<='Z')
                                i += 32;
                        }
                    }
                    */
                    if (dsol<15000&&(i>=SDLK_0 && i<=SDLK_9)) {
                        if (carry_type>-1)
                            carry_type = ((int)i-SDLK_0) * existent_objecttypes / 10;
                        if ((ctrlkeys[0]&64)&&carried_pixel>-1) { // caps lock && is carrying pixel
                            carried_pixel = ((int)i-SDLK_0) * existent_pixeltypes / 10;
                            pixeltype[pixels-1] = carried_pixel;
                        }
                    }
                }
              }
            }
        }
        if (quit_now) return false;
        //keybuffer_cleaner();
//            }
//            else

        // apply key hold mechanics

        if (thrust_keyhold) {
            ispd ();
        }
        if (back_keyhold) {
            dspd ();
        }

//no_keys:
        fermo_li = 0;

        if (mpul==0) { // no button
            rclick = -1;
        }
        if (mpul == 1) { // left mouse button
            ispd ();
        }
        if (mpul == 2) { // right mouse button
            dspd ();
            if (!EVA_in_progress) {
                if (rclick==-1)
                    rclick = SDL_GetTicks();
                else {
                    // keep it pressed for half a second to invert
                    if (SDL_GetTicks()-rclick>TICKS_IN_A_SECOND/2) {
                        fid_on (); // invert
                        i = SDLK_F1; //59;
                    }
                }
            }
            extra_stop_extra ();
            if (trackframe) {
                i = 61;
                _x = mx;
                _y = my;
                // restrict nr of iterations,
                // because sometimes this would lead to an infinite loop
                for (int k = 0; mpul && k < 16; k++) {
                    mpul = 0;
                    mouse_input();
                }
                mx = _x;
                my = _y;
            }
        }

        // pressing both keys
        if (mpul == 3) {
            if (!trackframe||trackframe==23) {
                undock ();
                i = 62;
            }
            if (EVA_in_progress) {
                if (carry_type==-1)
                    taking = 1;
                else {
                    lascia_cadere ();
                }
            }
            _x = mx;
            _y = my;

            // restrict nr of iterations,
            // because sometimes this would lead to an infinite loop
            for (int k = 0; mpul && k < 16; k++) {
                mpul = 0;
                mouse_input();
            }
            mx = _x;
            my = _y;
        }

//    nonpul:

        // calculate speed and distance from Sunny
        double veloc = sqrt(spd_x*spd_x+spd_y*spd_y+spd_z*spd_z);
        dsol = sqrt(cam_x*cam_x+cam_y*cam_y+cam_z*cam_z);

        if (dsol<15000) {
            // adjusted from (63-dsol/240)
            // to account for new sample value range (0..256)
            tinte (static_cast<unsigned char>(255 - dsol / 60.));
            yel = true;
        }
        else {
            if (yel) {
                // reset to no yellow if last game frame had some yellow
                tinte (0);
                yel = false;
            }
        }

        // Ridimensionamento angolazione verticale in "extra attivit".
        if (EVA_in_progress && alpha>90 && alpha<270) {
            v_alpha = 0;
            if (alpha>180)
                alpha = 270;
            else
                alpha = 90;
            my = alpha * 5;
        }

        if (reset_trackframe) {
            tracking = 0;
            trackframe--; spd_y -= 0.1;
            reset_trackframe = trackframe;
        }

        // FID (freno inerziale diamagnetico).
        // Non pi cos facile: ora c' lo SPIN.
        if (fid||lead||orig) {
            v_alpha = alpha90 - alpha;
            if (v_alpha>5) v_alpha = 5;
            if (v_alpha<-5) v_alpha = -5;
            alpha += v_alpha;
            v_beta = beta90 - beta;
            if (v_beta>5) v_beta = 5;
            if (v_beta<-5) v_beta = -5;
            beta += v_beta;
            if (alpha==alpha90 && beta==beta90) {
                m = comera_m;
                mx = beta * 5;
                my = alpha * 5;
                if (orig && carried_pixel>-1) {
                    carried_pixel--;
                    if (carried_pixel<0) carried_pixel = existent_pixeltypes - 1;
                    pixeltype[pixels-1] = carried_pixel;
                }
                fid = 0;
                lead = 0;
                orig = 0;
                play (TARGET);
            }
            v_alpha = 0;
            v_beta = 0;
        }

        obj = -1;

        if (EVA_in_progress) {
            if (fabs(disl)>1)
                rel_y += disl / 3;
            else
                rel_y += disl;
            cam_xt = dsx + rel_x;
            cam_yt = dsy + rel_y;
            cam_zt = dsz + rel_z;
            CollyZone (pix, 0); // Controllo collisioni.
            cam_x = dsx + rel_x;
            cam_y = dsy + rel_y;
            cam_z = dsz + rel_z;
        }

        // Sezione ridisegno spazio illusorio.
        /// Create hallucinating effect.
        if (ctrlkeys[0]&32) { // ctrlkeys[0]&32 is right alt
            darken_once();
                        /*
                        asm {   pusha
                                les di, dword ptr adapted
                                mov cx, 64000 }
                chics:  asm {   cmp byte ptr es:[di], 0
                                je zeros
                                dec byte ptr es:[di] }
                zeros:  asm {   inc di
                                dec cx
                                jnz chics
                                popa }
                        */
        } else {
            rx = pixel_xdisloc[pix] - cam_x;
            ry = pixel_ydisloc[pix] - cam_y;
            rz = pixel_zdisloc[pix] - cam_z;
            d = sqrt (rx*rx + ry*ry + rz*rz);
            if (subsignal[9*pixeltype[pix]]&&d<628) {
                char dist[20];

                // backdrop
                strcpy (dist, &subsignal[9*pixeltype[pix]]);
                strcat (dist, ".ATM");
                FILE* file = std::fopen(dist, "rb");
                if (file) {
                    // TODO: do the following calculations take the original resolution for granted?
                    // does changing the resolution also change the behaviour of the following code?
                    // -- draw operation begin
                    auto ax = 360u;
                    ax -= beta;
                    auto dx = 32u;
                    ax = ax * dx; // 360*32
                    dx = 0;
                    auto cx = 36u;
                    ax = ax / cx; // 360*32/36 = 320
                    ax <<= 2; // 320 * 4 = 1280
                    dx = 0;
                    cx = WIDTH;
                    auto si = ax % cx; // si = 1280 % WIDTH
                    ax = alpha;
                    cx = 360;
                    dx = 0;
                    ax = ax % cx; // alpha % 360
                    dx = 3;
                    ax = ax * dx; // (alpha % 360) * 3
                    dx = 0;
                    cx = WIDTH;
                    ax = ax * cx; // ax = (alpha % 360) * 3 * WIDTH
                    cx = ax >> 16; // cx = ax >> 16
                    dx = ax;
                    dx += si;
                    std::fseek(file, dx, SEEK_SET);
                    auto p_data = &video_buffer[0];
                    si = 8;
                    do {
                        cx = WIDTH*HEIGHT / 8;
                        std::fread(p_data, 1, cx, file);
                        p_data += WIDTH*HEIGHT / 8;
                    } while (--si != 0);
                    std::fclose (file);
                    // -- draw operation end

                    if (d>500) {
                        a = (d - 500) / 4;

                        // -- draw operation begin
                        auto si = &video_buffer[0];
                        unsigned int cx = WIDTH*HEIGHT;
                        do
                        {
                            if (*si < (a&0xFF)) {
                                *si -= a&0xFF;
                                *si = (a>>8)&0xFF;
                            }
                            else *si -= a&0xFF;
                            si++;
                        } while (--cx != 0);
                        // -- draw operation end
                        }
                    } else {
                        pclear (&video_buffer[0], 0);
                    }
            }
            else {
                pclear (&video_buffer[0], 0);
            }
        }

        // Disegno oggetto prelevato.

        if (carry_type!=-1) {
            id = 0;
            if (trackframe==23) {
                cox += pixel_xdisloc[pix] - prevpixx;
                coz += pixel_zdisloc[pix] - prevpixz;
            }

            coy += ((cam_y + 9 * tsin[alpha]) - coy) / justloaded;
            cox += ((cam_x - 9 * tsin[beta] * tcos[alpha]) - cox) / justloaded;
            coz += ((cam_z + 9 * tcos[beta] * tcos[alpha]) - coz) / justloaded;
            if (trackframe==23&&!memcmp(&subsignal[9*(carry_type+FRONTIER_M3)], "MAGNIFY" /*"VISORE"*/, 7/*6*/)) {
                cam_x = cox;
                cam_y = coy;
                cam_z = coz;
            }
            else {
                ox = cox; oy = coy; oz = coz;
                Object (carry_type);
            }
        }

        // Gestione pixels "donati" dal Solicchio.
        if (dsol<500)
            if (pixels<MAX_PIXELS && carried_pixel==-1) {
                carried_pixel = existent_pixeltypes - 1;
                pixeltype[pixels] = carried_pixel;
                pixel_support[pixels] = 0;
                pixel_rot[pixels] = 0;
                pixels++;
            }

        if (carried_pixel>-1) {
            pixel_ydisloc[pixels-1] = cam_y + 1000 * tsin[alpha];
            pixel_xdisloc[pixels-1] = cam_x - 1000 * tsin[beta] * tcos[alpha];
            pixel_zdisloc[pixels-1] = cam_z + 1000 * tcos[beta] * tcos[alpha];
            if (veloc<2 && dsol>500&&!trackframe) carried_pixel = -1;
        }
        auto tmpticks = SDL_GetTicks();
        for (a=static_cast<int>(tmpticks%18); a<180+static_cast<int>(tmpticks%18); a+=18) // "Sole" centrale (il Solicchio).
            for (b=static_cast<int>(tmpticks%18); b<180+static_cast<int>(tmpticks%18); b+=18) {
                ox = random (14000) + 1000;
                oz = ox * tcos[b];
                z2 = ox * tsin[b];
                ox = z2 * tcos[a];
                oy = z2 * tsin[a];
                Line3D (-ox, -oz, oy, ox, oz, -oy);
                ox = random (7000) + 500;
                oz = ox * tcos[b];
                z2 = ox * tsin[b];
                ox = z2 * tcos[a];
                oy = z2 * tsin[a];
                Line3D (-ox, oy, -oz, ox, -oy, oz);
            }

        vicini = 0;

        for (u16 p = 0; p < pixels; p++) { // Disegna tutti i pixels.
            nopix = p;
            Pixel (pixeltype[p]);
            Oggetti_sul_Pixel (0);
        }

        if (globalvocfile[0]!='.') {
            nopix = pixel_sonante;
            Oggetti_sul_Pixel (1);
        }

        if (!vicini && globalvocfile[0]!='.') chiudi_filedriver ();

        if (trackframe==23)
            justloaded = 5;
        else
            justloaded = 1;

        for (u16 o=0; o<_objects; o++) { // Per gli oggetti vagabondi.
            if (object_location[o]==-1) {
                ox = absolute_x[o];
                oy = absolute_y[o];
                oz = absolute_z[o];
                xrel (0, 0, 0);
                kk = sqrt (ox*ox+oy*oy+oz*oz);
                if (kk<15000||kk>3.3333333E8) {
                    /* Assorbiti dal Solicchio o usciti dal
                    campo di definizione (10000 Km.) */
                    scoppia (o, 10000, 1);
                    o--;
                }
                else {
                    rx = ox - cam_x;
                    ry = oy - cam_y;
                    rz = oz - cam_z;
                    kk = sqrt (rx*rx+ry*ry+rz*rz);
                    //if (kk<pixel_absd[pix]) gap = kk / 500 + 5;
                    if (kk<1000) Object (objecttype[o]);
                    absolute_x[o] += relative_x[o];
                    absolute_y[o] += relative_y[o];
                    absolute_z[o] += relative_z[o];
                }
            }
        }

/*              if (sc) {
                        rx = (double)alpha*Deg + micro_x;
                        ry = (double)beta*Deg + micro_y;
                        cam_y -= sc * sin(rx);
                        cam_x += sc * sin(ry) * cos(rx);
                        cam_z -= sc * cos(ry) * cos(rx);
                        sc = 0;
                } */

        // handle object pick up request while flying
        if (taking&&!EVA_in_progress) {
            kk = 500;
            obj = -1;
            for (u16 o=_objects-1; o>=0; o--) {
                if (object_location[o]==-1) {
                    _x = absolute_x[o] - cam_x;
                    _y = absolute_y[o] - cam_y;
                    _z = absolute_z[o] - cam_z;
                    d = sqrt (_x*_x+_y*_y+_z*_z);
                    if (d<kk) {
                        kk = d;
                        obj = o;
                    }
                }
            }
        }

        if (taking && obj>-1) {
            coy = pixel_ydisloc[pix] + relative_y[obj] - object_elevation[objecttype[obj]];
            cox = pixel_xdisloc[pix] + relative_x[obj];
            coz = pixel_zdisloc[pix] + relative_z[obj];
            preleva_oggetto (obj);
        }
        taking = 0;

        // Disegno navicella e sua gestione.
   // Disegno navicella e sua gestione.
    if (!EVA_in_progress) {
        nav_x = cam_x;
        nav_y = cam_y;
        nav_z = cam_z;
        nav_a = 360 - alpha;
        if (nav_a==360) nav_a=0;
        nav_b = 360 - beta;
        if (nav_b==360) nav_b=0;
    }
    else {
        nav_x = pixel_xdisloc[pix] + docksite_x[pixeltype[pix]];
        nav_z = pixel_zdisloc[pix] + docksite_z[pixeltype[pix]];
        nav_y = pixel_ydisloc[pix] + docksite_y[pixeltype[pix]] - 16;
    }

    if (req_end_extra) {
        if (req_end_extra<50) req_end_extra++;
        if (alpha>179) alpha-=360;
        alpha = (double) alpha / 1.1;
        if (alpha<0) alpha+=360;
        auto p = ((360-nav_b) - beta) / 4;
        if (p)
            beta += (double) p;
        else
            beta = 360 - nav_b;
        //if (beta<0) beta+=360;
        //if (beta>359) beta-=360;
        v_alpha = 0;
        v_beta = 0;
        rel_x /= 1.25; rel_z /= 1.25;
        if (alpha==0 && beta==360-nav_b
                &&(rel_x<1 && rel_x>-1)
                &&(rel_z<1 && rel_z>-1)
                &&req_end_extra>=24) {
            play (READY);
            subs = 0;
            req_end_extra = 0;
            EVA_in_progress = 0;
        }
    }

    if (tracking)
        trackframe += tracking;

    // draw direction indicators if enabled
    if (!(echo&8) && !trackframe) {
        draw_indicator_crosshair();
        // why do we need to run this code twice?
        for (c=0; c<2; c++) {
            // prograde indicator
            if (C32(nav_x+spd_x, nav_y+spd_y, nav_z+spd_z)) {
                draw_indicator_prograde(share_x, share_y);
            }
            else {
                // retrograde indicator
                if (C32(nav_x-spd_x, nav_y-spd_y, nav_z-spd_z)) {
                   draw_indicator_retrograde(share_x, share_y);
                }
            }
            // closest pixel indicator
            if (pixel_absd[pix]>8000) {
                if (C32(pixel_xdisloc[pix], pixel_ydisloc[pix], pixel_zdisloc[pix])) {
                    draw_indicator_closest(share_x, share_y);
                }
            }
        }
    }
    
    // draw navigation HUD if enabled
    if (!(echo&2)) {

        if (!EVA_in_progress) {
            tsinx += 361;
            tcosx += 361;
            tsiny += 361;
            tcosy += 361;
        }

        draw_vehicle_interior();

        if (EVA_in_progress) {
            draw_vehicle_exterior();
        }

        // Sezione ridisegno pannello di controllo informativo.
        ox = nav_x; oy = nav_y; oz = nav_z;

        draw_readings_force(spd);

        if (!trackframe) {
            draw_readings_speed(veloc);
        }
        else draw_readings_docked();

        draw_readings_heading(nav_a, nav_b);
        draw_readings_position(cam_x, cam_y, cam_z);

        kk = pixel_absd[pix]/33.33333333;
        draw_readings_closest(kk);

        // Disegno tasti funzione, interruttori e/m, ecc...
        if (!trackframe) {
           draw_vehicle_attitude(nav_a, nav_b, blink);
        }

        int bki0 = bki;
        int bki1 = bki;
        int bki2 = bki;
        int bki3 = bki;
        int bki4 = bki;

        if (!trackframe && !EVA_in_progress && i==58) bki0 = i;
        if (!EVA_in_progress && i==59 && !trackframe) bki1 = i;
        if (trackframe && i==60) bki2 = i;
        if (trackframe && !EVA_in_progress && i==61) bki3 = i;
        if (orig && i==62) bki4 = i;

        draw_console_key ("SPIN", -6.0, 58, i, i, bki0);
        draw_console_key ("LEAD", -4.9, 59, i, i, bki1);
        draw_console_key ("EXTR", -3.8, 60, i, i, bki2);
        draw_console_key ("DOCK", -2.7, 61, i, i, bki3);
        draw_console_key ("ORIG", -1.6, 62, i, i, bki4);

        draw_console_key ("ECHO", 5.0, echo&1, 1, echo, bkecho);

        // end drawing the fly

        bki = i;
        bkecho = echo;

        if (!EVA_in_progress) {
            tsinx -= 361;
            tcosx -= 361;
            tsiny -= 361;
            tcosy -= 361;
        }
    }
        // Esplosioni: Ragassi, esplodete gli oggetti... per favore!
            if (!explode_count) {
                if (!trackframe) {
                    for (u16 o=0; o<_objects; o++) {
                        if (object_location[o]==-1&&!memcmp(&subsignal[9*(objecttype[o]+FRONTIER_M3)], "BOMB", 4)) {
                            rx = pixel_xdisloc[pix] - absolute_x[o];
                            ry = pixel_ydisloc[pix] - absolute_y[o];
                            rz = pixel_zdisloc[pix] - absolute_z[o];
                            if (sqrt(rx*rx+ry*ry+rz*rz)<500) {
                                explode_count = 100;
                                scoppia (o, 5000, 1);
                                break;
                            }
                        }
                    }
                }
            }
            else {
                explode_count--;
                if (explode_count == 60) {
                    play (DESTROY);
                    subs = 0;
                }
                if (!explode_count) {
                    explode = 0;
                    if (pixel_sonante==pix)
                        chiudi_filedriver ();
                    else {
                        if (pixel_sonante>pix)
                            pixel_sonante--;
                    }
                    for (u16 o=0; o<_objects; o++) {
                        if (object_location[o]==pix) {
                            absolute_x[o] = pixel_xdisloc[pix] + relative_x[o];
                            absolute_y[o] = pixel_ydisloc[pix] + relative_y[o];
                            absolute_z[o] = pixel_zdisloc[pix] + relative_z[o];
                            //adapted = adaptor;
                            scoppia (o, (double)(random(10)+10)*object_elevation[objecttype[o]] + 50, 5);
                            // perform a frame skipping of some sort
//                    asm {   pusha
//                            xor ah, ah
//                            int 0x1a
//                            mov si, dx }
                            auto si = SDL_GetTicks();
//            ky: asm {       xor ah, ah
//                            int 0x1a
//                            cmp si, dx
//                            je ky
                            while (si + TICKS_PER_FRAME > SDL_GetTicks());
//                            popa }
                            //adapted = fake_adaptor;
                            //pcopy (adaptor, adapted);
                            o--;
                        }
                    }
                    for (u16 o=0; o<_objects; o++) {
                        if (object_location[o]>pix)
                            object_location[o]--;
                    }
                    for (c=pix; c<pixels; c++) {
                        pixeltype[c] = pixeltype[c+1];
                        pixel_rot[c] = pixel_rot[c+1];
                        pixel_absd[c] = pixel_absd[c+1];
                        pixel_support[c] = pixel_support[c+1];
                        pixel_xdisloc[c] = pixel_xdisloc[c+1];
                        pixel_ydisloc[c] = pixel_ydisloc[c+1];
                        pixel_zdisloc[c] = pixel_zdisloc[c+1];
                    }
                    pixels--;
                }
            }

            // Cosette finali.
            /* Sound Off for now
            if (!sbp_stat&&!sbf_stat) { // Gestione audio in sottofondo...
                subs = 1;
                if (globalvocfile[0]!='.') {
                    chiudi_filedriver ();
                    for (o=0; o<_objects; o++) {
                        if (object_location[o]==pixel_sonante)
                            if (absolute_y[o]==11E11)
                                absolute_y[o] = 0;
                    }
                    // Per poter
                    // riprodurre registrazioni
                    // in modo ciclico.
                    nopix = pixel_sonante;
                    // Per poter
                    // rogrammare
                    // sequenze audio.
                    Oggetti_sul_Pixel (1);
                }
            }
            */

            /* Sound stuff
            if (!EVA_in_progress&&!trackframe) { // Segnale dell'ecoscandaglio.
                if (SDL_GetTicks()-stso>=gap) {
                    stso = SDL_GetTicks();
                    if (subs&&(echo&1)) play (SOTTOFONDO);
                }
            }
            */

            if (moving_last_object) { // "Adagia" l'oggetto lasciato.
                _x = cfx - relative_x[_objects-1];
                _y = cfy - relative_y[_objects-1];
                _z = cfz - relative_z[_objects-1];
                relative_x[_objects-1] += _x / 5;
                relative_y[_objects-1] += _y / 5;
                relative_z[_objects-1] += _z / 5;
                if (sqrt(_x*_x+_y*_y+_z*_z)<0.1) {
                    moving_last_object = 0;
                    relative_x[_objects-1] = cfx;
                    relative_y[_objects-1] = cfy;
                    relative_z[_objects-1] = cfz;
                    if (globalvocfile[0]=='.') play (LASCIARE);
                }
            }

            Render();

            blink = 1 - blink; // Lampeggo della spia "volo rovesciato".

            unsigned long cticks = SDL_GetTicks();
            while (sync + TICKS_PER_FRAME > cticks) {
                SDL_Delay(3);
                cticks = SDL_GetTicks();
            }
            //while (blink && (clock()==sync)); // Sincronizzatore (max 37 fps.)

            //cout << " " << (TICKS_IN_A_SECOND / (cticks-sync)) << " fps" << endl;
            //cout << "\t\t" << (cticks-sync) << "\t\t\r"; cout.flush();
    return running;
}

inline void init_start()
{
    int r;
    auto mem_ok = allocation_farm();
    if (!mem_ok) {
        cerr << "Not enough memory (seriously!?)" << endl;
        throw 1;
    }

	// Init SDL
	r = SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO);
	if (r < 0)
	{	cerr << "Failed to init SDL: " << SDL_GetError() << endl;
		throw 2;
    }

    atexit(SDL_Quit);
}

void read_args(int argc, char** argv, char& flag, char& sit)
{
    flag = 0;
    char dist[20]; // Stringhe usate per conversioni.
    if (argc>2) {
        pixels = 0;
        if (strcasecmp(argv[2], "PIXELS") == 0) {
            pixels = (int)atof(argv[1]);
            if (pixels>MAX_PIXELS) pixels = MAX_PIXELS;
        }
        if (argc > 4 && strcasecmp(argv[4], "OBJECTS") == 0) {
            objects = (int)atof(argv[3]);
            if (objects>MAX_OBJECTS) objects = MAX_OBJECTS;
        }
        if (objects<=0||pixels<=0) {
            cerr << "Usage: CRYXTELS ([<n> PIXELS] [<n> OBJECTS] | <gamesave>)" << endl;
            throw 1;
        }
    }
    else if (argc == 2) {
        sit = argv[1][0];
        if (sit >= 'a' && sit <= 'z') {
            sit -= 'a' - 'A';
        }

        sprintf (dist, "CRYXTELS.%cIT", sit);
        FILE* fh = std::fopen(dist, "rb");
        if (fh) {
            std::fclose (fh);
            flag = 1;
        } else {
            cerr << "Could not load game: game save \"" << dist << "\" is missing." << endl;
            throw 1;
        }
    }
    else {
        // Use default pixel and object numbers
        pixels = 250;
        objects = 250;
    }
}


void fail_pixel_def (int el, int pix)
{
    alfin (0);
    cerr << "Pixel definition error: Pixel of type " << pix << " does not specify element nr. " << (el+1) << endl
         << "0-valued parameters are invalid." << endl;
    exit (1);
}

template <class T>
T* x_farmalloc (unsigned long nelems)
{
    T * fp = new T[nelems];
    if (!fp) {
        throw 0;
    }

    memset (fp, 0, nelems*sizeof(T));

    return fp;
}

inline bool allocation_farm()
{
    try {
    pixel_elem_b = x_farmalloc<char>(40*ELEMS*BUFFERS);

    pixeltype = x_farmalloc<PixelTypeId> (MAX_PIXELS);
    pixelmass = x_farmalloc<float> (MAX_PIXEL_TYPES); // 2600
    pixel_rot = x_farmalloc<u8>(MAX_PIXELS * 2);

    pixel_absd = x_farmalloc<float>(MAX_PIXELS);
    pixel_support = x_farmalloc<double> (MAX_PIXELS);
    pixel_xdisloc = x_farmalloc<double> (MAX_PIXELS);
    pixel_ydisloc = x_farmalloc<double> (MAX_PIXELS);
    pixel_zdisloc = x_farmalloc<double> (MAX_PIXELS);

    objecttype = x_farmalloc<i16> (MAX_OBJECTS);
    relative_x = x_farmalloc<double> (MAX_OBJECTS);
    relative_y = x_farmalloc<double> (MAX_OBJECTS);
    relative_z = x_farmalloc<double> (MAX_OBJECTS);
    absolute_x = x_farmalloc<double> (MAX_OBJECTS);
    absolute_y = x_farmalloc<double> (MAX_OBJECTS);
    absolute_z = x_farmalloc<double> (MAX_OBJECTS);
    object_location = x_farmalloc<i16> (MAX_OBJECTS);

    pixeltype_elements = x_farmalloc<u8>(BUFFERS);
    pixeltype_type = x_farmalloc<i16> (BUFFERS);

    pixel_elem_t = x_farmalloc<u8> (ELEMS*BUFFERS);
    pixel_elem_x = x_farmalloc<double> (ELEMS*BUFFERS);
    pixel_elem_y = x_farmalloc<double> (ELEMS*BUFFERS);
    pixel_elem_z = x_farmalloc<double> (ELEMS*BUFFERS);
    pixel_elem_1 = x_farmalloc<float> (ELEMS*BUFFERS);
    pixel_elem_2 = x_farmalloc<float> (ELEMS*BUFFERS);
    pixel_elem_3 = x_farmalloc<float> (ELEMS*BUFFERS);
    pixel_elem_4 = x_farmalloc<float> (ELEMS*BUFFERS);

    // Pixel type data
    docksite_x = x_farmalloc<float>(MAX_PIXEL_TYPES);
    docksite_y = x_farmalloc<float>(MAX_PIXEL_TYPES);
    docksite_z = x_farmalloc<float>(MAX_PIXEL_TYPES);
    docksite_w = x_farmalloc<float>(MAX_PIXEL_TYPES);
    docksite_h = x_farmalloc<float>(MAX_PIXEL_TYPES);
    subsignal = x_farmalloc<char> (9 * MAX_PIXEL_TYPES);

    buffer = x_farmalloc<u8> (1057);
    } catch (const int&) {
        return false;
    }
    return true;
}

void tinte (unsigned char satu)
{
    constexpr unsigned char K = 255;
    constexpr unsigned int GRAD_COUNT_1 = 16 * 3;
    constexpr unsigned int GRAD_COUNT_2 = 16 * 3;

    constexpr float F1 = 256.f / GRAD_COUNT_1;
    constexpr float F2 = 256.f / GRAD_COUNT_2;

    unsigned int i;
    for (i=0; i < 768; i++) {
        buffer[i] = K;
    }
    for (i=0; i < GRAD_COUNT_1; i += 3) {
        buffer[i] = buffer[i + 1] = satu;
        buffer[i + 2] = static_cast<float>(i) * F1;
    }
    for (i=0; i < GRAD_COUNT_2; i += 3) {
        unsigned int v = static_cast<float>(i) * F2 + satu;
        if (v > K) v = K;
        buffer[i + GRAD_COUNT_1] = v;
        buffer[i + GRAD_COUNT_1 + 1] = v;
    }
    tavola_colori (buffer, 0, 256, 63, 63, 63);
}

/// redefinition of random(int),
/// which probably became deprecated.
/// Generates a random number between 0 and max_num-1
int random(int max_num) {
    int result, hi_num=max_num;
    result = (rand()%(hi_num));
    assert(result >= 0 && result < max_num);
    return result;
}

void build_cosm(char& flag)
{
    /* Costruisce il microcosmo, assegnando una posizione, casuale ma costante,
        ad ogni pixel e ad ogni oggetto. Inoltre, vengono scelte le
        forme di ogni corpo del microcosmo. */

    for (u32 p = 0; p < MAX_PIXELS; p++) {
            //pixelmass[p] = 0;
            pixeltype[p] = p % existent_pixeltypes;
            pixel_xdisloc[p] = (double)(random(10000) - random(10000)) * 150.0;
            pixel_ydisloc[p] = (double)(random(1000) - 500) * 150.0;
            //pixel_support[p] = (double)(random(1000)) / 200;
            pixel_zdisloc[p] = (double)(random(10000) - random(10000)) * 150.0;
            //pixel_absd[p] = 0;
            pixel_rot[p] = 0;
    }

    //ctk = ctrlkeys[0];

    cout << "Loading pixels..." << endl;
    for (u16 p=0; p<existent_pixeltypes; p++) {
        cout << "Percentage complete: " << 100 * p / existent_pixeltypes << "\r";
        loaded_pixeltypes = 0; LoadPtyp (p);
    }
    cout << "Percentage complete: 100";
    cout << "\n\nLoading objects..." << endl;
    pixelmass[FRONTIER_M3] = 100; // Massa del fottifoh.
    pixelmass[FRONTIER_M2] = 100; // del motore orbitale.
    pixelmass[FRONTIER_M1] = 100; // del lettore di cd.
    for (u16 p = 3; p < existent_objecttypes; p++) {
        cout << "Percentage complete: " << 100* p / existent_objecttypes << "\r";
        loaded_pixeltypes = 0; LoadPtyp (p+FRONTIER_M3);
    }
    cout << "Percentage complete: 100\n" << endl;

    if (!flag) {
        //ctrlkeys[0] = 0;
        dropping = 1;
        EVA_in_progress = 1;
        _objects = 0;
        cout << "Object positioning taking place." << endl;
        for (int o=0; o<objects; o++) {
            cout << "Percentage complete: "
                << (100*o/objects) << "\r";
            object_location[o] = random (pixels);
            loaded_pixeltypes = 0;
            LoadPtyp (pixeltype[object_location[o]]);
            objecttype[o] = o % existent_objecttypes;
            relative_x[o] = random (docksite_w[pixeltype[object_location[o]]])
                    - random (docksite_w[pixeltype[object_location[o]]]);
            if (docksite_h[pixeltype[object_location[o]]]>=0) {
                relative_z[o] = random (docksite_h[pixeltype[object_location[o]]])
                        - random (docksite_h[pixeltype[object_location[o]]]);
            } else {
                relative_x[o] /= 1.4142;
                relative_z[o] = random (docksite_w[pixeltype[object_location[o]]])
                        - random (docksite_w[pixeltype[object_location[o]]]);
                relative_z[o] /= 1.4142;
            }
            rel_y = 0;
            rel_x = relative_x[o];
            rel_z = relative_z[o];
            CollyZone (object_location[o], 1);
            if (onblock==2) {
                o--;
                continue;
            }
            if (objecttype[o]==1) pixel_rot[object_location[o]]++;
            if (!onblock)
                relative_y[o] = docksite_y[pixeltype[object_location[o]]];
            else
                relative_y[o] = rel_y;
            _objects++;
        }
        cout << "Percentage complete: 100" << endl;

        dropping = 0;
        EVA_in_progress = 0;
    }

}

void rot ()
{
    // Assegnazione posizioni a pixels gravitanti.

    prevpixx = pixel_xdisloc[pix];
    prevpixz = pixel_zdisloc[pix];

    for (c=0; c<pixels; c++) {
        if (pixel_rot[c]) {
            _ox = pixel_xdisloc[c];
            _oz = pixel_zdisloc[c];
            rx = sqrt (_ox*_ox + _oz*_oz);
            pixel_support[c] -= (double) pixel_rot[c] * (50.85 / rx);
            pixel_xdisloc[c] = rx * cos (pixel_support[c]);
            pixel_zdisloc[c] = rx * sin (pixel_support[c]);
            _x = pixel_xdisloc[c] - _ox;
            _z = pixel_zdisloc[c] - _oz;
            if (sqrt(_x*_x+_z*_z)>rx/2) {
                pixel_support[c] += Pi;
                pixel_xdisloc[c] = rx * cos (pixel_support[c]);
                pixel_zdisloc[c] = rx * sin (pixel_support[c]);
            }
        }
    }
}

/// enter a synchronous routine
/// to fade out the screen
/// (can be skipped by pressing any key)
void fade (unsigned char speed) {
    keybuffer_cleaner ();
    unsigned int dx = 0;
    auto skip = false;
    do {
        unsigned int sync = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN
                || event.type == SDL_MOUSEBUTTONDOWN) {
                skip = true;
            }
        }

        darken_once(speed);
        Render();
        unsigned long cticks = SDL_GetTicks();
        while (sync + TICKS_PER_FRAME > cticks) {
            SDL_Delay(3);
            cticks = SDL_GetTicks();
        }
    }
    while(!skip && dx++ < (70 / speed));
/*
rip:    mpul = 0; mouse_input ();
        _BL = tasto_premuto ();
        asm {   les di, dword ptr adaptor
                mov cx, 64000
                xor dx, dx }
__chic: asm {   cmp byte ptr es:[di], 0
                je __zero
                dec byte ptr es:[di]
                inc dx }
__zero: asm {   inc di
                dec cx
                jnz __chic
                cmp bl, 0
                jne halt
                cmp mpul, 0
                jne halt
                cmp dx, 100
                jnb rip }
halt:   keybuffer_cleaner ();
*/
}

void load_situation(char i, bool skip_fade) {
    if (i >= 'a' && i <= 'z') {
        i -= 'a' - 'A';
    }
    try {
        load_game(i);
        cout << "Game [" << i << "] successfully loaded." << endl;
        if (!skip_fade) {
            fade (4);
        }
        rot ();
        dists ();
        dock_effects ();
        //sta_suonando = -1;
        //pixel_sonante = -1;
        //globalvocfile[0] = '.';
        mx = beta * 5; my = alpha * 5;
        r_x = rel_x; r_y = rel_y; r_z = rel_z;

        pclear (&video_buffer[0], 0);
        //for (c = 0; c<objects; c++)
                //if (absolute_y[c]>=10E10) absolute_y[c]=0;
        justloaded = 1;
    } catch (int e) {
        cerr << "Failed to read game from \"CRYXTELS." << i << "IT\": " << strerror(e) << endl;
    }
}

void save_situation(char i) {
    if (i >= 'a' && i <= 'z') {
        i -= 'a' - 'A';
    }
    try {
    save_game(i);
        cout << "Game [" << i << "] successfully saved." << endl;
    } catch (int e) {
        cerr << "Failed to save game to \"CRYXTELS." << i << "\": " << strerror(e) << endl;
    }
}

void cerca_e_carica (PixelTypeId typ) {
    if (loaded_pixeltypes) {
        for (auto iii = 0u; iii<loaded_pixeltypes; iii++) {
            if (typ==pixeltype_type[iii])
                return;
        }
    }
    LoadPtyp (typ);
}

void Aggiornamento ()
{
    int p, o, f;

    loaded_pixeltypes = 0;

    for (p=0; p<pixels; p++) {
        if (pixel_absd[p]<pixelmass[pixeltype[p]])
            cerca_e_carica (pixeltype[p]);
    }

    for (o=0; o<_objects; o++) {
        if (objecttype[o]>=3) {
                f = 0;
                if (object_location[o]>-1) {
                        if (pixel_absd[object_location[o]]<4000)
                            f = 1;
                }
                else {
                    rx = absolute_x[o] - cam_x;
                        ry = absolute_y[o] - cam_y;
                        rz = absolute_z[o] - cam_z;
                        if (sqrt(rx*rx+ry*ry+rz*rz)<1000) f = 1;
                        }
                        if (f) cerca_e_carica (objecttype[o]+FRONTIER_M3);
                }
        }

        if (carry_type>=3)
                cerca_e_carica (carry_type+FRONTIER_M3);
}

void dists ()
{
    int p;

    double related_x; // Usate nel calcolo delle distanze dei pixels.
    double related_y;
    double related_z;

    for (p=0; p<pixels; p++) {
            related_x = pixel_xdisloc[p] - cam_x;
            related_y = pixel_ydisloc[p] - cam_y;
            related_z = pixel_zdisloc[p] - cam_z;
            pixel_absd[p] = sqrt(related_x*related_x+
                                related_y*related_y+
                                related_z*related_z);
    }
}

/// When flying, increase thrusting.
/// When in a pixel, walk forward.
void ispd ()
{

    if (trackframe&&!EVA_in_progress) return;
    if (!EVA_in_progress && trackframe<23) {
        //spd = 2*spd + 1;
        spd = 1.5*spd + 0.5;
        if (spd>300) spd = 300;
    } else {
        _x = rel_x;
        _z = rel_z;
        int acount = (SDL_GetTicks() % 520) / 4 - 80;
        // Sound off
        //if (acount<0&&!sbp_stat) play (PASSO);
        rel_y += acount / 300.0;
        rel_x -= 4 * tsin[beta] * tcos[alpha];
        rel_z += 4 * tcos[beta] * tcos[alpha];
        if (docksite_h[pixeltype[pix]]>=0) {
            if (rel_x>docksite_w[pixeltype[pix]]-docksite_x[pixeltype[pix]]) rel_x = docksite_w[pixeltype[pix]]-docksite_x[pixeltype[pix]];
            if (rel_x<-docksite_x[pixeltype[pix]]-docksite_w[pixeltype[pix]]) rel_x = -docksite_x[pixeltype[pix]]-docksite_w[pixeltype[pix]];
            if (rel_z>docksite_h[pixeltype[pix]]-docksite_z[pixeltype[pix]]) rel_z = docksite_h[pixeltype[pix]]-docksite_z[pixeltype[pix]];
            if (rel_z<-docksite_z[pixeltype[pix]]-docksite_h[pixeltype[pix]]) rel_z = -docksite_z[pixeltype[pix]]-docksite_h[pixeltype[pix]];
        }
        else {
            rel_x += docksite_x[pixeltype[pix]];
            rel_z += docksite_z[pixeltype[pix]];
            if (sqrt(rel_x*rel_x+rel_z*rel_z)>docksite_w[pixeltype[pix]]) {
                rel_x = _x;
                rel_z = _z;
            }
            else {
                rel_x -= docksite_x[pixeltype[pix]];
                rel_z -= docksite_z[pixeltype[pix]];
            }
        }
    }
}

/// When flying, stop thrusting.
/// When in a pixel, walk backward.
void dspd ()
{
        if (trackframe&&!EVA_in_progress) return;
        if (!EVA_in_progress && trackframe<23)
                spd = 0;
        else {
                // Sound off
                //if (!sbp_stat) play (PASSO);
                _x = rel_x;
                _z = rel_z;
                rel_x += 4 * tsin[beta] * tcos[alpha];
                rel_z -= 4 * tcos[beta] * tcos[alpha];
                if (docksite_h[pixeltype[pix]]>=0) {
                        if (rel_x>docksite_w[pixeltype[pix]]-docksite_x[pixeltype[pix]]) rel_x = docksite_w[pixeltype[pix]]-docksite_x[pixeltype[pix]];
                        if (rel_x<-docksite_x[pixeltype[pix]]-docksite_w[pixeltype[pix]]) rel_x = -docksite_x[pixeltype[pix]]-docksite_w[pixeltype[pix]];
                        if (rel_z>docksite_h[pixeltype[pix]]-docksite_z[pixeltype[pix]]) rel_z = docksite_h[pixeltype[pix]]-docksite_z[pixeltype[pix]];
                        if (rel_z<-docksite_z[pixeltype[pix]]-docksite_h[pixeltype[pix]]) rel_z = -docksite_z[pixeltype[pix]]-docksite_h[pixeltype[pix]];
                }
                else {
                        rel_x += docksite_x[pixeltype[pix]];
                        rel_z += docksite_z[pixeltype[pix]];
                        if (sqrt(rel_x*rel_x+rel_z*rel_z)>docksite_w[pixeltype[pix]]) {
                                rel_x = _x;
                                rel_z = _z;
                        }
                        else {
                                rel_x -= docksite_x[pixeltype[pix]];
                                rel_z -= docksite_z[pixeltype[pix]];
                        }
                }
        }
}

void attracco ()
{
    play (ATTRACCO);
    trackframe = 0;
    tracking = 1;
    v_alpha = 0;
    v_beta = 0;
}

void undock ()
{
    if (!EVA_in_progress) {
        if (trackframe==23) {
            spd_x = pixel_xdisloc[pix] - prevpixx;
            spd_z = pixel_zdisloc[pix] - prevpixz;
            reset_trackframe = 1;
            play (DISTACCO); subs = 0;
        }
        else {
            if (pixel_absd[pix] < 500 + 1000*pixel_rot[pix]
                    &&pixel_ydisloc[pix] + docksite_y[pixeltype[pix]] - 16 > cam_y
                    &&!trackframe&&!explode_count)
                attracco ();
            }
    }
}

void extra_stop_extra ()
{
    if (EVA_in_progress) {
        d = sqrt(rel_x*rel_x+(rel_z+10)*(rel_z+10));
        if (d<25) {
            req_end_extra = 1;
            type_mode = false;
            mpul = 0;
            play (FLY_ON);
        }
    }
    else {
        if (alpha==0 && trackframe==23 && req_end_extra==0) {
            if (!moving_last_object) {
                play (FLY_OFF);
                rel_x = 0;
                rel_y = 7;
                rel_z = 0;
                EVA_in_progress = 1;
                dock_effects ();
            }
        }
    }
}

void find_alphabeta()
{
    kk = 1E9;
    for (c=0; c<360; c++) {
        x2 = cam_x - 100 * tsin[c] * tcos[alpha];
        z2 = cam_z + 100 * tcos[c] * tcos[alpha];
        y2 = cam_y + 100 * tsin[alpha];
        ox = rx - x2; oy = ry - y2; oz = rz - z2;
        ox = ox*ox+oy*oy+oz*oz;
        if (ox<kk) {
            kk = ox;
            beta90 = c;
        }
    }
    kk = 1E9;
    for (c=0; c<360; c++) {
        x2 = cam_x - 100 * tsin[beta90] * tcos[c];
        z2 = cam_z + 100 * tcos[beta90] * tcos[c];
        y2 = cam_y + 100 * tsin[c];
        ox = rx - x2; oy = ry - y2; oz = rz - z2;
        ox = ox*ox+oy*oy+oz*oz;
        if (ox<kk) {
            kk = ox;
            alpha90 = c;
        }
    }
    play (FID);
    subs = 0;
    comera_m = m;
    m = 0;
}

void fid_on ()
{
        if (EVA_in_progress||trackframe||fid||lead||orig) return;
        rx = cam_x + 100 * tsin[beta] * tcos[alpha];
        rz = cam_z - 100 * tcos[beta] * tcos[alpha];
        ry = cam_y - 100 * tsin[alpha];
        find_alphabeta ();
        fid = 1;
}

void lead_on ()
{
    if (EVA_in_progress||trackframe||fid||lead||orig) return;
    rx = cam_x + spd_x;
    ry = cam_y + spd_y;
    rz = cam_z + spd_z;
    find_alphabeta ();
    lead = 1;
}

void orig_on ()
{
    if (EVA_in_progress||trackframe||fid||lead||orig) return;
    rx = cam_x / 1.001;
    ry = cam_y / 1.001;
    rz = cam_z / 1.001;
    find_alphabeta ();
    orig = 1;
}

void dock_effects ()
{
    // Effetti dell'attracco.

    if (trackframe) {
        spd = 0;
        ox = pixel_xdisloc[pix] + docksite_x[pixeltype[pix]] - cam_x;
        oz = pixel_zdisloc[pix] + docksite_z[pixeltype[pix]] - cam_z;
        oy = pixel_ydisloc[pix] + docksite_y[pixeltype[pix]] - 16 - cam_y;
        if (oy<0&&!EVA_in_progress) cam_y = pixel_ydisloc[pix] + docksite_y[pixeltype[pix]] - 16;
        if (trackframe>12) tracking = 0.5;
        if (trackframe>21) tracking = 0.2;
        if (trackframe>22) tracking = 0.1;
        if (trackframe>=23) {
            spd_y = 0; // Perch altrimenti questa rimane.
            tracking = 0;
            trackframe = 23;
        }
        cam_x += ox / (24-trackframe);
        cam_y += oy / (24-trackframe);
        cam_z += oz / (24-trackframe);
        dsx = cam_x; dsy = cam_y; dsz = cam_z;
        if (fabs(oy)<152*fabs(tsin[alpha])&&!EVA_in_progress && alpha) {
            if (alpha>179) alpha-=360;
            alpha = (double) alpha / 1.1;
            if (alpha<0) alpha+=360;
        }
        if (EVA_in_progress) {
            if (!req_end_extra)
                dsy -= 36;
            else
                dsy -= 24 / (double)req_end_extra;
        }
    }
}

void scoppia (int nr_ogg, double potenza, int var)
{
        for (a=0; a<180; a+=18)
                for (b=36; b<180; b+=18) {
                        oz = potenza * tcos[b];
                        z2 = potenza * tsin[b];
                        ox = z2 * tcos[a];
                        oy = z2 * tsin[a];
                        Line3D (absolute_x[nr_ogg]-ox, absolute_y[nr_ogg]-oz, absolute_z[nr_ogg]+oy,
                                absolute_x[nr_ogg]+ox, absolute_y[nr_ogg]+oz, absolute_z[nr_ogg]-oy);
                }

        int bk = carry_type;
        preleva_oggetto (nr_ogg);
        carry_type = bk;

        if (!(SDL_GetTicks()%(random(var)+1))) play (BOM);
        subs = 0;
}

void chiudi_filedriver ()
{
/* No audio
    globalvocfile[0] = '.';
    if (recfile>-1) {
        audio_stop ();
        write (recfile, "\0", 1);
        close (recfile);
        recfile = -1;
    }
    file_driver_off ();
    dsp_driver_on ();
*/
}

void alfin (char arc)
{
        if (arc) fade (5);
        //dsp_driver_off (); // unused right now
        if (recfile) {
                //audio_stop (); // unused right now
                std::fwrite("\0", 1, 1, recfile);
                std::fclose(recfile);
                recfile = nullptr;
        }
        //file_driver_off ();
        ctrlkeys[0] = ctk;
}

void preleva_oggetto (int nr_ogg)
{
    if (moving_last_object) {
        return;
    }

    if (globalvocfile[0]=='.'&&EVA_in_progress) play (PRENDERE);

    carry_type = objecttype[nr_ogg];
    // update pixel translation speed if picking up an orbital engine
    if (EVA_in_progress && object_location[nr_ogg]>-1 && carry_type==1)
        pixel_rot[pix]--;

    // remove object and shift the others
    // so that the object picked up is always the last one
    for (int o = nr_ogg; o < _objects - 1; o++) {
        objecttype[o] = objecttype[o+1];
        absolute_x[o] = absolute_x[o+1];
        absolute_y[o] = absolute_y[o+1];
        absolute_z[o] = absolute_z[o+1];
        relative_x[o] = relative_x[o+1];
        relative_y[o] = relative_y[o+1];
        relative_z[o] = relative_z[o+1];
        object_location[o] = object_location[o+1];
    }
    // and reset properties of the last object,
    // so that they do not leak into the future object
    // when it is dropped
    {
        auto o = _objects - 1;
        objecttype[o] = -1;
        absolute_x[o] = 0;
        absolute_y[o] = 0;
        absolute_z[o] = 0;
        relative_x[o] = 0;
        relative_y[o] = 0;
        relative_z[o] = 0;
        object_location[o] = -1;
    }

    _objects--;
}

void Oggetti_sul_Pixel (char oblige)
{
    int o;

    if (pixel_absd[nopix]>4000&&!oblige) return;
//    int fdstatus = 0;
    for (o=0; o<_objects; o++) {
                if (object_location[o]==nopix) {
                        ox = relative_x[o] + pixel_xdisloc[nopix];
                        oy = relative_y[o] + pixel_ydisloc[nopix];
                        oz = relative_z[o] + pixel_zdisloc[nopix];
                        oy -= object_elevation[objecttype[o]];
                        if (nopix==pix||oblige) {
                                if (explode_count) explode = explode_count;
                                if (objecttype[o]==2||objecttype[o]==0) {
                                        // object copier or music player,
                                        // which work by placing an object on top
                                        for (a=o+1; a<_objects; a++) {
                                                if (object_location[a]==nopix) {
                                                        _oy = relative_y[o] - relative_y[a];
                                                        if (_oy>=0) {
                                                                _ox = relative_x[o] - relative_x[a];
                                                                _oz = relative_z[o] - relative_z[a];
                                                                if (sqrt(_ox*_ox+_oy*_oy+_oz*_oz)<object_elevation[objecttype[a]]+5) {
                                                                        // object is close enough, trigger effect
                                                                        if (objecttype[o]==2) {
                                                                                /* Audio object type. Off you go for now.
                                                                                if (objecttype[a] == sta_suonando) vicini = 1;
                                                                                if (!fd_status&&!moving_last_object) {
                                                                                        if (absolute_y[a]<10E10) {
                                                                                                if (!repeat)
                                                                                                        absolute_y[a] = 10E10;
                                                                                                else
                                                                                                        absolute_y[a] = 11E11;
                                                                                                ptr = 9*(objecttype[a]+FRONTIER_M3);
                                                                                                if (subsignal[ptr]) {
                                                                                                        if (!memcmp(&subsignal[ptr], "VISORE", 6)) {     goto nopoint; } // Do nothing.
                                                                                                        if (!memcmp(&subsignal[ptr], "TESTO-", 6)) {     goto nopoint; } // Do nothing.
                                                                                                        if (!memcmp(&subsignal[ptr], "BOMBA" , 5)) {     goto nopoint; } // Do nothing.
                                                                                                        if (!memcmp(&subsignal[ptr], "SORG-D", 6)) { source = 0;          goto nopoint; }
                                                                                                        if (!memcmp(&subsignal[ptr], "SORG-C", 6)) { source = 1;          goto nopoint; }
                                                                                                        if (!memcmp(&subsignal[ptr], "SORG-M", 6)) { source = 2;          goto nopoint; }
                                                                                                        if (!memcmp(&subsignal[ptr], "SORG-L", 6)) { source = 3;          goto nopoint; }
                                                                                                        if (!memcmp(&subsignal[ptr], "N-TAPE", 6)) { quality = 0;         goto nopoint; }
                                                                                                        if (!memcmp(&subsignal[ptr], "Q-TAPE", 6)) { quality = 1;         goto nopoint; }
                                                                                                        if (!memcmp(&subsignal[ptr], "C-DISC", 6)) { quality = 2;         goto nopoint; }
                                                                                                        if (!memcmp(&subsignal[ptr], "REPEAT", 6)) { repeat = 1 - repeat; goto nopoint; }
                                                                                                        if (recfile==-1) {
                                                                                                                memcpy (globalvocfile, &subsignal[ptr], 9);
                                                                                                                strcat (globalvocfile, ".voc");
                                                                                                                dsp_driver_off ();
                                                                                                                file_driver_on ();
                                                                                                                filtraggio (record_filt[quality]);
                                                                                                                if (audio_play (globalvocfile) == -1) {
                                                                                                                        sorgente (source);
                                                                                                                        recfile = _creat (globalvocfile, 0);
                                                                                                                        if (recfile>-1) rec (recfile, record_frq[quality]);
                                                                                                                }
                                                                                                                sta_suonando = objecttype[a];
                                                                                                                pixel_sonante = nopix;
                                                                                                        }
                                                                                                        vicini = 1;
                                                                                                        nopoint:
                                                                                                }
                                                                                        }
                                                                                }
                                                                                */
                                                                        }
                                                                        // object type 0: object copier
                                                                        else {
                                                                                if (!moving_last_object) {
                                                                                        if (_objects<MAX_OBJECTS && absolute_y[a]<10E10) {
                                                                                                absolute_y[_objects] = 10E10;
                                                                                                objecttype[_objects] = objecttype[a];
                                                                                                relative_x[_objects] = relative_x[a];
                                                                                                relative_y[_objects] = relative_y[a] - object_elevation[a];
                                                                                                relative_z[_objects] = relative_z[a];
                                                                                                object_location[_objects] = nopix;
                                                                                                if (objecttype[_objects]==1) pixel_rot[nopix]++;
                                                                                                _objects++;
                                                                                        }
                                                                                        absolute_y[a] = 10E10;
                                                                                }
                                                                        }
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
            if (!oblige) Object (objecttype[o]);
            explode = 0;
        }
    }
}

void lascia_cadere ()
{
    //int a;

    if (trackframe&&!EVA_in_progress) return;
    if (moving_last_object) return;

    objecttype[_objects] = carry_type;

    if (EVA_in_progress) {
        relative_x[_objects] = cox - prevpixx;
        relative_z[_objects] = coz - prevpixz;
        relative_y[_objects] = coy - pixel_ydisloc[pix] + object_elevation[carry_type];
        _x = rel_x;
        _z = rel_z;
        rel_x -= 9 * tsin[beta] * tcos[alpha];
        rel_z += 9 * tcos[beta] * tcos[alpha];
        if (docksite_h[pixeltype[pix]]>=0) {
            if (rel_x>docksite_w[pixeltype[pix]]-docksite_x[pixeltype[pix]]) rel_x = docksite_w[pixeltype[pix]]-docksite_x[pixeltype[pix]];
            if (rel_x<-docksite_x[pixeltype[pix]]-docksite_w[pixeltype[pix]]) rel_x = -docksite_x[pixeltype[pix]]-docksite_w[pixeltype[pix]];
            if (rel_z>docksite_h[pixeltype[pix]]-docksite_z[pixeltype[pix]]) rel_z = docksite_h[pixeltype[pix]]-docksite_z[pixeltype[pix]];
            if (rel_z<-docksite_z[pixeltype[pix]]-docksite_h[pixeltype[pix]]) rel_z = -docksite_z[pixeltype[pix]]-docksite_h[pixeltype[pix]];
        }
        else {
            if (sqrt(rel_x*rel_x+rel_z*rel_z)>docksite_w[pixeltype[pix]]) {
                rel_x = _x;
                rel_z = _z;
            }
        }
        cam_xt = dsx + rel_x;
        cam_yt = dsy + rel_y;
        cam_zt = dsz + rel_z;
        lastblock_real_x = 1E9;
        CollyZone (pix, 1);
        cfx = rel_x;
        cfz = rel_z;
        if ((ctrlkeys[0]&3)&&lastblock_real_x<1E9) {
            cfx = lastblock_real_x - pixel_xdisloc[pix];
            cfz = lastblock_real_z - pixel_zdisloc[pix];
        }
        else {
            cfx += docksite_x[pixeltype[pix]];
            cfz += docksite_z[pixeltype[pix]];
        }
        cfy = rel_y + disl + docksite_y[pixeltype[pix]] - 7;
        object_location[_objects] = pix;

        // increase pixel translation if object dropped is an orbital engine
        if (carry_type==1) {
            pixel_support[pix] = atan (pixel_zdisloc[pix] / pixel_xdisloc[pix]);
            pixel_rot[pix]++;
        }
        rel_x = _x;
        rel_z = _z;
        disl = 0;
        moving_last_object = 1;
    }
    else {
        object_location[_objects] = -1;
        absolute_y[_objects] = cam_y + 9 * tsin[alpha];
        absolute_x[_objects] = cam_x - 9 * tsin[beta] * tcos[alpha];
        absolute_z[_objects] = cam_z + 9 * tcos[beta] * tcos[alpha];
        relative_y[_objects] = spd_y;
        relative_x[_objects] = spd_x;
        relative_z[_objects] = spd_z;
        if (ctrlkeys[0]&64) {
            relative_y[_objects] += 5 * tsin[alpha];
            relative_x[_objects] -= 5 * tsin[beta] * tcos[alpha];
            relative_z[_objects] += 5 * tcos[beta] * tcos[alpha];
        }
        /* Audio off
        if (subsignal[9*(carry_type+FRONTIER_M3)]) {
            if (carry_type==sta_suonando && fd_status) break; //goto noeli;
            strcpy (t, &subsignal[9*(carry_type+FRONTIER_M3)]);
            strcat (t, ".voc");
            a = open (t, 0);
            if (a>-1) {
                close (a);
                remove (t);
            }
            //noeli:
        }
        */
    }

    carry_type = -1;
    _objects++;
}

void collyblock (double cx, double cy, double cz,
                 double hx, double hy, double hz,
                 char blocktype)
{
    if (!dropping) {
        r_y = rel_y;
        cx += ox; cy += oy; cz += oz;
        if (cam_xt>=cx-hx && cam_xt<=cx+hx) {
            if (cam_zt>=cz-hz && cam_zt<=cz+hz) {
                if ((blocktype==BLOCCO_ELEVATO||blocktype==SOLID_BOX)&&(cam_yt+45>cy+hy))
                    return;
                onblock = 1;
                if (blocktype==BLOCCO_PROIBITO) {
                    rel_x = r_x;
                    rel_y = r_y;
                    rel_z = r_z;
                    return;
                }
                if (cy-hy>=max_y) return;
                max_y = cy-hy;
                if (cam_yt+45!=cy-hy) {
                    rel_y = (cy-hy)-45-dsy;
                    cam_yt = dsy + rel_y;
                }
                disl = rel_y-r_y;
                rel_y = r_y;
                if (fabs(disl)>LIMITE_DISLIVELLI) {
                    rel_x = r_x;
                    rel_z = r_z;
                    disl = 0;
                }
                else {
                    lastblock_real_x = cx;
                    lastblock_real_z = cz;
                    }
                }
            }
        }
        else {
            if (rel_x>=cx-hx && rel_x<=cx+hx) {
                if (rel_z>=cz-hz && rel_z<=cz+hz) {
                    if (blocktype==BLOCCO_ELEVATO || blocktype == SOLID_BOX) return;
                    if (blocktype==BLOCCO_PROIBITO)
                        onblock = 2;
                    else
                        onblock = 1;
                    if (cy-hy>=max_y) return;
                    max_y = cy-hy;
                    if (rel_y!=cy-hy-dsy)
                        rel_y = cy-hy-dsy;
            }
        }
    }
}

void CollyZone (int nopix, char objectblocks)
{
        int o;
        int jjj;
        int iii, nr_elem = 0;

        max_y = 10E9;
        onblock = 0;
        disl = 0;

        ox = pixel_xdisloc[nopix];
        oy = pixel_ydisloc[nopix];
        oz = pixel_zdisloc[nopix];

ricerca:if (loaded_pixeltypes) {
                for (iii = 0; iii<loaded_pixeltypes; iii++) {
                        if (pixeltype[nopix]==pixeltype_type[iii])
                                goto trovato;
                }
        }
        LoadPtyp (pixeltype[nopix]);
        goto ricerca;
trovato:while (nr_elem<pixeltype_elements[iii]) {
                jjj = ELEMS * iii + nr_elem;
                if (pixel_elem_t[jjj]==BLOCCO_COLLISIONE
                  ||pixel_elem_t[jjj]==BLOCCO_PROIBITO
                  ||pixel_elem_t[jjj]==BLOCCO_ELEVATO
                  ||pixel_elem_t[jjj]==SOLID_BOX ) {
                        collyblock (pixel_elem_x[jjj],
                                    pixel_elem_y[jjj],
                                    pixel_elem_z[jjj],
                                    pixel_elem_1[jjj],
                                    pixel_elem_2[jjj],
                                    pixel_elem_3[jjj],
                                    pixel_elem_t[jjj]);
                }
                nr_elem ++;
        }

        if (objectblocks||(echo&4)) {
                for (o=0; o<_objects; o++) {
                        if (object_location[o]==nopix) {
                                ox = relative_x[o] + pixel_xdisloc[nopix];
                                oy = relative_y[o] + pixel_ydisloc[nopix];
                                oz = relative_z[o] + pixel_zdisloc[nopix];
                                oy -= object_elevation[objecttype[o]];
                                if (objecttype[o]>2) {
                                        c = objecttype[o] + FRONTIER_M3;
                                        collyblock (0, object_collyblockshifting[c-FRONTIER_M3], 0, docksite_x[c], docksite_y[c], docksite_z[c], BLOCCO_COLLISIONE);
                                }
                                else
                                        collyblock (0, 0, 0, 6, 0.01, 6, BLOCCO_COLLISIONE);
                        }
                }
        }

        if (!onblock) {
                if (fabs(7 - rel_y) > LIMITE_DISLIVELLI) {
                        rel_x = r_x;
                        rel_z = r_z;
                }
                else
                        disl = 7 - rel_y;
        }

        // handle object pick up request while in a pixel
        if (taking) {
                kk = 40;
                for (o=_objects-1; o>=0; o--) {
                        // ctrlkeys[0]&64 is Caps Lock
                        if (object_location[o]==nopix&&(pixelmass[objecttype[o]+FRONTIER_M3]<100||(ctrlkeys[0]&64))) {
                                _x = (relative_x[o] + pixel_xdisloc[nopix]) - (cam_xt - 10 * tsin[beta] * tcos[alpha]);
                                _y = (relative_y[o] + pixel_ydisloc[nopix] - object_elevation[objecttype[o]]) - (cam_yt + 10 * tsin[alpha]);
                                _z = (relative_z[o] + pixel_zdisloc[nopix]) - (cam_zt + 10 * tcos[beta] * tcos[alpha]);
                                _y /= 3;
                                d = sqrt (_x*_x+_y*_y+_z*_z);
                                if (d<kk) {
                                        obj = o;
                                        kk = d;
                                }
                        }
                }
        }
}


void Pixel (int typ)
{
    double p0, p1, p2, p3, p4, p5, p6, crx=0, cry=0, crz=0;
    double first_x, first_y, first_z;

    int iii, pid = 3, nr_elem = 0;
    int jjj, bkuneg = uneg;
    unsigned vptr;

    if (typ>FRONTIER_M1) goto ricerca;

    ox = pixel_xdisloc[nopix];
    oy = pixel_ydisloc[nopix];
    oz = pixel_zdisloc[nopix];

    if (subsignal[9*typ]&&pixel_absd[nopix]>512) {
        p0 = pixel_absd[nopix] * 0.01; if (p0<628) p0 = 628;
        c = pixel_absd[nopix] / 700; if (c>45) c = 45; if (c<18) c = 18;
        for (a=0; a<360; a+=c) {
            for (b=c; b<180; b+=c) {
                if (C32(ox+p0*tsin[b]*tcos[a], oy+p0*tcos[b], oz-p0*tsin[a]*tsin[b])) {
                    // --- draw operation begin
                    auto di = share_x + WIDTH*share_y;
                    auto si = &video_buffer[0] + di;

                    if (*si < 32) {
                        *si += 7;
                        *(si+1) += 5; *(si-1) += 5;
                        *(si+WIDTH) += 5;  *(si-WIDTH) += 5;
                        *(si+WIDTH+1) += 3; *(si-WIDTH-1) += 3;
                        *(si-WIDTH+1) += 3; *(si+WIDTH-1) += 3;
                    }
                    // --- draw operation end
                }
            }
        }
    }

    if (pixel_absd[nopix]>pixelmass[typ]) {
        if (!C32(ox, oy, oz)) return;
        vptr = share_x+WIDTH*share_y;

        // --- draw operation begin
        auto cl = pixelmass[typ] / 1000; if (cl > 32) cl = 32;
        auto ch = cl / 2; auto dl = cl / 4;

        auto si = &video_buffer[0] + vptr;

        if (*si < 32 ) {
            *si += cl;
            *(si+1) += ch;
            *(si-1) += ch;
            *(si+2) += dl;
            *(si-2) += dl;
            *(si+WIDTH-1) += dl;
            *(si-WIDTH+1) += dl;
            *(si+WIDTH) += ch;
            *(si-WIDTH) += ch;
            *(si+WIDTH+1) += dl;
            *(si-WIDTH-1) += dl;
            *(si+WIDTH*2) += dl;
            *(si-WIDTH*2) += dl;
        }
        // --- draw operation end
    } else {
ricerca:
        if (loaded_pixeltypes) {
            for (iii = 0; iii<loaded_pixeltypes; iii++) {
                if (typ==pixeltype_type[iii])
                    goto trovato;
            }
        }
        Aggiornamento ();
        goto ricerca;
trovato:
        if (typ>FRONTIER_M1) {
            _x = ox - cam_x; _y = oy - cam_y; _z = oz - cam_z;
            d = sqrt(_x*_x+_y*_y+_z*_z);
            id = static_cast<int>(d / pixelmass[typ]);
            goto ogg_2;
        } else {
            id = static_cast<int>(pixel_absd[nopix]/CULLING);
            if (nopix==pix) explode = explode_count;
        }
        uneg += pixel_absd[nopix] / 100;
        if (uneg>1000) uneg = 1000;
ogg_2:
        if (id>3) id = 3;
        while (nr_elem<pixeltype_elements[iii]) {
            jjj = ELEMS * iii + nr_elem;
            if (pixel_elem_t[jjj]==DETTAGLIO) {
                if (id==pid) {
                    uneg = bkuneg;
                    explode = 0;
                    return;
                }
                pid--;
            }
            p0 = pixel_elem_x[jjj];
            p1 = pixel_elem_y[jjj];
            p2 = pixel_elem_z[jjj];
            p3 = pixel_elem_1[jjj];
            p4 = pixel_elem_2[jjj];
            p5 = pixel_elem_3[jjj];
            p6 = pixel_elem_4[jjj];
            switch (pixel_elem_t[jjj]) {
                case SPIRALE:
                    if (!p5) fail_pixel_def (nr_elem, pixeltype[nopix]);
                    c = p5 * (id+1);
                    p3 *= id+1;
                    switch (static_cast<int>(p4)) {
                        case 0:
                            a = c;
                            k0 = 0; k1 = p3;
                            crx = c; cry = 0;
                            while (a<1080) {
                                rel (p0+k1*tcos[(int)crx], p1+k1*tsin[(int)crx], p2,
                                        p0+k0*tcos[(int)cry], p1+k0*tsin[(int)cry], p2);
                                cry = crx; crx += c;
                                if (crx>359) crx-=360;
                                k0 += p3; k1 += p3;
                                a += c;
                            }
                            break;
                        case 1:
                            a = c;
                            k0 = 0; k1 = p3;
                            crx = c; cry = 0;
                            while (a<1080) {
                                rel (p0+k1*tcos[(int)crx], p1, p2+k1*tsin[(int)crx],
                                        p0+k0*tcos[(int)cry], p1, p2+k0*tsin[(int)cry]);
                                cry = crx; crx += c;
                                if (crx>359) crx-=360;
                                k0 += p3; k1 += p3;
                                a += c;
                            }
                            break;
                        case 2:
                            a = c;
                            k0 = 0; k1 = p3;
                            crx = c; cry = 0;
                            while (a<1080) {
                                rel (p0, p1+k1*tcos[(int)crx], p2+k1*tsin[(int)crx], p0,
                                        p1+k0*tcos[(int)cry], p2+k0*tsin[(int)cry]);
                                cry = crx; crx += c;
                                if (crx>359) crx-=360;
                                k0 += p3; k1 += p3;
                                a += c;
                            }
                        }
                        break;
                    case PUNTO:
                        if (!id) xrel (p0, p1, p2);
                        break;
                    case LINEA:
                        rel (p0, p1, p2, p3, p4, p5);
                        break;
                    case RETTANGOLO:
                        rectrel (p0, p1, p2, p3, p4, p5);
                        break;
                    case SCATOLA:
                    case SOLID_BOX:
                        boxrel (p0, p1, p2, p3, p4, p5);
                        break;
                    case GRIGLIA:
                        p5 = (int) p5;
                        if (p5<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                        if (id) {
                            c = p5;
                            p5 /= id;
                            if (p5<1) p5 = 1;
                            p3 *= (float)c / p5;
                            p4 *= (float)c / p5;
                        }
                        _x = p3;
                        _y = p4;
                        switch ((int)p6) {
                            case 0:
                                p0 -= (p5*_x) / 2;
                                p1 -= (p5*_y) / 2;
                                p6 = p1 + _y*p5; _z = p0;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p6, p2);
                                    p0 += _x;
                                }
                                p0 = _z; p6 = p0 + p5*_x;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p6, p1, p2);
                                    p1 += _y;
                                }
                                break;
                            case 1:
                                p0 -= (p5*_x) / 2;
                                p2 -= (p5*_y) / 2;
                                p6 = p2 + _y*p5; _z = p0;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p1, p6);
                                    p0 += _x;
                                }
                                p0 = _z; p6 = p0 + p5*_x;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p6, p1, p2);
                                    p2 += _y;
                                }
                                break;
                            case 2:
                                p2 -= (p5*_x) / 2;
                                p1 -= (p5*_y) / 2;
                                p6 = p1 + _y*p5; _z = p2;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p6, p2);
                                    p2 += _x;
                                }
                                p2 = _z; p6 = p2 + p5*_x;
                                for (a=0; a<=p5; a++) {
                                    rel (p0, p1, p2, p0, p1, p6);
                                    p1 += _y;
                                }
                            }
                            break;
                        case DISEGNO_ELLITTICO:
                            p6 *= id+1;
                            if (p6>90) p6 = 90;
                            if (p5==0) {
                                for (c=0; c<360; c+=p6) {
                                    if (C32 (ox+p0+tcos[c]*p3, oy+p1+tsin[c]*p4, oz+p2)) {
                                        aux_plot();
                                    }
                                }
                                break;
                            }
                            if (p5==1) {
                                for (c=0; c<360; c+=p6) {
                                    if (C32 (ox+p0+tcos[c]*p3, oy+p1, oz+p2+tsin[c]*p4)) {
                                        aux_plot();
                                    }
                                }
                                break;
                            }
                            if (p5==2) {
                                for (c=0; c<360; c+=p6) {
                                    if (C32 (ox+p0, oy+p1+tsin[c]*p4, oz+p2+tcos[c]*p3)) {
                                        aux_plot();
                                    }
                                }
                                break;
                            }
                            break;
                        case ONDA:
                            p6 *= id+1;
                            if (p6>90) p6 = 90;
                            switch ((int)p5) {
                                case 0:
                                    _x = p0 - p3*180 + p6*p3;
                                    _oy = p1; _ox = _x - p6*p3;
                                    for (c=p6; c<=360; c+=p6) {
                                        _y = p1 + tsin[c] * p4;
                                        rel (_x, _y, p2, _ox, _oy, p2);
                                        _oy = _y;
                                        _ox = _x;
                                        _x += p6*p3;
                                    }
                                    break;
                                case 1:
                                    _z = p2 - p3*180 + p6*p3;
                                    _ox = p0; _oz = _z - p6*p3;
                                    for (c=p6; c<=360; c+=p6) {
                                        _x = p0 + tsin[c] * p4;
                                        rel (_x, p1, _z, _ox, p1, _oz);
                                        _ox = _x;
                                        _oz = _z;
                                        _z += p6*p3;
                                    }
                                    break;
                                case 2:
                                    _z = p2 - p3*180 + p6*p3;
                                    _oy = p1; _oz = _z - p6*p3;
                                    for (c=p6; c<=360; c+=p6) {
                                        _y = p1 + tsin[c] * p4;
                                        rel (p0, _y, _z, p0, _oy, _oz);
                                        _oy = _y;
                                        _oz = _z;
                                        _z += p6*p3;
                                    }
                                }
                                break;
                            case COLONNA:
                                if (p6<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p6 *= id+1;
                                if (p6>90) p6 = 90;
                                _x = p0 + tcos[0]*p4;
                                _z = p2 + tsin[0]*p4;
                                _y = p1 - p5 / 2;
                                crx = p0 + tcos[0]*p3;
                                crz = p2 + tsin[0]*p3;
                                cry = p1 + p5 / 2;
                                for (a=p6; a<360; a+=p6) {
                                    _ox = _x;
                                    _oz = _z;
                                    first_x = crx;
                                    first_z = crz;
                                    _x = p0 + tcos[a]*p4;
                                    _z = p2 + tsin[a]*p4;
                                    crx = p0 + tcos[a]*p3;
                                    crz = p2 + tsin[a]*p3;
                                    rel (_x, _y, _z, crx, cry, crz);
                                    if (p4) rel (_x, _y, _z, _ox, _y, _oz);
                                    if (p3) rel (first_x, cry, first_z, crx, cry, crz);
                                }
                                _ox = _x;
                                _oz = _z;
                                first_x = crx;
                                first_z = crz;
                                _x = p0 + tcos[0]*p4;
                                _z = p2 + tsin[0]*p4;
                                crx = p0 + tcos[0]*p3;
                                crz = p2 + tsin[0]*p3;
                                rel (_x, _y, _z, crx, cry, crz);
                                if (p4) rel (_x, _y, _z, _ox, _y, _oz);
                                if (p3) rel (first_x, cry, first_z, crx, cry, crz);
                                break;
                            case ASTERISCO:
                                if (p4<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p3 = -p3;
                                p4 *= id+1;
                                if (p4>90) p4 = 90;
                                for (a=0; a<180; a+=p4)
                                    for (b=p4; b<180; b+=p4) {
                                        _oz = p3 * tcos[b];
                                        z2 = p3 * tsin[b];
                                        _ox = z2 * tcos[a];
                                        _oy = z2 * tsin[a];
                                        rel (p0-_ox, p1-_oz, p2+_oy, p0+_ox, p1+_oz, p2-_oy);
                                    }
                                break;
                            case SFERA:
                                if (p5<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                c = p5 * (id+1);
                                if (c>90) c = 90;
                                p3 = -p3;
                                _ox = ox;
                                _oy = oy;
                                _oz = oz;
                                crx = ox + p0;
                                cry = oy + p1;
                                crz = oz + p2;
                                for (a=0; a<360; a+=c)
                                    for (b=c; b<180; b+=c) {
                                        oz = p3 * tcos[b] * p4;
                                        z2 = p3 * tsin[b];
                                        ox = z2 * tcos[a];
                                        oy = z2 * tsin[a];
                                        if (C32(ox+crx, oz+cry, crz-oy)) {
                                            aux_plot();
                                        }
                                    }
                                ox = _ox;
                                oy = _oy;
                                oz = _oz;
                                break;
                            case TESTO:
                                sprintf (t, &pixel_elem_b[40*jjj], nopix);
                                Txt (t, p0, p1, p2, p3, p4, p5, p6);
                                break;
                            case ELLISSE:
                                if (p6<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p6 *= id+1;
                                if (p6>90) p6 = 90;
                                if (p5==0) {
                                    _x = p0+p3;
                                    _y = p1;
                                    for (c=p6; c<360; c+=p6) {
                                        crx = p0+tcos[c]*p3;
                                        cry = p1+tsin[c]*p4;
                                        rel (_x, _y, p2, crx, cry, p2);
                                        _x = crx;
                                        _y = cry;
                                    }
                                    rel (_x, _y, p2, p0+p3, p1, p2);
                                    break;
                                }
                                if (p5==1) {
                                    _x = p0+p3;
                                    _z = p2;
                                    for (c=p6; c<360; c+=p6) {
                                        crx = p0+tcos[c]*p3;
                                        crz = p2+tsin[c]*p4;
                                        rel (_x, p1, _z, crx, p1, crz);
                                        _x = crx;
                                        _z = crz;
                                    }
                                    rel (_x, p1, _z, p0+p3, p1, p2);
                                    break;
                                }
                                if (p5==2) {
                                    _y = p1+p3;
                                    _z = p2;
                                    for (c=p6; c<360; c+=p6) {
                                        cry = p1+tcos[c]*p3;
                                        crz = p2+tsin[c]*p4;
                                        rel (p0, _y, _z, p0, cry, crz);
                                        _y = cry;
                                        _z = crz;
                                    }
                                    rel (p0, _y, _z, p0, p1+p3, p2);
                                    break;
                                }
                                break;
                            case CIAMBELLA:
                                if (p6<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                p6 *= id+1;
                                if (p6>90) p6 = 90;
                                _ox = ox;
                                _oy = oy;
                                _oz = oz;
                                ox += p0;
                                oy += p1;
                                oz += p2;
                                switch (int(p5)) {
                                    case 0:
                                        for (a=p6; a<360+p6; a+=p6) {
                                            first_x = (p4 - p3) * tcos[a];
                                            cry = first_x;
                                            first_y = 0;
                                            first_z = (p3-p4)*tsin[a];
                                            for (c=p6; c<360; c+=p6*2) {
                                                _x = tcos[c]*p4 - p3;
                                                _y = tsin[c]*p4;
                                                crx = _x*tcos[a];
                                                crz = - _x*tsin[a];
                                                rel (crx, crz, _y, first_x, first_z, first_y);
                                                first_x = crx; first_y = _y; first_z = crz;
                                                rel (crx, crz, _y, _x*tcos[(int)(a-p6)], - _x*tsin[(int)(a-p6)], _y);
                                            }
                                            rel (crx, crz, _y, cry, (p3-p4) * tsin[a], 0);
                                        }
                                        break;
                                    case 1:
                                        for (a=p6; a<360+p6; a+=p6) {
                                            first_x = (p4 - p3) * tcos[a];
                                            cry = first_x;
                                            first_y = 0;
                                            first_z = (p3-p4)*tsin[a];
                                            for (c=p6; c<360; c+=p6*2) {
                                                _x = tcos[c]*p4 - p3;
                                                _y = tsin[c]*p4;
                                                crx = _x*tcos[a];
                                                crz = - _x*tsin[a];
                                                rel (crx, _y, crz, first_x, first_y, first_z);
                                                first_x = crx; first_y = _y; first_z = crz;
                                                rel (crx, _y, crz, _x*tcos[(int)(a-p6)], _y, - _x*tsin[static_cast<int>(a-p6)]);
                                            }
                                            rel (crx, _y, crz, cry, 0, (p3-p4) * tsin[a]);
                                        }
                                        break;
                                    case 2:
                                        for (a=p6; a<360+p6; a+=p6) {
                                            first_x = (p4 - p3) * tcos[a];
                                            cry = first_x;
                                            first_y = 0;
                                            first_z = (p3-p4)*tsin[a];
                                            for (c=p6; c<360; c+=p6*2) {
                                                _x = tcos[c]*p4 - p3;
                                                _y = tsin[c]*p4;
                                                crx = _x*tcos[a];
                                                crz = - _x*tsin[a];
                                                rel (_y, crz, crx, first_y, first_z, first_x);
                                                first_x = crx; first_y = _y; first_z = crz;
                                                rel (_y, crz, crx, _y, - _x*tsin[(int)(a-p6)], _x*tcos[a-static_cast<int>(p6)]);
                                            }
                                            rel (_y, crz, crx, 0, (p3-p4) * tsin[a], cry);
                                        }
                                        break;
                                }
                                ox = _ox;
                                oy = _oy;
                                oz = _oz;
                                break;
                            case SFERA_RETICOLARE:
                                if (p5<=0) fail_pixel_def (nr_elem, pixeltype[nopix]);
                                c = p5 * (id+1);
                                if (c>90) c = 90;
                                p3 = -p3;
                                _ox = ox; ox += p0;
                                _oy = oy; oy += p1;
                                _oz = oz; oz += p2;
                                for (a=0; a<360; a+=c) {
                                    first_z = p3 * tcos[c] * p4;
                                    z2 = p3 * tsin[c];
                                    first_x = z2 * tcos[a];
                                    first_y = z2 * tsin[a];
                                    for (b=2*c; b<180; b+=c) {
                                        crz = p3 * tcos[b] * p4;
                                        k1 = p3 * tsin[b];
                                        crx = k1 * tcos[a];
                                        cry = k1 * tsin[a];
                                        rel (crx, crz, cry, first_x, first_z, first_y);
                                        rel (k1 * tcos[a+c], crz, k1 * tsin[a+c], first_x, first_z, first_y);
                                        first_x = crx;
                                        first_y = cry;
                                        first_z = crz;
                                    }
                                }
                                ox = _ox;
                                oy = _oy;
                                oz = _oz;
                                break;
            }
            nr_elem++;
        }
        uneg = bkuneg;
        explode = 0;
    }
}

void Object (int tipo)
{
    FILE* th;

    switch (tipo) {
        // Fottifoh
        case 0:
            a = 0;
            while (fotty[a]!=10) {
                rel (fotty[a], fotty[a+1], fotty[a+2],
                        fotty[a+3], fotty[a+4], fotty[a+5]);
                a += 6;
            }
            rectrel (-2, 0, 4, 0.5, 0.5, 1);
            rectrel (2, 0, 4, 0.5, 0.5, 1);
            break;
        // Orbital engine
        case 1:
            for (   a = static_cast<int>((SDL_GetTicks()/TICKS_PER_FRAME)%45);
                    a <= 135+(int)((SDL_GetTicks()/TICKS_PER_FRAME)%45);
                    a += 45) {
                _ox = 25 * tcos[a]; _oy = 25 * tsin[a];
                rel (-_ox, 0, _oy, _ox, 0, -_oy);
            }
            break;
        // Record player
        case 2:
            b = 0;
            if (/* sbf_stat&& */pix==pixel_sonante)
                b = ((SDL_GetTicks()/TICKS_PER_FRAME)%3)*15;
            for (a=b; a<=135+b; a+=45) {
                _ox = 5 * tcos[a]; _oy = 5 * tsin[a];
                rel (-_ox, 0, _oy, _ox, 0, -_oy);
            }
            rectrel (0, 0, 0, 6, 6, 1);
            if (recfile) Txt ("> RECORD >", -5.5, 0, -5, 0.1, 0.15, 270, 0);
            if (globalvocfile[0]!='.') Txt ("> PLAY >", -5.5, 0, -4, 0.1, 0.15, 270, 0);
            sprintf (t, "INPUT: %s", source_name[static_cast<int>(source)]);
            Txt (t, -5.5, 0, 5, 0.1, 0.15, 270, 0);
            sprintf (t, "ACCURATEZZA: %s", record_qlty[static_cast<int>(quality)]);
            Txt (t, -5.5, 0, 4, 0.1, 0.15, 270, 0);
            if (repeat) Txt ("REPEAT", -5.5, 0, 3, 0.1, 0.15, 270, 0);
            break;
        
        // other objects
        // TODO: is this code reading from a file on each tick? Aren't objects loaded into memory on startup?
        default:
            Pixel (tipo+FRONTIER_M3);
            if (id) break;
            if (!type_mode) break; // ctrlkeys[0]&16 is scroll lock, use something else
            if (!memcmp(&subsignal[9*(tipo+FRONTIER_M3)], "TEXT-" /*"TESTO-"*/, 5/*6*/)) {
                a = subsignal[9*(tipo+FRONTIER_M3)+5/*6*/];
                memset (buffer, ' ', 512);
                // safeguard buffer from reading beyond the end of the text
                buffer[512] = '\0';
                sprintf (autore_forme, "TEXT-%03d.FIX", tipo-3);
                th = std::fopen (autore_forme, "r+b");
                if (th) {
                    buffer[32] = '\0';
                    k1 = 5.32;
                    int bytes_read = std::fread(&buffer[33], 1, 512, th);
                    for (c=0; c<bytes_read; c+=32) {
                        memcpy (buffer, buffer+33+c, 32);
                        if (a=='V') {
                            Txt (reinterpret_cast<char*>(buffer), -6.82, -k1, 0, 0.11, 0.14, 0, 0);
                        } else if (a=='H') {
                            Txt (reinterpret_cast<char*>(buffer), -6.82, 0, k1, 0.11, 0.14, 270, 0);
                        }
                        k1 -= 0.7;
                    }
                    if (d<15||tipo==carry_type) {
                        fermo_li = 1;
                        memcpy (buffer+545, buffer+33, 512);
                        /*
                        if (tasto_premuto()) {
                            c = attendi_pressione_tasto();
                            if ((globalvocfile[0]=='.'&&EVA_in_progress)||!EVA_in_progress) play (TARGET);
                            switch (c) {
                                case 0:
                                    switch (attendi_pressione_tasto()) {
                                        case 77:
                                            if (cursore<511) cursore++;
                                            break;
                                        case 75:
                                            if (cursore) cursore--;
                                            break;
                                        case 80:
                                            if (cursore<480) cursore += 32;
                                            break;
                                        case 72:
                                            if (cursore>31) cursore -= 32;
                                            break;
                                        case 64:
                                            snapshot();
                                }
                                break;
                        case 13:
                                if (cursore<480) {
                                    cursore = (cursore/32) * 32;
                                    cursore += 32;
                                }
                                break;
                        case 8:
                                if (cursore) {
                                        cursore--;
                                        buffer[cursore+33] = 32;
                                }
                                break;
                        case 27:
                                alfin (1);
                                exit (0);
                        default:
                            if (cursore<512) {
                                if (c>='a'&&c<='z') c -= 32;
                                if (c>31 && c<97) {
                                    buffer[cursore+33] = c;
                                    if (cursore<511) cursore++;
                                }
                            }
                            */
                    }
                    if (cursore<512) {
                        if (type_this != 0) {
                            assert (type_this >= 32 && type_this <= 96);
                            buffer[cursore+33] = type_this;
                            if (cursore<511) cursore++;
                        }
                        type_this = 0;
                    }
                    //}
                    if (a=='V') Txt ("*", -6.82+(cursore%32)*0.44, (cursore/32)*0.7-5.32, 0, 0.5, 0.5, 0, 0);
                    else if (a=='H') Txt ("*", -6.82+(cursore%32)*0.44, 0, 5.32-(cursore/32)*0.7, 0.5, 0.5, 270, 0);
                    if (memcmp (buffer+33, buffer+545, 512)) {
                        std::fseek (th, 0, SEEK_SET);
                        std::fwrite (buffer+33, 1, 512, th);
                    }
                //}
                    std::fclose (th);
                }
                else {
                    th = std::fopen(autore_forme, "wb");
                    if (th) {
                        std::fwrite (buffer, 1, 512, th);
                        std::fclose (th);
                    }
                }
        }
    }
}

// Disegna la forma di un oggetto.

i8 fotty[277] = {
        -6, 0, +6,          -4, 0, +8,
        -4, 0, +8,          -2, 0, +8,
        -2, 0, +8,          -2, 0, +6,
        -2, 0, +6,           0, 0, +7,
         0, 0, +7,          +2, 0, +6,
        +2, 0, +6,          +2, 0, +8,
        +2, 0, +8,          +4, 0, +8,
        +4, 0, +8,          +6, 0, +6,
        +6, 0, +6,          +6, 0, +4,
        +6, 0, +4,          +4, 0, +4,
        +4, 0, +4,          +4, 0, +2,
        +4, 0, +2,          +2, 0, +1,
        +2, 0, +1,          +4, 0,  0,
        +4, 0,  0,          +5, 0, -2,
        +5, 0, -2,          +4, 0, -4,
        +4, 0, -4,          +4, 0, -6,
        +4, 0, -6,          +2, 0, -8,
        +2, 0, -8,           0, 0, -7,
         0, 0, -7,          -2, 0, -8,
        -2, 0, -8,          -4, 0, -6,
        -4, 0, -6,          -4, 0, -4,
        -4, 0, -4,          -5, 0, -2,
        -5, 0, -2,          -4, 0,  0,
        -4, 0,  0,          -2, 0, +1,
        -2, 0, +1,          -4, 0, +2,
        -4, 0, +2,          -4, 0, +4,
        -4, 0, +4,          -6, 0, +4,
        -6, 0, +4,          -6, 0, +6,
        -2, 0,  0,          -1, 0, -2,
        -1, 0, -2,          -2, 0, -4,
        -2, 0, -4,           0, 0, -5,
         0, 0, -5,          +2, 0, -4,
        +2, 0, -4,          +1, 0, -2,
        +1, 0, -2,          +2, 0,  0,
        -4, 0, -4,          -2, 0, -4,
        +2, 0, -4,          +4, 0, -4,
         0, 0, -5,           0, 0, -7,
        -2, 0, +1,          -2, 0, +3,
        -2, 0, +3,           0, 0, +4,
         0, 0, +4,          +2, 0, +3,
        +2, 0, +3,          +2, 0, +1,
        +2, 0, +1,           0, 0,  0,
         0, 0,  0,          -2, 0, +1,
        -1, 0, +3,          +1, 0, +3,
        +1, 0, +3,           0, 0, +1,
         0, 0, +1,          -1, 0, +3,
10 };
