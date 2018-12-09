#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

POINT p;    //structure with coordinates to mouse location`

void main(){
  COLORREF color = RGB(255,0,0); // COLORREF to hold the color info`

  HWND hwnd = FindWindowA("ConsoleWindowClass",NULL); // Get the HWND
  HDC hdc = GetDC(hwnd); // Get the DC from that HWND

  for (int i = 0 ; i < 400 ; i++) {
    for (int j=0;j<50;j++)
      SetPixel(hdc, i, j, color);
  }
  while (1) {
        GetCursorPos(&p);
        ScreenToClient(hwnd,&p);
        printf("%d,%d\n", p.x, p.y);

        system("cls");
        //if(GetAsyncKeyState(1)) //checks if left mouse button is pressed
        {
              //fool around with these functions:
              SetPixel(hdc,0,0,color);
              LineTo(hdc,p.x,p.y);
              Rectangle(hdc,200,200,p.x,p.y);
              Sleep(10);
        }
  }
  ReleaseDC(hwnd, hdc); // Release the DC
  DeleteDC(hdc); // Delete the DC
  system("pause");
}

