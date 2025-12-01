/** \file conf.cpp
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

// Configuration loader

#include <fstream>
#include <string>
#include "conf.h"
#include "ini_parser.hpp"

Config load_config(const std::string& fpath) {
    Config config;
    iniparser::ini ini;
    std::ifstream in(fpath);
    if (in.fail()) {
        std::cout << "Could not read config file " << fpath << std::endl;
        std::cout << "Default settings will be used." << std::endl;
    } else {
        in >> ini;
    }
    
    config.framerate_intro = ini["render"]["framerate_intro"] | 20;
    config.framerate = ini["render"]["framerate"] | 30;
    config.ticks_per_second = ini["render"]["tickrate"] | 1000;
    config.ticks_per_frame = config.ticks_per_second / config.framerate;
    config.ticks_per_frame_intro = config.ticks_per_second / config.framerate_intro;

    config.render_width = ini["render"]["width"] | 320;
    config.render_height = ini["render"]["height"] | 200;
    config.window_scale_x = ini["render"]["scale_x"] | 2.0;
    config.window_scale_y = ini["render"]["scale_y"] | 2.0;
    config.window_width = config.render_width * config.window_scale_x;
    config.window_height = config.render_height * config.window_scale_y;

    config.cosm_pixels = ini["cosm"]["num_pixels"] | 250;
    config.cosm_objects = ini["cosm"]["num_objects"] | 250;
    config.situation_file = ini["cosm"]["situation_file"] | "";
    config.definition_file = ini["cosm"]["definition_file"] | "PIXELS.DEF";

    config.audio_enabled = ini["audio"]["enabled"] | true;

    return config;
}

const Config& get_config() {
    // will only parse the file on the first call
    static const Config conf = load_config(CONFIG_FILE_NAME);
    return conf;
}