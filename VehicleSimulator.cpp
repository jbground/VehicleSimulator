#include "VehicleSimulator.h"

#include <iostream>
#include <process.h>
#include <atlstr.h>
#include "framework.h"
#include "CommCtrl.h"
#include "wingdi.h"


#include "BasicThread.h"
#include "TaskManager.h"
#include "VehicleManager.h"

#define MAX_LOADSTRING 100
#define X_COUNT 100
#define Y_COUNT 70

#define START_X 0
#define START_Y 0

#define INTERVAL 10
#define HALF_INTERVAL INTERVAL/2

#define XPOS(x) ((x) * INTERVAL)
#define YPOS(y) ((y) * INTERVAL)
#define IDC_RUN 1003


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND v_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND auto_job_button;
HWND job_view;
HWND process_view;

//Simulator 관련 전역 변수
unsigned char map[100][70];
vector<Machine*> machine_list;
vector<Vehicle*> vehicle_list;
queue<TransportJob*> job_list;

TaskManager *tm;
VehicleManager* vm;

HANDLE hThread;
UINT dwThreadID;

HANDLE autoRepaintThread;
UINT autoRepaintThreadID;

HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH blue = CreateSolidBrush(RGB(0, 0, 255));
HBRUSH green = CreateSolidBrush(RGB(0, 255, 0));
HBRUSH gray = CreateSolidBrush(RGB(128, 128, 128));
HBRUSH rail = CreateHatchBrush(HS_DIAGCROSS, RGB(128, 128, 128));


UINT WINAPI AutoRepaintThread(void* arg)
{
    while (true)
    {
        InvalidateRect(v_hWnd, NULL, FALSE);
        ::Sleep(300);
    }
    return 0;
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    



    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VEHICLESIMULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VEHICLESIMULATOR));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//  함수: MyRegisterClass()
//  용도: 창 클래스를 등록합니다.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VEHICLESIMULATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_VEHICLESIMULATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//   함수: InitInstance(HINSTANCE, int)
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//   주석:이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //자동 새로고침 쓰레드 실행
   std::cout << "Start Auto Repaint Thread" << "\n";
   v_hWnd = hWnd;
   autoRepaintThread = (HANDLE)_beginthreadex(NULL, 0, AutoRepaintThread, NULL, 0, (unsigned*)&autoRepaintThreadID);

   std::cout << "Starting Vehicle Simulator" << "\n";

   tm = new TaskManager(&map, &machine_list, &vehicle_list, &job_list, hWnd);
   tm->Start();

   vm = new VehicleManager(&vehicle_list, hWnd);
   vm->Start();

   return TRUE;
}

void create_job(vector<Machine*>& machine_list, queue<TransportJob*>* out) {
    mt19937 gen;
    gen.seed(random_device()());
    uniform_int_distribution<mt19937::result_type> dist(0, machine_list.size() - 1);


    int source = dist(gen);
    int dest = dist(gen);

    while (source == dest) {
        dest = dist(gen);
    }

    if (machine_list.size() != 0) {
        Machine* source_machine = machine_list[source];
        Machine* dest_machine = machine_list[dest];

        TransportJob *job = new TransportJob(source_machine, dest_machine);
        out->push(job);
    }
    else {
        cout << "machine_list's size is zero \n";
    }
}

//왼쪽 마우스 클릭했을 시 실행
void OnLButtonDown(HWND ah_wnd, int a_x, int a_y) {


}

void setColumns(HWND hwnd, string& columnName)
{
    LVCOLUMN col;
    int chx = LOWORD(GetDialogBaseUnits());

    col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    col.fmt = LVCFMT_LEFT;
    col.cx = chx * 10 * columnName.length();
    col.pszText = (LPSTR)columnName.c_str();
    col.iSubItem = 1;
    ListView_InsertColumn(hwnd, 1, &col);
}

void addRow(HWND hwnd, string& colValue)
{
    
    if (colValue.length() < 1) return;
    int reto = 0;
    LVITEM ite = { 0 };

    ite.mask = LVIF_TEXT;
    ite.pszText = (LPSTR)colValue.c_str();
    reto = ListView_InsertItem(hwnd, &ite);
    if (reto >= 0)
        ListView_SetItemText(hwnd, reto, 1, (LPSTR)colValue.c_str());

}



void OnCheckerBoardPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    for (int x = 0; x <= 1000; x += 10) {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, 700);
    }

    for (int y = 0; y <= 700; y += 10) {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, 1000, y);
    }

    

    for (int x = 0; x < X_COUNT; x++) {
        for (int y = 0; y < Y_COUNT; y++) {

            if (map[x][y] == '0') {
                SelectObject(hdc, rail);
                Rectangle(hdc, XPOS(x), XPOS(y), XPOS(x) + INTERVAL, XPOS(y) + INTERVAL);
            }

            //machine 위치 표시
            if (map[x][y] == 'm') {
                SelectObject(hdc, gray);
                Rectangle(hdc, XPOS(x), XPOS(y), XPOS(x) + INTERVAL, XPOS(y) + INTERVAL);
            }

            //vehicle 위치 표시
            for (Vehicle *v : vehicle_list) {
                if (v->getX() == x && v->getY() == y) {
                    if (v->isMoved()) {
                        SelectObject(hdc, green);
                        Rectangle(hdc, XPOS(x), XPOS(y), XPOS(x) + INTERVAL, XPOS(y) + INTERVAL);
                    }
                    else 
                    {
                        SelectObject(hdc, blue);
                        Rectangle(hdc, XPOS(x), XPOS(y), XPOS(x) + INTERVAL, XPOS(y) + INTERVAL);
                    }
                    
                }
            }
        }
    }

    EndPaint(hWnd, &ps);
}

//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//  용도: 주 창의 메시지를 처리합니다.
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message) {
        case WM_CREATE: 
        {
            auto_job_button = CreateWindow((LPCSTR)"Button", (LPCSTR)"작업 자동 생성", WS_CHILD | WS_VISIBLE, 1000, 15, 175, 25, hWnd, (HMENU)IDC_RUN, hInst, 0);
            job_view = CreateWindow(WC_LISTVIEW, (LPCSTR)"reserved job list", WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT | LVS_EDITLABELS, 1000, 50, 400, 300, hWnd, 0, hInst, 0);
            string columnName = "job log";

            setColumns(job_view, columnName);
            process_view = CreateWindow(WC_LISTVIEW, (LPCSTR)"process list", WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT | LVS_EDITLABELS, 1000, 350, 400, 300, hWnd, 0, hInst, 0);
            
            columnName = "vehicle log";

            setColumns(process_view, columnName);

            break;
        }

        case WM_COMMAND: 
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId) 
            {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); 
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                case IDC_RUN: 
                {
                    //버튼 클릭시 작업 생성
                    create_job(machine_list, &job_list);
                    //string test = "testetstets";
                    //addRow(job_view, test);
                    break;
                }
                case 1000: //job log section
                {
                    std::string str = *(std::string*)lParam;
                    addRow(job_view, str);
                    break;
                }
                case 1001: // vehicle log section
                {
                   
                    std::string str = *(std::string*)lParam;
                    addRow(process_view, str);

                    break;
                }
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
          
        case WM_PAINT:
        {
            OnCheckerBoardPaint(hWnd);
            break;
        }
      

        case WM_LBUTTONDOWN:
        {

            int x = LOWORD(lParam); //하위 16비트 값 분리, 마우스가 클릭된 x 좌표
            int y = HIWORD(lParam); //상위 16비트 값 분리, 마우스카 클릭된 y 좌표

            OnLButtonDown(hWnd, x, y);
            break;
        }
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;

        }
      
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}




