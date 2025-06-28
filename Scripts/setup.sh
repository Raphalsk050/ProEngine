  #!/usr/bin/env bash
  set -e

  # Atualiza lista de pacotes
  sudo apt update

  # Instala pacotes de OpenGL e utilitários
  sudo apt install -y \
      mesa-common-dev \
      libgl1-mesa-dev \
      libglu1-mesa-dev \
      freeglut3-dev \
      mesa-utils


  # Wayland (wayland-scanner)
  sudo apt install -y \
      libwayland-dev \
      wayland-protocols

  # XKB (xkbcommon >= 0.5.0)
  sudo apt install -y \
      libxkbcommon-dev

  # Xrandr
  sudo apt install -y \
      libxrandr-dev

  # Xinerama
  sudo apt install -y \
      libxinerama-dev

  # Xcursor
  sudo apt install -y \
      libxcursor-dev

  # XInput (libXi)
  sudo apt install -y \
      libxi-dev

  # zlib (necessário para Assimp)
  sudo apt install -y \
      zlib1g-dev

  echo
  echo "Setup complete!"