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

#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <ossie/PropertyMap.h>
#include "PersistenceStore.h"

class Application_impl;

namespace ossie {

    class ComponentDeployment;

    class deployment_error : public std::runtime_error {
    public:
        deployment_error(const ComponentDeployment* deployment, const std::string& message) :
            std::runtime_error(message),
            _deployment(deployment)
        {
        }

        virtual ~deployment_error() throw ()
        {
        }

        const ComponentDeployment* deployment() const
        {
            return _deployment;
        }

    private:
        const ComponentDeployment* _deployment;
    };

    class configure_error : public deployment_error {
    public:
        configure_error(const ComponentDeployment* deployment,
                        const CF::Properties& properties,
                        const std::string& message) :
            deployment_error(deployment, message),
            _properties(properties)
        {
        }

        virtual ~configure_error() throw ()
        {
        }

        const redhawk::PropertyMap& properties() const
        {
            return _properties;
        }

    private:
        const redhawk::PropertyMap _properties;
    };

    class UsesDeviceAssignment
    {
    public:
        UsesDeviceAssignment(const UsesDevice* usesDevice);

        const UsesDevice* getUsesDevice() const;

        void setAssignedDevice(CF::Device_ptr device);
        CF::Device_ptr getAssignedDevice() const;

    private:
        const UsesDevice* usesDevice;
        CF::Device_var assignedDevice;
    };

    class UsesDeviceDeployment
    {
    public:
        typedef std::vector<UsesDeviceAssignment*> AssignmentList;

        ~UsesDeviceDeployment();

        void addUsesDeviceAssignment(UsesDeviceAssignment* assignment);
        UsesDeviceAssignment* getUsesDeviceAssignment(const std::string identifier);
        const AssignmentList& getUsesDeviceAssignments();

        void transferUsesDeviceAssignments(UsesDeviceDeployment& other);

    protected:
        AssignmentList assignments;
    };

    class SoftpkgDeployment
    {
    public:
        typedef std::vector<SoftpkgDeployment*> DeploymentList;

        SoftpkgDeployment(const SoftPkg* softpkg, const SPD::Implementation* implementation=0);
        ~SoftpkgDeployment();

        const SoftPkg* getSoftPkg() const;

        void setImplementation(const SPD::Implementation* implementation);
        const SPD::Implementation* getImplementation() const;

        std::string getLocalFile();
        CF::LoadableDevice::LoadType getCodeType() const;
        bool isExecutable() const;

        void addDependency(SoftpkgDeployment* dependency);
        const DeploymentList& getDependencies();
        void clearDependencies();

        std::vector<std::string> getDependencyLocalFiles();

    protected:
        void load(Application_impl* application, CF::FileSystem_ptr fileSystem,
                  CF::LoadableDevice_ptr device, const std::string& componentId);

        const SoftPkg* softpkg;
        const SPD::Implementation* implementation;
        DeploymentList dependencies;
    };

    class ComponentDeployment : public SoftpkgDeployment, public UsesDeviceDeployment
    {
    public:
        ComponentDeployment(const SoftPkg* softpkg, const ComponentInstantiation* instantiation,
                            const std::string& identifier);

        /**
         * @brief  Returns the component's runtime identifier
         */
        const std::string& getIdentifier() const;

        const ComponentInstantiation* getInstantiation() const;

        bool isResource() const;
        bool isConfigurable() const;

        bool isAssemblyController() const;
        void setIsAssemblyController(bool state);

        std::string getEntryPoint();

        redhawk::PropertyMap getOptions();

        redhawk::PropertyMap getAffinityOptionsWithAssignment() const;
        void mergeAffinityOptions(const CF::Properties& affinity);

        void setNicAssignment(const std::string& nic);
        bool hasNicAssignment() const;
        const std::string& getNicAssignment() const;

        void setCpuReservation(float reservation);
        bool hasCpuReservation() const;
        float getCpuReservation() const;

        /**
         * Returns the properties used for evaluating math statements in
         * allocation
         */
        redhawk::PropertyMap getAllocationContext() const;

        /**
         * Returns the properties whose values are passed on the command line
         * in execute
         */
        redhawk::PropertyMap getCommandLineParameters() const;

        /**
         * Returns the properties used for the initial call to configure()
         * during deployment
         */
        redhawk::PropertyMap getInitialConfigureProperties() const;

        /**
         * Returns the properties used for initializePropertes() during
         * deployment
         */
        redhawk::PropertyMap getInitializeProperties() const;

        void overrideProperty(const std::string& id, const CORBA::Any& value);

        void setAssignedDevice(const boost::shared_ptr<DeviceNode>& device);
        boost::shared_ptr<DeviceNode> getAssignedDevice();

        void setResourcePtr(CF::Resource_ptr resource);
        CF::Resource_ptr getResourcePtr() const;

        void load(Application_impl* application, CF::FileSystem_ptr fileSystem,
                  CF::LoadableDevice_ptr device);

        std::string getLoggingConfiguration() const;

        void configure();

    protected:
        CF::DataType getPropertyValue(const Property* property) const;
        const ComponentProperty* getPropertyOverride(const std::string& id) const;

        const ComponentInstantiation* instantiation;
        const std::string identifier;
        bool assemblyController;

        boost::shared_ptr<DeviceNode> assignedDevice;
        CF::Resource_var resource;

        redhawk::PropertyMap overrides;
        std::string nicAssignment;
        ossie::optional_value<float> cpuReservation;
        redhawk::PropertyMap affinityOptions;
    };

}

#endif // DEPLOYMENT_H
