#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <iostream>

#include <stdio.h>
#if defined(_WIN32) && !defined(_XBOX)
#include <windows.h>
#endif
#include "libretro.h"

#include "common.hpp"
#include "apu.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "ppu.hpp"
#include "input.hpp"

static struct retro_log_callback logging;
static retro_log_printf_t log_cb;
static bool use_audio_cb;
static float last_aspect;
static float last_sample_rate;
char retro_base_directory[4096];
char retro_game_path[4096];
static retro_environment_t environ_cb;

uint64_t frameNum = 0;
int screenWidth, screenHeight, screenTotalPixels;
void (*wh_callback)(int w, int h);
void (*syscall_callback)(int address, int value);

void change_wh(int w, int h) {
	if(w<0 || w>maxScreenWidth || h < 0 || h > maxScreenHeight) {
		return;
	}
	retro_game_geometry geo;
	geo.base_width = w;
	geo.base_height = h;
	geo.aspect_ratio = 0.0f;
	if(environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &geo)) {
		screenWidth = w;
		screenHeight = h;
		screenTotalPixels = screenWidth * screenHeight;
	}
}

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   va_list va;
   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}

void retro_init(void)
{
   const char *dir = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
   {
      snprintf(retro_base_directory, sizeof(retro_base_directory), "%s", dir);
   }
   
   wh_callback = &change_wh;
   
   input::init();
   apu::init();
   ppu::init();
   cpu::init();
}

void retro_deinit(void)
{   
   apu::deinit();
   ppu::deinit();
   cpu::deinit();
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
  // log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "Puma";
   info->library_version  = "0.1";
   info->need_fullpath    = true;
   info->valid_extensions = "";
}

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   float aspect                = 0.0f;
   float sampling_rate         = audioSampleRate*1.0f;


   info->geometry.base_width   = screenWidth;
   info->geometry.base_height  = screenHeight;
   info->geometry.max_width    = screenWidth;
   info->geometry.max_height   = screenHeight;
   info->geometry.aspect_ratio = aspect;

   last_aspect                 = aspect;
   last_sample_rate            = sampling_rate;
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;

   static const struct retro_controller_description controllers[] = {
      { "Nintendo DS", RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 0) },
   };

   static const struct retro_controller_info ports[] = {
      { controllers, 1 },
      { NULL, 0 },
   };

   cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_reset(void)
{

}

static void update_input(void)
{
	input::previouslyPressedButtons = input::pressedButtons;
	input::pressedButtons = 0;
	uint16_t state;
	
	for (int i = 0; i< numButtons; i++) {
		state = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, input::buttonMapping[i]);
		if(state) {
			input::pressedButtons |= (1 <<i);
		}
	}
	
	input::justPressedButtons = (~ input::previouslyPressedButtons) & input::pressedButtons;
	input::justReleasedButtons = input::previouslyPressedButtons & (~ input::pressedButtons);
}


static void check_variables(void)
{

}

static void audio_callback(void)
{
   int16_t* buf = apu::callback();
   
   for(int i = 0; i<samplesPerFrame; i++) {
	   audio_cb(*buf++, *buf++);
   }
   
}

static void audio_set_state(bool enable)
{
   (void)enable;
}

static void keyboard_cb(bool down, unsigned keycode,
      uint32_t character, uint16_t mod)
{
	if(down) {
		std::cout<<"Pressed key "<<keycode<<std::endl;
	}
}

void retro_run(void)
{
   update_input();
   input::beforeFrame();
	
	ppu::beforeFrame();
	
	cpu::frame();
	ppu::afterFrame();
	 apu::afterFrame();

	video_cb(ppu::getBuffer(), screenWidth, screenHeight, screenWidth*sizeof(uint32_t)); 
	
	frameNum++;

   bool updated = false;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();
}

bool retro_load_game(const struct retro_game_info *info)
{
   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
      { 0 },
   };

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      log_cb(RETRO_LOG_INFO, "XRGB8888 is not supported.\n");
      return false;
   }

   snprintf(retro_game_path, sizeof(retro_game_path), "%s", info->path);
   struct retro_audio_callback audio_cb = { audio_callback, audio_set_state };
   use_audio_cb = environ_cb(RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK, &audio_cb);

	struct retro_keyboard_callback cb = { keyboard_cb };
   environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cb);

   check_variables();

	std::cout<<retro_game_path<<std::endl;
	if(!bus::load(retro_game_path)) {
		return false;
	}
	
	cpu::onLoad();
	ppu::afterLoad();

	frameNum = 0;
   (void)info;
   return true;
}

void retro_unload_game(void)
{

}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   return false;
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data_, size_t size)
{
   return false;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return false;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

