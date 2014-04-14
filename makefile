CC=gcc
CFLAGS=-g -Wall
INCS=-I/usr/include/SDL
OBJS=game.o general_game.o tic_tac_toe_bl.o connect4_bl.o reversi_bl.o ttc_ui.o connect4_ui.o reversi_ui.o controls.o save_game.o minimax.o

all: games

clean:
	-rm games $(OBJS)

games: $(OBJS)
	$(CC) -o games $(OBJS) -lSDL -lSDL_ttf

game.o:game.c game.h general_game.h save_game.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

general_game.o:general_game.c general_game.h game.h tic_tac_toe_bl.h connect4_bl.h reversi_bl.h game_enum.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

tic_tac_toe_bl.o:tic_tac_toe_bl.c tic_tac_toe_bl.h ttc_ui.h minimax.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

connect4_bl.o:connect4_bl.c connect4_bl.h connect4_ui.h minimax.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

reversi_bl.o:reversi_bl.c reversi_bl.h reversi_ui.h minimax.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

ttc_ui.o:ttc_ui.c ttc_ui.h controls.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

connect4_ui.o:connect4_ui.c connect4_ui.h controls.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

reversi_ui.o:reversi_ui.c reversi_ui.h controls.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

controls.o:controls.c controls.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

minimax.o:minimax.c minimax.h
	$(CC) -c $*.c

save_game.o:save_game.c save_game.h game_enum.h
	$(CC) -c $*.c
