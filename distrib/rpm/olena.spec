%bcond_with apps
%bcond_with doc

Name: olena
Version: 1.0
Release: %mkrel 4
Epoch: 2
License: GPLv2
Summary: Olena is a platform dedicated to image processing
Group: Development/C++
URL: http://www.lrde.epita.fr/cgi-bin/twiki/view/Olena/WebHome
# Get from https://svn.lrde.epita.fr/svn/oln/tags/olena-1.0 to have scribo
Source0:  %name-%version.tar.bz2
Patch0: olena-1.0-subdirs.patch
BuildRoot: %{_tmppath}/%{name}-%{version}-root
BuildRequires: cfitsio-devel
BuildRequires: tiff-devel
BuildRequires: imagemagick-devel
BuildRequires: mesaglut-devel
BuildRequires: mesagl-devel
BuildRequires: vtk-devel
BuildRequires: gdcm-devel
BuildRequires: tesseract-devel >= 2.04-3

%description
Olena is a platform dedicated to image processing. At the
moment it is mainly composed of a C++ library: Milena. This library features
many tools to easily perform image processing tasks. Its main characteristic is
its genericity: it allows to write an algorithm once and run it over many kinds
of images (grey scale, color, 1D, 2D, 3D, ...).

#------------------------------------------------------------------------------

%if %with doc

%package doc
Summary: Olena documentation
Group: Books/Howtos

%description doc
This is the documentation for Olena.
Olena is a platform dedicated to image processing. At the
moment it is mainly composed of a C++ library: Milena. This library features
many tools to easily perform image processing tasks. Its main characteristic is
its genericity: it allows to write an algorithm once and run it over many kinds
of images (grey scale, color, 1D, 2D, 3D, ...).

%files doc
%defattr(-,root,root,-)
%_docdir/olena

%endif

#------------------------------------------------------------------------------

%package tools
Summary: Olena tools
Group: Development/C++

%description tools
Olena is a platform dedicated to image processing. At the
moment it is mainly composed of a C++ library: Milena. This library features
many tools to easily perform image processing tasks. Its main characteristic is
its genericity: it allows to write an algorithm once and run it over many kinds
of images (grey scale, color, 1D, 2D, 3D, ...).

%files tools
%defattr(-,root,root,-)
%_bindir/*
%_datadir/olena/images

#------------------------------------------------------------------------------

%define gluit_major 0
%define libgluit %mklibname gluit %{gluit_major}

%package -n %{libgluit}
Summary: Gluit C++ main Olena library
Group: Development/C++

%description -n %{libgluit}
Gluit C++ main Olena library.

%files -n %{libgluit}
%_libdir/libgluit.so.%{gluit_major}*

#------------------------------------------------------------------------------

%define tri_major 0
%define libtrimesh %mklibname trimesh %{tri_major}

%package -n %{libtrimesh}
Summary: trimesh C++ main Olena library
Group: Development/C++

%description -n %{libtrimesh}
trimesh C++ main Olena library.

%files -n %{libtrimesh}
%defattr(-,root,root,-)
%{_libdir}/libtrimesh.so.%{tri_major}*

#------------------------------------------------------------------------------

%package devel
Summary: Olena devel files
Group: Development/C++
Requires: %{libtrimesh} = %{epoch}:%{version}
Requires: %{libgluit} = %{epoch}:%{version}
Requires: olena-tools

%description devel
Olena devel files
Olena is a platform dedicated to image processing. At the
moment it is mainly composed of a C++ library: Milena. This library features
many tools to easily perform image processing tasks. Its main characteristic is
its genericity: it allows to write an algorithm once and run it over many kinds
of images (grey scale, color, 1D, 2D, 3D, ...).

%files devel
%defattr(-,root,root,-)
%{_libdir}/*.so
%{_libdir}/*.la
%{_includedir}/*
%exclude %_libdir/*.a

#------------------------------------------------------------------------------

%prep
%setup -q
%if ! %with doc
%patch0 -p0 -b .orig
%endif

%build
CXXFLAGS="$CXXFLAGS -I%{_includedir}/ImageMagick"
export CPPFLAGS CXXFLAGS

%configure2_5x \
	--enable-scribo \
	--enable-trimesh \
%if %with apps
	--enable-apps \
%endif
	--enable-tools

%make

%install
rm -rf %{buildroot}
%makeinstall_std

%clean
rm -rf %buildroot



%changelog
* Wed Sep 23 2009 Helio Chissini de Castro <helio@mandriva.com> 2:1.0-4mdv2010.0
+ Revision: 448013
- Respect description, as requested in Mandriva bug #53959

* Wed Sep 16 2009 Helio Chissini de Castro <helio@mandriva.com> 2:1.0-3mdv2010.0
+ Revision: 443577
- Build against tesseract shared.

* Wed Sep 16 2009 Helio Chissini de Castro <helio@mandriva.com> 2:1.0-2mdv2010.0
+ Revision: 443356
- Fix the mess with olena revision

* Mon Sep 14 2009 Helio Chissini de Castro <helio@mandriva.com> 1:1.0-0.0.419mdv2010.0
+ Revision: 440844
- finally fixed olen package as requested by nepomuk team. Disable milena doc
  was necessary and use current trunk sources

* Tue Aug 25 2009 Helio Chissini de Castro <helio@mandriva.com> 1.0-1mdv2010.0
+ Revision: 421273
- imported package olena

  + Nicolas LÃ©cureuil <neoclust@mandriva.org>
    - import olena