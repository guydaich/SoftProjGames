#include "controls.h"

#define DEBUG 1

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

int controlElementNum=0;
int buttomNum=0;
int labelNum=0;
int panelNum=0;
int windowNum=0;
int surfaceNum=0;


/*creates a rectangle, according to control size, position and panel boundaries*/
void get_rect(SDL_Rect *rect_dest, control *cntrl, control *parent_panel)
{
	rect_dest->x = cntrl->x + parent_panel->x;
	rect_dest->y = cntrl->y + parent_panel->y;
	rect_dest->w = MIN(cntrl->w, parent_panel->w - cntrl->x);
	rect_dest->h = MIN(cntrl->h, parent_panel->h - cntrl->y);

}


/* loads image/rect as surface and saves control surface and associated metrics*/
void handle_control_surface_load(control *cntrl, control *container)
{
	SDL_Surface *surface; 
	SDL_Rect *rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	
	char *err; 

	if (cntrl->ownSurface == NULL){
		if (cntrl->is_bg_img)
		{
			surface = SDL_LoadBMP(cntrl->img);
			if (surface == NULL)
			{
				err=SDL_GetError();
			printf("ERROR: to load t image: %s\n", SDL_GetError());
				SDL_FreeSurface(surface);
				return;
			}
		}
		else if (cntrl->is_bg_rect)
		{
			surface = SDL_CreateRGBSurface(SDL_HWSURFACE, cntrl->w, cntrl->h, 32, 0, 0, 0, 0);
			if (surface == NULL)
			{
				err=SDL_GetError();
				SDL_FreeSurface(surface);
				return;
			}
		}
		
		cntrl->srfc = container->srfc;
		cntrl->ownSurface=surface;
		
		/*find rect according to parent*/
		get_rect(rect,cntrl,container);
		
		/*find picture proportions*/
		cntrl->h = surface->h;
		cntrl->w = surface->w;
			
		/*update to final position on board */
		cntrl->offsetx = rect->x;
		cntrl->offsety = rect->y;
		
		cntrl->destination_rect = rect;

		/*set transparancy*/
		if (cntrl->is_transparant){
			SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
				SDL_MapRGB(surface->format, MAGNETAR, MAGNETAG, MAGNETAB));
		}
	}

}


/*gets a rectnagle for the text, in the middle of parent rect*/
void get_text_position(SDL_Rect *rect_source, char* text,TTF_Font *font, SDL_Rect *rect_dest)
{

	int w,h;
	if (TTF_SizeText(font,text,&w,&h)) {
    // perhaps print the current TTF_GetError()
		return;
	} 
	rect_dest->w = w; 
	rect_dest->h = h; 
	rect_dest->x = rect_source->x + (rect_source->w - w)/2;
	rect_dest->y = rect_source->y + (rect_source->h - h)/2;
}

int is_valid_rect(SDL_Rect *rect)
{
	/*basic*/
	if (rect->h < 0 || rect->w  < 0 || rect->x  < 0 || rect->y  < 0 ){
		return 1; 
	}
	/* relative to parent - TODO */
	return 0;
}

void make_rect(SDL_Rect *rect, int h, int w, int x, int y)
{

	rect->h = h;
	rect->w = w;
	rect->x = x;
	rect->y = y;
	return; 
}

/*draws control->caption text to control*/
void draw_caption_to_control(control *cntrl)
{
	char* err; 
	TTF_Font *font;
	SDL_Surface *surface;
	SDL_Color text_color = {255, 255, 255};
	SDL_Rect text_rect = {0,0,0,0};
	SDL_Rect soure_rect = {0,0,0,0};


	if (cntrl->caption == NULL)	
		return;

	/*make all text Calculations*/
	make_rect(&soure_rect, cntrl->h,cntrl->w,cntrl->offsetx,cntrl->offsety);
	//if (!is_valid_rect(&soure_rect))
	//	return; 

	TTF_Init();
	
	if ((font = TTF_OpenFont("arial.ttf", 16)) == NULL) {
		err=TTF_GetError();
		return;
	}
	
	get_text_position(&soure_rect, cntrl->caption, font, &text_rect);

	/*if text surface wan't previously loaded*/
	if (cntrl->text_surface == NULL)
	{
		if ((surface = TTF_RenderText_Blended(font, cntrl->caption, text_color))==NULL){
			err=TTF_GetError();
			SDL_FreeSurface(surface);
			return;
		}
		cntrl->text_surface = surface; 
	}

	/* Blit surface*/
	if (SDL_BlitSurface(cntrl->text_surface, NULL, cntrl->srfc, &text_rect) != 0){
		err=TTF_GetError();
		return;
	}
	
	return; 
}


/* return pointer to element containing pointer to give control*/
element_cntrl new_control_element(control* cntrl)
{
	element_cntrl elem = (element_cntrl)malloc(sizeof(struct element_s_cntrl));
	controlElementNum++;
	elem->cntrl = cntrl;
	elem->next=NULL;
	elem->prev=NULL;
	elem->children=NULL;
	elem->parent=NULL;
	return elem;
}

linked_list_cntrl new_control_list()
{
	linked_list_cntrl lst = (linked_list_cntrl)malloc(sizeof(struct linked_list_s_cntrl));
	lst->head = NULL;
	lst->tail = NULL;
	return lst;
}

void add_control_element_to_list(linked_list_cntrl list, element_cntrl elem)
{
	element_cntrl prev_tail;

	if (list->head == NULL)
	{	
		list->head = elem;
		list->tail = elem;
		list->tail->next = NULL;
	}
	else
	{
		prev_tail =			list->tail;
		prev_tail->next =	elem;
		elem->prev =	prev_tail;
		list->tail =		elem;
		list->tail->next = NULL;
	}

}

void set_list_as_children(linked_list_cntrl list, element_cntrl elem)
{
	element_cntrl cur_elem,run;
	
	//free prev list
	if (elem->children!=NULL){
		for (run=elem->children->head;run!=NULL;run=run->next)
		{
			freeControlList(run);
		}
		free(elem->children);
	}

	// set list as children
	elem->children = list; 
	
	// for every child, assign parent
	for (cur_elem = list->head; cur_elem!= NULL ;cur_elem=cur_elem->next)
			{
				/*draw each child*/
				cur_elem->parent = elem;
			} 
}

/*gets UI tree root, draws it in DFS trasverse*/
void draw_ui_tree(element_cntrl root)
{
		element_cntrl cur_elem;
		/* call the controls drawing function */
		if (root->parent== NULL)
			root->cntrl->draw(root->cntrl,NULL);
		else
			root->cntrl->draw(root->cntrl,root->parent->cntrl);

		/* reached leaves */
		if (root->children != NULL)
		{
			/*iterate over children*/
			for (cur_elem = root->children->head; cur_elem!= NULL ;cur_elem=cur_elem->next)
			{
				/*draw each child*/
				draw_ui_tree(cur_elem);
			}
		}
		else
		{
			return;
		}
}

/*init functions*/

control* new_label(int x, int y, int w, int h, char *img, int R, int G, int B, int is_trans, char* caption)
{
	// allocate button
	control *label = (control*)malloc(sizeof(control));
	labelNum++;
	label->is_button = 0;
	label->is_label = 1;
	label->is_panel = 0;
	label->is_window = 0;
	label->x = x;
	label->y = y;
	label->w = w;
	label->h = h;	
	label->is_transparant = is_trans;
	label->R = R;
	label->B = B;
	label->G = G;
	label->img = img;
	label->draw=draw_label;
	label->ownSurface=NULL;
	label->caption=caption;
	label->pressedButton=emptryButton;
	label->srfc=NULL;
	label->buttonChoise=0;
	label->text_surface=NULL;
	label->destination_rect=NULL;
	label->is_grid=0;
	label->is_bg_img =1;
	label->is_bg_rect =0;


	return label;
}


control* new_button(int x, int y, char *img, int is_trans ,char *caption, int is_grid)
{
	// allocate button
	control *button = (control*)malloc(sizeof(control));
	buttomNum++;
	button->is_button = 1;
	button->is_label = 0;
	button->is_panel = 0;
	button->is_window = 0;
	button->x = x;
	button->y = y;
	button->w = 0;
	button->h = 0;	
	button->is_transparant = is_trans;
	button->R = 0;
	button->B = 0;
	button->G = 0;
	button->img = img;
	button->draw=draw_button;
	button->caption=caption;
	button->ownSurface=NULL;
	button->pressedButton=emptryButton;
	button->srfc=NULL;
	button->text_surface=NULL;
	button->buttonChoise=0;
	button->destination_rect=NULL;
	button->is_grid=is_grid;
	button->is_bg_img =1;
	button->is_bg_rect =0;



	return button;
}

/*panel is a logical sub-window, meant to organise a part of the game area*/
control* new_panel(int x, int y, int w, int h, int R, int B, int G)
{
	// allocate window
	control *panel = (control*)malloc(sizeof(control));
	panelNum++;
	panel->is_button = 0;
	panel->is_label = 0;
	panel->is_panel = 1;
	panel->is_window = 0;
	panel->x = x;
	panel->y = y;
	panel->h = h;
	panel->w = w;
	panel->R = R;
	panel->B = B;
	panel->G = G;
	panel->draw = draw_panel;// drawing funct
	panel->ownSurface=NULL;
	panel->caption=NULL;
	panel->pressedButton=emptryButton;
	panel->srfc=NULL;
	panel->buttonChoise=0;
	panel->text_surface = NULL;
	panel->destination_rect=NULL;
	panel->is_grid=0;
	panel->is_bg_img =0;
	panel->is_bg_rect =1;

	return panel;
}

control* new_window(int x, int y, int w, int h)
{
	// allocate window
	control *window = (control*)malloc(sizeof(control));
	windowNum++;
	window->is_button = 0;
	window->is_label = 0;
	window->is_panel = 0;
	window->is_window = 1;
	window->x = x;
	window->y = y;
	window->h = h;
	window->w = w;
	window->draw = draw_window;// drawing funct
	window->ownSurface=NULL;
	window->caption=NULL;
	window->pressedButton=emptryButton;
	window->srfc=NULL;
	window->buttonChoise=0;
	window->text_surface = NULL;
	window->destination_rect=NULL;
	window->is_grid=0;
	window->is_bg_img =0;
	window->is_bg_rect =0;


	return window;
}


/* drawing function*/
void draw_button(control *button, control *container)
{

	SDL_Rect rect = {0,0,0,0}; 
	SDL_Surface *surface;
	char *err; 

	handle_control_surface_load(button,container);

	/* Blit */
	if (SDL_BlitSurface(button->ownSurface,NULL, container->srfc, button->destination_rect) != 0){
		char *x= SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}
	
	/*add caption*/
	if (button->caption != NULL)
	{
		draw_caption_to_control(button);	
	}
	
}


void draw_label(control *label, control *container)
{
	char *err; 
	SDL_Rect rect = {0,0,0,0}; 	
	SDL_Surface *surface;

	handle_control_surface_load(label,container);
	
	if (SDL_BlitSurface(label->ownSurface,NULL, container->srfc, label->destination_rect) != 0)
	{
		char *x= SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}

	if (label->caption != NULL)
	{
		draw_caption_to_control(label);	
	}
}


void draw_window(control* window)
{
	SDL_WM_SetCaption("SDL Test", "SDL Test");
	if (window->srfc==NULL)
	{
		window->srfc = SDL_SetVideoMode(window->w,window->h,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
		surfaceNum++;
		window->x = 0; 
		window->y = 0; 
		window->offsetx = 0; 
		window->offsety = 0; 
	}
}

/*add this functionality to window as well. use it to paiunt background in white*/
void draw_panel(control* panel, control *container)
{
	SDL_Surface *surface;
	char *err; 
	SDL_Rect rect;

	handle_control_surface_load(panel,container);

	/* blit panel surface*/
	if (SDL_FillRect(container->srfc,panel->destination_rect, 
		SDL_MapRGB(panel->ownSurface->format, panel->R, panel->G, panel->B)) != 0)
	{
		err=SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}
}


/*BFS on UI tree, to find elemnt, according to final (offset) coordinates*/
void find_element_by_coordinates(element_cntrl root,int x, int y, element_cntrl *target)
{
	element_cntrl cur_elem;
	/*if is a button and a tree leaf*/
	if ((root->cntrl->is_grid))
	{
		if ((x >= root->cntrl->offsetx && x <= (root->cntrl->offsetx + root->cntrl->w) &&
			(y >= root->cntrl->offsety && y <= (root->cntrl->offsety + root->cntrl->h))))
		{
			*target = NULL; 
			for (cur_elem = root->children->head; 
			cur_elem!= NULL ;cur_elem=cur_elem->next){
				find_element_by_coordinates(cur_elem,x,y,target);
			}
			if (*target == NULL)
			{
				*target = root;
			}
			return;		
		}
		
	}
	else if (root->cntrl->is_button && root->children == NULL)
	{
		if ((x >= root->cntrl->offsetx && x <= (root->cntrl->offsetx + root->cntrl->w) &&
			(y >= root->cntrl->offsety && y <= (root->cntrl->offsety + root->cntrl->h))))
		{
			*target = root; 
			return;
		}
	}
	//if out of bounds-return
	if(x< root->cntrl->offsetx){
		return;
	}
	if(y<root->cntrl->offsety){
		return;
	}
	if (x > (root->cntrl->offsetx + root->cntrl->w)){
		return;
	}
	if (y > (root->cntrl->offsety + root->cntrl->h)){
		return;
	}

	if (root->children == NULL){
		return;
	}
	else{
		for (cur_elem = root->children->head; 
			cur_elem!= NULL ;cur_elem=cur_elem->next){
				find_element_by_coordinates(cur_elem,x,y,target);
			}
	}
	return;
}

void clear_game_panel(element_cntrl ui_tree)
{
	element_cntrl game_panel,pre_tail;

	/*no previous game elements*/
	if(ui_tree->children==NULL ||
		ui_tree->children->head==ui_tree->children->tail) 
	{
		return;
	}
	pre_tail=ui_tree->children->tail->prev;
	game_panel=ui_tree->children->tail; //assumption!
	freeControlList(game_panel);
	pre_tail->next=NULL;
	ui_tree->children->tail=pre_tail;
}

/* Recursively free all Dynamically Allocated Memory : 
 * SDL objects, strings, and data structure */
void freeControlList(element_cntrl node)
{
	element_cntrl cur_elem,next_elem;

	if (node == NULL){
		return;
	}

	/*free node children*/
	if (node->children != NULL){
		for (cur_elem=node->children->head;
			cur_elem != NULL; cur_elem=next_elem){
			next_elem=cur_elem->next;
			freeControlList(cur_elem);
		}
	}

	/*free SDL surface associated with control*/
	if (node->cntrl->ownSurface != NULL){
		SDL_FreeSurface(node->cntrl->ownSurface);
	}
	node->cntrl->ownSurface = NULL;
	if (node->cntrl->text_surface != NULL){
		SDL_FreeSurface(node->cntrl->text_surface);
	}
	if (node->cntrl->destination_rect != NULL){
		free(node->cntrl->destination_rect);
	}

	
	if (node->cntrl->is_button==1)
	{
		buttomNum--;
	}
	if (node->cntrl->is_label==1)
	{
		labelNum--;
	}
	if (node->cntrl->is_panel==1)
	{
		panelNum--;
	}
	if (node->cntrl->is_window==1)
	{
		windowNum--;
	}
	
	/*free node caption*/
	if (node->cntrl->caption != NULL 
		&& node->cntrl->caption[2] == NULL)
	{
		free(node->cntrl->caption);
	}
	/*free control and node*/
	free(node->cntrl);
	free(node);

	controlElementNum--;

}

void emptryButton(void* cur_game,element_cntrl* ui_tree,int *choice,SDL_Event* test_event)
{
	return;
}
