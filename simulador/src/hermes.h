#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>

// Globlal variables
extern HWND input1_field, input2_field, output_field, memory_list, current_instruction_field, hDataGrid, trackbarText, trackbar, pc_field, acc_field, sp_field, mop_field, ri_field, re_field;

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