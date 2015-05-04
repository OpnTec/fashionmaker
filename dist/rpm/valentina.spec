Name:valentina

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires: ccache

# Fedora specifics
%if 0%{?fedora_version} > 0 || 0%{?rhel_version} > 0 || 0%{?centos_version} > 0
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Network)
BuildRequires: pkgconfig(Qt5PrintSupport)
BuildRequires: pkgconfig(Qt5Widgets)
BuildRequires: pkgconfig(Qt5Xml)
BuildRequires: qt5-qtxmlpatterns-devel  >= 5.0.0
BuildRequires: qt5-qtsvg-devel >= 5.0.0
BuildRequires: qt5-qttools-devel >= 5.0.0

Requires:      qt5-qtsvg >= 5.0.0
Requires:      qt5-qtbase-gui >= 5.0.0
Requires:      qt5-qtxmlpatterns >= 5.0.0
%endif

# SUSE Specifics
%if 0%{?suse_version} > 0
BuildRequires: libqt5-qtbase-devel
BuildRequires: libqt5-qttools
BuildRequires: libQt5Svg-devel
BuildRequires: update-desktop-files

%if 0%{?suse_version} >= 1310
BuildRequires: libQt5XmlPatterns-devel
%endif

%if 0%{?suse_version} >= 1320
BuildRequires: libqt5-linguist-devel
BuildRequires: libqt5-qtxmlpatterns-devel
%endif

%endif

Requires:   poppler-utils

Version:	0.3.1
Release:	0
URL:		https://bitbucket.org/dismine/valentina
License:	GPL-3.0+
Source0:	%{name}-%{version}.tar
Group:		Graphics
Summary:	Pattern Making Application
BuildRoot:  %{_tmppath}/%{name}-%{version}-build 
Packager:   Roman Telezhinskyi <dismine@gmail.com>   

# Disables debug packages and stripping of binaries:
%global _enable_debug_package 0
%global __debug_install_post %{nil} 
%global debug_package %{nil}
%if 0%{?suse_version} >= 1320
%global suse_insert_debug_package %{nil} 
%global _suse_insert_debug_package %{nil} 
%global _suse_insert_debug_package_seen %{nil}
%endif

%description
Valentina is a cross-platform patternmaking program which allows designers 
to create and model patterns of clothing. This software allows pattern 
creation, using either standard sizing tables or an individual’s set of 
measurements. It blends new technologies with traditional methods to create 
a unique pattern making tool. 

%prep
%setup -q -n %{name}-%{version}

%build
%if 0%{?suse_version} >= 1320
qmake-qt5 PREFIX=%{buildroot}%{_prefix} LRELEASE=lrelease-qt5 Valentina.pro -r
%else
qmake-qt5 PREFIX=%{buildroot}%{_prefix} Valentina.pro -r
%endif
%{__make} %{?jobs:-j %jobs}

%install
export NO_DEBUGINFO_STRIP_DEBUG=true
%{__make} install
gzip -9c dist/debian/%{name}.1 > dist/debian/%{name}.1.gz &&
%{__install} -Dm 644 dist/debian/%{name}.1.gz %{buildroot}%{_mandir}/man1/%{name}.1.gz

%if 0%{?suse_version} > 0
%suse_update_desktop_file -r %{name} VectorGraphics
%endif

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc README.txt LICENSE_GPL.txt 
%doc %{_mandir}/man1/%{name}.1.gz
%{_bindir}/valentina
%{_libdir}/libvpropertyexplorer.so
%{_libdir}/libvpropertyexplorer.so.*
%{_libdir}/libqmuparser.so
%{_libdir}/libqmuparser.so.*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/*
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/translations
%{_datadir}/%{name}/translations/*.qm
%dir %{_datadir}/%{name}/tables
%dir %{_datadir}/%{name}/tables/standard
%{_datadir}/%{name}/tables/standard/*.vst

%clean
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}


%changelog
* Mon Dec 22 2014 Roman Telezhinskyi
 - Initial build

