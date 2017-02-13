///////////////////////////////////////////////////////////////////////////////
//
//            Copyright (c) 2010 by Blue Water Embedded, Inc.
//
//  This software is copyrighted by and is the sole property of Blue Water
//  Embedded, Inc. All rights, title, ownership, or other intersts in the
//  software remain the property of Blue Water Embedded, Inc. This software
//  may only be used in accordance with the corresponding license agreement.
//  Any unauthorized use, duplications, transmission, distribution, or
//  disclosure of this software is expressly forbidden.
//
//  This Copyright notice may not be removed or modified without prior
//  written consent of Blue Water Embedded, Inc.
//
//  Blue Water Embedded, Inc. reserves the right to modify this software
//  without notice.
//
//  Blue Water Embedded, Inc.               info@bwembedded.com
//  3847 Pine Grove Ave                     http://www.bwembedded.com
//  Suite A
//  Fort Gratiot, MI 48059
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//  engine_common_24bpp.h - Prism drawing engine for Windows applications
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRISM_GENERIC_ENGINE_
#define _PRISM_GENERIC_ENGINE_

#include "prism_pe_base_24bpp.h"   // Include the template class

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class PrismGenericPaintEngine : public Pm_Paint_Engine_24Bit
{
    public:
        PrismGenericPaintEngine(const Pm_Region &);
        virtual ~PrismGenericPaintEngine();

    protected:

        uint8_t *GetFrameBufferAddress(void);
        void ConfigureGraphicsHardware(void);

        virtual void MemoryToVisible(void);
        uint8_t *mpFrameBuffer;

    private:
        
       #if defined(PRISM_MULT_SW_CANVASES)
        uint8_t *mpCompositeBuffer;
       #endif
};


#endif

