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
#define ID_TRACKBAR 109
#define ID_SAVE_BUTTON 110

#define ID_MEMORYSIZE 256

// Variaveis globais
HWND input1_field, input2_field, output_field, memory_list, current_instruction_field, hDataGrid, trackbarText, trackbar, pc_field, acc_field, sp_field, mop_field, ri_field, re_field;
// Get input2_field text
char* getInput2FieldText() {
    int textLength = GetWindowTextLength(input2_field);
    char* text = (char*)malloc(textLength + 1);
    GetWindowText(input2_field, text, textLength + 1);
    return text;
}
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
// Seta o texto de PC
void setPC(char* text) {
    char pc_text[50] = "PC:";
    strcat(pc_text, text);
    SetWindowText(pc_field, pc_text);
}
// Seta o texto de ACC
void setACC(char* text) {
    char acc_text[50] = "ACC:";
    strcat(acc_text, text);
    SetWindowText(acc_field, acc_text);
}
// Seta o texto de SP
void setSP(char* text) {
    char sp_text[50] = "SP:";
    strcat(sp_text, text);
    SetWindowText(sp_field, sp_text);
}
// Seta o texto de MOP
void setMOP(char* text) {
    char mop_text[50] = "MOP:";
    strcat(mop_text, text);
    SetWindowText(mop_field, mop_text);
}
// Seta o texto de RI
void setRI(char* text) {
    char ri_text[50] = "RI:";
    strcat(ri_text, text);
    SetWindowText(ri_field, ri_text);
}
// Seta o texto de RE
void setRE(char* text) {
    char re_text[50] = "RE:";
    strcat(re_text, text);
    SetWindowText(re_field, re_text);
}
// Cria o memory datagrid
HWND CreateMemoryDataGrid(HWND hwnd, HINSTANCE hInstance){
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
// Adiciona texto ao output
void AppendTextToOutput(const char* text) {
    int textLength = GetWindowTextLength(output_field);
    // Definir o início da seleção como o final do texto atual
    SendMessage(output_field, EM_SETSEL, textLength, textLength);
    // Inserir o novo texto em uma nova linha
    SendMessage(output_field, EM_REPLACESEL, FALSE, (LPARAM)text);
    // Inserir uma nova linha após o novo texto
    SendMessage(output_field, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
}
// Adiciona items ao datagrid
void AddDataGridItem(char *text){
    // verifica se o datagrid foi criado
    if(hDataGrid == NULL){
        return;
    }
    // Conta a quantidade de items no datagrid
    int count = SendMessage(hDataGrid, LVM_GETITEMCOUNT, 0, 0);

    char str[100];
    sprintf(str, "%d", count);

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iSubItem = 0;
    lvItem.pszText = str;
    SendMessage(hDataGrid, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

    lvItem.iSubItem = 1;
    lvItem.pszText = text;
    SendMessage(hDataGrid, LVM_SETITEMTEXT, 0, (LPARAM)&lvItem);
}
// Seta o texto do datagrid
void SetMemoryDataGridItem(int index, char* text){
    // Setar o texto da coluna 1
    // Conta a quantidade de items no datagrid
    char str1[100];
    sprintf(str1, "%d", index);

    LVITEM lvItem = {0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iSubItem = 1;
    lvItem.pszText = text;

    // search for item in column 0 with value of indext and print it
    LVFINDINFO lvFindInfo = {0};
    lvFindInfo.flags = LVFI_STRING;
    lvFindInfo.psz = str1;
    int i = ListView_FindItem(hDataGrid, -1, &lvFindInfo);
    if (i != -1) {
        SendMessage(hDataGrid, LVM_SETITEMTEXT, i, (LPARAM)&lvItem);
    }

}
// Função para salvar o conteúdo do campo de texto multi-linhas em um arquivo de texto
void SaveTextToFile() {
    // Cria arquivo de texto para output
    char filename[100];
    sprintf(filename, "%s_output.txt", getInput2FieldText());
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        MessageBox(NULL, "Erro ao abrir o arquivo.", "Erro", MB_OK | MB_ICONERROR);
        return;
    }

    // Obter o comprimento do texto no campo de texto
    int textLength = GetWindowTextLength(output_field);

    // Alocar memória para o buffer de texto
    char* buffer = (char*)malloc((textLength + 1) * sizeof(char));
    if (buffer == NULL) {
        MessageBox(NULL, "Erro de memória.", "Erro", MB_OK | MB_ICONERROR);
        fclose(file);
        return;
    }

    // Obter o texto do campo de texto e copiá-lo para o buffer
    GetWindowText(output_field, buffer, textLength + 1);
    // Escrever o texto no arquivo de texto
    fprintf(file, "%s", buffer);
    // Liberar a memória alocada para o buffer
    free(buffer);
    // Fechar o arquivo de texto
    fclose(file);
        
    // Cria arquivo de texto para memória
    char filename1[100];
    sprintf(filename1, "%s_memory.txt", getInput2FieldText());

    file = fopen(filename1, "w");
    if (file == NULL) {
        MessageBox(NULL, "Erro ao abrir o arquivo.", "Erro", MB_OK | MB_ICONERROR);
        return;
    }
    // Obter o número de itens no controle de lista
    int itemCount = ListView_GetItemCount(hDataGrid);
    // Iterar sobre todos os itens e salvar os textos das colunas em linhas separadas no arquivo de texto
    for (int i = 0; i < itemCount; i++) {
        char buffer[256]; // Buffer para armazenar o texto de cada coluna
        for (int j = 0; j < 2; j++) {
            ListView_GetItemText(hDataGrid, i, j, buffer, sizeof(buffer));
            fprintf(file, "%s\t", buffer); // Escrever o texto no arquivo de texto, separado por uma tabulação
        }
        fprintf(file, "\n"); // Escrever uma quebra de linha após cada item
    }

    // Fechar o arquivo de texto
    fclose(file);

    MessageBox(NULL, "Memory e Output salvos com sucesso.", "Sucesso", MB_OK | MB_ICONINFORMATION);
}
// Funcao para criar a janela
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            //Aloca um console
            //AllocConsole(); 
            //Oculta o console
            ShowWindow(GetConsoleWindow(), SW_HIDE); 

            // Criar as caixas com título
            CreateWindow("button", "Select a File", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 510, 50, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Output", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 60, 510, 200, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Memory", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 260, 250, 395, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Current Instruction", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 260, 250, 50, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Registers", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 315, 250, 120, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Buttons", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 440, 250, 50, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Speed", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 495, 250, 75, hwnd, NULL, NULL, NULL);
            // Criar os rótulos para os registradores
            pc_field = CreateWindow("static", "PC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 333, 90, 25, hwnd, NULL, NULL, NULL);
            acc_field = CreateWindow("static", "ACC:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 363, 90, 25, hwnd, NULL, NULL, NULL);
            sp_field = CreateWindow("static", "SP:", WS_CHILD | WS_VISIBLE | SS_LEFT, 280, 393, 90, 25, hwnd, NULL, NULL, NULL);
            mop_field = CreateWindow("static", "MOP:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 333, 90, 25, hwnd, NULL, NULL, NULL);
            ri_field = CreateWindow("static", "RI:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 363, 90, 25, hwnd, NULL, NULL, NULL);
            re_field = CreateWindow("static", "RE:", WS_CHILD | WS_VISIBLE | SS_LEFT, 400, 393, 90, 25, hwnd, NULL, NULL, NULL);
            //Cria rótulo para a trackbar
            trackbarText = CreateWindow("static", "0", WS_CHILD | WS_VISIBLE | SS_LEFT, 395, 540, 50, 25, hwnd,(HMENU) ID_TRACKBAR, NULL, NULL);
            // Criar os controles
            input1_field = CreateWindow("edit", "Insert Input File Name", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 20, 28, 245, 25, hwnd, (HMENU) ID_INPUT1, NULL, NULL);
            input2_field = CreateWindow("edit", "Insert Output File Name", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 270, 28, 245, 25, hwnd, (HMENU) ID_INPUT2, NULL, NULL);
            output_field = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 20, 78, 490, 170, hwnd, NULL, GetModuleHandle(NULL), NULL);        
            current_instruction_field = CreateWindow("edit", "None", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 275, 278, 240, 25, hwnd, (HMENU) ID_CURRENT_INSTRUCTION, NULL, NULL);
            // Cria a trackbar
            trackbar = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE, 280, 513, 230, 25, hwnd, NULL, NULL, NULL);
            // Criar os botões
            CreateWindow("button", "Run", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 275, 458, 75, 25, hwnd, (HMENU) ID_RUN_BUTTON, NULL, NULL);
            CreateWindow("button", "Step", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 458, 75, 25, hwnd, (HMENU) ID_STEP_BUTTON, NULL, NULL);
            CreateWindow("button", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 458, 75, 25, hwnd, (HMENU) ID_CLEAR_BUTTON, NULL, NULL);
            CreateWindow("button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 600, 75, 25, hwnd, (HMENU) ID_SAVE_BUTTON, NULL, NULL);

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
            if (controlId == ID_RUN_BUTTON) {
                SetMemoryDataGridItem(1, "xxxx");
            }
            if (controlId == ID_STEP_BUTTON) {

            }
            if (controlId == ID_CLEAR_BUTTON) {
                setPC("1234");
                setACC("1234");
                setSP("1234");
                setMOP("1234");
                setRI("1234");
                setRE("1234");
                setCurrentInstruction("1234");
                AppendTextToOutput("1234");
                AddDataGridItem("teste");
            }
            if (controlId == ID_SAVE_BUTTON) {
                SaveTextToFile();
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
    hDataGrid = CreateMemoryDataGrid(hwnd, hInstance);

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
