Dependency
---

pkg-config
xorg.libXtst
libchewing
gtk3

qt5  (QtCore)
  { nixpkgs.config.allowUnsupportedSystem = true; }
  { allowUnsupportedSystem = true; }

