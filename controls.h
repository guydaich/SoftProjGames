#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_ttf.h>

#define MAGNETAR 255
#define MAGNETAG 0
#define MAGNETAB 255

/* generic control struct */
typedef struct _control{
	//* type identifiers */
	int is_window;
	int is_panel;
	int is_button;
	int is_label; 
	/*drawing function pointer*/
	int (* draw)(struct _control *mySelf,struct _control *container); 
	/* caption for window */
	char *caption; 
	/* coordinates and sizes */
	int x;// x and y offset are relative to the x and y offset of the controls parent panel 
	int y;
	int h;//actual sizes.they are caculated in the code
	int w;
	int offsetx;//actual coordinates (unlike x and y). they are caculated in the code
	int offsety;
	SDL_Rect *destination_rect;//rect for SDL_Blit.uses actual coordinates
	char *img; //background images path
	// colors for tranparancy 
	int is_transparant;
	//colors in case there is no background image (a RGBSurface will be created to serve as background)
	int R;
	int B;
	int G;
	SDL_Surface *srfc;//the srfc is the surface of the window upon which the control will be drawn
	SDL_Surface *ownSurface;//this surface represents the control to the user
	SDL_Surface *text_surface;
	SDL_Surface **multitext;
	int num_texts;//number of lines in control's text
	int (*pressedButton)(int *choise,SDL_Event* test_event);//which function sould be invoked in this button is "pressed".
	int buttonChoise;
	/*if is grid - when pressed, will act like a lowest-level button*/
	int is_grid; 
	/*flags for loading background from image or painting RGB rect*/
	int is_bg_img; 
	int is_bg_rect;


} control;

/*linked list for control tree*/

struct element_s_cntrl {
	control	*cntrl;
	struct linked_list_s_cntrl *children;
	struct element_s_cntrl	*parent;
	struct element_s_cntrl	*next;
	struct element_s_cntrl	*prev;
	
};

typedef struct element_s_cntrl*  element_cntrl;

struct linked_list_s_cntrl {
	element_cntrl head;
	element_cntrl tail;
};

typedef struct linked_list_s_cntrl *  linked_list_cntrl;


/*element functions*/
element_cntrl new_control_element(control* cntrl);
linked_list_cntrl new_control_list();
void add_control_element_to_list(linked_list_cntrl list, element_cntrl elem);
void set_list_as_children(linked_list_cntrl list, element_cntrl elem);
void find_element_by_coordinates(element_cntrl root,int x, int y, element_cntrl *target);


/* control functions */
int draw_ui_tree(element_cntrl root);
int draw_button(control *button, control *container);
int draw_label(control *label, control *container);
int draw_window(control* window);
int draw_panel(control* panel, control *container);
control* new_label(int x, int y, int w, int h, char *img, int R, int G, int B, int is_trans,char *caption);
control* new_button(int x, int y, char *img, int is_trans,char *caption, int is_grid);
control* new_window(int x, int y, int w, int h);
control* new_panel(int x, int y, int w, int h,int R, int G, int B, int is_bg_rect);
void free_control(control *cntrl);
int draw_with_panel(element_cntrl draw_cntrl, element_cntrl owning_panel);

void clear_game_panel(element_cntrl ui_tree);
void freeControlList(element_cntrl node);
int emptryButton(int *quit,SDL_Event* test_event);

int newButtonGeneric(linked_list_cntrl fathersList,int x,int y,char* caption,int (*pressedButton)(int *choice,SDL_Event* test_event),int buttonChoise);
int addNewControlToList(control* control,linked_list_cntrl fathersList);
void freeUnconnectedList(linked_list_cntrl fathersList);
#endif

