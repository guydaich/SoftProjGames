#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL.h>
#include <SDL_video.h>

/* generic control struct */
typedef struct _control{
	/* type identifiers */
	int is_window;
	int is_panel;
	int is_button;
	int is_label; 
	/*drawing function pointer*/
	void (* draw)(); 
	/* caption for window */
	char *caption; 
	/* coordinates and sizes */
	int x; 
	int y;
	int h;
	int w;
	/*image path */
	char *img; 
	/* colors for tranparancy */
	int is_transparant;
	int R;
	int B;
	int G;
	/*surface to blit to, always window*/
	SDL_Surface *srfc;
	SDL_Surface *ownSurface;
	void (*pressedButton)(void* cur_game,void* ui_tree,int *quit,void* test_event);

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
void draw_ui_tree(element_cntrl root);
void draw_button(control *button, control *container);
void draw_label(control *label, control *container);
void draw_window(control* window);
void draw_panel(control* panel);
control* new_label(int x, int y, int w, int h, char *img, int R, int G, int B, int is_trans);
control* new_button(int x, int y, int w, int h, char *img, int R, int G, int B, int is_trans,char *caption);
control* new_window(int x, int y, int w, int h);
control* new_panel(int x, int y, int w, int h,int R, int G, int B);

void clear_game_panel(element_cntrl ui_tree);
void freeControlList(element_cntrl node);
#endif

