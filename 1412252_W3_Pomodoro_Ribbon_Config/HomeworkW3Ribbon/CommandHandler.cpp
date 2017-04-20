// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include <UIRibbon.h>

#include "CommandHandler.h"
#include "RibbonIDs.h"
#include "RibbonFramework.h"
//#include "PropertySet.h"



// Static method to create an instance of the object.
HRESULT CCommandHandler::CreateInstance(IUICommandHandler **ppCommandHandler)
{
	if (!ppCommandHandler)
	{
		return E_POINTER;
	}

	*ppCommandHandler = NULL;

	HRESULT hr = S_OK;

	CCommandHandler* pCommandHandler = new CCommandHandler();

	if (pCommandHandler != NULL)
	{
		*ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(IUICommandHandler))
	{
		*ppv = static_cast<IUICommandHandler*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//
STDMETHODIMP CCommandHandler::UpdateProperty(
	UINT nCmdID,
	REFPROPERTYKEY key,
	const PROPVARIANT* ppropvarCurrentValue,
	PROPVARIANT* ppropvarNewValue)
{
	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(ppropvarCurrentValue);
	UNREFERENCED_PARAMETER(ppropvarNewValue);
	
	HRESULT hr = S_FALSE;

	if (key == UI_PKEY_ItemsSource)
	{
		if (nCmdID == ID_DDG_ShortBreak)
		{
			IUICollection* pCollection;

			hr = ppropvarCurrentValue->punkVal->QueryInterface(
				IID_PPV_ARGS(&pCollection));

			int labelIds[] = { IDS_SBREAK_1, IDS_SBREAK_2, IDS_SBREAK_3 };

			// Populate the combobox with the three layout options.
			for (int i = 0; i<_countof(labelIds); i++)
			{
				// Create a new property set for each item.
				CCommandHandler* pItem;
				hr = this->CreateInstance((IUICommandHandler**)&pItem);

				// Load the label from the resource file.
				WCHAR wszLabel[256];
				////LoadStringW(GetModuleHandle(NULL), labelIds[i], wszLabel,256);
				wsprintf(wszLabel, L"%d", labelIds[i]);

				// Initialize the property set with no image, the label that was just
				// loaded, and no category.
				this->InitializeItemProperties(
					NULL, wszLabel, UI_COLLECTION_INVALIDINDEX);

				pCollection->Add((IUnknown *)pItem);
			}
			pCollection->Release();
			hr = S_OK;
		}		
	}

	return E_NOTIMPL;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
STDMETHODIMP CCommandHandler::Execute(
	UINT nCmdID,
	UI_EXECUTIONVERB verb,
	const PROPERTYKEY* key,
	const PROPVARIANT* ppropvarValue,
	IUISimplePropertySet* pCommandExecutionProperties)
{
	/*UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(verb);
	UNREFERENCED_PARAMETER(nCmdID);*/
	HWND hWnd = GetForegroundWindow();
	switch(nCmdID)
	{		
		case ID_BTN_EXIT:
			PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;
		case ID_BTN_START:
			Pomodoro.StartPomodoro();
			break;
		case ID_BTN_ABANDON:
			Pomodoro.AbandonPomodoro();
			break;	
			//Click on Short Break
		case IDS_SBREAK_1:
			Pomodoro.SetShortTimeBreak(1);
			break;
		case IDS_SBREAK_2:			
			Pomodoro.SetShortTimeBreak(2);
			break;
		case IDS_SBREAK_3:
			Pomodoro.SetShortTimeBreak(3);
			break;
			//Click on Long Break
		case IDS_LBREAK_1:
			Pomodoro.SetLongTimeBreak(1);
			break;
		case IDS_LBREAK_2:
			Pomodoro.SetLongTimeBreak(2);
			break;
		case IDS_LBREAK_3:
			Pomodoro.SetLongTimeBreak(3);
			break;
		case IDS_LBREAK_4:
			Pomodoro.SetLongTimeBreak(4);
			break;		
	}
	
	return S_OK;
}

void CCommandHandler::InitializeItemProperties(IUIImage *image,
	__in PCWSTR label,
	int categoryId)
{
	if (image)
	{
		image->AddRef();
	}
}
