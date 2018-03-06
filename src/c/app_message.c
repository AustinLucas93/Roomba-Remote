#include <pebble.h>

Window *window;	
TextLayer *text_layer;
	
// Key values for AppMessage Dictionary
enum {
	STATUS_KEY = 0,	
	MESSAGE_KEY = 1
};

// A more general send function
void send_int(uint8_t key, char* cmd)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    dict_write_cstring(iter, MESSAGE_KEY, cmd);
    //dict_write_tuplet(iter, &value);
 
    app_message_outbox_send();
}


// Write message to buffer & send
void send_message(void){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, STATUS_KEY, 0x1);
	
	dict_write_end(iter);
  	app_message_outbox_send();
}


// SELECT BUTTON EVENT LISTENER HANDLER 
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
  send_int(0,"S");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
  send_int(0,"U");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
  send_int(0,"D");
}

// SETUP FUNCTION FOR BUTTON EVENT LISTENER SUBSCRIPTION 
static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, STATUS_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
	}
	
	tuple = dict_find(received, MESSAGE_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
	}}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void init(void) {
  // Manage Window Initilization
	window = window_create();
	window_stack_push(window, true);
  
  window_set_click_config_provider(window, click_config_provider);

  //window_set_window_handlers(window, (WindowHandlers) {
	//.load = window_load,
  //  .unload = window_unload,
  // });
  
  //Manage the Text Layer
  text_layer = text_layer_create(GRect(0, 0, 144, 154));
  text_layer_set_text(text_layer, "Initilizaion");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	// send_message();
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}