/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "client_manager.h"
#include "console.h"
#include "signpost_api.h"

using namespace erpc;
#if !(__embedded_cplusplus)
using namespace std;
#endif

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

RequestContext ClientManager::createRequest(bool isOneway)
{
    // Create codec to read and write the request.
    Codec *codec = createBufferAndCodec();

    return RequestContext(++m_sequence, codec, isOneway);
}

erpc_status_t ClientManager::performRequest(RequestContext &request)
{
    //this implementation was changed to work with the signpost stack
    //this is much (much) cleaner than trying to get it to work with
    //their whole stack - just need to make sure that we can still make
    //that header
    if(request.isOneway()) {
        signpost_processing_oneway_send(request.getCodec()->getBuffer()->get(),
                                    request.getCodec()->getBuffer()->getUsed());
    } else {
        //the first call should be nonblocking and perform the send
        signpost_processing_twoway_send(request.getCodec()->getBuffer()->get(),
                                    request.getCodec()->getBuffer()->getUsed());
        //the second call provides a clean buffer for receiving
        request.getCodec()->getBuffer()->setUsed(0);
        request.getCodec()->reset();
        uint16_t size = 0;
        signpost_processing_twoway_receive(request.getCodec()->getBuffer()->get(),
                                                                        &size);
        request.getCodec()->getBuffer()->setUsed(size);

        erpc_status_t err = verifyReply(request);
        if(err) {
            return err;
        }
    }

    return kErpcStatus_Success;

    /*erpc_status_t err = m_transport->send(request.getCodec()->getBuffer());
    if (err)
    {
        return err;
    }

    // If the request is oneway, then there is nothing more to do.
    if (!request.isOneway())
    {
        // reset codec for receiving data
        request.getCodec()->getBuffer()->setUsed(0);
        request.getCodec()->reset();

        // Receive reply.
        err = m_transport->receive(request.getCodec()->getBuffer());
        if (err)
        {
            return err;
        }

        // Check the reply.
        err = verifyReply(request);
        if (err)
        {
            return err;
        }
    }

    return kErpcStatus_Success;*/
}

erpc_status_t ClientManager::verifyReply(RequestContext &request)
{
    // Some transport layers change the request's message buffer pointer (for things like zero
    // copy support), so inCodec must be reset to work with correct buffer.
    request.getCodec()->reset();

    // Extract the reply header.
    message_type_t msgType;
    uint32_t service;
    uint32_t requestNumber;
    uint32_t sequence;
    erpc_status_t err = request.getCodec()->startReadMessage(&msgType, &service, &requestNumber, &sequence);
    if (err)
    {
        return err;
    }

    // Verify that this is a reply to the request we just sent.
    if (msgType != kReplyMessage || sequence != request.getSequence())
    {
        return kErpcStatus_ExpectedReply;
    }

    return kErpcStatus_Success;
}

Codec *ClientManager::createBufferAndCodec()
{
    Codec *codec = m_codecFactory->create();
    if (!codec)
    {
        return NULL;
    }

    MessageBuffer message = m_messageFactory->create();
    if (!message.get())
    {
        // Dispose of buffers and codecs.
        m_codecFactory->dispose(codec);
        return NULL;
    }

    codec->setBuffer(message);

    return codec;
}

void ClientManager::releaseRequest(RequestContext &request)
{
    m_messageFactory->dispose(request.getCodec()->getBuffer());
    m_codecFactory->dispose(request.getCodec());
}
