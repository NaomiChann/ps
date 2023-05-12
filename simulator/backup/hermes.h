#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>

// Globlal variables
extern HWND input1_field, input2_field, output_field, memory_list, current_instruction_field, hDataGrid, trackbarText, trackbar, pc_field, acc_field, sw_field, rb_field, rs_field, rt_field, rx_field, rl_field;
extern int posUI_g;


void addDataGridItem( char *text );

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
void setSW( char* text ) {
    char sw_text[50] = "SW:";
    strcat( sw_text, text );
    SetWindowText( sw_field, sw_text );
}
/*
===================
setMOP
-------------------
Set the MOP into MOP Field
Receive a String into: char* text
===================
*/
void setRB( char* text ) {
    char rb_text[50] = "B:";
    strcat( rb_text, text );
    SetWindowText( rb_field, rb_text );
}
/*
===================
setRi
-------------------
Set the RI into RI Field
Receive a String into: char* text
===================
*/
void setRL( char* text ) {
    char rl_text[50] = "L:";
    strcat( rl_text, text );
    SetWindowText( rl_field, rl_text );
}
/*
===================
setRE
-------------------
Set the RE into RE Field
Receive a String into: char* text
===================
*/
void setRX( char* text ) {
    char rx_text[50] = "X:";
    strcat( rx_text, text );
    SetWindowText( rx_field, rx_text );
}
/*
===================
setRE
-------------------
Set the RE into RE Field
Receive a String into: char* text
===================
*/
void setRS( char* text ) {
    char rs_text[50] = "S:";
    strcat( rs_text, text );
    SetWindowText( rs_field, rs_text );
}
/*
===================
setRE
-------------------
Set the RE into RE Field
Receive a String into: char* text
===================
*/
void setRT( char* text ) {
    char rt_text[50] = "T:";
    strcat( rt_text, text );
    SetWindowText( rt_field, rt_text );
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