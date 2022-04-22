/** \file input.cpp
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
#include "input.h"

#include <iostream>
#include <cstdio>

#include <cmath>
#include <cstdlib>
#include <fcntl.h>

#include "fast3d.h"
#include "global.h"
#include "conf.h"
#include "sdl_exception.h"
#include "SDL.h"

using namespace std;

std::array<unsigned char,2> ctrlkeys = {{0,0}};

const char* loaded_background = nullptr;

const bool grab_mouse = true;

//int fh = 0;
class RWHandler {
    SDL_RWops* p_rw;
public:
    RWHandler(void) noexcept
    : p_rw(nullptr) {
    }
    RWHandler(const char* file, const char* mode)
    : p_rw(nullptr) {
        open(file, mode);
    }
    ~RWHandler(void) {
        close();
    }

    /// Open file, closing an existing one
    void open(const char* file, const char* mode) {
        close();
        p_rw = SDL_RWFromFile(file, mode);
        if (p_rw == nullptr) {
            throw sdl_exception();
        }
    }
    /// Seek
    /// \param either RW_SEEK_SET, RW_SEEK_CUR or RW_SEEK_END
    unsigned long long seek(unsigned long long offset, int whence) {
        return SDL_RWseek(p_rw, offset, whence);
    }

    template<typename T>
    size_t write(const T& data) {
        return write(&data, sizeof(T));
    }
    size_t write(const void* data, size_t size) {
        return SDL_RWwrite(p_rw, data, size, 1);
    }

    template<typename T>
    size_t read(T& data) {
        return read(&data, sizeof(T));
    }
    size_t read(void* dest, size_t size) {
        return SDL_RWread(p_rw, dest, size, 1);
    }

    /// Close file, silently ignores if no file in handle
    void close(void) noexcept {
        if (p_rw) {
            SDL_RWclose(p_rw);
            p_rw = nullptr;
        }
    }
};

// aspetta un tasto e d qual'.
int attendi_pressione_tasto ()
{
    SDL_PumpEvents();
    int n = 0;
    auto arr = SDL_GetKeyboardState(&n);
    if (!arr) return -1;

    for (int i = 1; i < n ; i++) {
        if (arr[i] == 1) {
            return i;
        }
    }

    return 0;
}

// torna 1 se c' un tasto premuto da estrarre.
int tasto_premuto () {
    return attendi_pressione_tasto() != 0;
}

/* Lettura del mouse e ritorno nelle variabili indicate. */

int mdltx = 0, mdlty = 0, mx = 0, my = 0, mpul = 0;

void mouse_input() {
    // mouse delta is saved in mdltx and mdlty.
    auto r = SDL_GetRelativeMouseState(&mdltx, &mdlty);

    // Update mouse key presses
    auto lmb = (r & SDL_BUTTON(1)) ? 1 : 0;
    auto rmb = (r & SDL_BUTTON(3)) ? 2 : 0;
    mpul = lmb | rmb;
}

void update_ctrlkeys()
{
    SDL_PumpEvents();
    ctrlkeys.fill(0);
    auto r = SDL_GetModState();

    if (r & KMOD_RSHIFT)
        ctrlkeys[0] |= 1;

    if (r & KMOD_LSHIFT)
        ctrlkeys[0] |= 2;

    //if (r & KMOD_SCROLL)
    //    ctrlkeys[0] |= 16;

    if (r & KMOD_LALT)
        ctrlkeys[0] |= 16;

    // relocating this to right alt instead of left alt
    if (r & KMOD_RALT)
        ctrlkeys[0] |= 32;

    if (r & KMOD_CAPS)
        ctrlkeys[0] |= 64;
}

void keybuffer_cleaner()
{
    // yes, this will clean other events too...
    SDL_Event event;
    while( SDL_PollEvent( &event ));
}

char trova_id (FILE* fh, const char *id) {
    if (!fh) return '\0';

    std::fseek(fh, 0, SEEK_SET);

    while (1) {
        char *idpos;

        int cl = std::fread(buffer, 1, 1024, fh);
        buffer[cl-1] = '\0';
        if ((idpos = strcasestr ((char*)buffer, id)) != NULL) {
            int dlt = (unsigned char*)idpos - buffer;
            int spostam = (int)strlen(id)-(cl-dlt);
            std::fseek (fh, spostam, SEEK_CUR);
            return 1;
        }
        if (std::fread(buffer, 1, 1, fh) == 0) return '\0';
        std::fseek (fh, -129, SEEK_CUR);
    }
}

static char eol = 0;
void leggi_t_fino_a (FILE* fh, char codcar, int ptyp)
{
    int c;
    //char f;

    if (eol) {
        std::fclose (fh);
        //dsp_driver_off ();
        _80_25_C();
        cerr << "Parameter not found\nElement "
            << (pixeltype_elements[static_cast<int>(loaded_pixeltypes)]+1)
            << " of pixel type " << ptyp << "." << endl;
        exit (1); // FIXME exit bomb
    }

    c = 0;
    t[0] = '\0';

    while (c<80) {
            // assembly replaced with a simple read() function
//            asm {
//                    pusha
//                    mov ah, 3fh
//                    mov bx, fh
//                    mov cx, 1
//                    lea dx, t
//                    add dx, c
//                    int 0x21 /* E` ottimizzata per leggere un char.
//                                senza che il c si stia a fare troppe
//                                seghe. */
//                    mov f, al
//                    popa
//            }
        if (std::fread(&t[c], 1, 1, fh) == 0) break;
        //if (!f) break;
        if (t[c] > 32) {
            if (t[c-1]!='%' && t[c]>='a' && t[c]<='z')
                t[c] -= 32;
            if (t[c] == '_')
                t[c] = ' ';
            if (codcar == t[c] || t[c] == ';') {
                if (t[c] == ';')
                    eol = 1;
                t[c] = '\0';
                return;
            }
            c++;
        }
    }
    t[79] = '\0';
}

void load_pixels_def(int& ptyp)
{
    FILE* fh = std::fopen ("PIXELS.DEF", "rb");
    if (fh) {
        if (!trova_id (fh, "SEED")) {
            std::fclose (fh);
            _80_25_C();
            cerr << "Missing command in PIXELS.DEF: SEED = n;"
                 << "\n<n> must be a number between 0 and 65535." << endl;
            throw 3;
        }
        leggi_t_fino_a (fh, '=', -1);
        eol = 0; leggi_t_fino_a (fh, ';', -1);
        srand ((unsigned)atof(t));
        std::fseek (fh, 0, SEEK_SET);
        if (!trova_id (fh, "AUTHOR")) {
            std::fclose (fh);
            _80_25_C();
            cerr << "Missing command in PIXELS.DEF: AUTHOR = AUTHOR_NAME;" << endl;
            throw 4;
        }
        eol = 0; leggi_t_fino_a (fh, '=', -1);
        eol = 0; leggi_t_fino_a (fh, ';', -1);
        strcpy (autore_forme, t);
        std::fseek (fh, 0, SEEK_SET);
        sprintf (t, "TYPE %d;\r\n", ptyp);
        while (trova_id (fh, t)) {
            existant_pixeltypes++;
            ptyp++; sprintf (t, "TYPE %d;\r\n", ptyp);
            if (ptyp>FRONTIER_M1) {
                _80_25_C();
                cout << "Too many pixels.\nIt will only load "
                     << FRONTIER << " pixels (from type 0 to type "
                     << FRONTIER_M1 << ")." << endl;
                break;
            }
        }
        ptyp = 0;
        sprintf (t, "MODEL %d;\r\n", ptyp);
        while (trova_id (fh, t)) {
            existant_objecttypes++;
            ptyp++; sprintf (t, "MODEL %d;\r\n", ptyp);
            if (ptyp>FRONTIER_COMPL_M1) {
                _80_25_C();
                cout << "Too many object models.\nIt will only load "
                    << FRONTIER_COMPL << " objects (from model 0 to model "
                    << FRONTIER_COMPL_M1 << ")." << endl;
                std::fclose (fh);
                fh = nullptr;
                break;
            }
        }
    }
    else {
        cerr << "Missing file PIXELS.DEF" << endl;
        throw 5;
    }
    if (fh)
        std::fclose (fh);
}

// Carica il tipo di pixel specificato.

void LoadPtyp (int ptyp) {
    int c;
    unsigned int jjj;

    memcpy (&subsignal[9*ptyp], "\0\0\0\0\0\0\0\0\0", 9);

    if (loaded_pixeltypes>=BUFFERS) {
        //alfin (0);
        cerr << "Too many pixels were loaded here!!!\n"
            "There's no more space in the game's buffers." << endl;
        exit (255);
    }

    pixeltype_elements[static_cast<int>(loaded_pixeltypes)] = 0;
    pixeltype_type[static_cast<int>(loaded_pixeltypes)] = ptyp;
    pixelmass[ptyp] = 10000;

    FILE* fh = std::fopen("PIXELS.DEF", "rb");
    if (fh) {
        if (ptyp>FRONTIER_M1)
            sprintf (t, "MODEL %d;\r\n", static_cast<int>(ptyp-FRONTIER));
        else
            sprintf (t, "TYPE %d;\r\n", ptyp);
        trova_id (fh, t);
        do {
            eol = 0;
            jjj = ELEMS * loaded_pixeltypes + pixeltype_elements[static_cast<int>(loaded_pixeltypes)];
            leggi_t_fino_a (fh, ',', ptyp);
            pixel_elem_t[jjj] = 0;
            while (pixel_elem_t[jjj]<coms && strcasecmp(t, comspec[pixel_elem_t[jjj]]))
                (pixel_elem_t[jjj])++;
            if (pixel_elem_t[jjj] == coms) {
                std::fclose (fh);
                //alfin (0);
                cerr << "Command not recognized.\nElement "
                    << (pixeltype_elements[static_cast<int>(loaded_pixeltypes)]+1)
                    << " in model nr. " << ptyp << "." << endl;
                //printf ("Comando specificato nel file: %s.", t);
                exit (1);
            }
            if (pixel_elem_t[jjj] == FINEPIXEL) break;
            if (pixel_elem_t[jjj] == SOTTOSEGNALE) {
                leggi_t_fino_a (fh, ';', ptyp);
                strcpy (&pixel_elem_b[40*jjj], t);
                memcpy (&subsignal[9*ptyp], &pixel_elem_b[40*jjj], 8);
                //goto pros;
                continue;
            }
            c = params[pixel_elem_t[jjj]];
            if (c>=1) { leggi_t_fino_a (fh, ',', ptyp); pixel_elem_x[jjj] = atof(t); }
            if (c>=2) { leggi_t_fino_a (fh, ',', ptyp); pixel_elem_y[jjj] = atof(t); }
            if (c>=3) { leggi_t_fino_a (fh, ',', ptyp); pixel_elem_z[jjj] = atof(t); }
            if (c>=4) { leggi_t_fino_a (fh, ',', ptyp); pixel_elem_1[jjj] = atof(t); }
            if (c>=5) { leggi_t_fino_a (fh, ',', ptyp); pixel_elem_2[jjj] = atof(t); }
            if (c>=6) { leggi_t_fino_a (fh, ',', ptyp); pixel_elem_3[jjj] = atof(t); }
            if (c>=7) { leggi_t_fino_a (fh, ',', ptyp); pixel_elem_4[jjj] = atof(t); }
            if (!eol) leggi_t_fino_a (fh, ';', ptyp);
            if (pixel_elem_t[jjj] == TESTO) {
                strcpy (&pixel_elem_b[40*jjj], t);
                //goto count_;
            } else {
                if (pixel_elem_t[jjj] == ATTRACCO) {
                    docksite_x[ptyp] = pixel_elem_x[jjj];
                    docksite_y[ptyp] = pixel_elem_y[jjj];
                    docksite_z[ptyp] = pixel_elem_z[jjj];
                    docksite_w[ptyp] = pixel_elem_1[jjj];
                    docksite_h[ptyp] = pixel_elem_2[jjj];
                    if (ptyp>FRONTIER_M1) {
                        object_collyblockshifting[ptyp-FRONTIER_M3] = pixel_elem_1[jjj];
                        object_elevation[ptyp-FRONTIER_M3] = fabs(pixel_elem_2[jjj]) + 0.001;
                    }
                    continue;
                    //goto pros;
                }
                if (pixel_elem_t[jjj] == MASSA) {
                    pixelmass[ptyp] = pixel_elem_x[jjj];
                    //goto pros;
                    continue;
                }
            }
//count_:
            pixeltype_elements[static_cast<int>(loaded_pixeltypes)]++;
//pros:
        } while (pixeltype_elements[static_cast<int>(loaded_pixeltypes)] < ELEMS);
        std::fclose (fh);
        if (pixeltype_elements[static_cast<int>(loaded_pixeltypes)]==ELEMS) {
            //alfin (0);
            cerr << "Definition too long.\nModel nr. " << ptyp << endl;
            exit (1); // FIXME exit bomb
        }
        loaded_pixeltypes++;
    }
}

void load_game (char i)
{
    if (i >= 'a' && i <= 'z') {
        i -= 'a' - 'A';
    }

    sprintf (t, "CRYXTELS.%cIT", i);

    FILE* fh = std::fopen(t, "rb");
    if (fh) {
        short tmp_pixels = 0;
        std::fread (&tmp_pixels, sizeof(tmp_pixels), 1, fh);
        if (tmp_pixels == 0) {
            cerr << "Failed to read game from \"" << t << "\": Invalid situation file." << endl;
            std::fclose(fh);
            return;
        }
        pixels = tmp_pixels;
        std::fread (&pixel_support[0], sizeof(double), pixels, fh);
        std::fread (&pixel_xdisloc[0], sizeof(double), pixels, fh);
        std::fread (&pixel_ydisloc[0], sizeof(double), pixels, fh);
        std::fread (&pixel_zdisloc[0], sizeof(double), pixels, fh);
        std::fread (&objects, sizeof(short), 1, fh);
        _objects = objects;
        std::fread (&objecttype[0], sizeof(short), objects, fh);
        std::fread (&relative_x[0], sizeof(double), objects, fh);
        std::fread (&relative_y[0], sizeof(double), objects, fh);
        std::fread (&relative_z[0], sizeof(double), objects, fh);
        std::fread (&absolute_x[0], sizeof(double), objects, fh);
        std::fread (&absolute_y[0], sizeof(double), objects, fh);
        std::fread (&absolute_z[0], sizeof(double), objects, fh);
        std::fread (&object_location[0], sizeof(short), objects, fh);
        std::fread (&cam_x, sizeof(double), 1, fh);
        std::fread (&cam_y, sizeof(double), 1, fh);
        std::fread (&cam_z, sizeof(double), 1, fh);
        std::fread (&alfa, sizeof(short), 1, fh);
        std::fread (&beta, sizeof(short), 1, fh);
        std::fread (&nav_a, sizeof(short), 1, fh);
        std::fread (&nav_b, sizeof(short), 1, fh);
        std::fread (&taking, 1, 1, fh);
        std::fread (&carry_type, sizeof(short),1 ,fh);
        std::fread (&trackframe, sizeof(double), 1, fh);
        std::fread (&reset_trackframe, 1, 1, fh);
        std::fread (&tracking, sizeof(double), 1, fh);
        std::fread (&req_end_extra, 1, 1, fh);
        std::fread (&alfad, sizeof(short), 1, fh);
        std::fread (&betad, sizeof(short), 1, fh);
        std::fread (&pix, sizeof(short), 1, fh);
        std::fread (&alfa90, sizeof(short), 1, fh);
        std::fread (&beta90, sizeof(short), 1, fh);
        std::fread (&fid, 1, 1, fh);
        std::fread (&lead, 1, 1, fh);
        std::fread (&orig, 1, 1, fh);
        std::fread (&comera_m, 1, 1, fh);
        std::fread (&spd_x, sizeof(double), 1, fh);
        std::fread (&spd_y, sizeof(double), 1, fh);
        std::fread (&spd_z, sizeof(double), 1, fh);
        std::fread (&spd, sizeof(double), 1, fh);
        std::fread (&extra, 1, 1, fh);
        std::fread (&rel_x, sizeof(double), 1, fh);
        std::fread (&rel_y, sizeof(double), 1, fh);
        std::fread (&rel_z, sizeof(double), 1, fh);
        std::fread (&obj, sizeof(short), 1, fh);
        std::fread (&m, 1, 1, fh);
        std::fread (&echo, 1, 1, fh);
        std::fread (&carried_pixel, sizeof(short), 1, fh);
        std::fread (&disl, sizeof(double), 1, fh);
        std::fread (&cursore, sizeof(short), 1, fh);
        std::fread (&explode_count, 1, 1, fh);
        std::fread (&ctrlkeys[0], 1, 1, fh);
        std::fread (&pixel_rot[0], 1, pixels, fh);
        std::fread (&pixeltype[0], sizeof(short), pixels, fh);
        std::fread (&repeat, 1, 1, fh);
        std::fread (&source, 1, 1, fh);
        std::fread (&quality, 1, 1, fh);
        std::fclose (fh);
    } else {
        throw errno;
    }
}

void save_game (char i)
{

    if (moving_last_object) return;

    objects = _objects;

    if (i >= 'a' && i <= 'z') {
        i -= 'a' - 'A';
    }

    sprintf (t, "CRYXTELS.%cIT", i);
    FILE* fh = std::fopen (t, "wb");
    if (fh) {
        unsigned char a;

        std::fwrite (&pixels, sizeof(short), 1, fh);
        std::fwrite (&pixel_support[0], sizeof(double), pixels, fh);
        std::fwrite (&pixel_xdisloc[0], sizeof(double), pixels, fh);
        std::fwrite (&pixel_ydisloc[0], sizeof(double), pixels, fh);
        std::fwrite (&pixel_zdisloc[0], sizeof(double), pixels, fh);
        std::fwrite (&objects, sizeof(short), 1, fh);
        std::fwrite (&objecttype[0], sizeof(short), objects, fh);
        std::fwrite (&relative_x[0], sizeof(double), objects, fh);
        std::fwrite (&relative_y[0], sizeof(double), objects, fh);
        std::fwrite (&relative_z[0], sizeof(double), objects, fh);
        std::fwrite (&absolute_x[0], sizeof(double), objects, fh);
        std::fwrite (&absolute_y[0], sizeof(double), objects, fh);
        std::fwrite (&absolute_z[0], sizeof(double), objects, fh);
        std::fwrite (&object_location[0], sizeof(short int), objects, fh);
        std::fwrite (&cam_x, sizeof(double), 1, fh);
        std::fwrite (&cam_y, sizeof(double), 1, fh);
        std::fwrite (&cam_z, sizeof(double), 1, fh);
        std::fwrite (&alfa, sizeof(short), 1, fh);
        std::fwrite (&beta, sizeof(short), 1, fh);
        std::fwrite (&nav_a, sizeof(short), 1, fh);
        std::fwrite (&nav_b, sizeof(short), 1, fh);
        std::fwrite (&taking, sizeof(char), 1, fh);
        std::fwrite (&carry_type, sizeof(short), 1, fh);
        std::fwrite (&trackframe, sizeof(double), 1, fh);
        std::fwrite (&reset_trackframe, sizeof(char), 1, fh);
        std::fwrite (&tracking, sizeof(double), 1, fh);
        std::fwrite (&req_end_extra, sizeof(char), 1, fh);
        std::fwrite (&alfad, sizeof(short), 1, fh);
        std::fwrite (&betad, sizeof(short), 1, fh);
        std::fwrite (&pix, sizeof(short), 1, fh);
        std::fwrite (&alfa90, sizeof(short), 1, fh);
        std::fwrite (&beta90, sizeof(short), 1, fh);
        std::fwrite (&fid, 1, 1, fh);
        std::fwrite (&lead, 1, 1, fh);
        std::fwrite (&orig, 1, 1, fh);
        std::fwrite (&comera_m, 1, 1, fh);
        std::fwrite (&spd_x, sizeof(double), 1, fh);
        std::fwrite (&spd_y, sizeof(double), 1, fh);
        std::fwrite (&spd_z, sizeof(double), 1, fh);
        std::fwrite (&spd, sizeof(double), 1, fh);
        std::fwrite (&extra, 1, 1, fh);
        std::fwrite (&rel_x, sizeof(double), 1, fh);
        std::fwrite (&rel_y, sizeof(double), 1, fh);
        std::fwrite (&rel_z, sizeof(double), 1, fh);
        std::fwrite (&obj, sizeof(short), 1, fh);
        std::fwrite (&m, 1, 1, fh);
        std::fwrite (&echo, 1, 1, fh);
        std::fwrite (&carried_pixel, sizeof(short), 1, fh);
        std::fwrite (&disl, sizeof(double), 1, fh);
        std::fwrite (&cursore, sizeof(short), 1, fh);
        std::fwrite (&explode_count, 1, 1, fh);
        a = ctrlkeys[0]; if (a&2) a ^= 2;
        std::fwrite (&a, 1, 1, fh);
        std::fwrite (&pixel_rot[0], 1, pixels, fh);
        std::fwrite (&pixeltype[0], sizeof(short), pixels, fh);
        std::fwrite (&repeat, 1, 1, fh);
        std::fwrite (&source, 1, 1, fh);
        auto conta = std::fwrite (&quality, 1, 1, fh);
        if (conta!=1) {
            std::fclose (fh);
            std::remove (t);
            return;
        }
        std::fclose (fh);
    } else {
        throw errno; // FIXME use a better exception type
    }
}
