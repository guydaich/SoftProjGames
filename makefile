CC=gcc
CFLAGS=-g -Wall -lm -std=c99 -pedantic-errors
INCS=-I/usr/include/SDL -Isdl-ttf-lib
OBJS=game.o general_game.o tic_tac_toe_bl.o connect4_bl.o reversi_bl.o ttc_ui.o connect4_ui.o reversi_ui.o controls.o save_game.o minimax.o buttonActions.o windowsDrawing.o

all: gamesprog

clean:
	-rm gamesprog $(OBJS)

gamesprog: $(OBJS)
	$(CC) -o gamesprog $(OBJS) -lSDL -Lsdl-ttf-lib -lSDL_ttf $(CFLAGS)

game.o:game.c game.h windowsDrawing.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

general_game.o:general_game.c general_game.h game.h tic_tac_toe_bl.h connect4_bl.h reversi_bl.h
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
	$(CC) $(CFLAGS) -c $*.c

save_game.o:save_game.c save_game.h game_enum.h
	$(CC) $(CFLAGS) -c $*.c

buttonActions.o:buttonActions.c buttonActions.h general_game.h game.h tic_tac_toe_bl.h connect4_bl.h reversi_bl.h game_enum.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)

windowsDrawing.o:windowsDrawing.c windowsDrawing.h buttonActions.h save_game.h general_game.h
	$(CC) $(CFLAGS) -c $*.c $(INCS)
