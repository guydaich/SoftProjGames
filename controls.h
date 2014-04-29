/* controls.h */
/* This class handles the controls of the GUI. It defines the general control Factory 
 * and specifies concrete control initializers. all controls have a specific drawing
 * drawing function */

#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL.h>
#include <SDL_video.h>
#include "SDL_ttf.h"

#define MAGNETAR 255
#define MAGNETAG 0
#define MAGNETAB 255

#define MAX_CAPTION_LINES 25
#define FONT_SIZE 16
#define GENERIC_BTN_PATH "./gfx/generic_button.bmp"
#define GAME_WINDOW_NAME "gamesprog"

/* Data structres - control abstract class, and control linked list*/

/* generic control struct */
typedef struct _control{
	/* identifiers */
	int				is_window;			/*boolean indicating this control is a window*/
	int				is_panel;			/*boolean indicating this control is a panel*/
	int				is_button;			/*boolean indicating this control is a button*/
	int				is_label;			/*boolean indicating this control is a label*/

	/* position */
	int				x;					/* relative x-coordinate within panel */
	int				y;					/* relative y-coordinate within panel */
	int				h;					/* control rect height */		
	int				w;					/* control rect width */
	int				offsetx;			/* abosulte x-coordinate in window*/
	int				offsety;			/* abosulte y-coordinate in window*/
	SDL_Rect		*destination_rect;	/* rect for blitting onto this surface */
	
	/* captions */
	char			*caption;			/* caption to write on control */
	SDL_Surface		*text_surface;		/* surface of written caption */
	SDL_Surface		**multitext;		/* surface array of multiline texts*/
	int				num_texts;			/* size of text surface array */

	/* background */
	char			*img;				/* image path to this control's background*/
	int				is_transparant;		/* boolean indicating if image is to be loaded transparant */
	int				R;					/* R-elemnt of background, if BG is simply and RGB surface */
	int				B;					/* B-elemnt of background, if BG is simply and RGB surface */
	int				G;					/* G-elemnt of background, if BG is simply and RGB surface */
	int				is_bg_img;			/* boolean indicating if BG is an image */
	int				is_bg_rect;			/* boolean indicating if BG is an RGB rect */
	
	/* surfaces */
	SDL_Surface		*srfc;				/* the surface of the panel blitted onto */
	SDL_Surface		*ownSurface;		/* this controls own surface*/

	/*misc*/
	int				button_choice;		/* a button behavior associated with this control*/
	int				is_grid;			/* determines finding of element according to coordinates, in the game grid area*/
	
	/*methods */
	int				(*draw)(struct _control *mySelf,struct _control *container);	/*drawing function relevant to this element*/
	int				(*pressed_button)(int *choise,SDL_Event* test_event);			/*function to be invoked on clicking the button*/


} control;

/* a link in a control linked list represantation of a tree*/
struct element_s_cntrl {
	control						*cntrl;		/* the actual control stored in this link */
	struct linked_list_s_cntrl	*children;	/* children of link */
	struct element_s_cntrl		*parent;	/* parent of link */
	struct element_s_cntrl		*next;		/* next link */	
	struct element_s_cntrl		*prev;		/*	previous link */
	
};

typedef struct element_s_cntrl*  element_cntrl;

/* a linked list of controls*/
struct linked_list_s_cntrl {
	element_cntrl head;
	element_cntrl tail;
};

typedef struct linked_list_s_cntrl *  linked_list_cntrl;

/* functions 
 * detailed documentation next to implementation */

/*data structure functions*/
element_cntrl		new_control_element(control* cntrl);
linked_list_cntrl	new_control_list();
void				add_control_element_to_list(linked_list_cntrl list, element_cntrl elem);
void				set_list_as_children(linked_list_cntrl list, element_cntrl elem);
void				find_element_by_coordinates(element_cntrl root,int x, int y, element_cntrl *target);
int					add_control_to_element_list(control* control,linked_list_cntrl parent_element_list);

/* control initializers */
control*	new_label(int x, int y, int w, int h, char *img, int R, 
					  int G, int B, int is_trans,char *caption);
control*	new_button(int x, int y, char *img, int is_trans,char *caption, int is_grid);
control*	new_window(int x, int y, int w, int h);
control*	new_panel(int x, int y, int w, int h,int R, int G, int B, int is_bg_rect);
int			new_generic_button(linked_list_cntrl parent_element_list,int x,int y,char* caption,
							   int (*pressed_button)(int *choice,SDL_Event* test_event),int button_choice);
int			empty_click_handle(int *quit,SDL_Event* test_event);

/* control drawing*/
int			draw_button(control *button, control *container);
int			draw_label(control *label, control *container);
int			draw_window(control* window, control *container);
int			draw_panel(control* panel, control *container);

/* ui tree drawing */
int			draw_ui_tree(element_cntrl root);
int			draw_with_panel(element_cntrl draw_cntrl, element_cntrl owning_panel);

/* memory cleanup*/
void		free_control(control *cntrl);
void		free_control_list(element_cntrl node);
void		clear_game_panel(element_cntrl ui_tree);
void		free_detached_list(linked_list_cntrl parent_element_list);

#endif

