#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>

//#include "src/prog.h"

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

/* how to use

setPC( "1234" );
setACC( "1234" );
setSP( "1234" );
setMOP( "1234" );
setRI( "1234" );
setRE( "1234" );
setCurrentInstruction( "1234" );
appendTextToOutput( "1234" );

*/

// Globlal variables
HWND input1_field, input2_field, output_field, memory_list, current_instruction_field, hDataGrid, trackbarText, trackbar, pc_field, acc_field, sp_field, mop_field, ri_field, re_field;
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
getTrackbarPos
-------------------
Return TrackBar Value
===================
*/
void getTrackbarPos() {
    int pos = SendMessage( trackbar, TBM_GETPOS, 0, 0 );
    char buffer[10];
    sprintf( buffer, "%d", pos );
    SetWindowText( trackbarText, buffer );
}
/*
===================
setCurrentInstruction
-------------------
Set the Current Instruction into Current Instruction Field
Receive a String into: char* text
===================
*/
void setCurrentInstruction( char* text ) {
    SetWindowText( current_instruction_field, text );
}
/*
===================
setPC
-------------------
Set the PC into PC Field
Receive a String into: char* text
===================
*/
void setPC( char* text ) {
    char pc_text[50] = "PC:";
    strcat( pc_text, text );
    SetWindowText( pc_field, pc_text );
}
/*
===================
setACC
-------------------
Set the ACC into ACC Field
Receive a String into: char* text
===================
*/
void setACC( char* text ) {
    char acc_text[50] = "ACC:";
    strcat( acc_text, text );
    SetWindowText( acc_field, acc_text );
}
/*
===================
setSP
-------------------
Set the SP into SP Field
Receive a String into: char* text
===================
*/
void setSP( char* text ) {
    char sp_text[50] = "SP:";
    strcat( sp_text, text );
    SetWindowText( sp_field, sp_text );
}
/*
===================
setMOP
-------------------
Set the MOP into MOP Field
Receive a String into: char* text
===================
*/
void setMOP( char* text ) {
    char mop_text[50] = "MOP:";
    strcat( mop_text, text );
    SetWindowText( mop_field, mop_text );
}
/*
===================
setRi
-------------------
Set the RI into RI Field
Receive a String into: char* text
===================
*/
void setRI( char* text ) {
    char ri_text[50] = "RI:";
    strcat( ri_text, text );
    SetWindowText( ri_field, ri_text );
}
/*
===================
setRE
-------------------
Set the RE into RE Field
Receive a String into: char* text
===================
*/
void setRE( char* text ) {
    char re_text[50] = "RE:";
    strcat( re_text, text );
    SetWindowText( re_field, re_text );
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
appendTextToOutput
-------------------
Append text to the output field
Receive a String into: char* text
===================
*/
void appendTextToOutput( const char* text ) {
    int textLength = GetWindowTextLength( output_field );
    // Define the start of the selection as the end of the current text
    SendMessage( output_field, EM_SETSEL, textLength, textLength );
    // Insert the new text on a new line
    SendMessage( output_field, EM_REPLACESEL, FALSE, ( LPARAM )text );
    // Insert the new line after the new text
    SendMessage( output_field, EM_REPLACESEL, FALSE, ( LPARAM )"\r\n" );
}
/*
===================
addDataGridItem
-------------------
Add a new memory block to the datagrid
Receive a String into: char* text
The String is the memory block value
===================
*/
void addDataGridItem( char *text ){
    // Check if hDataGrid is NULL/Empty
    if ( hDataGrid == NULL ){
        return;
    }
    // Check how many items are in the datagrid
    int count = SendMessage( hDataGrid, LVM_GETITEMCOUNT, 0, 0 );

    char str[100];
    sprintf( str, "%d", count );

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iSubItem = 0;
    lvItem.pszText = str;
    SendMessage( hDataGrid, LVM_INSERTITEM, 0, ( LPARAM )&lvItem );

    lvItem.iSubItem = 1;
    lvItem.pszText = text;
    SendMessage( hDataGrid, LVM_SETITEMTEXT, 0, ( LPARAM )&lvItem );
}
/*
===================
createMemory
-------------------
Create the hole memory, with the size of the memory
Receive a int into: int size
The int is the size of the memory
===================
*/
void createMemory( int size ) {
    for ( int i = 0; i < size; i++ ) {
            addDataGridItem( "0" );
    }
}
/*
===================
setMemoryDataGridItem
-------------------
Set the value of a memory block
Receive a int into: int index
The int is the index of the memory block
Receive a String into: char* text
The String is the memory block value
===================
*/
void setMemoryDataGridItem( int index, char* text ){
    char str1[100];
    sprintf( str1, "%d", index );

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iSubItem = 1;
    lvItem.pszText = text;

    // Search for item in column 0 with value of indext and print it
    LVFINDINFO lvFindInfo = {0};
    lvFindInfo.flags = LVFI_STRING;
    lvFindInfo.psz = str1;
    int i = ListView_FindItem( hDataGrid, -1, &lvFindInfo );
    if ( i != -1 ) {
        SendMessage( hDataGrid, LVM_SETITEMTEXT, i, ( LPARAM )&lvItem );
    }

}
/*
===================
resetMemory
-------------------
Reset the entire memory to 0
===================
*/
void resetMemory() {
    for ( int i = 0; i < ID_MEMORYSIZE; i++ ) {
        setMemoryDataGridItem( i, "0" );
    }
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
    printf( "sim" );
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
            pc_field = CreateWindow( "static", "PC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 333, 90, 25, hwnd, NULL, NULL, NULL );
            acc_field = CreateWindow( "static", "ACC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 363, 90, 25, hwnd, NULL, NULL, NULL );
            sp_field = CreateWindow( "static", "SP:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 393, 90, 25, hwnd, NULL, NULL, NULL );
            mop_field = CreateWindow( "static", "MOP:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 333, 90, 25, hwnd, NULL, NULL, NULL );
            ri_field = CreateWindow( "static", "RI:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 363, 90, 25, hwnd, NULL, NULL, NULL );
            re_field = CreateWindow( "static", "RE:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 393, 90, 25, hwnd, NULL, NULL, NULL );
            // Create label for trackbar speed
            trackbarText = CreateWindow( "static", "0", WS_CHILD | WS_VISIBLE | SS_LEFT, 395, 540, 50, 25, hwnd,( HMENU ) ID_TRACKBAR, NULL, NULL );
            // Create labels for controls 
            input1_field = CreateWindow( "edit", "Insert Input File Name", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 20, 28, 245, 25, hwnd, ( HMENU ) ID_INPUT1, NULL, NULL );
            input2_field = CreateWindow( "edit", "Insert Output File Name", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 270, 28, 245, 25, hwnd, ( HMENU ) ID_INPUT2, NULL, NULL );
            output_field = CreateWindowEx( WS_EX_CLIENTEDGE, "Edit", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 20, 78, 490, 170, hwnd, NULL, GetModuleHandle( NULL ), NULL );        
            current_instruction_field = CreateWindow( "edit", "None", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 275, 278, 240, 25, hwnd, ( HMENU ) ID_CURRENT_INSTRUCTION, NULL, NULL );
            // Create the trackbar
            trackbar = CreateWindow( TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE, 280, 513, 230, 25, hwnd, NULL, NULL, NULL );
            // Create the buttons 
            CreateWindow( "button", "Run", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 275, 458, 75, 25, hwnd, ( HMENU ) ID_RUN_BUTTON, NULL, NULL );
            CreateWindow( "button", "Step", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 458, 75, 25, hwnd, ( HMENU ) ID_STEP_BUTTON, NULL, NULL );
            CreateWindow( "button", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 458, 75, 25, hwnd, ( HMENU ) ID_CLEAR_BUTTON, NULL, NULL );
            CreateWindow( "button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 600, 75, 25, hwnd, ( HMENU ) ID_SAVE_BUTTON, NULL, NULL );
            // Set current_instruction_field and output_field to only read
            SendMessage( current_instruction_field, EM_SETREADONLY, TRUE, 0 );
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

            }
            if ( controlId == ID_CLEAR_BUTTON ) {
                setPC( "" );
                setACC( "" );
                setSP( "" );
                setMOP( "" );
                setRI( "" );
                setRE( "" );
                setCurrentInstruction( "None" );
                resetMemory();
            }
            if ( controlId == ID_SAVE_BUTTON ) {
                saveTextToFile();
            }
            break;
        case WM_HSCROLL:
        {
            // Se a barra de rolagem foi alterada pelo usuário
            if ( ( HWND )lParam == trackbar )
            {
                // Obtem a posição atual da barra de rolagem
                int pos = SendMessage( trackbar, TBM_GETPOS, 0, 0 );
                // Converte a posição para string
                char str[10];
                sprintf( str, "%d", pos );
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
    createMemory( ID_MEMORYSIZE );

    // Insert a BMP Tardis image
    HBITMAP hBitmap = ( HBITMAP ) LoadImage( NULL, "dalek.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
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
