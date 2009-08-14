lib_LTLIBRARIES = libScimPanelController.la
include_HEADERS = scim_panelcontrol_client.h
libScimPanelController_la_SOURCES = scim_panelcontrol_client.cpp scim_panelcontrol_client.h
libScimPanelController_la_CPPFLAGS = @SCIM_CFLAGS@
libScimPanelController_la_LIBADD = -lX11 @SCIM_LIBS@
libScimPanelController_la_LDFLAGS = -version-info 1:0:0 -no-undefined