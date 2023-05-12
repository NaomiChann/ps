#include "marcos.h"

// Window ID Controller
#define ID_INPUT1 100
#define ID_INPUT2 101
#define ID_OUTPUT 102
#define ID_MEMORY 103
#define ID_CURRENT_INSTRUCTION 104
#define ID_RUN_BUTTON 105
#define ID_STEP_BUTTON 106
#define ID_CLEAR_BUTTON 107
#define ID_DATAGRID 108
#define ID_TRACKBAR 109
#define ID_TRACKBAR 109
#define ID_SAVE_BUTTON 110
#define ID_MEMORYSIZE 1024

// Globlal variables
HWND input1_field, input2_field, output_field, memory_list, current_instruction_field, hDataGrid, trackbarText, trackbar, pc_field, acc_field, sw_field, rb_field, rs_field, rt_field, rx_field, rl_field;
int posUI_g;
int stepClick = 0;

void StepClickSet1(){
	stepClick = 1;
};

void StepClickSet0(){
	stepClick = 0;
};

int CheckStepClick(){
	return stepClick;
}

/*
===================
getInput2FieldText
-------------------
Return the text of the input2 field
===================
*/
char* getInput2FieldText() {
	int textLength = GetWindowTextLength( input2_field );
	char* text = ( char* )malloc( textLength + 1 );
	GetWindowText( input2_field, text, textLength + 1 );
	return text;
}
/*
===================
getInput1FieldText
-------------------
Return the text of the input1 field
===================
*/
char* getInput1FieldText() {
	int textLength = GetWindowTextLength( input1_field );
	char* text = ( char* )malloc( textLength + 1 );
	GetWindowText( input1_field, text, textLength + 1 );
	return text;
}
/*
===================
createMemoryDataGrid
-------------------
Create the Memory Data Grid
===================
*/
HWND createMemoryDataGrid( HWND hwnd, HINSTANCE hInstance ){
	HWND hDataGrid = CreateWindowEx( WS_EX_CLIENTEDGE, WC_LISTVIEW, "MemoryList",  WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS, 15, 278, 240, 375, hwnd, ( HMENU )ID_DATAGRID,hInstance, NULL );
	
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.cx = 118;
	lvc.pszText = "Memory";
	ListView_InsertColumn( hDataGrid, 0, &lvc );
	lvc.pszText = "Value";
	ListView_InsertColumn( hDataGrid, 1, &lvc );
	return hDataGrid;
}
/*
===================
resetMemory
-------------------
Remove the Memory from Data Grid
===================
*/
void resetMemory() {
	ListView_DeleteAllItems( hDataGrid );
}
/*
===================
saveTextToFile
-------------------
Save the current program to a text file in the same directory
Saves the memory to a file
Saves the output to a file
===================
*/
void saveTextToFile() {
	// Create the output file
	char filename[100];
	sprintf( filename, "%s_output.txt", getInput2FieldText() );
	FILE* file = fopen( filename, "w" );
	if ( file == NULL ) {
		MessageBox( NULL, "Erro ao abrir o arquivo.", "Erro", MB_OK | MB_ICONERROR );
		return;
	}

	// Get the text length of the output field
	int textLength = GetWindowTextLength( output_field );

	// Allocate memory for the text buffer
	char* buffer = ( char* )malloc( ( textLength + 1 ) * sizeof( char ) );
	if ( buffer == NULL ) {
		MessageBox( NULL, "Erro de memória.", "Erro", MB_OK | MB_ICONERROR );
		fclose( file );
		return;
	}
	// Get the text from the text field and copy it to the buffer
	GetWindowText( output_field, buffer, textLength + 1 );
	// Write the text to the text file
	fprintf( file, "%s", buffer );
	free( buffer );
	fclose( file );
		
	// Create the memory file
	char filename1[100];
	sprintf( filename1, "%s_memory.txt", getInput2FieldText() );

	file = fopen( filename1, "w" );
	if ( file == NULL ) {
		MessageBox( NULL, "Erro ao abrir o arquivo.", "Erro", MB_OK | MB_ICONERROR );
		return;
	}
	// Get the text length of the memory field
	int itemCount = ListView_GetItemCount( hDataGrid );
	for ( int i = 0; i < itemCount; i++ ) {
	// Buffer to get the text of each item    
		char buffer[256];
		for ( int j = 0; j < 2; j++ ) {
			ListView_GetItemText( hDataGrid, i, j, buffer, sizeof( buffer ) );
	// Write the text to the text file, separated by a tab
			fprintf( file, "%s\t", buffer ); 
		}
		fprintf( file, "\n" );
	}

	fclose( file );

	MessageBox( NULL, "Memory and Output successfully saved.", "Warning!", MB_OK | MB_ICONINFORMATION );
}
/*
===================
Sim
-------------------
Start the Simulation
===================
*/
void Sim(){
	Mamma(getInput1FieldText());
}
/*
===================
WndProc
-------------------
Windows Funcion to create the window
===================
*/
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch ( msg ) {
		case WM_CREATE:
			// Hide the console
			ShowWindow( GetConsoleWindow(), SW_HIDE ); 

			// Create box and title
			CreateWindow( "button", "Select a File", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 510, 50, hwnd, NULL, NULL, NULL );
			CreateWindow( "button", "Output", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 60, 510, 200, hwnd, NULL, NULL, NULL );
			CreateWindow( "button", "Memory", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 260, 250, 395, hwnd, NULL, NULL, NULL );
			CreateWindow( "button", "Current Instruction", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 260, 250, 50, hwnd, NULL, NULL, NULL );
			CreateWindow( "button", "Registers", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 315, 250, 120, hwnd, NULL, NULL, NULL );
			CreateWindow( "button", "Buttons", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 440, 250, 50, hwnd, NULL, NULL, NULL );
			CreateWindow( "button", "Speed", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 495, 250, 75, hwnd, NULL, NULL, NULL );
			// Create labels for registers
			acc_field = CreateWindow( "static", "ACC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 333, 90, 25, hwnd, NULL, NULL, NULL );
			rl_field = CreateWindow( "static", "L:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 355, 90, 25, hwnd, NULL, NULL, NULL );
			rs_field = CreateWindow( "static", "S:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 377, 90, 25, hwnd, NULL, NULL, NULL );
			pc_field = CreateWindow( "static", "PC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 399, 90, 25, hwnd, NULL, NULL, NULL );
			rx_field = CreateWindow( "static", "X:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 333, 90, 25, hwnd, NULL, NULL, NULL );
			rb_field = CreateWindow( "static", "B:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 355, 90, 25, hwnd, NULL, NULL, NULL );
			rt_field = CreateWindow( "static", "T:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 377, 90, 25, hwnd, NULL, NULL, NULL );
			sw_field = CreateWindow( "static", "SW:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 399, 90, 25, hwnd, NULL, NULL, NULL );
			current_instruction_field = CreateWindow( "static", "None", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 275, 278, 240, 25, hwnd, ( HMENU ) ID_CURRENT_INSTRUCTION, NULL, NULL );
			// Create label for trackbar speed
			trackbarText = CreateWindow( "static", "0", WS_CHILD | WS_VISIBLE | SS_LEFT, 395, 540, 50, 25, hwnd,( HMENU ) ID_TRACKBAR, NULL, NULL );
			// Create labels for controls 
			input1_field = CreateWindow( "edit", "program/input.asm", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 20, 28, 245, 25, hwnd, ( HMENU ) ID_INPUT1, NULL, NULL );
			input2_field = CreateWindow( "edit", "Insert Output File Name", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 270, 28, 245, 25, hwnd, ( HMENU ) ID_INPUT2, NULL, NULL );
			output_field = CreateWindowEx( WS_EX_CLIENTEDGE, "Edit", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 20, 78, 490, 170, hwnd, NULL, GetModuleHandle( NULL ), NULL );        
			// Create the trackbar
			trackbar = CreateWindow( TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE, 280, 513, 230, 25, hwnd, NULL, NULL, NULL );
			// Create the buttons 
			CreateWindow( "button", "Run", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 275, 458, 75, 25, hwnd, ( HMENU ) ID_RUN_BUTTON, NULL, NULL );
			CreateWindow( "button", "Step", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 458, 75, 25, hwnd, ( HMENU ) ID_STEP_BUTTON, NULL, NULL );
			CreateWindow( "button", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 458, 75, 25, hwnd, ( HMENU ) ID_CLEAR_BUTTON, NULL, NULL );
			CreateWindow( "button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 600, 75, 25, hwnd, ( HMENU ) ID_SAVE_BUTTON, NULL, NULL );
			// Set current_instruction_field and output_field to only read
			SendMessage( output_field, EM_SETREADONLY, TRUE, 0 );
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_COMMAND: {
			// Identify the control that sent the message
			int controlId = LOWORD( wParam );
			if ( controlId == ID_RUN_BUTTON ) {
				Sim();
			}
			if ( controlId == ID_STEP_BUTTON ) {
				setRB( "V" );
				StepClickSet0();
			}
			if ( controlId == ID_CLEAR_BUTTON ) {
				setPC( "0" );
				setACC( "0" );
				setSW( "0" );
				setRX( "0" );
				setRT( "0" );
				setRL( "0" );
				setRS( "0" );
				setRB( "0" );
				setCurrentInstruction( "None" );
				resetMemory();
			}
			if ( controlId == ID_SAVE_BUTTON ) {
				saveTextToFile();
				appendTextToOutput("File saved successfully!\n");
			}
			break;
		case WM_HSCROLL:
		{
			// Se a barra de rolagem foi alterada pelo usuário
			if ( ( HWND )lParam == trackbar )
			{
				// Obtem a posição atual da barra de rolagem
				posUI_g = SendMessage( trackbar, TBM_GETPOS, 0, 0 );
				// Converte a posição para string
				char str[10];
				sprintf( str, "%d", posUI_g );
				// Atualiza o texto da janela
				SetWindowText( trackbarText, str );
			}
			break;
		}
		}
		default:
			return DefWindowProc( hwnd, msg, wParam, lParam );
	}
	return 0;
}
/*
===================
WinMain
-------------------
Windows function to Start the program
===================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// Register the window class
	WNDCLASS wc = {0};
	wc.lpszClassName = "MyWindowClass";
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = ( HBRUSH ) COLOR_WINDOW;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	RegisterClass( &wc );

	// Get the screen resolution
	int width = GetSystemMetrics( SM_CXSCREEN );
	int height = GetSystemMetrics( SM_CYSCREEN );

	// Define the window resolution
	int windowResWidth = 535;
	int windowResHeight = 700;
	
	// Calculate the window position to center
	width = width/2-windowResWidth/2;
	height = height/2-windowResHeight/2;

	// Create the window
	HWND hwnd = CreateWindow( "MyWindowClass", "Ada Lovelace", WS_OVERLAPPED | WS_SYSMENU, width, height, windowResWidth, windowResHeight, NULL, NULL, hInstance, NULL );
	ShowWindow( hwnd, nCmdShow );
	
	// Create the memory data grid
	hDataGrid = createMemoryDataGrid( hwnd, hInstance );
	//createMemory( ID_MEMORYSIZE );

	// Insert a BMP Tardis image
	HBITMAP hBitmap = ( HBITMAP ) LoadImage( NULL, "misc/dalek.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	HWND hImage = CreateWindow( "static", NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 460, 580, 75, 25, hwnd, NULL, hInstance, NULL );
	SendMessage( hImage, STM_SETIMAGE, IMAGE_BITMAP, ( LPARAM ) hBitmap );

	// Message Loop
	MSG msg = {0};
	while ( GetMessage( &msg, NULL, 0, 0 ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	// Close console and kill process
	FreeConsole(); 
	system( "taskkill/IM cb_console_runner.exe" );
	return ( 0 );
}
