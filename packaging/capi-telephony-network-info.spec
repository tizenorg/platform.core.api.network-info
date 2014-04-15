%define major 0
%define minor 1
%define patchlevel 0

Name:       capi-telephony-network-info
Summary:    A Telephony Network Information library in Tizen Native API
Version:    %{major}.%{minor}.%{patchlevel}
Release:    1
Group:      C API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: 	capi-telephony-network-info.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)

%description
A Telephony Network Information library in Tizen Native API.


%package devel
Summary:  A Telephony Network Information library in Tizen Native API (Development)
Group:    C API
Requires: %{name} = %{version}-%{release}

%description devel
A Telephony Network Information library in Tizen Native API.



%prep
%setup -q
cp %{SOURCE1001} .


%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%{_libdir}/libcapi-telephony-network-info.so.*

%files devel
%manifest %{name}.manifest
%{_includedir}/telephony/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-telephony-network-info.so

