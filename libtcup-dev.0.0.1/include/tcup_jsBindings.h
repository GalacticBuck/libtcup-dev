#ifndef TCUP_JSBINDINGS_H
#define TCUP_JSBINDINGS_H


namespace tcup{
	duk_ret_t js_print(duk_context *ctx);
	duk_ret_t js_mvprint(duk_context *ctx);
	duk_ret_t js_readFile(duk_context *ctx);
	duk_ret_t js_refresh(duk_context* ctx);
	duk_ret_t js_timeout(duk_context* ctx);
	duk_ret_t js_window(duk_context* ctx);
	duk_ret_t js_exit(duk_context* ctx);



	void set_bindings(duk_context *ctx);
}

#endif
