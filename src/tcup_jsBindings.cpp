#include "tcup.h"
#include <string>
#include <unistd.h>
#include <fstream>
#include "tcup_jsBindings.h"


namespace tcup{
	duk_ret_t js_print(duk_context *ctx) {
		int n = duk_get_top(ctx);
		duk_push_string(ctx,"");
		duk_insert(ctx,0);
		duk_join(ctx,n);
		//printw(duk_get_string(ctx,-1));
		duk_pop(ctx);
		return 0;  /* one return value */
	}


	duk_ret_t js_mvprint(duk_context *ctx) {
		//mvprintw(duk_to_number(ctx,1),duk_to_number(ctx,0),duk_to_string(ctx,2));
		return 0;  /* one return value */
	}

	int binaryToBase10(int n){
		int output = 0;
		for(int i=0;n>0;i++){
			if(n % 10 == 1){
				output += pow(2, i);
			}
			n /= 10;
		}
		return output;
	}

	duk_ret_t js_readFile(duk_context *ctx){
				int op = duk_get_int(ctx,1);
			if(op != 1){
				std::ifstream file(duk_to_string(ctx,0));
				duk_pop(ctx);
				if(file.is_open()){
					std::string str;
					char c;
					while(file.get(c) && file.good()){
						str.append(1,c);
					}
					duk_push_string(ctx,str.c_str());
				}else{
					duk_push_null(ctx);
				}
				file.close();
				return 1;
			}else{
				std::ifstream file(duk_to_string(ctx,0),std::ios::in | std::ios::binary);
				char buffer[256];
				file.read(buffer,256);
				file.close();
				int buff[256];
				for(int i=0;i<256;++i){
					//printw("%c,",buffer[i]);
				}
			}
			return 0;
	}

	//need save file

	duk_ret_t js_refresh(duk_context* ctx){
		//refresh();
		return 0;
	}
	duk_ret_t js_timeout(duk_context* ctx){
		//timeout((double)duk_get_number(ctx,0));
	
		return 0;
	}

	//window bindings

	duk_ret_t js_window(duk_context* ctx){
		if(duk_is_constructor_call(ctx)){
		
		}else{
			duk_push_string(ctx,"must use new operator");
			duk_throw(ctx);
		}
	}

	duk_ret_t js_exit(duk_context* ctx){
		clean_up(ctx);
		return 0;
	}


	//must be at the bottom
	void set_bindings(duk_context *ctx){
		duk_push_c_function(ctx, js_print, DUK_VARARGS);
		duk_put_global_string(ctx, "print");
		duk_push_c_function(ctx, js_mvprint,3);
		duk_put_global_string(ctx, "mvprint");
		duk_push_c_function(ctx, js_readFile,DUK_VARARGS);
		duk_put_global_string(ctx, "readFile");
		duk_push_c_function(ctx, js_refresh,0);
		duk_put_global_string(ctx, "refresh");
		duk_push_c_function(ctx, js_timeout,2);
		duk_put_global_string(ctx, "timeout");
		duk_push_c_function(ctx,js_window,0);
		duk_put_global_string(ctx,"Window");
		duk_push_c_function(ctx,js_exit,0);
		duk_put_global_string(ctx,"Exit");
	}

}
