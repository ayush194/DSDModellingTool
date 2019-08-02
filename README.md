# DSDModellingTool

A tool to design and model DNA Nanomolecules at the domain level. It can be used to interactively design DNA Strands and Species and render them for visualization purposes. It can also read new species which need to be fed in as text file in the BNGL format (for an example, see the sample text file). The tool is built in Qt5 and hence has cross-platform support. For building the tool, have a look at the instructions below.

## Dependencies
For OSX and Linux:
You should have clang-llvm 7.0 compiler or greater to run this program. You can check this
```bash
clang --version
```

You should have qmake for compiling the .pro file to generate the Makefile for the building the project. You can install qmake by installing it standalone or by installing the Qt package. The easiest way to install qmake is to use Homebrew to install the lates version of Qt (Qmake is bundled with Qt).
```bash
brew install qt --HEAD
```

## Instructions
Go to the directory conatining the project file (.pro). Build the .pro file using qmake
```bash
qmake -spec macx-g++ x.pro (on Mac OS X to generate Makefile)

qmake x.pro (on linux to generate Makefile)
```

for Windows
```bash
qmake -spec win32-g++ x.pro
```

To build the application, you can use OSX's built-in make tool. Simply run make in the directory containing the Makefile.
```bash
sudo make
```
