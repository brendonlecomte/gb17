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
#include <memory>
#include <array>

#include "libretro.h"
#include "Gameboy.h"

Gameboy* gb = new Gameboy();

// Callbacks
static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

static std::array<std::pair<std::size_t, Buttons>, 8> key_map =
{
  {
    { RETRO_DEVICE_ID_JOYPAD_UP,     Buttons::UP },
    { RETRO_DEVICE_ID_JOYPAD_DOWN,   Buttons::DOWN },
    { RETRO_DEVICE_ID_JOYPAD_LEFT,   Buttons::LEFT },
    { RETRO_DEVICE_ID_JOYPAD_RIGHT,  Buttons::RIGHT },
    { RETRO_DEVICE_ID_JOYPAD_A,      Buttons::A },
    { RETRO_DEVICE_ID_JOYPAD_B,      Buttons::B },
    { RETRO_DEVICE_ID_JOYPAD_START,  Buttons::START },
    { RETRO_DEVICE_ID_JOYPAD_SELECT, Buttons::SELECT }
  }
};

void process_inputs(void) {
  input_poll_cb();
  for(uint8_t i =0; i < 8; i ++){
    auto val = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, key_map[i].first);
    gb->pressButton(key_map[i].second, val);
  }

}

// Cheats
void retro_cheat_reset(void) {}
void retro_cheat_set(unsigned index, bool enabled, const char *code) {}

// Load a cartridge
bool retro_load_game(const struct retro_game_info *info)
{
    gb->reset();
    gb->loadRom(info->path);

    if (info && info->data) { // ensure there is ROM data
        return true; //gb->loadRom((const uint8_t*)info->data, info->size);
    }
    return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) { return false; }

// Unload the cartridge
void retro_unload_game(void) { gb->unloadRom();
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
    bool no_rom = false;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {} //use this for a frames audio
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; } //use this for a sample of audio
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_init(void)
{
    gb->reset();
}


/*
 * Tell libretro about this core, it's name, version and which rom files it supports.
 */
void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = gb->getName();
    info->library_version = gb->getVersion();
    info->need_fullpath = true;
    info->valid_extensions = "gb | gbc";
}



void retro_reset(void) {
}

const uint16_t col = 0xFFFF;
const uint16_t frame_width = 160;
const uint16_t frame_height = 144;
const uint32_t frame_size = frame_width*frame_height;
const uint32_t frame_pitch = sizeof(uint16_t) * frame_width;
uint16_t framebuffer[frame_size] = {col};
uint8_t audiobuffer[1] = {0};

/*
 * Tell libretro about the AV system; the fps, sound sample rate and the
 * resolution of the display.
 */
void retro_get_system_av_info(struct retro_system_av_info *info) {

    int pixel_format = RETRO_PIXEL_FORMAT_0RGB1555;

    memset(info, 0, sizeof(*info));
    info->timing.fps            = 59.7f; //16.7ms per frame
    info->timing.sample_rate    = 441000;
    info->geometry.base_width   = frame_width;
    info->geometry.base_height  = frame_height;
    info->geometry.max_width    = frame_width;
    info->geometry.max_height   = frame_height;
    info->geometry.aspect_ratio = float(frame_width) / float(frame_height);

    // the performance level is guide to frontend to give an idea of how intensive this core is to run
    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

// Run a single frames with out Vectrex emulation.
void retro_run(void)
{
    gb->runFrame(framebuffer, audiobuffer);
    process_inputs();
    video_cb(framebuffer, frame_width, frame_height, frame_pitch);
}
