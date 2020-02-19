
// DlgProxy.cpp: файл реализации
//

#include "stdafx.h"
#include "framework.h"
#include "DXExternal.h"
#include "DlgProxy.h"
#include "DXExternalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDXExternalDlgAutoProxy

IMPLEMENT_DYNCREATE(CDXExternalDlgAutoProxy, CCmdTarget)

CDXExternalDlgAutoProxy::CDXExternalDlgAutoProxy()
{
	EnableAutomation();

	// Чтобы приложение работало, пока объект
	//	автоматизации активен, конструктор вызывает AfxOleLockApp.
	AfxOleLockApp();

	// Получает доступ к диалоговому окну через указатель
	//  главного окна приложения.  Задает внутренний указатель прокси-сервера для
	//  доступа к диалогу, а также задает обратный указатель диалога на этот
	//  прокси-сервер.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CDXExternalDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CDXExternalDlg)))
		{
			m_pDialog = reinterpret_cast<CDXExternalDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CDXExternalDlgAutoProxy::~CDXExternalDlgAutoProxy()
{
	// Чтобы прервать работу приложения, когда все объекты создаются
	// 	автоматически, деструктор вызывает AfxOleUnlockApp.
	//  Среди прочего будет уничтожено главное диалоговое окно
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CDXExternalDlgAutoProxy::OnFinalRelease()
{
	// Когда будет освобождена последняя ссылка на объект автоматизации,
	// Будет вызван OnFinalRelease.  Базовый класс автоматически
	// удалит объект.  Перед вызовом базового класса требуется
	// дополнительная очистка объекта.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CDXExternalDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDXExternalDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Примечание. Мы добавили поддержку для IID_IDXExternal, чтобы обеспечить безопасную с точки зрения типов привязку
//  из VBA.  Этот IID должен соответствовать GUID, связанному с
//  disp-интерфейс в файле .IDL.

// {8735de3b-4678-4a53-a888-0532bc1ce282}
static const IID IID_IDXExternal =
{0x8735de3b,0x4678,0x4a53,{0xa8,0x88,0x05,0x32,0xbc,0x1c,0xe2,0x82}};

BEGIN_INTERFACE_MAP(CDXExternalDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CDXExternalDlgAutoProxy, IID_IDXExternal, Dispatch)
END_INTERFACE_MAP()

// Макрос IMPLEMENT_OLECREATE2 определен в pch.h этого проекта
// {2f181556-31b1-48ee-b102-36edde0eb55d}
IMPLEMENT_OLECREATE2(CDXExternalDlgAutoProxy, "DXExternal.Application", 0x2f181556,0x31b1,0x48ee,0xb1,0x02,0x36,0xed,0xde,0x0e,0xb5,0x5d)


// Обработчики сообщений CDXExternalDlgAutoProxy
