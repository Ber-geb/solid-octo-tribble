# invoke SourceDir generated makefile for empty.pem4f
empty.pem4f: .libraries,empty.pem4f
.libraries,empty.pem4f: package/cfg/empty_pem4f.xdl
	$(MAKE) -f C:\Users\ggmar\workspace_v9_2\blink_TM4C_RTOS/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\ggmar\workspace_v9_2\blink_TM4C_RTOS/src/makefile.libs clean

