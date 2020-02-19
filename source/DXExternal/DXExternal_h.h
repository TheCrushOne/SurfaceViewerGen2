

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 06:14:07 2038
 */
/* Compiler settings for source\DXExternal.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __DXExternal_h_h__
#define __DXExternal_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDXExternal_FWD_DEFINED__
#define __IDXExternal_FWD_DEFINED__
typedef interface IDXExternal IDXExternal;

#endif 	/* __IDXExternal_FWD_DEFINED__ */


#ifndef __DXExternal_FWD_DEFINED__
#define __DXExternal_FWD_DEFINED__

#ifdef __cplusplus
typedef class DXExternal DXExternal;
#else
typedef struct DXExternal DXExternal;
#endif /* __cplusplus */

#endif 	/* __DXExternal_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DXExternal_LIBRARY_DEFINED__
#define __DXExternal_LIBRARY_DEFINED__

/* library DXExternal */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_DXExternal;

#ifndef __IDXExternal_DISPINTERFACE_DEFINED__
#define __IDXExternal_DISPINTERFACE_DEFINED__

/* dispinterface IDXExternal */
/* [uuid] */ 


EXTERN_C const IID DIID_IDXExternal;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("8735de3b-4678-4a53-a888-0532bc1ce282")
    IDXExternal : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDXExternalVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDXExternal * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDXExternal * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDXExternal * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDXExternal * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDXExternal * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDXExternal * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDXExternal * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IDXExternalVtbl;

    interface IDXExternal
    {
        CONST_VTBL struct IDXExternalVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDXExternal_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDXExternal_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDXExternal_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDXExternal_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDXExternal_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDXExternal_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDXExternal_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDXExternal_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DXExternal;

#ifdef __cplusplus

class DECLSPEC_UUID("2f181556-31b1-48ee-b102-36edde0eb55d")
DXExternal;
#endif
#endif /* __DXExternal_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


