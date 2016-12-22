#include "stdafx.h"
#include "Application.h"
Application* Application::m_theObj = NULL;
Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow, DWORD dwWaitTimer)
{
    m_hInstance = hInstance;
    m_hPrevInstance = hPrevInstance;
    m_lpCmdLine = lpCmdLine;
    m_nCmdShow = nCmdShow;
    m_dwWaitTime = dwWaitTimer;
    m_theObj = this;
}
Application::~Application()
{
    m_theObj = NULL;
}
Application& Application::CreateApplication(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow, DWORD dwWaitTimer)
{
    static Application theapp(hInstance, hPrevInstance, lpCmdLine, nCmdShow, dwWaitTimer);

    return theapp;
}
