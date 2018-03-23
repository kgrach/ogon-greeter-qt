#
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

# norootforbuild

Name:            ogon-qt-greeter
Version:         1.0.0
Release:         1
Summary:	 Login screen for ogon
Group:           Productivity/Networking/RDP
License:         Apache-2.0
Url:             http://ogon-project.com
BuildRequires:   cmake >= 2.8.12
BuildRequires:   pkg-config
BuildRequires:   ogon-platform-qt-devel
BuildRequires:   ogon-devel
BuildRequires:   protobuf-devel
BuildRequires:   libogon-message1-devel
BuildRequires:   libqt5-linguist-devel
BuildRequires:   libQt5Core-devel
BuildRequires:   libQt5Widgets-devel
BuildRequires:   libqt5-qtbase-private-headers-devel
Source:          ogon-qt-greeter.tar.xz
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
The ogon Remote Desktop Services provide graphical remote access to
desktop sessions and applications using the Remote Desktop Protocol
(RDP) and supports most modern RDP protocol extensions, bitmap
compression codecs and device redirections. ogon is build on the
FreeRDP library and is compatible with virtually any existing Remote
Desktop Client.

This package contains a qt based login screen ("greeter") that uses
the qt backend.

%prep
%setup -q

%build
%cmake
make %{?_smp_mflags}

%install
#export NO_BRP_STRIP_DEBUG=true
#export NO_DEBUGINFO_STRIP_DEBUG=true
#%%define __debug_install_post %{nil}
%cmake_install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,0755)
%{_bindir}/ogon-qt-greeter
%dir %{_datarootdir}/qt5/translations/
%dir %{_datarootdir}/qt5
%{_datarootdir}/qt5/translations/*.qm

%changelog
* Wed Jun 08 2016 - bernhard.miklautz@thincast.com
- Initial version
