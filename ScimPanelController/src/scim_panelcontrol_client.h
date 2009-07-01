#ifndef SCIM_PANELCONTROL_CLIENT_H_
#define SCIM_PANELCONTROL_CLIENT_H_

#include <string>

using namespace std;

extern "C" {

const int SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR							= 1;
const int SCIM_AUTOSWITCHER_PANEL_MESSAGE_TIMEOUT						= 2;
const int SCIM_AUTOSWITCHER_PANEL_RECEIVED_FAULTY_TRANSACTION_HEADER	= 3;

typedef basic_string<char> SimpleString;

struct KeyboardProperties
{
	SimpleString uuid;
	SimpleString name;
	SimpleString language;
	SimpleString pathToIcon;
};

bool 	OpenConnectionToScimPanel	();
void 	CloseConnectionToScimPanel	();
int 	GetListOfSupportedKeyboards	(KeyboardProperties* supportedKeyboards, int maxNumberOfKeyboards, int* numberOfReturnedKeyboards);
int 	SetKeyboard					(SimpleString uuidToChangeTo);
int		GetCurrentKeyboard			();

}

#endif /*SCIM_PANELCONTROL_CLIENT_H_*/