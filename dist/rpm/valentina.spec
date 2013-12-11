Name:valentina

# Fedora specifics
%if 0%{?fedora_version} > 0 || 0%{?rhel_version} > 0 || 0%{?centos_version} > 0
BuildRequires:  pkgconfig(Qt5Core) libqt5-qttools >= 5.0.0 libQtSvg-devel >= 5.0.0 ccache
%endif

# SUSE Specifics
%if 0%{?suse_version} > 0
BuildRequires:  libqt5-qtbase-devel >= 5.0.0 libqt5-qttools >= 5.0.0 libQt5Svg-devel >= 5.0.0 ccache update-desktop-files
%endif

Version:	0.2.1
Release:	1
URL:		https://bitbucket.org/dismine/valentina
License:	GPL-3.0+
Source0:	%{name}-%{version}.tar.gz
Group:		Graphics
Summary:	Pattern Making Application
BuildRoot:      %{_tmppath}/%{name}-%{version}-build 
Packager:       Roman Telezhinsky <dismine@gmail.com>   

%description
Open source project of creating a pattern making program, whose allow 
create and modeling patterns of clothing.

%prep
%setup -q -n %{name}-%{version}

%build
qmake-qt5
%{__make} %{?jobs:-j %jobs}

%install
mkdir -p $RPM_BUILD_ROOT/usr/{bin,share}
mkdir -p $RPM_BUILD_ROOT/usr/share/%{name}/translations
%{__install} -Dm 755 -s bin/%{name} %{buildroot}%{_bindir}/%{name}
%{__install} -Dm 644 dist/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop
%{__install} -Dm 644 dist/%{name}.png %{buildroot}%{_datadir}/pixmaps/%{name}.png
%{__install} -Dm 644 dist/%{name}.1.gz %{buildroot}%{_mandir}/man1/%{name}.1.gz
%{__install} -Dm 644 share/translations/valentina_uk.qm %{buildroot}%{_datadir}/%{name}/translations/valentina_uk.qm
%{__install} -Dm 644 share/translations/valentina_ru.qm %{buildroot}%{_datadir}/%{name}/translations/valentina_ru.qm
%if 0%{?suse_version} > 0
%suse_update_desktop_file -r %{name} VectorGraphics
%endif

%clean
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc README LICENSE 
%doc %{_mandir}/man1/%{name}.1.gz
%{_bindir}/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/%{name}/translations/valentina_uk.qm
%{_datadir}/%{name}/translations/valentina_ru.qm
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/translations


%changelog
* Mon Dec 9 2013 Roman Telezhinsky
 - Initial build



