Name:valentina

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

Requires(post): desktop-file-utils
Requires(postun): desktop-file-utils

# Fedora specifics
%if 0%{?fedora_version} > 0 
BuildRequires: qt5-qtbase-devel >= 5.0.0
BuildRequires: pkgconfig(Qt5Svg)
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

%if 0%{?suse_version} == 1310
BuildRequires: libQt5XmlPatterns-devel
%endif

%if 0%{?suse_version} >= 1315
BuildRequires: libqt5-linguist-devel
BuildRequires: libqt5-qtxmlpatterns-devel
%endif

%endif

Requires:   poppler-utils

Version:	0.4.3
Release:	0
URL:		https://bitbucket.org/dismine/valentina
License:	GPL-3.0+
Source0:	%{name}-%{version}.tar.gz
Group:		Graphics
Summary:	Pattern Making Application
BuildRoot:  %{_tmppath}/%{name}-%{version}-build 
Packager:   Roman Telezhinskyi <dismine@gmail.com>   

# Disables debug packages and stripping of binaries:
%global _enable_debug_package 0
%global __debug_install_post %{nil} 
%global debug_package %{nil}
%if 0%{?suse_version} >= 1315
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
%if 0%{?suse_version} >= 1315
qmake-qt5 PREFIX=%{_prefix} LRELEASE=lrelease-qt5 Valentina.pro -r "CONFIG += noTests noRunPath no_ccache noDebugSymbols"
%else
qmake-qt5 PREFIX=%{_prefix} Valentina.pro -r "CONFIG += noTests noRunPath no_ccache noDebugSymbols"
%endif
%{__make} %{?jobs:-j %jobs}

%install
export NO_DEBUGINFO_STRIP_DEBUG=true

%{__make} INSTALL_ROOT=%{buildroot} install

gzip -9c dist/debian/%{name}.1 > dist/debian/%{name}.1.gz &&
%{__install} -Dm 644 dist/debian/%{name}.1.gz %{buildroot}%{_mandir}/man1/%{name}.1.gz

gzip -9c dist/debian/tape.1 > dist/debian/tape.1.gz &&
%{__install} -Dm 644 dist/debian/tape.1.gz %{buildroot}%{_mandir}/man1/tape.1.gz

cp dist/debian/valentina.sharedmimeinfo dist/debian/%{name}.xml &&
%{__install} -Dm 644 dist/debian/%{name}.xml %{buildroot}%{_datadir}/mime/packages/%{name}.xml

cp dist/debian/valentina.mime dist/debian/%{name} &&
%{__install} -Dm 644 dist/debian/%{name} %{buildroot}%{_libdir}/mime/packages/%{name}

%if 0%{?suse_version} > 0
%suse_update_desktop_file -r %{name} Graphics VectorGraphics 2DGraphics
%suse_update_desktop_file -r tape Utility Applet
%endif

%post 
/sbin/ldconfig
/usr/bin/update-desktop-database &> /dev/null || :
/bin/touch --no-create %{_datadir}/mime/packages &>/dev/null || :

%postun
/sbin/ldconfig
/usr/bin/update-desktop-database &> /dev/null || :
if [ $1 -eq 0 ] ; then
  /usr/bin/update-mime-database %{_datadir}/mime &> /dev/null || :
fi

%posttrans
# Fedora specifics
%if 0%{?fedora_version} > 0
/usr/bin/update-mime-database %{?fedora:-n} %{_datadir}/mime &> /dev/null || :
%else
/usr/bin/update-mime-database %{_datadir}/mime &> /dev/null || :
%endif

%files
%defattr(-,root,root,-)
%doc README.txt LICENSE_GPL.txt 
%doc %{_mandir}/man1/%{name}.1.gz
%doc %{_mandir}/man1/tape.1.gz
%{_bindir}/valentina
%{_bindir}/tape
%{_libdir}/libvpropertyexplorer.so
%{_libdir}/libvpropertyexplorer.so.*
%{_libdir}/libqmuparser.so
%{_libdir}/libqmuparser.so.*
%dir %{_libdir}/mime
%dir %{_libdir}/mime/packages
%{_libdir}/mime/packages/%{name}
%dir %{_datadir}/mime
%dir %{_datadir}/mime/packages
%{_datadir}/mime/packages/%{name}.xml
%{_datadir}/applications/%{name}.desktop
%{_datadir}/applications/tape.desktop
%{_datadir}/pixmaps/*
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/diagrams.rcc
%dir %{_datadir}/%{name}/translations
%{_datadir}/%{name}/translations/*.qm
%dir %{_datadir}/%{name}/tables
%dir %{_datadir}/%{name}/tables/standard
%{_datadir}/%{name}/tables/standard/*.vst
%dir %{_datadir}/%{name}/tables/templates
%{_datadir}/%{name}/tables/templates/*.vit

%clean
rm -f dist/debian/%{name}.1.gz dist/debian/tape.1.gz dist/debian/%{name}.xml dist/debian/%{name}
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}


%changelog
* Fri Nov 27 2015 Roman Telezhinskyi
 - Auto build

