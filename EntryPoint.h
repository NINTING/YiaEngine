#include"pch.h"
#include<stdio.h>

#include"Common/Logger.h"

#ifdef YIA_WINDOW
int main()
{
	LOG_INIT();
	YIA_GRAPHIC_ERR("hr = 0x{:08X}", 3124542);
	
	return 0;
}

#endif // YIA_WINDOW
