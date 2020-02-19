
// DlgProxy.h: файл заголовка
//

#pragma once

class CDXExternalDlg;


// Целевой объект команды CDXExternalDlgAutoProxy

class CDXExternalDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CDXExternalDlgAutoProxy)

	CDXExternalDlgAutoProxy();           // защищенный конструктор, используемый при динамическом создании

// Атрибуты
public:
	CDXExternalDlg* m_pDialog;

// Операции
public:

// Переопределение
	public:
	virtual void OnFinalRelease();

// Реализация
protected:
	virtual ~CDXExternalDlgAutoProxy();

	// Созданные функции схемы сообщений

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CDXExternalDlgAutoProxy)

	// Автоматически созданные функции диспетчерской карты OLE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

