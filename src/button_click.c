#include <pebble.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static Window *window;
//static Layer *s_layer;
//GContext *Context;
static TextLayer *text_layer; 
DictionaryIterator iter;
int counter = 0;
char *text[] = {"up", "down", "right", "left", "north", "south"};
char *input[4];
int size = 0;
int arrayCounter = 1;
  
struct DiveSpots {
  
  char *name;
  char *lat;
  char *lon;
  char *rating;
};

struct DiveSpots list[5];

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  text_layer_set_text(text_layer, "Select");
  
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
      
  Tuplet value = TupletInteger(0, 143);
  dict_write_tuplet(iter, &value);
      
  app_message_outbox_send();
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  //GRect bounds = layer_get_frame(text_layer);
  
  //graphics_context_set_text_color(ctx, GColorBlack);
  //graphics_draw_text(ctx, "Text here.", fonts_get_system_font(FONT_KEY_FONT_FALLBACK), GRect(5, 5, bounds.size.w - 10, 100), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  //graphics_draw_text(ctx, "And text here as well.", fonts_get_system_font(FONT_KEY_FONT_FALLBACK), GRect(90, 100, bounds.size.w - 95, 60), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
  
  /*
  char * buf = "";
  strcat(buf, text[2]);
  strcat(buf, ",");
  strcat(buf, text[3]);
  text_layer_set_text(text_layer, buf);
  text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
  */
  
  text_layer_set_text(text_layer, list[counter].name);
  counter = (counter < size - 1) ? counter + 1 : 0;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {

    text_layer_set_text(text_layer, input[counter]);
    counter = (counter < 4 - 1) ? counter + 1 : 0;
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

/*
 void update_layer_callback(Layer *layer, GContext *ctx, Tuple * t) {
  GRect bounds = layer_get_frame(layer);
  APP_LOG(APP_LOG_LEVEL_INFO, "inside layer callback!");
  ctx = Context;
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, "Text here.", fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(5, 5, bounds.size.w - 10, 100), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  graphics_draw_text(ctx, t->value->cstring, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), GRect(90, 100, bounds.size.w - 95, 60), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
  APP_LOG(APP_LOG_LEVEL_INFO, "leaving callback!");
}
*/

static void window_load(Window *window) {
  
  //Layer *window_layer = window_get_root_layer(window);
  //GRect bounds = layer_get_frame(window_layer);
  
  //s_layer = layer_create(bounds);
  //(s_layer, update_layer_callback);
  //layer_add_child(window_layer, s_layer);
 
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 60 } });
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text(text_layer, "Press select to get \n the closest \n diving spot");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
}

static void window_unload(Window *window) {
    text_layer_destroy(text_layer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  
    Tuple *tuple = dict_read_first(iterator);
    size = 1;
    char * temp = tuple->value->cstring;
    /*for(int i = 1; i < 4; i++) {
      
      tuple = dict_read_next(iterator);
      input[i] = tuple->value->cstring;
    }*/
    /*int temp = tuple->length;
    static char buf[] = "123456";
    snprintf(buf, sizeof(buf), "%d", temp);
    text_layer_set_text(text_layer, buf);
    */
    /*for(int i = 0; i < size; i++) {
      
      for(int j = 0; j < 4; j++) {
          
        if(i == 0 && j == 0) {
          tuple = dict_read_first(iterator);
          list[i].name = tuple->value->cstring;
        }
        if(i != 0 && j == 0) {
          tuple = dict_read_next(iterator);
          list[i].name = tuple->value->cstring;
        }
        else if(j == 1) {
          tuple = dict_read_next(iterator);
          list[i].lat = tuple->value->cstring;
        }
        else if(j == 2) {
          tuple = dict_read_next(iterator);
          list[i].lon = tuple->value->cstring;
        }
        else if(j == 3) {
          tuple = dict_read_next(iterator);
          list[i].rating = tuple->value->cstring;
        }
      }
    }
  */
    /*char buf[500];
    strcat(buf, input[0]);
    strcat(buf, "\n");
    strcat(buf, input[1]);
    strcat(buf, "\n");
    strcat(buf, input[2]);
    strcat(buf, "\n");
    strcat(buf, input[3]);
    text_layer_set_text(text_layer, buf);*/
    //update_layer_callback(s_layer, context, t);
    text_layer_set_text(text_layer, temp);
    text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
    text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
  
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  
    // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}


char * strtok(s, delim)
	char *s;
	const char *delim;
{
	char *spanp;
	int c, sc;
	char *tok;
	static char *last;


	if (s == NULL && (s = last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
        APP_LOG(APP_LOG_LEVEL_INFO, "Inside strtok, return");
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}