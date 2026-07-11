# Libgfx: cross-plartform graphics library.

## installation

```bash
git clone https://codeberg.org/nyxvoid/libgfx.git
cd libgfx && make
```

## generate GLAD headers
```bash
glad --api gl:core=4.6 --out-path glad c
```

## dependencies

- [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/)
- [OpenGL 3.3+](https://www.opengl.org/)
- [GLFW](https://www.glfw.org/)
- [GLAD](https://github.com/Dav1dde/glad)
