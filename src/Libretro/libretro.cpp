/*
Copyright (C) 2016 Beardypig

This file is part of Vectrexia.

Vectrexia is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Vectrexia is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Vectrexia.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libretro.h"

// Callbacks
static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

// Cheats
void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned index, bool enabled, const char *code) {}

// Load a cartridge
bool retro_load_game(const struct retro_game_info *info)
{
    // Reset the Vectrex, clears the cart ROM and loads the System ROM
    // vectrex->Reset();

    if (info && info->data) { // ensure there is ROM data
        return true; //vectrex->LoadCartridge((const uint8_t*)info->data, info->size);
    }

    return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) { return false; }

// Unload the cartridge
void retro_unload_game(void) { //vectrex->UnloadCartridge();
}

unsigned retro_get_region(void) { return RETRO_REGION_PAL; }

// libretro unused api functions
void retro_set_controller_port_device(unsigned port, unsigned device) {}


void *retro_get_memory_data(unsigned id) { return NULL; }
size_t retro_get_memory_size(unsigned id){ return 0; }

// Serialisation methods
size_t retro_serialize_size(void) { return 0; }
bool retro_serialize(void *data, size_t size) { return false; }
bool retro_unserialize(const void *data, size_t size) { return false; }

// End of retrolib
void retro_deinit(void) {}

// libretro global setters
void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
    bool no_rom = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {} //use this for a frames audio
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; } //use this for a sample of audio
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_init(void)
{
    // vectrex->Reset();
}


/*
 * Tell libretro about this core, it's name, version and which rom files it supports.
 */
void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = "MyGB"; //vectrex->getName();
    info->library_version = "0.0.3"; //vectrex->GetVersion();
    info->need_fullpath = true;
    info->valid_extensions = "gb";
}

/*
 * Tell libretro about the AV system; the fps, sound sample rate and the
 * resolution of the display.
 */
void retro_get_system_av_info(struct retro_system_av_info *info) {

    int pixel_format = RETRO_PIXEL_FORMAT_0RGB1555;

    memset(info, 0, sizeof(*info));
    info->timing.fps            = 59.7f; //16.7ms per frame
    info->timing.sample_rate    = 441000;
    info->geometry.base_width   = 160;
    info->geometry.base_height  = 144;
    info->geometry.max_width    = 160;
    info->geometry.max_height   = 144;
    info->geometry.aspect_ratio = 160.0f / 144.0f;

    // the performance level is guide to frontend to give an idea of how intensive this core is to run
    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

void retro_reset(void) {
}

uint16_t col = 0x00F;
const uint16_t frame_width = 160;
const uint16_t frame_height = 144;
const uint32_t frame_size = frame_width*frame_height;
const uint32_t frame_pitch = sizeof(uint16_t) * frame_width;
uint16_t framebuffer[frame_size];

// Run a single frames with out Vectrex emulation.
void retro_run(void)
{
    // gb->runFrame(framebuffer, audioBuffer);
    for (int i = 0; i < 1200; i++) {
        audio_cb(1, 1);
    }

    memset(framebuffer, col, frame_size*2); //test random colour
    video_cb(framebuffer, frame_width, frame_height, frame_pitch);
}
