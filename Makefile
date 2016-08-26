CC = emcc
all: main.c engine.c engine.h utils.c utils.h sprite.c sprite.h decal.c decal.h \
 	actor.c actor.h bg_actor.c bg_actor.h waypoint_actor.c waypoint_actor.h
	$(CC) main.c engine.c utils.c sprite.c decal.c actor.c bg_actor.c waypoint_actor.c -O2 -s TOTAL_MEMORY=67108864 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets -o swim_upstream.html
