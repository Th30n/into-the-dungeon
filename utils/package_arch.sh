#!/bin/sh

PKG_PROTO_DIR=$1
CHANGELOG=$2

if [[ "$#" -ne 2 ]] ; then
  echo "Uses PKGBUILD and .install files from prototype directory with"
  echo "source package downloaded from github to create pacman package."
  echo "Usage: source $0 pkg_proto_dir changelog_file"
  return 1
fi

mkdir -p _arch_packages
cp ${PKG_PROTO_DIR}/* _arch_packages/
cp ${CHANGELOG} _arch_packages/ChangeLog
cd _arch_packages
makepkg -g >> PKGBUILD
makepkg -f
echo -e "package_arch.sh: Created binary package"
cp -v *.pkg.tar.xz ../
if which mkaurball > /dev/null 2>&1 ; then
  mkaurball -f
  echo -e "package_arch.sh: Created AUR package"
  cp -v *.src.tar.gz ../
else
  echo "package_arch.sh W: mkaurball missing - unable to create AUR package"
fi
if which namcap > /dev/null 2>&1 ; then
  namcap PKGBUILD
  namcap *.pkg.tar.xz
fi

return 0
