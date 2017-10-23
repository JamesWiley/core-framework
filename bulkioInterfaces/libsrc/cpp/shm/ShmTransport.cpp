/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK bulkioInterfaces.
 *
 * REDHAWK bulkioInterfaces is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK bulkioInterfaces is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "ShmTransport.h"
#include "ShmProvidesTransport.h"
#include "FifoIPC.h"
#include "MessageBuffer.h"

#include <bulkio_in_port.h>
#include <bulkio_out_port.h>

#include "bulkio_p.h"

namespace bulkio {

    template <typename PortType>
    class ShmTransport : public OutputTransport<PortType>
    {
    public:
        typedef typename PortType::_ptr_type PtrType;
        typedef typename OutputTransport<PortType>::BufferType BufferType;
        typedef typename CorbaTraits<PortType>::TransportType TransportType;

        ShmTransport(OutPort<PortType>* parent, PtrType port) :
            OutputTransport<PortType>(parent, port),
            _fifo()
        {
        }

        ~ShmTransport()
        {
        }

        virtual std::string transportType() const
        {
            return "shmipc";
        }

        virtual CF::Properties transportInfo() const
        {
            return CF::Properties();
        }

        const std::string& getFifoName()
        {
            return _fifo.name();
        }

        void finishConnect(const std::string& filename)
        {
            _fifo.connect(filename);
            _fifo.sync();
        }

        virtual void disconnect()
        {
            OutputTransport<PortType>::disconnect();
            _fifo.disconnect();
        }

    protected:
        virtual void _pushSRI(const BULKIO::StreamSRI& sri)
        {
            try {
                this->_objref->pushSRI(sri);
            } catch (const CORBA::SystemException& exc) {
                throw redhawk::FatalTransportError(ossie::corba::describeException(exc));
            }
        }

        virtual void _pushPacket(const BufferType& data,
                                 const BULKIO::PrecisionUTCTime& T,
                                 bool EOS,
                                 const std::string& streamID)
        {
            MessageBuffer msg;
            msg.write(data.size());

            // Temporary buffer to ensure that if a copy is made, it gets
            // released after the transfer
            BufferType copy;

            // If the packet is non-empty, write the additional shared memory
            // information for the remote side to pick up
            if (!data.empty()) {
                const void* base;
                size_t offset;

                // Check that the buffer is already in shared memory (this is
                // hoped to be the common case); if not, copy it into another
                // buffer that is allocated in shared memory
                if (data.is_process_shared()) {
                    base = data.base();
                    offset = reinterpret_cast<size_t>(data.data()) - reinterpret_cast<size_t>(base);
                } else {
                    copy = data.copy(redhawk::shm::Allocator<typename BufferType::value_type>());
                    base = copy.base();
                    offset = 0;
                }

                redhawk::shm::MemoryRef ref = redhawk::shm::Heap::getRef(base);
                msg.write(ref.heap);
                msg.write(ref.superblock);
                msg.write(ref.offset);
                msg.write(offset);
            }

            msg.write(T);
            msg.write(EOS);
            msg.write(streamID);

            try {
                _fifo.write(msg.buffer(), msg.size());
            } catch (const std::exception& exc) {
                throw redhawk::FatalTransportError(exc.what());
            }

            size_t status;
            if (_fifo.read(&status, sizeof(size_t)) != sizeof(size_t)) {
                throw redhawk::FatalTransportError("bad response");
            }
        }

    private:
        FifoEndpoint _fifo;
    };

    template <typename PortType>
    ShmOutputManager<PortType>::ShmOutputManager(OutPort<PortType>* port) :
        OutputManager<PortType>(port)
    {
        char host[HOST_NAME_MAX+1];
        gethostname(host, sizeof(host));
        _hostname = host;
    }

    template <typename PortType>
    std::string ShmOutputManager<PortType>::transportType()
    {
        return "shmipc";
    }

    template <typename PortType>
    CF::Properties ShmOutputManager<PortType>::transportProperties()
    {
        CF::Properties properties;
        ossie::corba::push_back(properties, redhawk::PropertyType("hostname", _hostname));
        return properties;
    }

    template <typename PortType>
    OutputTransport<PortType>*
    ShmOutputManager<PortType>::createOutputTransport(PtrType object,
                                                      const std::string& connectionId,
                                                      const redhawk::PropertyMap& properties)
    {
        // For testing, allow disabling
        const char* shm_env = getenv("BULKIO_SHM");
        if (shm_env && (strcmp(shm_env, "disable") == 0)) {
            return 0;
        }

        // If the other end of the connection has a different hostname, it
        // is reasonable to assume that we cannot use shared memory
        if (properties.get("hostname", "").toString() != _hostname) {
            RH_NL_TRACE("ShmTransport", "Connection '" << connectionId << "' is on another host");
            return 0;
        }

        return new ShmTransport<PortType>(this->_port, object);
    }

    template <typename PortType>
    redhawk::PropertyMap ShmOutputManager<PortType>::getNegotiationProperties(redhawk::UsesTransport* transport)
    {
        TransportType* shm_transport = dynamic_cast<TransportType*>(transport);
        if (!shm_transport) {
            throw std::logic_error("invalid transport type");
        }

        redhawk::PropertyMap properties;
        properties["fifo"] = shm_transport->getFifoName();
        return properties;
    }

    template <typename PortType>
    void ShmOutputManager<PortType>::setNegotiationResult(redhawk::UsesTransport* transport,
                                                          const redhawk::PropertyMap& properties)
    {
        TransportType* shm_transport = dynamic_cast<TransportType*>(transport);
        if (!shm_transport) {
            throw std::logic_error("invalid transport type");
        }

        if (!properties.contains("fifo")) {
            throw redhawk::FatalTransportError("invalid properties for shared memory connection");
        }

        std::string fifo_name = properties["fifo"].toString();
        RH_NL_DEBUG("ShmTransport", "Connecting to provides port FIFO: " << fifo_name);
        shm_transport->finishConnect(fifo_name);
    }

    template <typename PortType>
    class ShmTransportFactory : public BulkioTransportFactory<PortType>
    {
    public:
        ShmTransportFactory()
        {
        }

        virtual std::string transportType()
        {
            return "shmipc";
        }

        virtual int defaultPriority()
        {
            return 1;
        }

        virtual InputManager<PortType>* createInputManager(InPort<PortType>* port)
        {
            return new ShmInputManager<PortType>(port);
        }

        virtual OutputManager<PortType>* createOutputManager(OutPort<PortType>* port)
        {
            return new ShmOutputManager<PortType>(port);
        }
    };

#define INSTANTIATE_TEMPLATE(x)                 \
    template class ShmTransport<x>;             \
    template class ShmOutputManager<x>;         \
    template class ShmTransportFactory<x>;

    FOREACH_NUMERIC_PORT_TYPE(INSTANTIATE_TEMPLATE);

    static int initializeModule()
    {
#define REGISTER_FACTORY(x) \
        {                                                               \
            static ShmTransportFactory<x> factory;                      \
            redhawk::TransportRegistry::RegisterTransport(&factory);    \
        }

        FOREACH_NUMERIC_PORT_TYPE(REGISTER_FACTORY);

        return 0;
    }

    static int initialized = initializeModule();
}
