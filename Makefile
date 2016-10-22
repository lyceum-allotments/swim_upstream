CC = emcc
all: main.c engine.c engine.h utils.c utils.h sprite.c sprite.h decal.c decal.h \
 	actor.c actor.h bg_actor.c bg_actor.h waypoint_actor.c waypoint_actor.h \
	linkline_actor.c linkline_actor.h fish_actor.c fish_actor.h \
	hud_actor.c hud_actor.h level_end_screen.c level_end_screen.h \
	scenes.c scenes.h level1_setup_helpers.c level1_waypoint_initial_positions.h
	$(CC) main.c engine.c utils.c sprite.c decal.c actor.c bg_actor.c waypoint_actor.c linkline_actor.c fish_actor.c \
		hud_actor.c level_end_screen.c scenes.c \
		-O2 -s TOTAL_MEMORY=67108864 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]'\
	--preload-file assets -o swim_upstream.html


PC_CC = gcc

PC_CFLAGS = -pg -g -lm -Wall -lSDL2 -lSDL2_image -lSDL2_ttf

pc: main.c engine.c engine.h utils.c utils.h sprite.c sprite.h decal.c decal.h \
 	actor.c actor.h bg_actor.c bg_actor.h waypoint_actor.c waypoint_actor.h \
	linkline_actor.c linkline_actor.h fish_actor.c fish_actor.h \
	hud_actor.c hud_actor.h level_end_screen.c level_end_screen.h \
	scenes.c scenes.h level1_setup_helpers.c level1_waypoint_initial_positions.h
	$(PC_CC) main.c engine.c utils.c sprite.c decal.c actor.c bg_actor.c waypoint_actor.c linkline_actor.c fish_actor.c \
		hud_actor.c level_end_screen.c scenes.c ${PC_CFLAGS} -o swim_upstream

test_actor: test_actor.c actor.c actor.h
	$(PC_CC) test_actor.c actor.c -o test_actor

