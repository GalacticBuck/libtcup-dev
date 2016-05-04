#include "duktape.h"
#include "tcup_event.h"


duk_ret_t js_eventOn(duk_context* ctx){
	/*
		"eventName":[{bound:[OBJECT],func:[FUNCTION]}]
	*/
	int l;
	duk_require_string(ctx,0);
	duk_require_function(ctx,1);
	duk_push_global_stash(ctx);//0
	duk_get_prop_string(ctx,-1,"EventHandler");//1
	if(duk_get_prop_string(ctx,-1,duk_get_string(ctx,0))){//2
		l = duk_get_length(ctx,-1);
		duk_idx_t o = duk_push_object(ctx);
		duk_put_prop_index(ctx,-2,l);
		duk_get_prop_index(ctx,-1,l);
		duk_push_this(ctx);
		if(duk_is_undefined(ctx,-1)){
			duk_pop(ctx);
			duk_push_global_object(ctx);
		}
		duk_put_prop_string(ctx,-2,"bound");
		duk_dup(ctx,1);
		duk_put_prop_string(ctx,-2,"func");
	}else{
		duk_pop(ctx);
		l = 0;
		duk_idx_t a = duk_push_array(ctx);
		duk_put_prop_string(ctx,-2,duk_get_string(ctx,0));
		duk_get_prop_string(ctx,-1,duk_get_string(ctx,0));
		duk_idx_t o = duk_push_object(ctx);
		duk_put_prop_index(ctx,-2,0);
		duk_get_prop_index(ctx,-1,0);
		duk_push_this(ctx);
		if(duk_is_undefined(ctx,-1)){
			duk_pop(ctx);
			duk_push_global_object(ctx);
		}
		duk_put_prop_string(ctx,-2,"bound");
		duk_dup(ctx,1);
		duk_put_prop_string(ctx,-2,"func");
	}
	duk_push_int(ctx,l);
	return 1;
}

duk_ret_t js_eventEmit(duk_context* ctx){
	duk_require_string(ctx,0);
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx,-1,"EventHandler");
	if(duk_get_prop_string(ctx,-1,duk_get_string(ctx,0))){
		int l = duk_get_length(ctx,-1);
		int n = duk_get_top(ctx);
		for(int i=0;i<l;++i){
			if(!duk_get_prop_index(ctx,-1,i)){
				duk_pop_3(ctx);
				return 0;
			};
			duk_get_prop_string(ctx,-1,"func");
			duk_get_prop_string(ctx,-2,"bound");
			duk_push_this(ctx);
			if(duk_is_undefined(ctx,-1)){
				duk_pop(ctx);
				duk_push_global_object(ctx);
			}
			for(int j=1;j<n;++j){
				duk_dup(ctx,j);
			}
			duk_call_method(ctx,n);
			duk_pop_2(ctx);
		}
	}
	return 0;
}

duk_ret_t js_eventOff(duk_context* ctx){
	duk_require_string(ctx,0);
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx,-1,"EventHandler");
	if(!duk_is_number(ctx,1)){
		if(duk_get_prop_string(ctx,-1,duk_get_string(ctx,0))){
			int l = duk_get_length(ctx,-1);
			duk_push_array(ctx);
			for(int i=0;i<l;++i){
				int nl = duk_get_length(ctx,-1);
				duk_get_prop_index(ctx,-2,i);
				duk_get_prop_string(ctx,-1,"bound");
				duk_push_this(ctx);
				if(duk_equals(ctx,-1,-2)){
					duk_del_prop_index(ctx,-5,i);
				}else{
					duk_get_prop_index(ctx,-5,i);
					duk_put_prop_index(ctx,-5,nl);
				}
				duk_pop_3(ctx);
			}
			duk_put_prop_string(ctx,-2,duk_get_string(ctx,0));
		}else{
			duk_pop(ctx);
		}
	}else{
		if(duk_get_prop_string(ctx,-1,duk_get_string(ctx,0))){
			int en = duk_get_int(ctx,1);
			if(duk_has_prop_index(ctx,-1,en)){
				duk_del_prop_index(ctx,-1,en);
				int l = duk_get_length(ctx,-1);
				duk_push_array(ctx);
				for(int i=0;i<l;++i){
					int nl = duk_get_length(ctx,-1);
					if(i != en){
						duk_get_prop_index(ctx,-2,i);
						duk_put_prop_index(ctx,-2,nl);
					}
				}
				duk_put_prop_string(ctx,-3,duk_get_string(ctx,0));
			}
		}else{
			duk_pop(ctx);
		}
	}
	return 0;
}

duk_ret_t js_bindEventHandler(duk_context* ctx){
	duk_require_object_coercible(ctx,0);
	duk_push_global_object(ctx);
	duk_dup(ctx,0);
	duk_get_prop_string(ctx,-2,"on");
	duk_put_prop_string(ctx,-2,"on");
	duk_get_prop_string(ctx,-2,"emit");
	duk_put_prop_string(ctx,-2,"emit");
	duk_get_prop_string(ctx,-2,"off");
	duk_put_prop_string(ctx,-2,"off");
	return 0;
}

void create_eventHandler(duk_context* ctx){
	duk_push_global_stash(ctx);
	duk_idx_t o = duk_push_object(ctx);
	duk_put_prop_string(ctx,-2,"EventHandler");
	duk_push_c_function(ctx,js_eventOn,2);
	duk_put_global_string(ctx,"on");
	duk_push_c_function(ctx,js_eventEmit,DUK_VARARGS);
	duk_put_global_string(ctx,"emit");
	duk_push_c_function(ctx,js_eventOff,DUK_VARARGS);
	duk_put_global_string(ctx,"off");
	
	duk_push_c_function(ctx,js_bindEventHandler,1);
	duk_put_global_string(ctx,"bindEventHandler");
}


int event_on(duk_context* ctx,duk_idx_t listener,const char* event_name, duk_idx_t func){
	int l;
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx,-1,"EventHandler");
	if(duk_get_prop_string(ctx,-1,event_name)){
		l = duk_get_length(ctx,-1);
		duk_push_object(ctx);
		duk_dup(ctx,(listener<0)? listener-4:listener);
		duk_put_prop_string(ctx,-2,"bound");
		duk_dup(ctx,(func<0)? func-4:func);
		duk_remove(ctx,(func<0)? func-5:func);
		duk_put_prop_string(ctx,-2,"func");
		duk_put_prop_index(ctx,-2,l);
		duk_pop(ctx);
	}else{
		duk_pop(ctx);
		l = 0;
		duk_push_array(ctx);
		duk_push_object(ctx);
		duk_dup(ctx,(listener<0)? listener-4:listener);
		duk_put_prop_string(ctx,-2,"bound");
		duk_dup(ctx,(func<0)? func-4:func);
		duk_remove(ctx,(func<0)? func-5:func);
		duk_put_prop_string(ctx,-2,"func");
		duk_put_prop_index(ctx,-2,0);
		duk_put_prop_string(ctx,-2,event_name);
	}
	duk_pop_2(ctx);
	return l;
}

void event_emit(duk_context* ctx,duk_idx_t sender, const char* event_name, size_t args){
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx,-1,"EventHandler");
	if(duk_get_prop_string(ctx,-1,event_name)){
		int l = duk_get_length(ctx,-1);
		for(int i=0;i<l;++i){
			if(!duk_get_prop_index(ctx,-1,i)){
				duk_pop_3(ctx);
				return;
			}
			duk_get_prop_string(ctx,-1,"func");
			duk_get_prop_string(ctx,-2,"bound");
			duk_dup(ctx,(sender <0)? sender-6:sender);
			for(int j = args;j>0;--j){
				duk_dup(ctx,-(5+j));
			}
			duk_call_method(ctx,args+1);
			duk_pop_2(ctx);
		}
	}
	duk_pop_3(ctx);
}

void event_off(duk_context* ctx, const char* event_name, duk_idx_t obj, bool exact_event){
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx,-1,"EventHandler");
	if(!exact_event){
		if(duk_get_prop_string(ctx,-1,event_name)){
			int l = duk_get_length(ctx,-1);
			duk_push_array(ctx);
			for(int i=0;i<l;++i){
				int nl = duk_get_length(ctx,-1);
				duk_get_prop_index(ctx,-2,i);
				duk_get_prop_string(ctx,-1,"bound");
				duk_dup(ctx,(obj<0)? obj-6:obj);
				if(duk_equals(ctx,-1,-2)){
					duk_del_prop_index(ctx,-5,i);
				}else{
					duk_get_prop_index(ctx,-5,i);
					duk_put_prop_index(ctx,-5,nl);
				}
				duk_pop_3(ctx);
			}
			duk_put_prop_string(ctx,-2,event_name);
		}else{
			duk_pop(ctx);
		}
	}else{
		if(duk_get_prop_string(ctx,-1,event_name)){
			if(duk_has_prop_index(ctx,-1,obj)){
				duk_del_prop_index(ctx,-1,obj);
				int l = duk_get_length(ctx,-1);
				duk_push_array(ctx);
				for(int i=0;i<l;++i){
					int nl = duk_get_length(ctx,-1);
					if(i != obj){
						duk_get_prop_index(ctx,-2,i);
						duk_put_prop_index(ctx,-2,nl);
					}
				}
				duk_put_prop_string(ctx,-3,event_name);
			}
		}else{
			duk_pop(ctx);
		}
	}
}


int global_event_on(duk_context* ctx, const char* event_name, duk_idx_t func){
	duk_push_global_object(ctx);
	int en = event_on(ctx,-1,event_name,(func<0)? func-1:func);
	duk_pop(ctx);
	return en;
}

void global_event_emit(duk_context* ctx, const char* event_name, size_t args){
	duk_push_global_object(ctx);
	for(int i=args;i>0;--i){
		duk_dup(ctx,-(i+1));
	}
	event_emit(ctx,-1,event_name,args);
	duk_pop_n(ctx,args+1);
}

void global_event_off(duk_context* ctx, const char* event_name,int i){
	event_off(ctx,event_name,i,true);
}

void global_event_off(duk_context* ctx, const char* event_name){
	duk_push_global_object(ctx);
	event_off(ctx,event_name,-1,false);
}
