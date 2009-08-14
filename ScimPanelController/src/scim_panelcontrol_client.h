#ifndef SCIM_PANELCONTROL_CLIENT_H_
#define SCIM_PANELCONTROL_CLIENT_H_

extern "C" {

const int SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR							= 1;
const int SCIM_AUTOSWITCHER_PANEL_MESSAGE_TIMEOUT						= 2;
const int SCIM_AUTOSWITCHER_PANEL_RECEIVED_FAULTY_TRANSACTION_HEADER	= 3;
const int SCIM_AUTOSWITCHER_PANEL_INVALID_KEYBOARD_ID					= 4;
const int SCIM_AUTOSWITCHER_PANEL_NO_CONNECTION_TO_PANEL				= 5;
const int SCIM_AUTOSWITCHER_PANEL_FAULTY_DATA_RECEIVED					= 6;
const int SCIM_AUTOSWITCHER_PANEL_NO_CURRENT_INPUT_CONTEXT				= 7;

const int MAXSTRINGLENGTH = 100;
const int MAXNUMBEROFSUPPORTEDKEYBOARDS = 50;

struct KeyboardProperties
{
	char uuid[MAXSTRINGLENGTH];
	char name[MAXSTRINGLENGTH];
	char language[MAXSTRINGLENGTH];
	char pathToIcon[MAXSTRINGLENGTH];
};

struct ContextInfo
{
	int frontendClient;
	int context;
};

bool 	OpenConnectionToScimPanel	();
int 	CloseConnectionToScimPanel	();
int 	GetListOfSupportedKeyboards	(KeyboardProperties supportedKeyboards[], int maxNumberOfKeyboards, int* numberOfReturnedKeyboards);
int 	SetKeyboard					(char KeyboardIdToChangeTo[]);
int		GetCurrentKeyboard			(KeyboardProperties *currentKeyboard);
bool  	ConnectionToScimPanelIsOpen ();
int 	GetCurrentInputContext		(ContextInfo *currentContext);
}

#endif /*SCIM_PANELCONTROL_CLIENT_H_*/
