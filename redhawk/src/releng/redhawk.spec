#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK core.
#
# REDHAWK core is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# REDHAWK core is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#
%if 0%{?fedora} >= 17 || 0%{?rhel} >=7
%global with_systemd 1
%endif
%{!?_ossiehome:  %global _ossiehome  /usr/local/redhawk/core}
%{!?_sdrroot:    %global _sdrroot    /var/redhawk/sdr}
%global _prefix %{_ossiehome}
%global __python %{__python3}

Prefix:         %{_ossiehome}
Prefix:         %{_sdrroot}
Prefix:         %{_sysconfdir}

Name:           redhawk
Version:        3.0.0
Release:        1%{?dist}
Summary:        REDHAWK is a Software Defined Radio framework

Group:          Applications/Engineering
License:        LGPLv3+
URL:            http://redhawksdr.org/
Source:         %{name}-%{version}.tar.gz
Vendor:         REDHAWK

%global __arch_install_post %{nil}

Requires:       util-linux-ng

Requires:       gstreamer-python
%if 0%{?rhel} >= 7 || 0%{?fedora} >= 17
Requires:       gstreamer1-plugins-base
Requires:       python-matplotlib-qt4
Requires:       numactl-libs
%else
Requires:       gstreamer-plugins-base
Requires:       python-matplotlib
%endif

Requires:       python3
%if 0%{?rhel} >= 7 || 0%{?fedora} >= 17
Requires:       python3-omniORB > 4.2.2
Requires:       omniORB-devel > 4.2.2
%endif
Requires:       python36-numpy
Requires:       python36-qt5
Requires:       python36-qt5-base
Requires:       python36-lxml
Requires:       python36-jinja2
Requires:       python36-PyYAML
Requires:       python36-gobject-devel
Requires:       binutils
Requires:       numactl
Requires:       sqlite
Requires:       java-11-openjdk jacorb
Requires:       yaml-cpp-devel
Requires:       devtoolset-9-gcc-c++

BuildRequires:  libuuid-devel
BuildRequires:  boost-devel >= 1.41
BuildRequires:  autoconf automake libtool
BuildRequires:  expat-devel
BuildRequires:  java-11-openjdk-devel jacorb
BuildRequires:  python3-setuptools
BuildRequires:  python3-devel >= 3.6
BuildRequires:  python36-lxml
BuildRequires:  python36-numpy
BuildRequires:  python36-qt5
BuildRequires:  python36-qt5-base
BuildRequires:  python36-qt5-devel
BuildRequires:  python36-jinja2
BuildRequires:  python36-PyYAML
BuildRequires:  python36-gobject-devel
BuildRequires:  log4cxx-devel >= 0.10
%if 0%{?rhel} >= 7 || 0%{?fedora} >= 17
BuildRequires:  omniORB-devel > 4.2.2
BuildRequires:  omniORBpy-devel > 4.2.2
%endif
BuildRequires:  libomniEvents2-devel
BuildRequires:  xsd >= 3.3.0
BuildRequires:  cppunit-devel
BuildRequires:  numactl-devel
BuildRequires:  sqlite-devel
BuildRequires:  autoconf-archive
BuildRequires:  yaml-cpp-devel

%description
REDHAWK is a Software Defined Radio framework.
 * Commit: __REVISION__
 * Source Date/Time: __DATETIME__

%package qt-tools
Summary:        PyQt Tools
Group:          Applications/Engineering
Requires:       %{name} = %{version}-%{release}
Requires:       python36-qt5

%description qt-tools
PyQt-based applications (qtbrowse and rhlauncher)

%package sdrroot-dom-mgr
Summary:        Domain Manager
Group:          Applications/Engineering
Requires:       %{name} = %{version}-%{release}
Provides:       DomainManager = %{version}-%{release}

%description sdrroot-dom-mgr
The Domain Manager software package

%package sdrroot-dom-profile
Summary:        Basic domain manager profile
Group:          Applications/Engineering
Requires:       %{name}-sdrroot-dom-mgr = %{version}-%{release}

%description sdrroot-dom-profile
A generic domain profile and domain profile template

%package sdrroot-dev-mgr
Summary:        Device Manager
Group:          Applications/Engineering
Requires:       %{name} = %{version}-%{release}
Provides:       DeviceManager = %{version}-%{release}

%description sdrroot-dev-mgr
The Device Manager software package

%package devel
Summary:        The REDHAWK development package
Group:          Applications/Engineering

# REDHAWK
Requires:       redhawk = %{version}-%{release}

# Base dependencies
Requires:       libuuid-devel
Requires:       boost-devel >= 1.41
Requires:       autoconf automake libtool
Requires:       log4cxx-devel >= 0.10
Requires:       numactl-devel
# provide autoconf-archive to components at compile-time
Requires:       autoconf-archive

# omniORB / omniORBpy

%if 0%{?fedora} == 17 || 0%{?rhel} == 7
Requires:       omniORB-devel > 4.2.2
Requires:       omniORBpy-devel > 4.2.2
%endif
Requires:       omniORB-doc
# Languages
Requires:       gcc-c++
Requires:       python3-devel >= 3.6
Requires:       java-11-openjdk-devel jacorb

%description devel
This package ensures that all requirements for REDHAWK development are installed. It also provides a useful development utilities.

%prep

%if 0%{?_localbuild}
%setup -q -n redhawk
%else
%setup -q
%endif

%build
# build the core framework
cd src
./reconf
export PYTHON=%{__python3}
%configure --with-sdr=%{_sdrroot} --with-pyscheme=home --without-tests

make %{?_smp_mflags}


%install
rm -rf --preserve-root $RPM_BUILD_ROOT

# install ossie framework
cd src
# ----- this is /root/rpmbuild/BUILD/redhawk-3.0.0/src
make install DESTDIR=$RPM_BUILD_ROOT

# add symlinks to .jars into ossiehome/lib
ln -sf ${JACORB_HOME}/lib/jacorb-3.9.jar    %{buildroot}/usr/local/redhawk/core/lib/jacorb.jar
ln -sf ${JACORB_HOME}/lib/jacorb-3.9.jar    %{buildroot}/usr/local/redhawk/core/lib/jacorb-3.9.jar
ln -sf ${JACORB_HOME}/lib/jacorb-omgapi-3.9.jar    %{buildroot}/usr/local/redhawk/core/lib/jacorb-omgapi.jar
ln -sf ${JACORB_HOME}/lib/jacorb-omgapi-3.9.jar    %{buildroot}/usr/local/redhawk/core/lib/jacorb-omgapi-3.9.jar
ln -sf ${JACORB_HOME}/lib/jacorb-services-3.9.jar    %{buildroot}/usr/local/redhawk/core/lib/jacorb-services.jar
ln -sf ${JACORB_HOME}/lib/jacorb-services-3.9.jar    %{buildroot}/usr/local/redhawk/core/lib/jacorb-services-3.9.jar
ln -sf %{_prefix}/lib/slf4j-api-1.7.32.jar    %{buildroot}/usr/local/redhawk/core/lib/slf4j-api.jar
ln -sf %{_prefix}/lib/slf4j-log4j12-1.7.32.jar    %{buildroot}/usr/local/redhawk/core/lib/slf4j-log4j12.jar
ln -sf %{_prefix}/lib/reload4j-1.2.19.jar    %{buildroot}/usr/local/redhawk/core/lib/reload4j.jar

%clean
rm -rf --preserve-root $RPM_BUILD_ROOT


%pre
# -r is system account, -f is force (ignore already exists)
groupadd -r -f redhawk
if ! id redhawk &> /dev/null; then
  # -r is system account, -s is shell, -M is don't create home dir,
  # -d is the home directory, -c is comment, -n is don't create group,
  # -g is group name/id
  /usr/sbin/useradd -r -s /sbin/nologin -M -d /var/redhawk \
    -c "REDHAWK System Account" -n -g redhawk redhawk > /dev/null
elif [ `getent passwd redhawk | cut -d: -f6` == "/home/redhawk" ]; then
  if [ `ps -u redhawk | wc -l` != '1' ]; then
    echo "The redhawk user still has processes running, cannot update user account"
    exit 1
  fi
  # Reassign the redhawk home directory to something that exists
  /usr/sbin/usermod -d /var/redhawk redhawk
fi


%files
%defattr(-,root,root,-)
%{_bindir}
%exclude %{_bindir}/prf2py.py
%exclude %{_bindir}/py2prf
%exclude %{_bindir}/qtbrowse
%exclude %{_bindir}/rhlauncher
%dir %{_includedir}
%dir %{_prefix}/lib
%ifarch x86_64
%dir %{_prefix}/lib64
%endif
%{_prefix}/lib/CFInterfaces.jar
%{_prefix}/lib/jacorb.jar
%{_prefix}/lib/jacorb-3.9.jar
%{_prefix}/lib/jacorb-omgapi.jar
%{_prefix}/lib/jacorb-omgapi-3.9.jar
%{_prefix}/lib/jacorb-services.jar
%{_prefix}/lib/jacorb-services-3.9.jar
%{_prefix}/lib/slf4j-api.jar
%{_prefix}/lib/slf4j-api-1.7.32.jar
%{_prefix}/lib/slf4j-log4j12.jar
%{_prefix}/lib/slf4j-log4j12-1.7.32.jar
%{_prefix}/lib/reload4j-1.2.19.jar
%{_prefix}/lib/reload4j.jar
%{_prefix}/lib/commons-lang3-3.12.0.jar
%{_prefix}/lib/ossie.jar
%{_prefix}/lib/commons-lang3-3.12.0.jar
%{_prefix}/lib/jboss-rmi-api_1.0_spec-1.0.6.Final.jar
%{_prefix}/lib/python
%exclude %{_prefix}/lib/python/ossie/apps/qtbrowse
%exclude %{_prefix}/lib/python/ossie/apps/rhlauncher
%{_libdir}/libomnijni.so.*
%{_libdir}/libomnijni.so
%{_libdir}/libossiecluster.so
%{_libdir}/libossiecluster.so.*
%{_libdir}/libossiecluster.a
%{_libdir}/libossiecluster.la
%{_libdir}/libossiecf.so.*
%{_libdir}/libossiecfjni.so.*
%{_libdir}/libossieidl.so.*
%{_libdir}/libossieparser.so.*
%{_libdir}/libossielogcfg.so.*
%dir %{_libdir}/pkgconfig
%{_datadir}
%{_sysconfdir}/profile.d/redhawk.csh
%{_sysconfdir}/profile.d/redhawk.sh
%{_sysconfdir}/ld.so.conf.d/redhawk.conf
%{_sysconfdir}/bash_completion.d/nodeBooter

%files qt-tools
%defattr(-,root,root,-)
%{_bindir}/qtbrowse
%{_bindir}/rhlauncher
%{_prefix}/lib/python/ossie/apps/qtbrowse
%{_prefix}/lib/python/ossie/apps/rhlauncher

%files sdrroot-dom-mgr
%defattr(664,redhawk,redhawk)
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dom
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dom/components
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dom/deps
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dom/domain
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dom/mgr
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dom/mgr/rh
%attr(775,redhawk,redhawk) %{_sdrroot}/dom/mgr/DomainManager
%attr(775,redhawk,redhawk) %{_sdrroot}/dom/mgr/rh/ComponentHost
%{_sdrroot}/dom/mgr/*.xml
%{_sdrroot}/dom/mgr/rh/ComponentHost/*
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dom/waveforms
%attr(644,root,root) %{_sysconfdir}/profile.d/redhawk-sdrroot.csh
%attr(644,root,root) %{_sysconfdir}/profile.d/redhawk-sdrroot.sh

%files sdrroot-dom-profile
%defattr(664,redhawk,redhawk)
%config %{_sdrroot}/dom/domain/DomainManager.dmd.xml
%{_sdrroot}/dom/domain/DomainManager.dmd.xml.template

%files sdrroot-dev-mgr
%defattr(664,redhawk,redhawk)
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dev
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dev/devices
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dev/mgr
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dev/nodes
%attr(2775,redhawk,redhawk) %dir %{_sdrroot}/dev/services
%attr(775,redhawk,redhawk) %{_sdrroot}/dev/mgr/DeviceManager
%{_sdrroot}/dev/mgr/DeviceManager.*
%attr(644,root,root) %{_sysconfdir}/profile.d/redhawk-sdrroot.csh
%attr(644,root,root) %{_sysconfdir}/profile.d/redhawk-sdrroot.sh

%files devel
%defattr(-,root,root,-)
%{_bindir}/prf2py.py
%{_bindir}/py2prf
%{_includedir}/ossie
%{_libdir}/libomnijni.*a
%{_libdir}/libossiecf.*a
%{_libdir}/libossiecf.so
%{_libdir}/libossiecfjni.*a
%{_libdir}/libossiecfjni.so
%{_libdir}/libossieidl.so
%{_libdir}/libossieidl.*a
%{_libdir}/libossieparser.*a
%{_libdir}/libossieparser.so
%{_libdir}/libossielogcfg.*a
%{_libdir}/libossielogcfg.so
%{_libdir}/pkgconfig/ossie.pc


%post
/sbin/ldconfig


%postun
/sbin/ldconfig

%changelog
* Wed Jun 28 2017 Ryan Bauman <rbauman@lgsinnovations.com> - 2.1.2-1
- Update for 2.1.2-rc1

* Wed Jun 28 2017 Ryan Bauman <rbauman@lgsinnovations.com> - 2.1.1-2
- Bump for 2.1.1-rc2

* Sat Nov 26 2016 - 2.0.4
- Added service directory in redhawk-sdrroot-dev-mgr

* Fri Sep 16 2016 - 2.0.3-1
- Update for dependency on Java 8

* Wed Sep 9 2015 - 2.0.0-2
- Add qt-tools package
- Remove el5 support

* Mon Sep 15 2014 - 1.11.0-1
- Update for dependency on java7

* Wed May 21 2014 - 1.10.0-7
- Move a dependency that was on the wrong package

* Fri Apr 11 2014 - 1.10.0-5
- Improve OS version detection for RHEL/CentOS/Fedora
- Don't constrain boost to exact version
- Exclude qtbrowse on el5, return it to base package
- Clarify useradd/groupadd
- Add missing package requirement
- Switch to omniORBpy packaging that is compatible with Fedora

* Thu Aug 15 2013 - 1.9.0-1
- Re-work lots of dependencies
- Package for REDHAWK development
- Minor fixes for docs, licensing
- Explicitly require Java for build

* Tue Mar 12 2013 - 1.8.3-4
- Update licensing information
- Add URL for website
- Change group to a standard one, per Fedora
- Remove standard build dependencies

* Mon Sep 19 2011 - 1.7.2-2
- Further split RPMs to allow more granularity in installations

* Tue Jun 07 2011 - 1.7.0-0
- Split sdrroot into -dev and dom
- Attempt to fully capture Requires and BuildRequires
- Stop packaging components into SDRROOT

* Tue Jan 11 2011 - 1.6.0-0
- Initial spec file for redhawk and redhawk-sdrroot.

