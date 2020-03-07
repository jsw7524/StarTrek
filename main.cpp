#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

typedef struct Star
{
    int X;
    int Y;
    int Z;
};

Star  MyStar[1000];
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)GetStockObject (BLACK_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );
    srand(7524);
    SetTimer(hwnd,1,30,NULL);
    /* Make the window visible on the screen */
    ShowWindow (hwnd, SW_SHOWMAXIMIZED);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


void CreateNewStar(Star* NewStar)
{
    NewStar->X=-1500+rand()%3000;
    NewStar->Y=-1500+rand()%3000;
    NewStar->Z=rand()%100;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int I,J,K,L;
    HDC MyHDC,hdcMem,hdcScr;
    PAINTSTRUCT PS;
    RECT MyRect;
    HBITMAP hBitmap ;
    static int MyWidth,MyHeight;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        SetFocus(hwnd);
        GetWindowRect(hwnd,&MyRect);
        MyWidth=(MyRect.right - MyRect.left);
        MyHeight=(MyRect.bottom - MyRect.top);
        for (I=0; I<1000; I++)
        {
            CreateNewStar(&MyStar[I]);
        }
        break;
    case WM_SIZE:
        GetWindowRect(hwnd,&MyRect);
        MyWidth=(MyRect.right - MyRect.left);
        MyHeight=(MyRect.bottom - MyRect.top);
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_TIMER:
        InvalidateRect(hwnd,NULL,1);
        break;
    case WM_PAINT:
        LockWindowUpdate (hwnd = GetDesktopWindow ());
        //MyHDC=BeginPaint(hwnd, &PS);
        //EndPaint(hwnd, &PS);
        hdcScr = GetDCEx (hwnd, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;
        hdcMem = CreateCompatibleDC (hdcScr) ;
        hBitmap = CreateCompatibleBitmap (hdcScr, 1920, 1200) ;
        SelectObject (hdcMem, hBitmap) ;
        for (I=0; I<50; I++)
        {
            MyStar[I].Z--;
            J=2*(((float)100-MyStar[I].Z)/100)*MyStar[I].X+1920/2;
            K=2*(((float)100-MyStar[I].Z)/100)*MyStar[I].Y+1200/2;
            L=2;
            if ((J>1920||J<0)||(K>1200||K<0))
            {
                CreateNewStar(&MyStar[I]);
                continue;
            }

            Ellipse(hdcMem,J-L, K-L, J+L, K+L);
        }
        BitBlt (hdcScr, 0, 0, 1920,  1200, hdcMem, 0, 0, SRCCOPY) ;
        DeleteDC (hdcMem) ;
        ReleaseDC (hwnd, hdcScr) ;
        DeleteObject (hBitmap) ;
        LockWindowUpdate (NULL);
        break;
    case WM_CHAR:
            PostQuitMessage (0);
        break;
    case WM_LBUTTONDOWN:
        PostQuitMessage (0);
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
