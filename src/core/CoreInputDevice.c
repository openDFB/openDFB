/*
   (c) Copyright 2001-2011  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>

#include "CoreInputDevice.h"

#include <directfb_util.h>

#include <direct/debug.h>
#include <direct/mem.h>
#include <direct/memcpy.h>
#include <direct/messages.h>

#include <fusion/conf.h>

#include <core/core.h>

#include <core/CoreDFB_CallMode.h>


D_DEBUG_DOMAIN( DirectFB_CoreInputDevice, "DirectFB/CoreInputDevice", "DirectFB CoreInputDevice" );

/*********************************************************************************************************************/

DFBResult
CoreInputDevice_SetKeymapEntry(
                    CoreInputDevice                           *obj,
                    s32                                        key_code,
                    const DFBInputDeviceKeymapEntry           *entry)
{
    DFBResult ret;

    switch (CoreDFB_CallMode( core_dfb )) {
        case COREDFB_CALL_DIRECT:{
            IInputDeviceReal real;
            IInputDevice *super = (IInputDevice *)&real;
            IInputDeviceRealInit( &real, core_dfb, obj );

            Core_PushCalling();
            ret = super->SetKeymapEntry( super, key_code, entry );
            Core_PopCalling();

            return ret;
        }
        case COREDFB_CALL_INDIRECT: {
            IInputDeviceRequestor requestor;
            IInputDevice *super = (IInputDevice *)&requestor;
            IInputDeviceRequestorInit( &requestor, core_dfb, obj );

            Core_PushCalling();
            ret = super->SetKeymapEntry( super, key_code, entry );
            Core_PopCalling();

            return ret;
        }
        case COREDFB_CALL_DENY:
            return DFB_DEAD;
    }

    return DFB_UNIMPLEMENTED;
}

DFBResult
CoreInputDevice_ReloadKeymap(
                    CoreInputDevice                           *obj)
{
    DFBResult ret;

    switch (CoreDFB_CallMode( core_dfb )) {
        case COREDFB_CALL_DIRECT:{
            IInputDeviceReal real;
            IInputDevice *super = (IInputDevice *)&real;
            IInputDeviceRealInit( &real, core_dfb, obj );

            Core_PushCalling();
            ret = super->ReloadKeymap( super );
            Core_PopCalling();

            return ret;
        }
        case COREDFB_CALL_INDIRECT: {
            IInputDeviceRequestor requestor;
            IInputDevice *super = (IInputDevice *)&requestor;
            IInputDeviceRequestorInit( &requestor, core_dfb, obj );

            Core_PushCalling();
            ret = super->ReloadKeymap( super );
            Core_PopCalling();

            return ret;
        }
        case COREDFB_CALL_DENY:
            return DFB_DEAD;
    }

    return DFB_UNIMPLEMENTED;
}

DFBResult
CoreInputDevice_SetConfiguration(
                    CoreInputDevice                           *obj,
                    const DFBInputDeviceConfig                *config)
{
    DFBResult ret;

    switch (CoreDFB_CallMode( core_dfb )) {
        case COREDFB_CALL_DIRECT:{
            IInputDeviceReal real;
            IInputDevice *super = (IInputDevice *)&real;
            IInputDeviceRealInit( &real, core_dfb, obj );

            Core_PushCalling();
            ret = super->SetConfiguration( super, config );
            Core_PopCalling();

            return ret;
        }
        case COREDFB_CALL_INDIRECT: {
            IInputDeviceRequestor requestor;
            IInputDevice *super = (IInputDevice *)&requestor;
            IInputDeviceRequestorInit( &requestor, core_dfb, obj );

            Core_PushCalling();
            ret = super->SetConfiguration( super, config );
            Core_PopCalling();

            return ret;
        }
        case COREDFB_CALL_DENY:
            return DFB_DEAD;
    }

    return DFB_UNIMPLEMENTED;
}

/*********************************************************************************************************************/

static FusionCallHandlerResult
CoreInputDevice_Dispatch( int           caller,   /* fusion id of the caller */
                     int           call_arg, /* optional call parameter */
                     void         *ptr, /* optional call parameter */
                     unsigned int  length,
                     void         *ctx,      /* optional handler context */
                     unsigned int  serial,
                     void         *ret_ptr,
                     unsigned int  ret_size,
                     unsigned int *ret_length )
{
    CoreInputDevice *obj = (CoreInputDevice*) ctx;
    CoreInputDeviceDispatch__Dispatch( obj, caller, call_arg, ptr, length, ret_ptr, ret_size, ret_length );

    return FCHR_RETURN;
}

void CoreInputDevice_Init_Dispatch(
                    CoreDFB              *core,
                    CoreInputDevice      *obj,
                    FusionCall           *call
)
{
    fusion_call_init3( call, CoreInputDevice_Dispatch, obj, core->world );
}

void  CoreInputDevice_Deinit_Dispatch(
                    FusionCall           *call
)
{
     fusion_call_destroy( call );
}

/*********************************************************************************************************************/
static __inline__ void *args_alloc( void *static_buffer, size_t size )
{
    void *buffer = static_buffer;

    if (size > FLUXED_ARGS_BYTES) {
        buffer = D_MALLOC( size );
        if (!buffer)
            return NULL;
    }

    return buffer;
}

static __inline__ void args_free( void *static_buffer, void *buffer )
{
    if (buffer != static_buffer)
        D_FREE( buffer );
}


DFBResult
IInputDeviceRequestor_SetKeymapEntry(IInputDevice *thiz,
                    s32                                        key_code,
                    const DFBInputDeviceKeymapEntry           *entry
)
{
    DFBResult           ret = DFB_OK;
    char        args_static[FLUXED_ARGS_BYTES];
    char        return_args_static[FLUXED_ARGS_BYTES];
    CoreInputDeviceSetKeymapEntry       *args = (CoreInputDeviceSetKeymapEntry*) args_alloc( args_static, sizeof(CoreInputDeviceSetKeymapEntry) );
    CoreInputDeviceSetKeymapEntryReturn *return_args;

    if (!args)
        return (DFBResult) D_OOM();

    return_args = (CoreInputDeviceSetKeymapEntryReturn*) args_alloc( return_args_static, sizeof(CoreInputDeviceSetKeymapEntryReturn) );

    if (!return_args) {
        args_free( args_static, args );
        return (DFBResult) D_OOM();
    }

    D_DEBUG_AT( DirectFB_CoreInputDevice, "IInputDeviceRequestor_%s()\n", __FUNCTION__ );

    D_ASSERT( entry != NULL );

    args->key_code = key_code;
    args->entry = *entry;

    ret = (DFBResult) CoreInputDevice_Call( thiz->obj, FCEF_NONE, CoreInputDeviceCall_SetKeymapEntry, args, sizeof(CoreInputDeviceSetKeymapEntry), return_args, sizeof(CoreInputDeviceSetKeymapEntryReturn), NULL );
    if (ret) {
        D_DERROR( ret, "%s: CoreInputDevice_Call( CoreInputDevice_SetKeymapEntry ) failed!\n", __FUNCTION__ );
        goto out;
    }

    if (return_args->result) {
        /*D_DERROR( return_args->result, "%s: CoreInputDevice_SetKeymapEntry failed!\n", __FUNCTION__ );*/
        ret = return_args->result;
        goto out;
    }

out:
    args_free( return_args_static, return_args );
    args_free( args_static, args );
    return ret;
}


DFBResult
IInputDeviceRequestor_ReloadKeymap(IInputDevice *thiz)
{
    DFBResult           ret = DFB_OK;
    char        args_static[FLUXED_ARGS_BYTES];
    char        return_args_static[FLUXED_ARGS_BYTES];
    CoreInputDeviceReloadKeymap       *args = (CoreInputDeviceReloadKeymap*) args_alloc( args_static, sizeof(CoreInputDeviceReloadKeymap) );
    CoreInputDeviceReloadKeymapReturn *return_args;

    if (!args)
        return (DFBResult) D_OOM();

    return_args = (CoreInputDeviceReloadKeymapReturn*) args_alloc( return_args_static, sizeof(CoreInputDeviceReloadKeymapReturn) );

    if (!return_args) {
        args_free( args_static, args );
        return (DFBResult) D_OOM();
    }

    D_DEBUG_AT( DirectFB_CoreInputDevice, "IInputDeviceRequestor_%s()\n", __FUNCTION__ );



    ret = (DFBResult) CoreInputDevice_Call( thiz->obj, FCEF_NONE, CoreInputDeviceCall_ReloadKeymap, args, sizeof(CoreInputDeviceReloadKeymap), return_args, sizeof(CoreInputDeviceReloadKeymapReturn), NULL );
    if (ret) {
        D_DERROR( ret, "%s: CoreInputDevice_Call( CoreInputDevice_ReloadKeymap ) failed!\n", __FUNCTION__ );
        goto out;
    }

    if (return_args->result) {
        /*D_DERROR( return_args->result, "%s: CoreInputDevice_ReloadKeymap failed!\n", __FUNCTION__ );*/
        ret = return_args->result;
        goto out;
    }

out:
    args_free( return_args_static, return_args );
    args_free( args_static, args );
    return ret;
}


DFBResult
IInputDeviceRequestor_SetConfiguration(IInputDevice *thiz,
                    const DFBInputDeviceConfig                *config)
{
    DFBResult           ret = DFB_OK;
    char        args_static[FLUXED_ARGS_BYTES];
    char        return_args_static[FLUXED_ARGS_BYTES];
    CoreInputDeviceSetConfiguration       *args = (CoreInputDeviceSetConfiguration*) args_alloc( args_static, sizeof(CoreInputDeviceSetConfiguration) );
    CoreInputDeviceSetConfigurationReturn *return_args;

    if (!args)
        return (DFBResult) D_OOM();

    return_args = (CoreInputDeviceSetConfigurationReturn*) args_alloc( return_args_static, sizeof(CoreInputDeviceSetConfigurationReturn) );

    if (!return_args) {
        args_free( args_static, args );
        return (DFBResult) D_OOM();
    }

    D_DEBUG_AT( DirectFB_CoreInputDevice, "IInputDeviceRequestor_%s()\n", __FUNCTION__ );

    D_ASSERT( config != NULL );

    args->config = *config;

    ret = (DFBResult) CoreInputDevice_Call( thiz->obj, FCEF_NONE, CoreInputDeviceCall_SetConfiguration, args, sizeof(CoreInputDeviceSetConfiguration), return_args, sizeof(CoreInputDeviceSetConfigurationReturn), NULL );
    if (ret) {
        D_DERROR( ret, "%s: CoreInputDevice_Call( CoreInputDevice_SetConfiguration ) failed!\n", __FUNCTION__ );
        goto out;
    }

    if (return_args->result) {
        /*D_DERROR( return_args->result, "%s: CoreInputDevice_SetConfiguration failed!\n", __FUNCTION__ );*/
        ret = return_args->result;
        goto out;
    }

out:
    args_free( return_args_static, return_args );
    args_free( args_static, args );
    return ret;
}

void IInputDeviceRequestorInit(IInputDeviceRequestor *thiz, CoreDFB *core, CoreInputDevice *obj)
{
    thiz->base.core = core;
    thiz->base.obj = obj;

    thiz->base.SetKeymapEntry = IInputDeviceRequestor_SetKeymapEntry;
    thiz->base.ReloadKeymap = IInputDeviceRequestor_ReloadKeymap;
    thiz->base.SetConfiguration = IInputDeviceRequestor_SetConfiguration;
}

/*********************************************************************************************************************/

static DFBResult
__CoreInputDeviceDispatch__Dispatch( CoreInputDevice *obj,
                                FusionID      caller,
                                int           method,
                                void         *ptr,
                                unsigned int  length,
                                void         *ret_ptr,
                                unsigned int  ret_size,
                                unsigned int *ret_length )
{
    D_UNUSED
    DFBResult ret;

    IInputDeviceReal real;
    IInputDevice *super = (IInputDevice *)&real;
    IInputDeviceRealInit( &real, core_dfb, obj );


    switch (method) {
        case CoreInputDeviceCall_SetKeymapEntry: {
            D_UNUSED
            CoreInputDeviceSetKeymapEntry       *args        = (CoreInputDeviceSetKeymapEntry *) ptr;
            CoreInputDeviceSetKeymapEntryReturn *return_args = (CoreInputDeviceSetKeymapEntryReturn *) ret_ptr;

            D_DEBUG_AT( DirectFB_CoreInputDevice, "=-> CoreInputDevice_SetKeymapEntry\n" );

            return_args->result = super->SetKeymapEntry( super, args->key_code, &args->entry );
            if (return_args->result == DFB_OK) {
            }

            *ret_length = sizeof(CoreInputDeviceSetKeymapEntryReturn);

            return DFB_OK;
        }

        case CoreInputDeviceCall_ReloadKeymap: {
            D_UNUSED
            CoreInputDeviceReloadKeymap       *args        = (CoreInputDeviceReloadKeymap *) ptr;
            CoreInputDeviceReloadKeymapReturn *return_args = (CoreInputDeviceReloadKeymapReturn *) ret_ptr;

            D_DEBUG_AT( DirectFB_CoreInputDevice, "=-> CoreInputDevice_ReloadKeymap\n" );

            return_args->result = super->ReloadKeymap( super );
            if (return_args->result == DFB_OK) {
            }

            *ret_length = sizeof(CoreInputDeviceReloadKeymapReturn);

            return DFB_OK;
        }

        case CoreInputDeviceCall_SetConfiguration: {
            D_UNUSED
            CoreInputDeviceSetConfiguration       *args        = (CoreInputDeviceSetConfiguration *) ptr;
            CoreInputDeviceSetConfigurationReturn *return_args = (CoreInputDeviceSetConfigurationReturn *) ret_ptr;

            D_DEBUG_AT( DirectFB_CoreInputDevice, "=-> CoreInputDevice_SetConfiguration\n" );

            return_args->result = super->SetConfiguration( super, &args->config );
            if (return_args->result == DFB_OK) {
            }

            *ret_length = sizeof(CoreInputDeviceSetConfigurationReturn);

            return DFB_OK;
        }

    }

    return DFB_NOSUCHMETHOD;
}
/*********************************************************************************************************************/

DFBResult
CoreInputDeviceDispatch__Dispatch( CoreInputDevice *obj,
                                FusionID      caller,
                                int           method,
                                void         *ptr,
                                unsigned int  length,
                                void         *ret_ptr,
                                unsigned int  ret_size,
                                unsigned int *ret_length )
{
    DFBResult ret = DFB_OK;

    D_DEBUG_AT( DirectFB_CoreInputDevice, "CoreInputDeviceDispatch::%s( %p )\n", __FUNCTION__, obj );

    Core_PushIdentity( caller );

    ret = __CoreInputDeviceDispatch__Dispatch( obj, caller, method, ptr, length, ret_ptr, ret_size, ret_length );

    Core_PopIdentity();

    return ret;
}

