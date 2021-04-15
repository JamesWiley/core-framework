/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK core.
 *
 * REDHAWK core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */
#ifndef COMMANDLINE_PROP_BASE_IMPL_BASE_H
#define COMMANDLINE_PROP_BASE_IMPL_BASE_H

#include <boost/thread.hpp>
#include <ossie/Component.h>
#include <ossie/ThreadedComponent.h>


class commandline_prop_base : public Component, protected ThreadedComponent
{
    public:
        commandline_prop_base(const char *uuid, const char *label);
        ~commandline_prop_base();

        void start();

        void stop();

        void releaseObject();

        void loadProperties();

    protected:
        // Member variables exposed as properties
        /// Property: testprop
        std::string testprop;
        std::string emptytestprop;

    private:
};
#endif // COMMANDLINE_PROP_BASE_IMPL_BASE_H
