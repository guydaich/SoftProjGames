#include "controls.h"

#define DEBUG 1

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define MAX_CAPTION_LINES 25

int controlElementNum=0;
int buttomNum=0;
int labelNum=0;
int panelNum=0;
int windowNum=0;
int surfaceNum=0;


/*creates a rectangle, according to control size, position and panel boundaries*/
void get_rect(SDL_Rect *rect_dest, control *cntrl, control *parent_panel)
{
	//+ becuase cntrl offset is relative to his parent_panel
	rect_dest->x = cntrl->x + parent_panel->x;
	rect_dest->y = cntrl->y + parent_panel->y;
	//min in order to insure that the control won't be able to pass parent_panel borders
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
	char *sdl_err = NULL;

	if (cntrl->ownSurface == NULL)	//if there no surface assoicated with control yet.if it has- exit func
	{
		if (cntrl->is_bg_img) // control has background image
		{
			surface = SDL_LoadBMP(cntrl->img);//if control has background image(in case on button or panel)
			if (surface == NULL)
			{//if SDL_LoadBMP failed
				sdl_err = SDL_GetError();
				if (sdl_err != NULL)
					printf("%s",sdl_err);
				SDL_FreeSurface(surface);
				return -1;
			}
		}
		else if (cntrl->is_bg_rect) // control has background color
		{
			surface = SDL_CreateRGBSurface(SDL_HWSURFACE, 
				cntrl->w, cntrl->h, 32, 0, 0, 0, 0);
			if (surface == NULL)
			{//if SDL_CreateRGBSurface failed
				sdl_err = SDL_GetError();
				if (sdl_err != NULL)
					printf("%s",sdl_err);
				SDL_FreeSurface(surface);
				return -1;
			}
		}
		
		//save offsets and destination rectangles
		if (cntrl->is_bg_rect || cntrl->is_bg_img)
		{
			cntrl->srfc = container->srfc;
			cntrl->ownSurface=surface;
		
			//find rect according to parent
			get_rect(rect,cntrl,container);
		
			//find picture proportions
			cntrl->h = surface->h;
			cntrl->w = surface->w;
			
			//update to final position(position in relation to the window) on board
			cntrl->offsetx = rect->x;
			cntrl->offsety = rect->y;
		
			cntrl->destination_rect = rect;

			//set transparancy
			if (cntrl->is_transparant){
				if (SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
					SDL_MapRGB(surface->format, MAGNETAR, MAGNETAG, MAGNETAB))==-1)//evrey transparant parts of the image are MAGNETA
				{//enter in case SDL_SetColorKey failed. if SDL_MapRGB will fail SDL_SetColorKey will also fail.
					sdl_err = SDL_GetError();
					if (sdl_err != NULL)
						printf("%s",sdl_err);
				}
			}
		}
		else //if the control has no surface(and shouldn't have)
		{
			// assign all critical info - no real surface
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

/*gets a rectnagle for the text, in the middle of parent rect
*on success return 1, on failure -1
*/
int get_text_position(SDL_Rect *rect_source, char* text,TTF_Font *font, SDL_Rect *rect_dest)
{
	char *ttf_err = NULL;
	int w,h;

	if (TTF_SizeText(font,text,&w,&h)) {//find text size in pixels
		ttf_err = TTF_GetError();
		if (ttf_err != NULL)
			printf("ERROR: %s",ttf_err);
		return -1;
	} 
	
	if (w > rect_source->w || h > rect_source->h)
	{
		printf("ERORR: text rectangle overflows from parent panel");
		return -1;
	}

	rect_dest->w = w; 
	rect_dest->h = h; 
	rect_dest->x = rect_source->x + (rect_source->w - w)/2;//set in the middle of the control
	rect_dest->y = rect_source->y + (rect_source->h - h)/2;

	return 1;
}

/* creates a destination rect for text,
 * with lines aligned to upper left
 * with offset according to previous line.
 *1 on success,-1 on failure
 differs from get_text_position in the fact that it doesn't place the text in the middle,but allow offset*/
int get_text_position_multi(SDL_Rect *rect_source, char* text,TTF_Font *font, SDL_Rect *rect_dest,int offset)
{
	char *ttf_err;
	int w,h;

	if (TTF_SizeText(font,text,&w,&h)) {
		ttf_err = TTF_GetError();
		if (ttf_err != NULL)
			printf("ERROR: %s",ttf_err);
		return -1;
	} 
	rect_dest->w = w; 
	rect_dest->h = h; 
	rect_dest->x = rect_source->x;
	rect_dest->y = rect_source->y+offset;
	
	/* check for overflow */
	if (rect_dest->w > rect_source->w || 
		rect_dest->h > rect_source->w)
	{
		printf("ERORR: text rectangle overflows from parent panel");
		return -1;
	}
	
	return 1;
}

/*checks if rect values are legal*/
int is_valid_rect(SDL_Rect *rect)//?????
{
	/*basic*/
	/*if (rect->h < 0 || rect->w  < 0 || rect->x  < 0 || rect->y  < 0 ){
	return 0;
	}*/
	return 1;
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
	char* sdl_err = NULL;
	char* ttf_err = NULL;

	TTF_Font *font;
	SDL_Surface *surface=NULL;
	SDL_Color text_color = {255, 255, 255};
	SDL_Rect text_rect = {0,0,0,0};
	SDL_Rect soure_rect = {0,0,0,0};

	// no caption is drawn with success
	if (cntrl->caption == NULL || strcmp(cntrl->caption,"")== 0)	
		return 1;

	//make all text Calculations
	make_rect(&soure_rect, cntrl->h,cntrl->w,cntrl->offsetx,cntrl->offsety);
	if (!is_valid_rect(&soure_rect))
	{	
		printf("ERROR: control rectangle has illegal size");
		return -1; 
	}
	
	
	font = TTF_OpenFont("arial.ttf", 16);
	if (font == NULL)  // unable to load font
	{
		ttf_err = TTF_GetError();
		if (sdl_err != NULL)
			printf("%s",ttf_err);
		TTF_CloseFont(font);
		return -1;
	}
	
	//try to make a destination rectangle for caption
	if (get_text_position(&soure_rect, cntrl->caption, font, &text_rect) == -1)
		return -1;

	//if text surface wan't previously made-make it
	if (cntrl->text_surface == NULL)
	{
		surface = TTF_RenderText_Blended(font, cntrl->caption, text_color);
		if (surface==NULL)
		{
			ttf_err = TTF_GetError();
			if (ttf_err != NULL)
				printf("ERORR: %s", ttf_err);
			SDL_FreeSurface(surface);
			TTF_CloseFont(font);
			return -1;
		}
		cntrl->text_surface = surface; 
	}

	// Blit text's surface
	if (SDL_BlitSurface(cntrl->text_surface, NULL, cntrl->srfc, &text_rect) != 0)
	{
		sdl_err = SDL_GetError();
		if (sdl_err != NULL)
			printf("ERROR: %s", sdl_err);
		TTF_CloseFont(font);
		return -1;
	}
	TTF_CloseFont(font);
	return 1; 
}


/* regards line-breaks in controls caption
 *of up-to MAX_LINES lines, adjusted to top left */
int draw_caption_to_control_multi(control *cntrl)
{
	TTF_Font *font=NULL;
	SDL_Surface *surface=NULL;
	SDL_Color text_color = {0, 0, 0};
	SDL_Rect text_rect = {0,0,0,0};
	SDL_Rect soure_rect = {0,0,0,0};
	char* sdl_err = NULL;
	char* ttf_err = NULL;

	char *lines[MAX_CAPTION_LINES];
	char* token;
	int i=0;
	int linecount=0;
	char *s=NULL;
	int prev_x_offset = 0;

	if (cntrl->caption == NULL || strcmp(cntrl->caption,"")== 0	)
		return 1;

	//make all text Calculations
	
	if (!is_valid_rect(&soure_rect))
	{
		printf("ERROR: control rectangle has illegal size");
		return -1; 
	}
	
	/*get font */
	font = TTF_OpenFont("arial.ttf", 16);
	if (font == NULL) {
		ttf_err = TTF_GetError();
		if (ttf_err != NULL)
			printf("ERROR: %s", ttf_err);
		TTF_CloseFont(font);
	}


	/*assign a string to destory, and tokenize it*/
	s = (char*)malloc(strlen(cntrl->caption) + 1); 
	if (s == NULL)
	{
		printf("ERROR: standard function malloc has failed");
		TTF_CloseFont(font);
		return -1;
	}

	strcpy(s,cntrl->caption);
	token = strtok(s, "\n");
	while (token) {
		lines[linecount] = token;
		linecount++;
		token = strtok(NULL, "\n");
	}

	//record number of lines
	cntrl->num_texts = linecount;
	
	//this for makes sure that the a surface for the text will be made	
	for (i=0; i<linecount; i++)
	{
		//calculate text position
		make_rect(&soure_rect, cntrl->h,cntrl->w,cntrl->offsetx,cntrl->offsety);
		if (get_text_position_multi(&soure_rect, lines[i], font, &text_rect,prev_x_offset)==-1)
		{
			printf("ERROR: could not fit text in containing control");
			return -1;
		}
		prev_x_offset = prev_x_offset + text_rect.h;

		//if this line's text surface doesn't exist- make it
		if (cntrl->multitext[i] == NULL)
		{
			surface = TTF_RenderText_Blended(font, lines[i], text_color);
			if (surface==NULL){
				sdl_err = SDL_GetError();
				if (sdl_err != NULL)
					printf("ERROR: %s", sdl_err);
				SDL_FreeSurface(surface);
				TTF_CloseFont(font);
				return -1;
			}
			cntrl->multitext[i] = surface; 
		}

		//blit text's surface
		if (SDL_BlitSurface(cntrl->multitext[i], NULL, cntrl->srfc, &text_rect) != 0){
			sdl_err = SDL_GetError();
			if (sdl_err != NULL)
				printf("ERROR: %s", sdl_err);
			TTF_CloseFont(font);
			return -1;
		}
	}
	TTF_CloseFont(font);
	return 1;
}


/* makes an element in the ui tree
 return null on failure*/
element_cntrl new_control_element(control* cntrl)
{
	element_cntrl elem =NULL;

	elem=(element_cntrl)malloc(sizeof(struct element_s_cntrl));
	
	if (elem==NULL)
	{
		printf("ERROR: standard function malloc has failed");
		free_control(cntrl);
		return NULL;
	}

	controlElementNum++;
	elem->cntrl = cntrl;
	elem->next=NULL;
	elem->prev=NULL;
	elem->children=NULL;
	elem->parent=NULL;
	return elem;
}

/* makes an empty element linked list
return null on failure*/
linked_list_cntrl new_control_list()
{
	linked_list_cntrl lst = (linked_list_cntrl)malloc(sizeof(struct linked_list_s_cntrl));
	if (lst==NULL)
	{
		printf("ERROR: standard function malloc has failed");
		free(lst);
		return NULL;
	}
	lst->head = NULL;
	lst->tail = NULL;
	return lst;
}

/*adds control element to existing list*/
void add_control_element_to_list(linked_list_cntrl list, element_cntrl elem)
{
	element_cntrl prev_tail;

	if (list == NULL)
	{
		printf("ERROR: supplied list does not exsist");
		return; 
	}
	
	if (elem==NULL)
	{
		printf("ERROR: supplied elem does not exsist");
		return; 
	}

	if (list->head == NULL) //in case the list is empty- point both head and tail to the head
	{	
		list->head = elem;
		list->tail = elem;
		list->tail->next = NULL;
	}
	else // add to existing list- add the element at the tail
	{
		prev_tail =			list->tail;
		prev_tail->next =	elem;
		elem->prev =	prev_tail;
		list->tail =		elem;
		list->tail->next = NULL;
	}

}

/*sets a list of elements as children's list to the element*/
void set_list_as_children(linked_list_cntrl list, element_cntrl elem)
{
	
	element_cntrl cur_elem=NULL,run=NULL;
	
	if (list== NULL)
	{
		printf("ERROR: supplied list does not exsist");
		return;
	}
	
	if (elem== NULL)
	{
		printf("ERROR: supplied elem does not exsist");
		return;
	}
	
	//free prev list of the element (if he had one)
	if (elem->children!=NULL){
		for (run=elem->children->head;run!=NULL;run=run->next)
		{
			free_control_list(run);
		}
		free(elem->children);
	}

	// set list as children
	elem->children = list; 
	
	// for every child, assign parent
	for (cur_elem = list->head; cur_elem!= NULL ;cur_elem=cur_elem->next){
		cur_elem->parent = elem;
	} 
}

/*gets UI tree root, draws it in DFS trasverse with the recursive function draw_ui_tree 
 * returns -1 on failure*/
int draw_ui_tree(element_cntrl root)
{
	int ret_val = 1; 
	ret_val  = draw_with_panel(root,root);
	return ret_val;
}

/*recursive drawing, that passes owning panels
 * to elements for overlay construction 
 * returns 1 on success,-1 on failure*/
int draw_with_panel(element_cntrl draw_cntrl, element_cntrl owning_panel)
{
	element_cntrl cur_elem;
	int err = 0; 
	// call the controls drawing function
	if (draw_cntrl->parent== NULL)//in case of a window
	{
		err = draw_cntrl->cntrl->draw(draw_cntrl->cntrl,NULL);
		if (err == -1)
			return -1;
	}
	else//anything inside the window
	{
		err = draw_cntrl->cntrl->draw(draw_cntrl->cntrl,owning_panel->cntrl);
		if (err == -1)
			return -1;
	}
	// draw children
	if (draw_cntrl->children != NULL){
		for (cur_elem = draw_cntrl->children->head; 
			cur_elem!= NULL ;cur_elem=cur_elem->next)
			{
			if (draw_cntrl->cntrl->is_panel)
				draw_with_panel(cur_elem,draw_cntrl);//if draw_cntrl is a panel,his children should be drawn relativly to him 
			else
				draw_with_panel(cur_elem,owning_panel);//if draw_cntrl isn't a panel,draw children relativly to his "owning_panel"
			}
		return 0;
		}
	else{//no children
		return 0;
	}
}

/*init functions*/


/*makes new label. a label is a text with background. return null on failure*/
control* new_label(int x, int y, int w, int h, char *img, int R, int G, int B, int is_trans, char* caption)
{
	int i=0;
	control *label = (control*)malloc(sizeof(control));
	if (label==NULL)
	{
		printf("ERROR: standard function malloc has failed");
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
	label->ownSurface = NULL;
	label->caption = NULL;
	if (caption != NULL)
	{
		label->caption = (char *) malloc (strlen (caption) + 1); 
		if (label->caption == NULL)
		{
			printf("ERROR: standard function malloc has failed");
			return NULL;
		}
		strcpy(label->caption,caption);
	}
	label->pressed_button=empty_click_handle;
	label->srfc=NULL;
	label->button_choice=0;
	label->text_surface=NULL;
	label->destination_rect=NULL;
	label->is_grid=0;
	label->is_bg_img =1;
	label->is_bg_rect =0;
	label->multitext=NULL;
	label->multitext = (SDL_Surface**)malloc(sizeof(SDL_Surface*)*MAX_CAPTION_LINES);
	
	if (label->multitext==NULL)
	{
		printf("ERROR: standard function malloc has failed");
                free(label);
		return NULL;
	}
	
	for (i=0; i < MAX_CAPTION_LINES ; i++)
	{
		label->multitext[i] = (SDL_Surface*)NULL;
	}

	return label;
}

/*makes new button. return null on failure*/
control* new_button(int x, int y, char *img, int is_trans ,char *caption, int is_grid)
{
	control *button = (control*)malloc(sizeof(control));
	if (button==NULL)
	{
		printf("ERROR: standard function malloc has failed");
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
	button->pressed_button=empty_click_handle;
	button->srfc=NULL;
	button->text_surface=NULL;
	button->button_choice=0;
	button->destination_rect=NULL;
	button->is_grid=is_grid;
	button->is_bg_img =1;
	button->is_bg_rect =0;
	button->multitext=NULL;

	return button;
}

/**/
/* makes new panel.
* panel is a logical sub-window, meant to organise a part of the game area.
* evrey button and label must have a father(doesn't have to be direct) which is a panel.
return null on failure*/
control* new_panel(int x, int y, int w, int h, int R, int B, int G, int is_bg_rect)
{
	// allocate window
	control *panel = (control*)malloc(sizeof(control));
	if (panel==NULL)
	{
		printf("ERROR: standard function malloc has failed");
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
	panel->img = NULL;
	panel->draw = draw_panel;// drawing funct
	panel->ownSurface=NULL;
	panel->caption=NULL;
	panel->pressed_button=empty_click_handle;
	panel->srfc=NULL;
	panel->button_choice=0;
	panel->text_surface = NULL;
	panel->destination_rect=NULL;
	panel->is_grid=0;
	panel->is_bg_img =0;
	panel->is_bg_rect=is_bg_rect;
	panel->multitext=NULL;

	return panel;
}

/*makes new window.
the window's element is the ui tree root.
return 1 on success,-1 on failure
*/
control* new_window(int x, int y, int w, int h)
{
	// allocate window
	control *window = (control*)malloc(sizeof(control));
	if (window==NULL)
	{
		printf("ERROR: standard function malloc has failed");
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
	window->pressed_button=empty_click_handle;
	window->srfc=NULL;
	window->button_choice=0;
	window->text_surface = NULL;
	window->destination_rect=NULL;
	window->is_grid=0;
	window->is_bg_img =0;
	window->is_bg_rect =0;
	window->multitext=NULL;

	return window;
}


/* drawing functions- each kind of control has its own drawing function*/

int draw_button(control *button, control *container)
{
	char* sdl_err; 

	if (handle_control_surface_load(button,container)==-1)//the handle_control_surface_load makes evrey thing ready for bliting of the button
	{
		printf("ERROR: surface load failed");
		return -1;
	}

	//blit the controls surface
	if (SDL_BlitSurface(button->ownSurface,NULL, container->srfc, button->destination_rect) != 0)
	{
		sdl_err = SDL_GetError();
		if (sdl_err != NULL)
			printf("ERROR: %s", sdl_err);
		return -1;
	}
	
	//add caption
	if (button->caption != NULL){
		if (draw_caption_to_control(button)==-1){
			printf("ERROR: caption load failed");
			return -1;
		}
	}
	return 0;//sccussfuly drawn
}


int draw_label(control *label, control *container)
{
	char* sdl_err = NULL;

	if (handle_control_surface_load(label,container)==-1)//the handle_control_surface_load makes evrey thing ready for bliting of the label
	{
		printf("ERROR: surface load failed");
		return -1;
	}
	//blit the surface label
	if (SDL_BlitSurface(label->ownSurface,NULL, container->srfc, label->destination_rect) != 0)
	{
		sdl_err = SDL_GetError();
		if (sdl_err != NULL)
			printf("ERROR: %s", sdl_err);
		return -1;
	}
	//blit the label text (by using draw_caption_to_control_multi)
	if (label->caption != NULL){
		if (draw_caption_to_control_multi(label)==-1){	
			printf("ERROR: caption load failed");
			return -1;
		}
	}
	return 0;//sccussfuly drawn
}


int draw_window(control* window)
{
	char* sdl_err=NULL;
	SDL_Surface *surface; 
	SDL_WM_SetCaption("gamesprog", "gamesprog");

	surface = SDL_SetVideoMode(window->w,window->h,32,SDL_HWSURFACE|SDL_DOUBLEBUF);

	if (surface == NULL)
		{
		sdl_err = SDL_GetError();
		if (sdl_err != NULL)
			printf("ERROR: %s", sdl_err);
			SDL_FreeSurface(window->srfc);
			return -1;
		}
	
	window->srfc = surface; 

	surfaceNum++;//every time we blit a window we must make a new surface
	window->x = 0;//
	window->y = 0; 
	window->offsetx = 0; 
	window->offsety = 0; 

	return 0;
}

/*add this functionality to window as well. use it to paiunt background in white*/
int draw_panel(control* panel, control *container)
{ 
	char* sdl_err = NULL;

	if (handle_control_surface_load(panel,container)==-1)
	{
		printf("ERROR: loading surface failed");
		return -1;
	}

	// blit panel surface
	if (panel->ownSurface != NULL)
	{
	if ((int)SDL_FillRect(container->srfc,panel->destination_rect, 
			SDL_MapRGB(panel->ownSurface->format, panel->R, panel->G, panel->B)) != 0)
		{
			sdl_err = SDL_GetError();
			if (sdl_err != NULL)
				printf("ERROR: %s", sdl_err);
				return -1;
		}
	}
	return 0;//sccusses
}


/*DFS on UI tree, to find a pressable element, and set
* it to *target, according to final (with offset) coordinates 
* if pressed control was a grid panel, we return it, 
  iff there is no button with same coordinates on this grid
  in case the function didn't find anything it returns null in target*/
void find_element_by_coordinates(element_cntrl root,int x, int y, element_cntrl *target)
{
	element_cntrl cur_elem;
	if ((root->cntrl->is_grid))
	{
		// match coordinates
		if ((x >= root->cntrl->offsetx && x <= (root->cntrl->offsetx + root->cntrl->w) &&
			(y >= root->cntrl->offsety && y <= (root->cntrl->offsety + root->cntrl->h))))
		{
			*target = NULL;
			//look for button in grid panel- if it is a grid panel(the only button with children
			for (cur_elem = root->children->head; 
			cur_elem!= NULL ;cur_elem=cur_elem->next){
				find_element_by_coordinates(cur_elem,x,y,target);
			}
			if (*target == NULL)//no matching button under greid or a button with no children, return grid itself
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
		return;//if there is no children-return
	}
	else{
		for (cur_elem = root->children->head; 
			cur_elem!= NULL ;cur_elem=cur_elem->next){
				find_element_by_coordinates(cur_elem,x,y,target);//check the children
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

	if(ui_tree->children==NULL ||
		ui_tree->children->head==ui_tree->children->tail) //no previous game elements
	{
		return;
	}

	pre_tail=ui_tree->children->tail->prev;
	// choose game panel in ui tree
	game_panel=ui_tree->children->tail; 
	//call recursive free
	free_control_list(game_panel);
	//set pre_tail as new tail
	pre_tail->next=NULL;
	ui_tree->children->tail=pre_tail;
}

/* Recursively free all Dynamically Allocated Memory : 
 * SDL objects, strings, and data structure */
void free_control_list(element_cntrl node)
{
	element_cntrl cur_elem=NULL,next_elem=NULL;
	int i=0;

	if (node == NULL){
		return;
	}

	//free node children
	if (node->cntrl->img != NULL)
	{
		printf("%s\n",node->cntrl->img);	
	}
	if (node->children != NULL){
		for (cur_elem=node->children->head;
			cur_elem != NULL; cur_elem=next_elem){
			next_elem=cur_elem->next;
			free_control_list(cur_elem);
		}
	}

	//free SDL surface associated with control
	if (node->cntrl->ownSurface != NULL){
		SDL_FreeSurface(node->cntrl->ownSurface);
	}
	node->cntrl->ownSurface = NULL;//just in case
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
	
	//free node caption
	if (node->cntrl->caption != NULL)
	{
		printf("%s\n",node->cntrl->caption);	
	}

	if (node->cntrl->caption != NULL)
	{
		free(node->cntrl->caption);
	}
	//free control and node
	free(node->cntrl);
	free(node);

	controlElementNum--;

}

/*if a control with this function is pressed, do nothing and continue.*/
int empty_click_handle(int *choice,SDL_Event* test_event)
{
	return 0;
}

void free_control(control *cntrl)
{
	//free SDL surface associated with control
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

	//free node caption
	if (cntrl->caption != NULL)
	{
		free(cntrl->caption);
	}
}

/*a function which creates a button with generic_button.bmp and addes it to the list*/
int new_generic_button(linked_list_cntrl parent_element_list,int x,int y,char* caption,int (*pressed_button)(int *choice,SDL_Event* test_event),int button_choice){
	control* temp_control;
	int error;
	char *caption_cpy;

	//in order to free caption freely
	caption_cpy=calloc(strlen(caption)+1,sizeof(char));
	strcpy(caption_cpy,caption);
	//assigne control
	temp_control = new_button(x,y,"./gfx/generic_button.bmp",1,caption_cpy,0);
	if (temp_control == NULL)
	{
		free_detached_list(parent_element_list);
		return -1;
	}
	temp_control->button_choice=button_choice;
	temp_control->pressed_button=pressed_button;
	error=add_control_to_element_list(temp_control,parent_element_list);
	
	if (error==-1){
		free_control(temp_control);
		printf("ERROR: failed to add control to list");
		return -1;
	}

	return 0;
}

/* wraps control in elemnt and adds to a linked list */
int add_control_to_element_list(control* control,linked_list_cntrl parent_element_list){
	element_cntrl temp_elem=NULL;


	temp_elem = new_control_element(control);
	if (temp_elem==NULL)
	{
		free_detached_list(parent_element_list);
		return -1;
	}
	add_control_element_to_list(parent_element_list,temp_elem);
	return 0;
}

/*free a list and its children- withou the element which contains the list(if there is any)*/
void free_detached_list(linked_list_cntrl parent_element_list){
	element_cntrl cur_elem,next_elem;
	//free list's children
	if (parent_element_list != NULL){
		for (cur_elem=parent_element_list->head;
			cur_elem != NULL; cur_elem=next_elem){
			next_elem=cur_elem->next;
			free_control_list(cur_elem);
		}
		//free list
		free(parent_element_list);
	}
}