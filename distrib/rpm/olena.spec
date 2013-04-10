%bcond_with apps
%bcond_without doc
%bcond_without scribo

Name: olena
Version: 2.0
Release: 3
Epoch: 2
License: GPLv2
Summary: Olena is a platform dedicated to image processing
Group: Development/C++
URL: http://www.lrde.epita.fr/cgi-bin/twiki/view/Olena/WebHome
# Get from https://svn.lrde.epita.fr/svn/oln/tags/olena-1.0 to have scribo
Source0:  %name-%version.tar.bz2
Patch0: olena-1.0-subdirs.patch
Patch1: olena-1.0-linkage.patch
Patch2: olena-2.0-tesseract-3.01.patch
BuildRoot: %{_tmppath}/%{name}-%{version}-root
BuildRequires: cfitsio-devel
BuildRequires: tiff-devel
BuildRequires: imagemagick-devel
BuildRequires: mesaglut-devel
BuildRequires: mesagl-devel
BuildRequires: vtk-devel
%if %with scribo
BuildRequires: tesseract-devel >= 2.04-3
%endif
BuildRequires: imagemagick
BuildRequires: texlive-latex texlive-dvips
BuildRequires: latex2html
BuildRequires: doxygen

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
%_bindir/*
%_datadir/olena
%if %with scribo
%_libdir/scribo/*
%endif

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
%{_libdir}/*.so
%{_libdir}/*.la
%{_includedir}/*
%_libdir/*.a

#------------------------------------------------------------------------------

%prep
%setup -q
%patch1 -p0
%patch2
%if ! %with doc
%patch0 -p0 -b .orig
autoreconf -fi
%endif
pushd external/trimesh
autoreconf -fi
popd

%build
%configure2_5x \
%if %with scribo
	--enable-scribo \
%endif
	--enable-trimesh \
%if %with apps
	--enable-apps \
%endif
	--enable-tools

pushd external/trimesh
%configure2_5x
popd

%make

%install
%makeinstall_std



%changelog
* Thu Dec 01 2011 Dmitry Mikhirev <dmikhirev@mandriva.org> 2:2.0-3
+ Revision: 735945
- Fixed compatibility to tesseract 3.01

* Wed Nov 09 2011 Andrey Smirnov <asmirnov@mandriva.org> 2:2.0-2
+ Revision: 729310
- Disable scribo until compatibility with current tesseract fixed

* Wed Oct 05 2011 Nicolas LÃ©cureuil <nlecureuil@mandriva.com> 2:2.0-1
+ Revision: 703157
- Fix file list
- Install .a files
- Clean spec file
- New version 2.0

  + Oden Eriksson <oeriksson@mandriva.com>
    - rebuild

  + Pascal Terjan <pterjan@mandriva.org>
    - Drop require on gdcm-devel for now :
      - It is in contribs
      - It is unused now (configure does not find it)

* Sat May 08 2010 Funda Wang <fwang@mandriva.org> 2:1.0-6mdv2010.1
+ Revision: 543630
- fix link
- more BRs
- add BR
- add convert BR

  + Oden Eriksson <oeriksson@mandriva.com>
    - rebuilt for 2010.1

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

  + Nicolas LÃ©cureuil <nlecureuil@mandriva.com>
    - import olena
