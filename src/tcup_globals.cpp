#include "duktape.h"
#include "tcup_globals.h"

namespace tcup{
	int END_CALLED = false;
	void clean_up(duk_context* ctx){
		if(!END_CALLED){
			duk_destroy_heap(ctx);
			//endwin();
			DUK_EXIT(0);
			END_CALLED = true;
		}
	}
}
