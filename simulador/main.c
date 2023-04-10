#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>
//#include "src/prog.h"

// IDs para os controles da janela
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

#define ID_MEMORYSIZE 256

// Variaveis globais
HWND input1_field, input2_field, output_field, memory_list, current_instruction_field, hDataGrid, trackbarText, trackbar;
// Busca a posição da Trackbar
void getTrackbarPos() {
    int pos = SendMessage(trackbar, TBM_GETPOS, 0, 0);
    char buffer[10];
    sprintf(buffer, "%d", pos);
    SetWindowText(trackbarText, buffer);
}
// Seta o texto da Current Instruction
void setCurrentInstruction(char* text) {
    SetWindowText(current_instruction_field, text);
}
// Funcao para criar o datagrid
HWND CreateDataGrid(HWND hwnd, HINSTANCE hInstance){
    HWND hDataGrid = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "MemoryList",  WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS, 15, 278, 240, 375, hwnd, (HMENU)ID_DATAGRID,hInstance, NULL);
    
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvc.cx = 118;
    lvc.pszText = "Memory";
    ListView_InsertColumn(hDataGrid, 0, &lvc);
    lvc.pszText = "Value";
    ListView_InsertColumn(hDataGrid, 1, &lvc);
    return hDataGrid;
}
// Funcao para adicionar texto ao output 
void AppendTextToOutput(const char* text) {
    // Obter o texto atual da caixa de edição
    int text_length = GetWindowTextLength(output_field);
    char* current_text = (char*) malloc(sizeof(char) * (text_length + 1));
    GetWindowText(output_field, current_text, text_length + 1);
    
    // Adicionar o novo texto em uma nova linha
    char* new_text = (char*) malloc(sizeof(char) * (text_length + strlen(text) + 3));
    sprintf(new_text, "%s\n%s", current_text, text);
    
    // Definir o texto atualizado na caixa de edição
    SetWindowText(output_field, new_text);
    
    // Liberar a memória alocada
    free(current_text);
    free(new_text);
}
// Funcao para adicionar itens ao datagrid
void AddDataGridItem(){
    // Adicionar linhas ao Datagrid
    // verifica se o datagrid foi criado
    if(hDataGrid == NULL){
        return;
    }
    // print quantidade de linhas na coluna 0
    int count = SendMessage(hDataGrid, LVM_GETITEMCOUNT, 0, 0);
    count = count+1;
    char str[100];
    sprintf(str, "%d", count);

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iSubItem = 0;
    lvItem.pszText = str;
    SendMessage(hDataGrid, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 1;
    lvItem.pszText = str;
    SendMessage(hDataGrid, LVM_SETITEMTEXT, 0, (LPARAM)&lvItem);
}
// Funcao para remover itens do datagrid
void RemoveDataGridItem(){
    // Remove a linha selecionada
    int index = SendMessage(hDataGrid, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
    SendMessage(hDataGrid, LVM_DELETEITEM, index, 0);
}
// Funcao que seta o texto do datagrind na coluna 1 buscando o valor da coluna 0, recebe um inteiro como parametro para buscar o valor da coluna 0 e setar o texto 0000 na coluna 1
void SetDataGridItem(int index){
    // Setar o texto da coluna 1
    char *str = "0000";
    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iSubItem = 0;
    lvItem.pszText = str;
    SendMessage(hDataGrid, LVM_SETITEMTEXT, index, (LPARAM)&lvItem);
}

// Funcao para criar a janela
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            //Aloca um console
            //AllocConsole(); 
            //Oculta o console
            //ShowWindow(GetConsoleWindow(), SW_HIDE); 

            // Criar as caixas com título
            CreateWindow("button", "Select a File", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 510, 50, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Output", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 60, 510, 200, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Memory", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 260, 250, 395, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Current Instruction", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 260, 250, 50, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Registers", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 315, 250, 120, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Buttons", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 440, 250, 50, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Speed", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 495, 250, 75, hwnd, NULL, NULL, NULL);
            // Criar os rótulos para os registradores
            CreateWindow("static", "PC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 333, 50, 25, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "ACC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 348, 50, 25, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "SP:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 363, 50, 25, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "MOP:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 378, 50, 25, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "RI:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 393, 50, 25, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "RE:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 408, 50, 25, hwnd, NULL, NULL, NULL);
            //Cria rótulo para a trackbar
            trackbarText = CreateWindow("static", "0", WS_CHILD | WS_VISIBLE | SS_LEFT, 395, 540, 50, 25, hwnd,(HMENU) ID_TRACKBAR, NULL, NULL);
            // Criar os controles
            input1_field = CreateWindow("edit", "cod-obj.txt", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 20, 28, 245, 25, hwnd, (HMENU) ID_INPUT1, NULL, NULL);
            input2_field = CreateWindow("edit", "Insert File Name", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 270, 28, 245, 25, hwnd, (HMENU) ID_INPUT2, NULL, NULL);
            
            output_field = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 20, 78, 490, 170, hwnd, (HMENU) ID_OUTPUT, NULL, NULL);
            
            current_instruction_field = CreateWindow("edit", "None", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 275, 278, 240, 25, hwnd, (HMENU) ID_CURRENT_INSTRUCTION, NULL, NULL);
            // Cria a trackbar
            trackbar = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE, 280, 513, 230, 25, hwnd, NULL, NULL, NULL);
            // Criar os botões
            HWND runButton = CreateWindow("button", "Run", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 275, 458, 75, 25, hwnd, (HMENU) ID_RUN_BUTTON, NULL, NULL);
            CreateWindow("button", "Step", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 458, 75, 25, hwnd, (HMENU) ID_STEP_BUTTON, NULL, NULL);
            CreateWindow("button", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 458, 75, 25, hwnd, (HMENU) ID_CLEAR_BUTTON, NULL, NULL);
            // seta current_instruction_field e output_field para apenas leitura
            SendMessage(current_instruction_field, EM_SETREADONLY, TRUE, 0);
            SendMessage(output_field, EM_SETREADONLY, TRUE, 0);

            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_COMMAND: {
            // Identificando o controle que gerou a mensagem
  
            int controlId = LOWORD(wParam);
            printf("\n %d", controlId);
            if (controlId == ID_RUN_BUTTON) {
                // read text from input1_field
                char text[256];
                //GetWindowText(input1_field, text, 256);
                //char *a = prog(text);
                //printf("a");
                // set text to output_field
                //SetWindowText(output_field, a);
                getTrackbarPos();
            }
            if (controlId == ID_STEP_BUTTON) {
                AddDataGridItem();
            }
            if (controlId == ID_CLEAR_BUTTON) {
                AppendTextToOutput("azsdfg\n");
            }
            break;
        case WM_HSCROLL:
        {
            // Se a barra de rolagem foi alterada pelo usuário
            if ((HWND)lParam == trackbar)
            {
                // Obtem a posição atual da barra de rolagem
                int pos = SendMessage(trackbar, TBM_GETPOS, 0, 0);
                // Converte a posição para string
                char str[10];
                sprintf(str, "%d", pos);
                // Atualiza o texto da janela
                SetWindowText(trackbarText, str);
            }
            break;
        }
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Funcao main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Registrar a classe da janela
    WNDCLASS wc = {0};
    wc.lpszClassName = "MyWindowClass";
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClass(&wc);

    //Busca a resolucao da tela
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    //Define a resolucao da janela
    int windowResWidth = 535;
    int windowResHeight = 700;

    //Calcula o centro da tela para setar a janela
    width = width/2-windowResWidth/2;
    height = height/2-windowResHeight/2;

    // Criar a janela
    HWND hwnd = CreateWindow("MyWindowClass", "Ada Lovelace", WS_OVERLAPPED | WS_SYSMENU, width, height, windowResWidth, windowResHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    
    // Cria o datagrid
    hDataGrid = CreateDataGrid(hwnd, hInstance);
    AddDataGridItem();
    AddDataGridItem();
    setCurrentInstruction("0000");
    // Loop da mensagem
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // fecha o console e mata o processo
    FreeConsole(); 
    system("taskkill/IM cb_console_runner.exe");
    return (0);
}
