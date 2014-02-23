#include "controls.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/* return pointer to element containing pointer to give control*/
element_cntrl new_control_element(control* cntrl)
{
	element_cntrl elem = (element_cntrl)malloc(sizeof(struct element_s_cntrl));
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
	element_cntrl cur_elem;
	
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

control* new_label(int x, int y, int w, int h, char *img, int R, int G, int B, int is_trans)
{
	// allocate button
	control *label = (control*)malloc(sizeof(control));
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

	return label;
}


control* new_button(int x, int y, int w, int h, char *img, int R, int G, int B, int is_trans)
{
	// allocate button
	control *button = (control*)malloc(sizeof(control));
	button->is_button = 1;
	button->is_label = 0;
	button->is_panel = 0;
	button->is_window = 0;
	button->x = x;
	button->y = y;
	button->w = w;
	button->h = h;	
	button->is_transparant = is_trans;
	button->R = R;
	button->B = B;
	button->G = G;
	button->img = img;
	button->draw=draw_button;

	return button;
}

/*panel is a logical sub-window, meant to organise a part of the game area*/
control* new_panel(int x, int y, int w, int h, int R, int B, int G)
{
	// allocate window
	control *panel = (control*)malloc(sizeof(control));
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

	// drawing funct
	panel->draw = draw_panel;
	return panel;
}

control* new_window(int x, int y, int w, int h)
{
	// allocate window
	control *window = (control*)malloc(sizeof(control));
	window->is_button = 0;
	window->is_label = 0;
	window->is_panel = 1;
	window->is_window = 0;
	window->x = x;
	window->y = y;
	window->h = h;
	window->w = w;
	// drawing funct
	window->draw = draw_window;
	return window;
}


/* drawing function*/
void draw_button(control *button, control *container)
{
	// build rect
	SDL_Rect rect = {0,0,0,0}; 
	char *err; 
	
	// load pic
	SDL_Surface *surface;

	if ((surface = SDL_LoadBMP(button->img)) == NULL)
	{
		err=SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}
	
	// update surface in button object for further use
	button->srfc = container->srfc;
	// get rectangle according to container constraints
	get_rect(&rect,button,container);

	/* set transparancy according to button R,G,B values*/
	if (button->is_transparant)
		SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(surface->format, button->R, button->G, button->B));

	
	if (SDL_BlitSurface(surface,NULL, container->srfc, &rect) != 0)
	{
		char *x= SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}

	SDL_Flip( container->srfc );
}


void draw_label(control *label, control *container)
{
	// build rect
	SDL_Rect rect = {0,0,0,0}; 
	char *err; 
	
	// load pic
	SDL_Surface *surface;

	if ((surface = SDL_LoadBMP(label->img)) == NULL)
	{
		err=SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}
	
	// update surface in button object for further use
	label->srfc = container->srfc;
	// get rectangle according to container constraints
	get_rect(&rect,label,container);

	/* set transparancy according to button R,G,B values*/
	if (label->is_transparant)
		SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
		SDL_MapRGB(surface->format, label->R, label->G, label->B));

	
	if (SDL_BlitSurface(surface,NULL, container->srfc, &rect) != 0)
	{
		char *x= SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}

	SDL_Flip( container->srfc );
}


void draw_window(control* window)
{
	SDL_WM_SetCaption("SDL Test", "SDL Test");
	window->srfc = SDL_SetVideoMode(window->w,window->h,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
}

/*add this functionality to window as well. use it to paiunt background in white*/
void draw_panel(control* panel, control *container)
{
	SDL_Surface *surface;
	char *err; 
	SDL_Rect rect;

	get_rect(&rect,panel,container);

	/* create a coloreble surface*/ 
	if ((surface = SDL_CreateRGBSurface(SDL_HWSURFACE, panel->w, panel->h, 32, 0, 0, 0, 0)) == NULL)
	{
		err=SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}
	
	/*fill surface with defualt panel color*/
	if (SDL_FillRect(container->srfc,&rect, SDL_MapRGB(surface->format, panel->R, panel->G, panel->B)) != 0)
	{
		err=SDL_GetError();
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
	}
	panel->srfc = container->srfc;
	SDL_Flip( container->srfc );
	
}


/*gets current control, and parent control, return valid rectangle*/
int get_rect(SDL_Rect *rect, control *child, control *parent)
{

	if (parent == NULL)
	{
		rect->x = child->x;
		rect->y = child->x;
		rect->h = child->h;
		rect->w = child->w;
		return 0;
	}

	/* child x inside container borders*/
	if ((child->x) >= (parent->x) && (child->x) <= (parent->x + parent->w))
	{
		rect->x = child->x;
		rect->w = MIN(child->w,parent->w);
	}
	/*child x before parent x - paint what's possible*/
	else if ((child->x) < (parent->x))
	{
		rect->x = parent->x;
		/*the new position is the parents - paint as much as possible
		rect->w = MIN(child->w - parent->x, parent->w);*/
		rect->w = parent->w;
	}
	/*child after parent - can't paint*/
	else if ((child->x) > (parent->x + parent->w))
	{
		rect->x = child->x;
		rect->y = child->y;
		rect->w = 0;
		rect->h = 0;
		return 0;
	}

	/* child x inside container borders*/
	if ((child->y) >= (parent->y) && (child->y) <= (parent->y + parent->h))
	{
		rect->y = child->y;
		rect->h = MIN(child->h,parent->h);
	}
	/*child x before parent x - paint what's possible*/
	else if ((child->y) < (parent->y))
	{
		rect->y = parent->y;
		/*the new position is the parents - paint as much as possible
		rect->w = MIN(child->w - parent->x, parent->w);*/
		rect->h = parent->h;
	}
	/*child after parent - can't paint*/
	else if ((child->y) > (parent->y + parent->w))
	{
		rect->x = child->x;
		rect->y = child->y;
		rect->w = 0;
		rect->h = 0;
		return 0;
	}
	return 1;
}

void find_element_by_coordinates(element_cntrl root,int x, int y, element_cntrl target)
{
	element_cntrl cur_elem;
	// fix this! use buttons in lowest level only
	if (root->cntrl->is_button && root->children == NULL)
	{
		if ((x >= root->cntrl->x && x <= (root->cntrl->x + root->cntrl->w) &&
			(y >= root->cntrl->y && y <= (root->cntrl->y + root->cntrl->h))))
		{
			target = root; 
			return;
		}
	}

	if (root->children == NULL)
	{
		return;
	}
	else
	{
		for (cur_elem = root->children->head; cur_elem!= NULL ;cur_elem=cur_elem->next)
			{
				find_element_by_coordinates(cur_elem,x,y,target);
			}
	}

}