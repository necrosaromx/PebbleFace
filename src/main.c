#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer,*s_date_layer, *s_battery_layer,*s_percent_layer;
static GFont s_time_font,s_date_font,s_battery_font,s_percent_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static BitmapLayer *s_slime_layer;
static GBitmap *s_slime_bitmap;
static BitmapLayer *s_stat_layer;
static GBitmap *s_stat_bitmap;
static int s_battery_level = 99;


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char s_buffer_date[8];
  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  strftime(s_buffer_date, sizeof(s_buffer_date), "%a! %e", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_text(s_date_layer, s_buffer_date);
}

static void update_batt() {
  static char s_buffer_percent[4]; 
  // Record the new battery level
  snprintf(s_buffer_percent, sizeof(s_buffer_percent), "H%d", s_battery_level);
  text_layer_set_text(s_percent_layer, s_buffer_percent);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void battery_callback(BatteryChargeState state) {
    s_battery_level = state.charge_percent;
    update_batt();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  //GRect slime_frame = (0, 120, 33, 33);
  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DW_BG);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(GRect(bounds.origin.x, bounds.origin.y + 52, bounds.size.w, bounds.size.h));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

      // Create SLIME
  //GRect slime_bounds = gbitmap_get_bounds(s_slime_bitmap);
  s_slime_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SLIME);
  s_slime_layer = bitmap_layer_create(GRect((bounds.size.w / 2) - 16, 58, 33, 33));
  bitmap_layer_set_bitmap(s_slime_layer, s_slime_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_slime_layer));  

  // status box
  s_stat_bitmap = gbitmap_create_with_resource(RESOURCE_ID_STATBOX);
  s_stat_layer = bitmap_layer_create(GRect(4,11,49,35));
  bitmap_layer_set_bitmap(s_stat_layer,s_stat_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_stat_layer));
    
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(3, 120, bounds.size.w, 50));
  s_date_layer = text_layer_create(
      GRect(9, 79, bounds.size.w, bounds.size.h));
  s_battery_layer = text_layer_create(
      GRect(9,-30,43,48));
  s_percent_layer = text_layer_create(
      GRect(9,-12,bounds.size.w,48));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PERFECT_DOS_36));

  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // this is the date layer
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text(s_date_layer, "HALLO");
  // Create GFont
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DW_FONT_48));

  // Apply to TextLayer
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
 
  // this is the battery title
  text_layer_set_background_color(s_battery_layer, GColorBlack);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  text_layer_set_text(s_battery_layer, "Bat");
  // Create GFont
  s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DW_FONT_48));

  // Apply to TextLayer
  text_layer_set_font(s_battery_layer, s_battery_font);
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentLeft);
  
  // this is the battery percentage
  text_layer_set_background_color(s_percent_layer, GColorClear);
  text_layer_set_text_color(s_percent_layer, GColorWhite);
  text_layer_set_text(s_percent_layer, "H00");
  // Create GFont
  s_percent_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DW_FONT_48));

  // Apply to TextLayer
  text_layer_set_font(s_percent_layer, s_percent_font);
  text_layer_set_text_alignment(s_percent_layer, GTextAlignmentLeft);
    
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_percent_layer));  
}

static void main_window_unload(Window *window) {

  fonts_unload_custom_font(s_date_font);
    // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_battery_font);
  fonts_unload_custom_font(s_percent_font);

  // Destroy TextLayer
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_percent_layer);  
    // Destroy GBitmap
  gbitmap_destroy(s_slime_bitmap);
  bitmap_layer_destroy(s_slime_layer);
  gbitmap_destroy(s_background_bitmap);

// Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  gbitmap_destroy(s_stat_bitmap);
  bitmap_layer_destroy(s_stat_layer);
    
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_set_background_color(s_main_window, GColorBlack);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();
  battery_callback(battery_state_service_peek());

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);

}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}