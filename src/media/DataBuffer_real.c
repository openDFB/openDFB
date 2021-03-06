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

#include "DataBuffer.h"

#include <directfb_util.h>

#include <direct/debug.h>
#include <direct/mem.h>
#include <direct/memcpy.h>
#include <direct/messages.h>

#include <fusion/conf.h>

#include <core/core.h>

D_DEBUG_DOMAIN( DirectFB_DataBuffer, "DirectFB/DataBuffer", "DirectFB DataBuffer" );

/*********************************************************************************************************************/

DFBResult
IDataBufferReal_Flush(IDataBuffer *thiz

)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    return idbr->obj->Flush( idbr->obj );
}


DFBResult
IDataBufferReal_Finish(IDataBuffer *thiz

)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    return idbr->obj->Finish( idbr->obj );
}


DFBResult
IDataBufferReal_SeekTo(IDataBuffer *thiz,
                    u64                                        offset
)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;
    return idbr->obj->SeekTo( idbr->obj, offset );
}


DFBResult
IDataBufferReal_GetPosition(IDataBuffer *thiz,
                    u64                                       *ret_offset
)
{
    DFBResult    ret;
    unsigned int offset;
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    ret = idbr->obj->GetPosition( idbr->obj, &offset );
    if (ret == DFB_OK)
        *ret_offset = offset;

    return ret;
}


DFBResult
IDataBufferReal_GetLength(IDataBuffer *thiz,
                    u64                                       *ret_length
)
{
    DFBResult    ret;
    unsigned int length;
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    ret = idbr->obj->GetLength( idbr->obj, &length );
    if (ret == DFB_OK)
        *ret_length = length;

    return ret;
}


DFBResult
IDataBufferReal_WaitForData(IDataBuffer *thiz,
                    u64                                        length
)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    return idbr->obj->WaitForData( idbr->obj, length );
}


DFBResult
IDataBufferReal_WaitForDataWithTimeout(IDataBuffer *thiz,
                    u64                                        length,
                    u64                                        timeout_ms
)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    return idbr->obj->WaitForDataWithTimeout( idbr->obj, length, timeout_ms / 1000ULL, timeout_ms % 1000ULL );
}


DFBResult
IDataBufferReal_GetData(IDataBuffer *thiz,
                    u32                                        length,
                    u8                                        *ret_data,
                    u32                                       *ret_read
)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;
    return idbr->obj->GetData( idbr->obj, length, ret_data, ret_read );
}


DFBResult
IDataBufferReal_PeekData(IDataBuffer *thiz,
                    u32                                        length,
                    s64                                        offset,
                    u8                                        *ret_data,
                    u32                                       *ret_read
)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    return idbr->obj->PeekData( idbr->obj, length, offset, ret_data, ret_read );
}


DFBResult
IDataBufferReal_HasData(IDataBuffer *thiz

)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    return idbr->obj->HasData( idbr->obj );
}


DFBResult
IDataBufferReal_PutData(IDataBuffer *thiz,
                    const u8                                  *data,
                    u32                                        length
)
{
	IDataBufferReal *idbr = (IDataBufferReal *)thiz;

    return idbr->obj->PutData( idbr->obj, data, length );
}


void IDataBufferRealInit(IDataBufferReal *thiz, CoreDFB *core, IDirectFBDataBuffer *obj)
{
	thiz->base.core = core;
	thiz->obj = obj;

	thiz->base.Flush = IDataBufferReal_Flush;
	thiz->base.SeekTo = IDataBufferReal_SeekTo;
	thiz->base.GetPosition = IDataBufferReal_GetPosition;
	thiz->base.GetLength = IDataBufferReal_GetLength;
	thiz->base.WaitForData = IDataBufferReal_WaitForData;
	thiz->base.WaitForDataWithTimeout = IDataBufferReal_WaitForDataWithTimeout;
	thiz->base.GetData = IDataBufferReal_GetData;
	thiz->base.PeekData = IDataBufferReal_PeekData;
	thiz->base.HasData = IDataBufferReal_HasData;
	thiz->base.PutData = IDataBufferReal_PutData;
}

