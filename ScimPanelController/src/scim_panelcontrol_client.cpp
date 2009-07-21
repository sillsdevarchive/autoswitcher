#define Uses_SCIM_SOCKET
#define Uses_SCIM_TRANSACTION
#define Uses_SCIM_TRANS_COMMANDS
#define DEBUG
#include <cstring>
#include <X11/Xlib.h>
#include <scim.h>
#include <scim_panel_common.h>
#include "scim_panelcontrol_client.h"

using namespace std;
using namespace scim;

//forward declaration of private functions
bool	uuidIsValid									(String KeyboardIdToChangeTo);
int		populateListOfValidUuids					();
bool 	writeTransactionHeader 						();
bool 	sendTransaction 							();
int 	wait_for_response_from_agent				();
int 	verify_transaction_header					(int expected_cmd);
int 	wait_for_response_from_agent				();
int  	get_connection_number  						();

void 	copyPanelFactoryInfoToKeyboardProperties	(PanelFactoryInfo factoryInfo, KeyboardProperties *keyboardproperty);

//global variables
int				m_socket_timeout = scim_get_default_socket_timeout ();
uint32			m_socket_magic_key = 0;
Transaction     m_send_trans;
Transaction     m_recv_trans;
SocketClient	m_socket;
int 			m_send_refcount = 0;

//public functions
bool  OpenConnectionToScimPanel ()
{
#ifdef DEBUG
	cout << "OpenConnectionToScimPanel" << endl;
#endif
	bool connection_opened = false;

	Display *m_display = XOpenDisplay (NULL);

	if (!m_display)
		return connection_opened;

	SocketAddress addr (scim_get_default_panel_socket_address (String DisplayString(m_display)));

	if (m_socket.connect (addr)){
		if (scim_socket_open_connection (m_socket_magic_key, String ("PanelController"), String ("Panel"), m_socket, m_socket_timeout)){
			connection_opened = true;
		}
		else if (m_socket.is_connected ()){
			CloseConnectionToScimPanel ();
		}
	}

	XCloseDisplay (m_display);

	return connection_opened;
}

bool  ConnectionToScimPanelIsOpen ()
{
#ifdef DEBUG
	cout << "ConnectionToScimPanelIsOpen" << endl;
#endif
	return m_socket.is_connected();
}

int CloseConnectionToScimPanel ()
{
#ifdef DEBUG
	cout << "CloseConnectionToScimPanel" << endl;
#endif
	int return_status = 0;

	if(!m_socket.is_connected()) return SCIM_AUTOSWITCHER_PANEL_NO_CONNECTION_TO_PANEL;

	m_socket.close ();
	m_socket_magic_key = 0;
	return return_status;

}

int GetListOfSupportedKeyboards (KeyboardProperties supportedKeyboards[], int maxNumberOfKeyboards, int* numberOfReturnedKeyboards)
{
#ifdef DEBUG
	cout << "GetListOfSupportedKeyboards" << endl;
#endif
	int return_status = 0;

	if(!m_socket.is_connected()) return SCIM_AUTOSWITCHER_PANEL_NO_CONNECTION_TO_PANEL;

	writeTransactionHeader();
	m_send_trans.put_command (SCIM_TRANS_CMD_CONTROLLER_REQUEST_FACTORY_MENU);
	sendTransaction();

	return_status = wait_for_response_from_agent();

	if (return_status == 0){
		return_status = verify_transaction_header(SCIM_TRANS_CMD_PANEL_SHOW_FACTORY_MENU);
		if (return_status == 0){
			PanelFactoryInfo factoryInfo;
			*numberOfReturnedKeyboards = 0;

			while (m_recv_trans.get_data (factoryInfo.uuid) && m_recv_trans.get_data (factoryInfo.name) &&
				   m_recv_trans.get_data (factoryInfo.lang) && m_recv_trans.get_data (factoryInfo.icon) &&
				   *numberOfReturnedKeyboards < maxNumberOfKeyboards) {
				factoryInfo.lang = scim_get_normalized_language (factoryInfo.lang);

				copyPanelFactoryInfoToKeyboardProperties(factoryInfo, &supportedKeyboards[*numberOfReturnedKeyboards]);

				(*numberOfReturnedKeyboards)++;
			}
		}
	}
	return return_status;
}

int SetKeyboard (char KeyboardIdToChangeTo[MAXSTRINGLENGTH])
{
#ifdef DEBUG
	cout << "SetKeyboard" << endl;
#endif

	int return_status = 0;

	if (!m_socket.is_connected()) return SCIM_AUTOSWITCHER_PANEL_NO_CONNECTION_TO_PANEL;

	ContextInfo currentContext;
	return_status = GetCurrentInputContext(currentContext);

	if (return_status != 0) return return_status;

	if (currentContext.frontendClient == -1 && currentContext.context == 0)
		return SCIM_AUTOSWITCHER_PANEL_NO_CURRENT_INPUT_CONTEXT;

	if (!uuidIsValid(KeyboardIdToChangeTo)) return SCIM_AUTOSWITCHER_PANEL_INVALID_KEYBOARD_ID;

	cout << "uuid is valid" << endl;
	writeTransactionHeader();
	m_send_trans.put_command (SCIM_TRANS_CMD_CONTROLLER_CHANGE_FACTORY);
	m_send_trans.put_data (KeyboardIdToChangeTo);
	sendTransaction();
	cout << "sent request to change keyboard" << endl;
	return_status = wait_for_response_from_agent();

	if (return_status == 0){

		return_status = verify_transaction_header(SCIM_TRANS_CMD_PANEL_UPDATE_FACTORY_INFO);
		if (return_status == 0){

			PanelFactoryInfo info;
			if (m_recv_trans.get_data (info.uuid) && m_recv_trans.get_data (info.name) &&
				m_recv_trans.get_data (info.lang) && m_recv_trans.get_data (info.icon)) {
				info.lang = scim_get_normalized_language (info.lang);
			}
			else return_status = SCIM_AUTOSWITCHER_PANEL_FAULTY_DATA_RECEIVED;
		}
	}

	return return_status;
}

int GetCurrentKeyboard (KeyboardProperties *currentKeyboard)
{
#ifdef DEBUG
	cout << "GetCurrentKeyboard" << endl;
#endif

	int return_status = 0;

	if(!m_socket.is_connected()) return SCIM_AUTOSWITCHER_PANEL_NO_CONNECTION_TO_PANEL;

	writeTransactionHeader();
	m_send_trans.put_command (SCIM_TRANS_CMD_CONTROLLER_GET_CURRENT_FACTORY);
	sendTransaction();

	return_status = wait_for_response_from_agent();

	if (return_status == 0){
		return_status = verify_transaction_header(SCIM_TRANS_CMD_PANEL_RETURN_CURRENT_FACTORY_INFO);
		if (return_status == 0){

			PanelFactoryInfo info;
			if (m_recv_trans.get_data (info.uuid) && m_recv_trans.get_data (info.name) &&
				m_recv_trans.get_data (info.lang) && m_recv_trans.get_data (info.icon)) {
				info.lang = scim_get_normalized_language (info.lang);
				copyPanelFactoryInfoToKeyboardProperties( info, currentKeyboard);
			}
			else return_status = SCIM_AUTOSWITCHER_PANEL_FAULTY_DATA_RECEIVED;
		}
	}

	return return_status;
}

bool GetCurrentInputContext	(ContextInfo& currentContext){

#ifdef DEBUG
	cout << "GetCurrentInputContext" << endl;
#endif

	int return_status = 0;

	if(!m_socket.is_connected()) return SCIM_AUTOSWITCHER_PANEL_NO_CONNECTION_TO_PANEL;

	writeTransactionHeader();
	m_send_trans.put_command (SCIM_TRANS_CMD_CONTROLLER_GET_CURRENT_CONTEXT);
	sendTransaction();

	return_status = wait_for_response_from_agent();

	if (return_status == 0){
		return_status = verify_transaction_header(SCIM_TRANS_CMD_PANEL_RETURN_CURRENT_CONTEXT);
		if (return_status == 0){
			uint32 int32_client;
			uint32 int32_context;
			if (m_recv_trans.get_data (int32_client) && m_recv_trans.get_data (int32_context)) {
				currentContext.frontendClient = int32_client;
				currentContext.context = int32_context;
			}
			else return_status = SCIM_AUTOSWITCHER_PANEL_FAULTY_DATA_RECEIVED;
		}
	}

	return return_status;
}

//private functions
void copyPanelFactoryInfoToKeyboardProperties(PanelFactoryInfo factoryInfo, KeyboardProperties *keyboardproperty){
	strcpy(keyboardproperty->uuid, factoryInfo.uuid.c_str());
	strcpy(keyboardproperty->name, factoryInfo.name.c_str());
	strcpy(keyboardproperty->language, factoryInfo.lang.c_str());
	strcpy(keyboardproperty->pathToIcon, factoryInfo.icon.c_str());
}

bool uuidIsValid(String keyboardIdToChangeTo){
	bool return_status = false;
	KeyboardProperties supportedKeyboards[MAXNUMBEROFSUPPORTEDKEYBOARDS];
	int numSupportedKeyboards = 0;

	GetListOfSupportedKeyboards(supportedKeyboards, MAXNUMBEROFSUPPORTEDKEYBOARDS, &numSupportedKeyboards);

	for(int i = 0; i<numSupportedKeyboards; ++i){
		if(keyboardIdToChangeTo.compare(supportedKeyboards[i].uuid) == 0){
			return_status = true;
		}
	}
	return return_status;
}

int wait_for_response_from_agent (){
	int return_status = 0;

	fd_set active_fds;
	timeval	timeout;
	int panel_fd = get_connection_number();

	FD_ZERO(&active_fds);
	FD_SET(panel_fd, &active_fds);

	timeout.tv_sec = 3;
	timeout.tv_usec = 0; //3 * 1000 milliseconds timeout

	int select_result = select (panel_fd + 1, &active_fds, NULL, NULL, &timeout);

	if(select_result < 0){
		return_status = SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR;
	}
	else if(select_result == 0){
		return_status = SCIM_AUTOSWITCHER_PANEL_MESSAGE_TIMEOUT;
	}
	return return_status;
}

int verify_transaction_header (int expected_cmd){
	int return_status = 0;
	if (!m_socket.is_connected () || !m_recv_trans.read_from_socket (m_socket, m_socket_timeout))
		return SCIM_AUTOSWITCHER_PANEL_SOCKET_ERROR;

	int cmd_type;
	int cmd;
	uint32 context = (uint32)(-1);

	bool command_is_a_reply = (m_recv_trans.get_command (cmd_type)) && (cmd_type == SCIM_TRANS_CMD_REPLY);
	bool context_is_correct = (m_recv_trans.get_data (context)) /*&& context != SCIM_TRANS_CMD_REPLY &&  Fix Me! --TA*/;
	bool command_is_expected_command = (m_recv_trans.get_command (cmd))  && (cmd == expected_cmd);

	if ( !(command_is_a_reply &&
		 context_is_correct &&
		 command_is_expected_command))
		return_status = SCIM_AUTOSWITCHER_PANEL_RECEIVED_FAULTY_TRANSACTION_HEADER;
	return return_status;
}

int  get_connection_number  ()
{
	return m_socket.get_id ();
}

bool writeTransactionHeader ()
{
	if (!m_socket.is_connected ()) return false;

	m_send_trans.clear ();
	m_send_trans.put_command (SCIM_TRANS_CMD_REQUEST);
	m_send_trans.put_data (m_socket_magic_key);
	return true;
}

bool sendTransaction ()
{
	if (!m_socket.is_connected ()) return false;

	if (m_send_trans.get_data_type () != SCIM_TRANS_DATA_UNKNOWN)
		return m_send_trans.write_to_socket (m_socket, 0x4d494353);

	return false;
}