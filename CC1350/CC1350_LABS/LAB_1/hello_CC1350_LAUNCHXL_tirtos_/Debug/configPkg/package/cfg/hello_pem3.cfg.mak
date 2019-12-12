# invoke SourceDir generated makefile for hello.pem3
hello.pem3: .libraries,hello.pem3
.libraries,hello.pem3: package/cfg/hello_pem3.xdl
	$(MAKE) -f C:\Users\ggmar\workspace_v9_2\hello_CC1350_LAUNCHXL_tirtos_/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\ggmar\workspace_v9_2\hello_CC1350_LAUNCHXL_tirtos_/src/makefile.libs clean

