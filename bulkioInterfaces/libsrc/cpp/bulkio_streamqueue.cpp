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

#include "bulkio_streamqueue.h"
#include "bulkio_in_port.h"
#include "bulkio_p.h"

namespace bulkio {

    /*template <class T_port>
    streamQueue<T_port>::Record::Record(std::string _s, short _p, BULKIO::PrecisionUTCTime _t, typename streamQueue<T_port>::DataBlockType &_b) {
        Stream_id = _s;
        Priority = _p;
        Timestamp = _t;
        Block = _b;
    }*/

    template <class T_port>
    streamQueue<T_port>::streamQueue(bool ignore_timestamp, bool ignore_error) {
        _ignore_timestamp = false;
        _ignore_error = false;
        _port = NULL;
        before_ingest_time = bulkio::time::utils::now();
        before_ingest_time.twsec = 0;
        before_ingest_time.tfsec = 0;
        sleep_overhead = 0;
        int sleep_checks = 10;
        int sleep_time = 100;

        // calibrate the amount of overhead from entering/leaving sleep
        BULKIO::PrecisionUTCTime before_time = bulkio::time::utils::now();
        for (unsigned int i=0; i<sleep_checks; i++) {
            before_time = bulkio::time::utils::now();
            boost::this_thread::sleep_for(boost::chrono::microseconds(sleep_time));
            double sleep_diff = bulkio::time::utils::now() - before_time;
            sleep_overhead += sleep_diff;
        }
        double d_sleep_time = sleep_time;
        d_sleep_time /= 1e6;
        sleep_overhead = (sleep_overhead-(d_sleep_time*sleep_checks))/sleep_checks;
    }

    template <class T_port>
    void streamQueue<T_port>::update_port(T_port* port) {
        _port = port;
    }

    template <class T_port>
    void streamQueue<T_port>::update_ignore_timestamp(bool ignore_timestamp) {
        _ignore_timestamp = ignore_timestamp;
    }

    template <class T_port>
    void streamQueue<T_port>::update_ignore_error(bool ignore_error) {
        _ignore_error = ignore_error;
    }

    template <class T_port>
    void streamQueue<T_port>::throw_stream_error(std::string stream_id, std::string message) {
    }

    template <class T_port>
    double streamQueue<T_port>::getCenterFrequency(typename streamQueue<T_port>::DataBlockType &block) {
        return 0;
    }

    template <class T_port>
    std::vector<std::string> streamQueue<T_port>::held() {
        boost::mutex::scoped_lock lock(queuesMutex);
        std::vector<std::string> stream_ids;
        typename std::map<std::string, std::deque<Record> >::iterator it;
        for(it = held_queue.begin(); it != held_queue.end(); ++it) {
            stream_ids.push_back(it->first);
        }
        return stream_ids;
    }

    template <class T_port>
    bool streamQueue<T_port>::hold(std::string stream_id) {
        boost::mutex::scoped_lock lock(queuesMutex);
        bool retval = false;
        if (held_queue.find(stream_id) == held_queue.end()) {
            //held_queue.insert(std::pair<std::string, std::vector<Record> >(stream_id, std::vector<Record>()));
            held_queue[stream_id].resize(0);
        }
        typename std::deque<Record>::iterator it = transmit_queue.end();
        it--;
        while (it > transmit_queue.begin()) {
            if (it->Stream_id == stream_id) {
                retval = true;
                held_queue[stream_id].push_front(*it);
                it = transmit_queue.erase(it);
            } else {
                it--;
            }
        }
        return retval;
    }

    template <class T_port>
    bool streamQueue<T_port>::allow(std::string stream_id) {
        boost::mutex::scoped_lock lock(queuesMutex);
        bool retval = false;
        if (held_queue.find(stream_id) != held_queue.end()) {
            transmit_queue.insert(transmit_queue.begin(), held_queue[stream_id].begin(), held_queue[stream_id].end());
            held_queue.erase(stream_id);
            retval = true;
            std::sort(transmit_queue.begin(), transmit_queue.end(), CompareRecords);
        }
        return retval;
    }

    template <class T_port>
    void streamQueue<T_port>::reset() {
        boost::mutex::scoped_lock lock(queuesMutex);
        transmit_queue.clear();
        held_queue.clear();
    }

    template <class T_port>
    void streamQueue<T_port>::verifyTimes(BULKIO::PrecisionUTCTime now) {
        if (TimeZero(now))
            return;
        typename std::deque<Record>::iterator it = transmit_queue.begin();
        std::vector<std::string> error_stream_ids;
        while (it != transmit_queue.end()) {
            if (TimeZero(it->Timestamp)) {
                continue;
            }
            // no sense in checking the timestamp if the stream id has already been flagged
            if (std::find(error_stream_ids.begin(), error_stream_ids.end(), it->Stream_id) == error_stream_ids.end()) {
                if (it->Timestamp < now) {
                    error_stream_ids.push_back(it->Stream_id);
                }
            }
            it++;
        }
        for (std::vector<std::string>::iterator str_id=error_stream_ids.begin(); str_id!=error_stream_ids.end(); str_id++) {
            if (held_queue.erase(*str_id) == 0) {   // if nothing was erased from held_queue, there might be something in transmit_queue
                it = transmit_queue.end();
                it--;
                do {
                    if (it->Stream_id == *str_id) {
                        it = transmit_queue.erase(it);
                    } else {
                        it--;
                    }
                } while (it != transmit_queue.begin());
            }
        }
    }

    template <class T_port>
    void streamQueue<T_port>::ingestStreams(BULKIO::PrecisionUTCTime& now, double timeout) {
        bool transmit_updated = false;
        while (true) {  // loop until no streams are available
            typename T_port::StreamType inputStream = _port->getCurrentStream(timeout);
            if (!inputStream) { // No streams are available
                break;
            }
            std::string stream_id = ossie::corba::returnString(inputStream.sri().streamID);
            typename streamQueue<T_port>::DataBlockType block = inputStream.read();
            if (inputStream.eos()) {
            }
            if (!block) { // No data available
                continue;
            }
            short priority = 0;
            if (held_queue.find(stream_id) == held_queue.end()) {
                transmit_queue.push_back(Record(stream_id, priority, block.getStartTime(), block));
                transmit_updated = true;
            } else {
                held_queue[stream_id].push_back(Record(stream_id, priority, block.getStartTime(), block));
            }
        }
        if (transmit_updated)
            std::sort(transmit_queue.begin(), transmit_queue.end(), CompareRecords);
        verifyTimes(now);
    }

    template <class T_port>
    typename streamQueue<T_port>::DataBlockType streamQueue<T_port>::getNextBlock(BULKIO::PrecisionUTCTime now, double timeout, double future_window) {
        boost::mutex::scoped_lock lock(queuesMutex);
        BULKIO::PrecisionUTCTime initial_time = bulkio::time::utils::now();
        double fixed_time_offset = now - initial_time;
        initial_time += fixed_time_offset;

        typename streamQueue<T_port>::DataBlockType retval;
        double _timeout = 0;
        double orig_timeout = timeout;
        double half_future_window_us = future_window * 1e6 * 0.5;
        bool breakout = false;

        before_ingest_time = bulkio::time::utils::now()+fixed_time_offset;
        while (true) {
            ingestStreams(before_ingest_time, _timeout);
            double current_time_diff = (bulkio::time::utils::now()+fixed_time_offset) - initial_time;
            if ((current_time_diff > timeout) and (timeout != 0)) { // we've hit a timeout condition; leave now
                break;
            }
            if (not transmit_queue.empty()) {
                if (future_window > 0) { // future_window < 0 means "just give me the front of the queue
                    typename streamQueue<T_port>::DataBlockType inspect_block = transmit_queue.front().Block;
                    double block_timediff = inspect_block.getStartTime() - (bulkio::time::utils::now()+fixed_time_offset);
                    if (block_timediff > future_window+sleep_overhead) { // the time different is not quite there yet
                        long int sleep_time = (block_timediff-future_window)*1e6;
                        if (sleep_time > half_future_window_us) {
                            sleep_time = half_future_window_us;
                            breakout = false;
                        } else {
                            breakout = true;
                            sleep_time -= sleep_overhead;
                        }
                        boost::this_thread::sleep_for(boost::chrono::microseconds(sleep_time));
                        if (not breakout) { // check for any changes in the queue
                            continue;
                        }
                    }
                }
                retval = transmit_queue.front().Block;
                transmit_queue.pop_front();
                break;
            } else {    // transmit queue is emtpy
                double ingest_time = (bulkio::time::utils::now()+fixed_time_offset) - before_ingest_time;
                _timeout = (ingest_time > future_window*0.75)?future_window*0.5:ingest_time*0.5;
                if (_timeout < 1e9) {
                    _timeout = future_window*0.75;
                }
                double current_time_diff = (bulkio::time::utils::now()+fixed_time_offset) - initial_time;
                if (current_time_diff > (orig_timeout-_timeout)) {
                    _timeout = orig_timeout-current_time_diff-future_window;
                    if (_timeout<0) {
                        _timeout = 0;
                    }
                }
                before_ingest_time = (bulkio::time::utils::now()+fixed_time_offset);
            }
        }
        return retval;
    }

#define INSTANTIATE_TEMPLATE(x) template class streamQueue<x>;

FOREACH_INPUT_NUMERIC_PORT(INSTANTIATE_TEMPLATE);

}