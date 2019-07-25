Name:valentina

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

Requires(post): desktop-file-utils
Requires(postun): desktop-file-utils

Conflicts: seamly2d

%if 0%{?fedora_version} > 0 || 0%{?rhel_version} >= 700 || 0%{?centos_version} >= 700
BuildRequires: qt5-qtbase-devel >= 5.4.0
BuildRequires: pkgconfig(Qt5Svg)
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Network)
BuildRequires: pkgconfig(Qt5PrintSupport)
BuildRequires: pkgconfig(Qt5Widgets)
BuildRequires: pkgconfig(Qt5Xml)
BuildRequires: pkgconfig(Qt5Concurrent)
BuildRequires: pkgconfig(Qt5XmlPatterns)
BuildRequires: qt5-qtxmlpatterns-devel  >= 5.4.0
BuildRequires: qt5-qtsvg-devel >= 5.4.0
BuildRequires: qt5-qttools-devel >= 5.4.0

Requires:      qt5-qtsvg >= 5.4.0
Requires:      qt5-qtbase-gui >= 5.4.0
Requires:      qt5-qtxmlpatterns >= 5.4.0
%endif

%if 0%{?mageia} > 0
BuildRequires: libqt5-devel >= 5.4.0
BuildRequires: pkgconfig(Qt5Svg)
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Network)
BuildRequires: pkgconfig(Qt5PrintSupport)
BuildRequires: pkgconfig(Qt5Widgets)
BuildRequires: pkgconfig(Qt5Xml)
BuildRequires: pkgconfig(Qt5Concurrent)
BuildRequires: pkgconfig(Qt5XmlPatterns)
BuildRequires: libproxy-pacrunner
BuildRequires: qttools5
BuildRequires: qtbase5-common-devel >= 5.4.0

%if 0%{?mageia} == 6
BuildRequires: pkgconfig(openssl)
%else
BuildRequires: openssl-devel
%endif

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

%endif # %if 0%{?suse_version} > 0

Requires:   poppler-utils

Version:	0.7.0
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
%if 0%{?suse_version} >= 1315 && 0%{?suse_version} != 1550
qmake-qt5 PREFIX=%{_prefix} PREFIX_LIB=%{_prefix}/%{_lib} LRELEASE=lrelease-qt5 Valentina.pro -r "CONFIG += noTests noRunPath no_ccache noDebugSymbols"
%else

%if 0%{?mageia} >= 6
qmake PREFIX=%{_prefix} PREFIX_LIB=%{_prefix}/%{_lib} Valentina.pro -r "CONFIG += noTests noRunPath no_ccache noDebugSymbols"
%else
qmake-qt5 PREFIX=%{_prefix} PREFIX_LIB=%{_prefix}/%{_lib} Valentina.pro -r "CONFIG += noTests noRunPath no_ccache noDebugSymbols"
%endif

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
update-desktop-database &> /dev/null ||:
update-mime-database %{_datadir}/mime &> /dev/null || :
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi

%postun
/sbin/ldconfig
update-desktop-database &> /dev/null ||:
update-mime-database %{_datadir}/mime &> /dev/null || :
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
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
%doc README.txt 
%license LICENSE_GPL.txt 
%doc %{_mandir}/man1/%{name}.1*
%doc %{_mandir}/man1/tape.1*
%{_bindir}/valentina
%{_bindir}/tape
%{_libdir}/libvpropertyexplorer.so
%{_libdir}/libvpropertyexplorer.so.*
%{_libdir}/libqmuparser.so
%{_libdir}/libqmuparser.so.*
%dir %{_libdir}/mime/
%dir %{_libdir}/mime/packages/
%{_libdir}/mime/packages/%{name}
%dir %{_datadir}/mime/
%dir %{_datadir}/mime/packages/
%{_datadir}/mime/packages/%{name}.xml
%{_datadir}/applications/%{name}.desktop
%{_datadir}/applications/tape.desktop

%dir %{_datadir}/icons/
%dir %{_datadir}/icons/hicolor/
%dir %{_datadir}/icons/hicolor/48x48/
%dir %{_datadir}/icons/hicolor/48x48/apps/
%dir %{_datadir}/icons/hicolor/48x48/mimetypes/
%dir %{_datadir}/icons/hicolor/64x64/
%dir %{_datadir}/icons/hicolor/64x64/apps/
%dir %{_datadir}/icons/hicolor/64x64/mimetypes/
%dir %{_datadir}/icons/hicolor/128x128/
%dir %{_datadir}/icons/hicolor/128x128/apps/
%dir %{_datadir}/icons/hicolor/128x128/mimetypes/
%dir %{_datadir}/icons/hicolor/256x256/
%dir %{_datadir}/icons/hicolor/256x256/apps/
%dir %{_datadir}/icons/hicolor/256x256/mimetypes/
%dir %{_datadir}/icons/hicolor/512x512/
%dir %{_datadir}/icons/hicolor/512x512/apps/
%dir %{_datadir}/icons/hicolor/512x512/mimetypes/
%{_datadir}/icons/hicolor/*/apps/*.png
%{_datadir}/icons/hicolor/*/mimetypes/*.png

%dir %{_datadir}/%{name}/
%{_datadir}/%{name}/diagrams.rcc
%dir %{_datadir}/%{name}/translations/
%{_datadir}/%{name}/translations/*.qm
%dir %{_datadir}/%{name}/tables/
%dir %{_datadir}/%{name}/tables/multisize/
%{_datadir}/%{name}/tables/multisize/*.vst
%dir %{_datadir}/%{name}/tables/templates/
%{_datadir}/%{name}/tables/templates/*.vit
%dir %{_datadir}/%{name}/labels/
%{_datadir}/%{name}/labels/*.xml

%clean
rm -f dist/debian/%{name}.1.gz dist/debian/tape.1.gz dist/debian/%{name}.xml dist/debian/%{name}
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}


%changelog
* Mon Jun 11 2018 Roman Telezhynskyi
 - Auto build

