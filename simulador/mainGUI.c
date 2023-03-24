#include <windows.h>
#include <stdio.h>

// IDs para os controles da janela
#define ID_INPUT1 100
#define ID_INPUT2 101
#define ID_OUTPUT 102
#define ID_MEMORY 103
#define ID_CURRENT_INSTRUCTION 104
#define ID_RUN_BUTTON 105
#define ID_STEP_BUTTON 106
#define ID_CLEAR_BUTTON 107


// Variaveis globais
HWND input1_field, input2_field, output_field, memory_list, current_instruction_field;

// Funcao para criar a janela
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            AllocConsole(); // aloca um console
            ShowWindow(GetConsoleWindow(), SW_HIDE); // oculta o console
            // Criar as caixas com título
            CreateWindow("button", "Select a File", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 510, 50, hwnd, NULL, NULL, NULL);

            CreateWindow("button", "Output", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 60, 510, 200, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Memory", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 260, 250, 395, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Current Instruction", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 260, 250, 50, hwnd, NULL, NULL, NULL);
            CreateWindow("button", "Buttons", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 270, 310, 250, 50, hwnd, NULL, NULL, NULL);

            // Criar os controles
            input1_field = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 15, 28, 245, 25, hwnd, (HMENU) ID_INPUT1, NULL, NULL);
            input2_field = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 270, 28, 245, 25, hwnd, (HMENU) ID_INPUT2, NULL, NULL);
            output_field = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE, 20, 78, 490, 170, hwnd, (HMENU) ID_OUTPUT, NULL, NULL);
            memory_list = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD | LBS_SORT, 15, 278, 240, 385, hwnd, (HMENU) ID_MEMORY, NULL, NULL);
            current_instruction_field = CreateWindow("edit", "None", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 275, 278, 240, 25, hwnd, (HMENU) ID_CURRENT_INSTRUCTION, NULL, NULL);

            CreateWindow("button", "Run", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 275, 328, 75, 25, hwnd, (HMENU) ID_RUN_BUTTON, NULL, NULL);
            CreateWindow("button", "Step", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 358, 328, 75, 25, hwnd, (HMENU) ID_STEP_BUTTON, NULL, NULL);
            CreateWindow("button", "Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 440, 328, 75, 25, hwnd, (HMENU) ID_CLEAR_BUTTON, NULL, NULL);

            // Preencha a lista de memória com 256 itens de 0 a 255 e ortganiize-os em ordem crescente, utilize inteiros ao invés de characteres ou strings
            for (int i = 0; i < 256; i++) {
                char str[4];
                sprintf(str, "%d", i);
                SendMessage(memory_list, LB_ADDSTRING, 0, (LPARAM) str);
            }

            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
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
    // Criar a janela
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    int windowResWidth = 535;
    int windowResHeight = 700;

    width = width/2-windowResWidth/2;
    height = height/2-windowResHeight/2;

    HWND hwnd = CreateWindow("MyWindowClass", "Ada Lovelace", WS_OVERLAPPED | WS_SYSMENU, width, height, windowResWidth, windowResHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    // Loop da mensagem
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    FreeConsole(); // fecha o console
    system("taskkill/IM cb_console_runner.exe");
    return (0);
}
