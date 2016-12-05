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

#ifndef __bulkio_out_stream_h
#define __bulkio_out_stream_h

#include <string>
#include <complex>
#include <boost/shared_ptr.hpp>

#include <ossie/PropertyMap.h>
#include <ossie/shared_buffer.h>
#include <BULKIO/bulkioDataTypes.h>

#include "bulkio_traits.h"
#include "bulkio_datablock.h"
#include "bulkio_stream.h"

namespace bulkio {

    template <class PortTraits>
    class OutPort;

    template <class PortTraits>
    class OutputStreamBase : public StreamBase {
    public:
        using StreamBase::sri;
        void sri(const BULKIO::StreamSRI& sri);

        using StreamBase::xstart;
        void xstart(double start);
        using StreamBase::xdelta;
        void xdelta(double delta);
        using StreamBase::xunits;
        void xunits(short units);

        using StreamBase::subsize;
        void subsize(int size);

        using StreamBase::ystart;
        void ystart(double start);
        using StreamBase::ydelta;
        void ydelta(double delta);
        using StreamBase::yunits;
        void yunits(short units);

        using StreamBase::complex;
        void complex(bool mode);

        using StreamBase::blocking;
        void blocking(bool mode);

        using StreamBase::keywords;
        void keywords(const _CORBA_Unbounded_Sequence<CF::DataType>& props);
        void setKeyword(const std::string& name, const CORBA::Any& value);
        void setKeyword(const std::string& name, const redhawk::Value& value);
        template <typename T>
        void setKeyword(const std::string& name, const T& value)
        {
            setKeyword(name, redhawk::Value(value));
        }
        void eraseKeyword(const std::string& name);

        void close();

    protected:
        typedef OutPort<PortTraits> OutPortType;

        class Impl;
        Impl& impl();
        const Impl& impl() const;

        OutputStreamBase();
        OutputStreamBase(const BULKIO::StreamSRI& sri, OutPortType* port);
        OutputStreamBase(boost::shared_ptr<Impl> impl);

        int modcount() const;

        typedef const Impl& (OutputStreamBase::*unspecified_bool_type)() const;

    public:
        operator unspecified_bool_type() const;
    };


    template <class PortTraits>
    class OutputStream : public OutputStreamBase<PortTraits> {
    public:
        typedef typename PortTraits::DataTransferTraits::NativeDataType ScalarType;
        typedef std::complex<ScalarType> ComplexType;

        typedef redhawk::shared_buffer<ScalarType> ScalarBuffer;
        typedef redhawk::shared_buffer<ComplexType> ComplexBuffer;
       
        OutputStream();

        /**
         * @brief  Returns the internal buffer size.
         *
         * A buffer size of 0 indicates that buffering is disabled.
         */
        size_t bufferSize() const;

        /**
         * @brief  Sets the internal buffer size.
         * @param samples  Number of samples to buffer (0 disables buffering).
         */
        void setBufferSize(size_t samples);

        /**
         * Send all currently buffered data.
         */
        void flush();

        template <class T>
        void write(const std::vector<T>& data, const BULKIO::PrecisionUTCTime& time)
        {
            write(&data[0], data.size(), time);
        }

        template <class T>
        void write(const std::vector<T>& data, const std::list<bulkio::SampleTimestamp>& times)
        {
            write(&data[0], data.size(), times);
        }

        /**
         * @brief  Write scalar data to the stream.
         * @param data  The %read_buffer to write.
         * @param time  The timestamp of the first sample.
         */
        void write(const ScalarBuffer& data, const BULKIO::PrecisionUTCTime& time);

        /**
         * @brief  Write scalar data to the stream.
         * @param data  The %read_buffer to write.
         * @param times  A list of sample timestamps. Sample offsets must be in
         *               increasing order, starting at 0.
         *
         * Writes a buffer of data with multiple timestamps, breaking up the data
         * into chunks at the SampleTimestamp offsets.
         */
        void write(const ScalarBuffer& data, const std::list<bulkio::SampleTimestamp>& times);

        /**
         * @brief  Write complex data to the stream.
         * @param data  The %read_buffer to write.
         * @param time  The timestamp of the first sample.
         */
        void write(const ComplexBuffer& data, const BULKIO::PrecisionUTCTime& time);

        /**
         * @brief  Write complex data to the stream.
         * @param data  The %read_buffer to write.
         * @param times  A list of sample timestamps. Sample offsets must be in
         *               increasing order, starting at 0.
         *
         * Writes a buffer of data with multiple timestamps, breaking up the data
         * into chunks at the SampleTimestamp offsets.
         */
        void write(const ComplexBuffer& data, const std::list<bulkio::SampleTimestamp>& times);

        void write(const ScalarType* data, size_t count, const BULKIO::PrecisionUTCTime& time);
        void write(const ScalarType* data, size_t count, const std::list<bulkio::SampleTimestamp>& times);

        void write(const ComplexType* data, size_t count, const BULKIO::PrecisionUTCTime& time);
        void write(const ComplexType* data, size_t count, const std::list<bulkio::SampleTimestamp>& times);

    private:
        typedef OutputStreamBase<PortTraits> Base;

        friend class OutPort<PortTraits>;
        typedef OutPort<PortTraits> OutPortType;
        OutputStream(const BULKIO::StreamSRI& sri, OutPortType* port);

        class Impl;
        Impl& impl();
        const Impl& impl() const;
    };


    class OutXMLStream : public OutputStreamBase<XMLPortTraits> {
    public:
        OutXMLStream();

        /**
         * @brief  Write XML data to the stream.
         * @param xmlString  The XML string to write.
         */
        void write(const std::string& xmlString);

    private:
        typedef OutputStreamBase<XMLPortTraits> Base;

        friend class OutPort<XMLPortTraits>;
        typedef OutPort<XMLPortTraits> OutPortType;
        OutXMLStream(const BULKIO::StreamSRI& sri, OutPortType* port);
    };


    class OutFileStream : public OutputStreamBase<FilePortTraits> {
    public:
        OutFileStream();

        /**
         * @brief  Write a file URI to the stream.
         * @param data  The file URI to write.
         */
        void write(const std::string& URL, const BULKIO::PrecisionUTCTime& time);

    private:
        typedef OutputStreamBase<FilePortTraits> Base;

        friend class OutPort<FilePortTraits>;
        typedef OutPort<FilePortTraits> OutPortType;
        OutFileStream(const BULKIO::StreamSRI& sri, OutPortType* port);
    };

    typedef OutputStream<bulkio::CharPortTraits>      OutCharStream;
    typedef OutputStream<bulkio::OctetPortTraits>     OutOctetStream;
    typedef OutputStream<bulkio::ShortPortTraits>     OutShortStream;
    typedef OutputStream<bulkio::UShortPortTraits>    OutUShortStream;
    typedef OutputStream<bulkio::LongPortTraits>      OutLongStream;
    typedef OutputStream<bulkio::ULongPortTraits>     OutULongStream;
    typedef OutputStream<bulkio::LongLongPortTraits>  OutLongLongStream;
    typedef OutputStream<bulkio::ULongLongPortTraits> OutULongLongStream;
    typedef OutputStream<bulkio::FloatPortTraits>     OutFloatStream;
    typedef OutputStream<bulkio::DoublePortTraits>    OutDoubleStream;

    template <class PortTraits>
    struct OutStreamTraits
    {
        typedef OutputStream<PortTraits> OutStreamType;
    };

    template <>
    struct OutStreamTraits<XMLPortTraits> {
        typedef OutXMLStream OutStreamType;
    };

    template <>
    struct OutStreamTraits<FilePortTraits> {
        typedef OutFileStream OutStreamType;
    };

} // end of bulkio namespace

#endif
