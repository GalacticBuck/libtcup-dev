#ifndef TCUP_EVENT_H
#define TCUP_EVENT_H

duk_ret_t js_eventOn(duk_context* ctx);
duk_ret_t js_eventEmit(duk_context* ctx);
duk_ret_t js_eventOff(duk_context* ctx);
duk_ret_t js_bindEventListener(duk_context* ctx);
void create_eventHandler(duk_context* ctx);
int event_on(duk_context* ctx, duk_idx_t listener, const char* event_name, duk_idx_t func);
void event_emit(duk_context* ctx, duk_idx_t sender, const char* event_name, size_t args);
void event_off(duk_context* ctx, const char* event_name, duk_idx_t obj, bool exact_event);
int global_event_on(duk_context* ctx, const char* event_name, duk_idx_t func);
void global_event_emit(duk_context* ctx, const char* event_name, size_t args);
void global_event_off(duk_context* ctx, const char* event_name);
void global_event_off(duk_context* ctx, const char* event_name,int i);

#endif
