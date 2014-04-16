#include "controls.h"

#define DEBUG 1

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define MAX_CAPTION_LINES 5

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


/* loads image/rect as surface 
 * saves control surface 
 * returns -1 on failure*/
int handle_control_surface_load(control *cntrl, control *container)
{
	SDL_Surface *surface=NULL; 
	SDL_Rect *rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));	

	/*no surface assoicated with control yet*/
	if (cntrl->ownSurface == NULL)
	{
		if (cntrl->is_bg_img) // control has background image
		{
			surface = SDL_LoadBMP(cntrl->img);
			if (surface == NULL)
			{
				SDL_FreeSurface(surface);
				return -1;
			}
		}
		else if (cntrl->is_bg_rect) // control has background color
		{
			surface = SDL_CreateRGBSurface(SDL_HWSURFACE, 
				cntrl->w, cntrl->h, 32, 0, 0, 0, 0);
			if (surface == NULL)
			{
				SDL_FreeSurface(surface);
				return -1;
			}
		}
		
		/*save offsets and destination rectangles*/
		if (cntrl->is_bg_rect || cntrl->is_bg_img)
		{
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
		/*control has no surface*/
		else 
		{
			/* assign all critical info */
			cntrl->srfc = container->srfc;
			cntrl->ownSurface=(SDL_Surface*)NULL;
			get_rect(rect,cntrl,container);
			cntrl->offsetx = rect->x;
			cntrl->offsety = rect->y;
			cntrl->destination_rect = rect;
		}
	}
	return 1;
}

/*gets a rectnagle for the text, in the middle of parent rect*/
int get_text_position(SDL_Rect *rect_source, char* text,TTF_Font *font, SDL_Rect *rect_dest)
{

	int w,h;
	if (TTF_SizeText(font,text,&w,&h)) {
		return -1;
	} 
	
	if (w > rect_source->w)
			return -1;
	if (h > rect_source->h)
			return -1;

	rect_dest->w = w; 
	rect_dest->h = h; 
	rect_dest->x = rect_source->x + (rect_source->w - w)/2;
	rect_dest->y = rect_source->y + (rect_source->h - h)/2;

	return 1;
}

/* creates a destination rect for text,
 * with lines aligned to upper left
 * with offset according to previous line*/
int get_text_position_multi(SDL_Rect *rect_source, char* text,TTF_Font *font, SDL_Rect *rect_dest,int offset)
{

	int w,h;
	if (TTF_SizeText(font,text,&w,&h)) {
		return -1;
	} 
	rect_dest->w = w; 
	rect_dest->h = h; 
	rect_dest->x = rect_source->x;
	rect_dest->y = rect_source->y+offset;
	
	/* check for overflow */
	if (rect_dest->x + rect_dest->w > rect_source->w)
		return -1;
	if (rect_dest->y + rect_dest->h > rect_source->w)
		return -1;
	
	return 1;
}

/*checks if rect values are legal*/
int is_valid_rect(SDL_Rect *rect)
{
	/*basic*/
	if (rect->h < 0 || rect->w  < 0 || rect->x  < 0 || rect->y  < 0 ){
		return 1; 
	}
	/* relative to parent - TODO */
	return 0;
}

/*assigns values to SDL_Rect struct*/
void make_rect(SDL_Rect *rect, int h, int w, int x, int y)
{
	rect->h = h;
	rect->w = w;
	rect->x = x;
	rect->y = y;
	return; 
}

/*draws a single line of caption to center of control*/
int draw_caption_to_control(control *cntrl)
{
	//char* err; 
	TTF_Font *font;
	SDL_Surface *surface;
	SDL_Color text_color = {255, 255, 255};
	SDL_Rect text_rect = {0,0,0,0};
	SDL_Rect soure_rect = {0,0,0,0};

	// no caption is drawn with success
	if (cntrl->caption == NULL)	
		return 1;

	/*make all text Calculations*/
	make_rect(&soure_rect, cntrl->h,cntrl->w,cntrl->offsetx,cntrl->offsety);
	//if (!is_valid_rect(&soure_rect))
	//	return; 

	TTF_Init();
	
	font = TTF_OpenFont("arial.ttf", 16);
	if (font == NULL)  // unable to load font
	{
		return -1;
	}
	
	/*try to make a destination rectangle*/
	if (get_text_position(&soure_rect, cntrl->caption, font, &text_rect) == -1)
		return -1;

	/*if text surface wan't previously loaded*/
	if (cntrl->text_surface == NULL)
	{
		if ((surface = TTF_RenderText_Blended(font, cntrl->caption, text_color))==NULL)
		{
			SDL_FreeSurface(surface);
			return -1;
		}
		cntrl->text_surface = surface; 
	}

	/* Blit surface*/
	if (SDL_BlitSurface(cntrl->text_surface, NULL, cntrl->srfc, &text_rect) != 0){
		//err=TTF_GetError();
		return -1;
	}
	
	return 1; 
}


/*regards line-breaks in controls caption
 *of up-to 5 lines, adjusted to top left */
int draw_caption_to_control_multi(control *cntrl)
{
	TTF_Font *font;
	SDL_Surface *surface;
	SDL_Color text_color = {0, 0, 0};
	SDL_Rect text_rect = {0,0,0,0};
	SDL_Rect soure_rect = {0,0,0,0};

	char s[200];
	char *lines[5];
	char* token;
	int i=0;
	int linecount=0;
	int prev_x_offset = 0;


	if (cntrl->caption == NULL)	
		return -2;

	/*make all text Calculations*/
	
	//if (!is_valid_rect(&soure_rect))
	//	return; 

	//TTF_Init();
	
	font = TTF_OpenFont("arial.ttf", 16);
	if (font == NULL) {
		return -1;
	}

	/*tokenize string*/
	strcpy(s, cntrl->caption);
	token = strtok(s, "\n");
	while (token) {
		lines[linecount] = token;
		linecount++;
		token = strtok(NULL, "\n");
	}

	/*record number of lines*/
	cntrl->num_texts = linecount;
	
	/*if text surface wan't previously loaded*/		
	for (i=0; i<linecount; i++)
	{
		/*calculate text position*/
		make_rect(&soure_rect, cntrl->h,cntrl->w,cntrl->offsetx,cntrl->offsety);
		if (get_text_position_multi(&soure_rect, lines[i], font, &text_rect,prev_x_offset)==-1)
			return -1;
		prev_x_offset = prev_x_offset + text_rect.h;

		/*if this line's text surface doesn't exist*/
		if (cntrl->multitext[i] == NULL)
		{
			surface = TTF_RenderText_Blended(font, lines[i], text_color);
			if (surface==NULL){
				SDL_FreeSurface(surface);
				return -1;
			}
			cntrl->multitext[i] = surface; 
		}

		if (SDL_BlitSurface(cntrl->multitext[i], NULL, cntrl->srfc, &text_rect) != 0){
			return -1;
		}
	}
	return 1;
}


/* returns an element pointing to cntrl*/
element_cntrl new_control_element(control* cntrl)
{
	element_cntrl elem = (element_cntrl)malloc(sizeof(struct element_s_cntrl));
	if (elem==NULL)
	{
		perror("malloc has failed");
		free_control(cntrl);//important, otherowise thing like "new_control_element(new_panel())" will be lost.
		return NULL;
		//TODO: handle malloc failure? ia handled above in null check?
	}
	controlElementNum++;
	elem->cntrl = cntrl;
	elem->next=NULL;
	elem->prev=NULL;
	elem->children=NULL;
	elem->parent=NULL;
	return elem;
}

/* returns an element linked list*/
linked_list_cntrl new_control_list()
{
	linked_list_cntrl lst = (linked_list_cntrl)malloc(sizeof(struct linked_list_s_cntrl));
	if (lst==NULL)
	{
		perror("malloc has failed");
		return NULL;
		//TODO: handle malloc failure? ia handled above in null check?
	}
	lst->head = NULL;
	lst->tail = NULL;
	return lst;
}

/*adds control to existing list*/
void add_control_element_to_list(linked_list_cntrl list, element_cntrl elem)
{
	element_cntrl prev_tail;

	if (list == NULL)
		return; 
	if (elem==NULL)
		return; 

	if (list->head == NULL) // new head
	{	
		list->head = elem;
		list->tail = elem;
		list->tail->next = NULL;
	}
	else // add to existing list
	{
		prev_tail =			list->tail;
		prev_tail->next =	elem;
		elem->prev =	prev_tail;
		list->tail =		elem;
		list->tail->next = NULL;
	}

}

/*sets a list of elements as child of elem*/
void set_list_as_children(linked_list_cntrl list, element_cntrl elem)
{
	
	element_cntrl cur_elem,run;
	
	if (list== NULL)
		return;
	if (elem== NULL)
		return;
	
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
	int i;
	control *label = (control*)malloc(sizeof(control));
	if (label==NULL)
	{
		perror("malloc has failed");
		return NULL;
	}
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
	label->multitext=NULL;
	label->multitext = (SDL_Surface**)malloc(sizeof(SDL_Surface*)*MAX_CAPTION_LINES);
	
	if (label->multitext==NULL)
	{
		perror("malloc has failed");
		return NULL;
	}
	
	for (i=0; i < MAX_CAPTION_LINES ; i++)
	{
		label->multitext[i] = (SDL_Surface*)NULL;
	}

	return label;
}


control* new_button(int x, int y, char *img, int is_trans ,char *caption, int is_grid)
{
	// allocate button
	control *button = (control*)malloc(sizeof(control));
	if (button==NULL)
	{
		perror("malloc has failed");
		return NULL;
	}
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
	button->multitext=NULL;

	return button;
}

/*panel is a logical sub-window, meant to organise a part of the game area*/
control* new_panel(int x, int y, int w, int h, int R, int B, int G, int is_bg_rect)
{
	// allocate window
	control *panel = (control*)malloc(sizeof(control));
	if (panel==NULL)
	{
		perror("malloc has failed");
		return NULL;
	}
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
	panel->is_bg_rect=is_bg_rect;
	panel->multitext=NULL;

	return panel;
}

control* new_window(int x, int y, int w, int h)
{
	// allocate window
	control *window = (control*)malloc(sizeof(control));
	if (window==NULL)
	{
		perror("malloc has failed");
		return NULL;
	}
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
	window->multitext=NULL;

	return window;
}


/* drawing function*/
int draw_button(control *button, control *container)
{
	handle_control_surface_load(button,container);

	/* Blit */
	if (SDL_BlitSurface(button->ownSurface,NULL, container->srfc, button->destination_rect) != 0){
		return -1;
	}
	
	/*add caption*/
	if (button->caption != NULL){
		if (draw_caption_to_control(button)==-1){
			return -1;
		}
	}
	return 0;//sccussfuly drawn
}


int draw_label(control *label, control *container)
{
	if (handle_control_surface_load(label,container)==-1)
		return -1;
	
	if (SDL_BlitSurface(label->ownSurface,NULL, container->srfc, label->destination_rect) != 0)
		return -1;

	if (label->caption != NULL){
		if (draw_caption_to_control_multi(label)==-1){	
			return -1;
		}
	}
	return 0;//sccussfuly drawn
}


int draw_window(control* window)
{
	SDL_WM_SetCaption("SDL Test", "SDL Test");
	if (window->srfc==NULL)
	{
		window->srfc = SDL_SetVideoMode(window->w,window->h,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
		if (window->srfc == NULL)
		{
			SDL_FreeSurface(window->srfc);
			return -1;
			//todo: no window! terminate!
		}
		surfaceNum++;
		window->x = 0; 
		window->y = 0; 
		window->offsetx = 0; 
		window->offsety = 0; 
	}
	return 0;
}

/*add this functionality to window as well. use it to paiunt background in white*/
int draw_panel(control* panel, control *container)
{
	if (handle_control_surface_load(panel,container)==-1)
		return -1;

	/* blit panel surface*/
	if (panel->ownSurface != NULL)
	{
	if ((int)SDL_FillRect(container->srfc,panel->destination_rect, 
			SDL_MapRGB(panel->ownSurface->format, panel->R, panel->G, panel->B)) != 0)
		{
				return -1;
		}
	}
	return 0;//sccusses
}


/*BFS on UI tree, to find a pressable element, and set
* it to *target, according to final (with offset) coordinates 
* if pressed control was a grid panel, we return it, 
  iff there is no button with same coordinates on this grid*/
void find_element_by_coordinates(element_cntrl root,int x, int y, element_cntrl *target)
{
	element_cntrl cur_elem;
	if ((root->cntrl->is_grid))
	{
		/* match coordinates */
		if ((x >= root->cntrl->offsetx && x <= (root->cntrl->offsetx + root->cntrl->w) &&
			(y >= root->cntrl->offsety && y <= (root->cntrl->offsety + root->cntrl->h))))
		{
			*target = NULL;
			/*look for button in grid panel*/
			for (cur_elem = root->children->head; 
			cur_elem!= NULL ;cur_elem=cur_elem->next){
				find_element_by_coordinates(cur_elem,x,y,target);
			}
			/*no matching button, return grid itself*/
			if (*target == NULL)
			{
				*target = root;
			}
			return;		
		}
		
	}
	/*if is a button and a tree leaf*/
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

/*assume that ui_tree->children->tail is the game panel
 * recursively free all controls and data structures
 * associated with this panel, keeping the rest of the ui tree */
void clear_game_panel(element_cntrl ui_tree)
{
	element_cntrl game_panel,pre_tail;

	/*no previous game elements*/
	if(ui_tree->children==NULL ||
		ui_tree->children->head==ui_tree->children->tail) 
	{
		return;
	}

	/* choose game panel in ui tree*/
	pre_tail=ui_tree->children->tail->prev;
	game_panel=ui_tree->children->tail; 
	/*call recursive free*/
	freeControlList(game_panel);
	pre_tail->next=NULL;
	ui_tree->children->tail=pre_tail;
}

/* Recursively free all Dynamically Allocated Memory : 
 * SDL objects, strings, and data structure */
void freeControlList(element_cntrl node)
{
	element_cntrl cur_elem,next_elem;
	int i;

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

	/*frees up texts*/
	if (node->cntrl->multitext != NULL){
		for (i=0; i<node->cntrl->num_texts; i++)
		{
			if (node->cntrl->multitext[i] != NULL)
				SDL_FreeSurface(node->cntrl->multitext[i]);
		}
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
	if (node->cntrl->caption != NULL)
	{
		//free(node->cntrl->caption);
	}
	/*free control and node*/
	free(node->cntrl);
	free(node);

	controlElementNum--;

}

int emptryButton(int *choice,SDL_Event* test_event)
{
	return 0;
}

/*TODO : remove this part from free_control_list*/
void free_control(control *cntrl)
{
	/*free SDL surface associated with control*/
	if (cntrl->ownSurface != NULL){
		SDL_FreeSurface(cntrl->ownSurface);
	}
	cntrl->ownSurface = NULL;
	if (cntrl->text_surface != NULL){
		SDL_FreeSurface(cntrl->text_surface);
	}

	if (cntrl->destination_rect != NULL){
		free(cntrl->destination_rect);
	}

	/*free node caption*/
	if (cntrl->caption != NULL)
	{
		free(cntrl->caption);
	}
}

int newButtonGeneric(linked_list_cntrl fathersList,int x,int y,char* caption,int (*pressedButton)(int *choice,SDL_Event* test_event),int buttonChoise){
	control* temp_control;
	int error;
	char *captionCopy;

	//in order to free caprion freely
	captionCopy=calloc(strlen(caption)+1,sizeof(char));
	strcpy(captionCopy,caption);
	temp_control = new_button(x,y,"./gfx/generic_button.bmp",1,captionCopy,0);
	if (temp_control == NULL)
	{
		freeUnconnectedList(fathersList);
		return -1;
	}
	temp_control->buttonChoise=buttonChoise;
	temp_control->pressedButton=pressedButton;
	error=addNewControlToList(temp_control,fathersList);
	if (error==-1){
		free_control(temp_control);
		return -1;
	}
	return 0;
}

/* wraps control in elemnt and adds to a linked list */
int addNewControlToList(control* control,linked_list_cntrl fathersList){
	element_cntrl temp_elem;

	temp_elem = new_control_element(control);
	if (temp_elem==NULL)
	{
	// TODO: allocating element failed. remove entire tree? backout?
		freeUnconnectedList(fathersList);
		return -1;
	}
	add_control_element_to_list(fathersList,temp_elem);
	return 0;
}

void freeUnconnectedList(linked_list_cntrl fathersList){
	element_cntrl cur_elem,next_elem;
	//free list's children
	if (fathersList != NULL){
		for (cur_elem=fathersList->head;
			cur_elem != NULL; cur_elem=next_elem){
			next_elem=cur_elem->next;
			freeControlList(cur_elem);
		}
		//free list
		free(fathersList);
	}
}