
#ifdef SDL

#ifndef _EVENTS_SDL_
#define _EVENTS_SDL_

// functions prototypes
void events_sdl(Simu_realtime *realtime, int cur_t_usec);

#ifdef __cplusplus
extern "C" {
#endif
	double get_Joystick_axis(int joystickID, int axisID, Simu_realtime *realtime);
#ifdef __cplusplus
}
#endif

#endif
#endif
